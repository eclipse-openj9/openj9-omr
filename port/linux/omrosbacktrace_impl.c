/*******************************************************************************
 * Copyright (c) 1991, 2022 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at https://www.eclipse.org/legal/epl-2.0/
 * or the Apache License, Version 2.0 which accompanies this distribution and
 * is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following
 * Secondary Licenses when the conditions for such availability set
 * forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
 * General Public License, version 2 with the GNU Classpath
 * Exception [1] and GNU General Public License, version 2 with the
 * OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

/**
 * @file
 * @ingroup Port
 * @brief Stack backtracing support
 */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

/*
 * Include system header files first because omrsignal_context.h removes the
 * definition of __USE_GNU needed to see the declaration of dl_iterate_phdr(3).
 */

#include <dlfcn.h>
#include <elf.h>
#include <execinfo.h>
#include <fcntl.h>
#include <link.h>
#include <stdlib.h>
#include <string.h>

#include "omrport.h"
#include "omrportpriv.h"
#include "omrsignal_context.h"
#include "omrintrospect.h"

struct frameData {
	void **address_array;
	uintptr_t capacity;
};

/*
 * NULL handler. We only care about preventing the signal from propagating up the call stack, no need to do
 * anything in the handler.
 */
static uintptr_t
handler(struct OMRPortLibrary *portLibrary, uint32_t gpType, void *gpInfo, void *userData)
{
	return OMRPORT_SIG_EXCEPTION_RETURN;
}

/*
 * Wrapped call to libc backtrace function
 */
static uintptr_t
protectedBacktrace(struct OMRPortLibrary *port, void *arg)
{
	struct frameData *addresses = (struct frameData *)arg;
	return backtrace(addresses->address_array, addresses->capacity);
}

/*
 * Provides signal protection for the libc backtrace call. If the stack walk causes a segfault then we check the output
 * array to see if we got any frames as we may be able to provide a partial backtrace. We also record that the stack walk
 * was incomplete so that information is available after the fuction returns.
 */
static uintptr_t
backtrace_sigprotect(struct OMRPortLibrary *portLibrary, J9PlatformThread *threadInfo, void **address_array, uintptr_t capacity)
{
	uintptr_t ret = 0;

	if (omrthread_self()) {
		struct frameData args;
		args.address_array = address_array;
		args.capacity = capacity;

		memset(address_array, 0, sizeof(void *) * capacity);

		if (portLibrary->sig_protect(portLibrary, protectedBacktrace, &args, handler, NULL, OMRPORT_SIG_FLAG_SIGALLSYNC | OMRPORT_SIG_FLAG_MAY_RETURN, &ret) != 0) {
			/* check to see if there were any addresses populated */
			for (ret = 0; (ret < args.capacity) && (NULL != address_array[ret]);) {
				ret += 1; /* count frames */
			}

			threadInfo->error = FAULT_DURING_BACKTRACE;
		}
	} else {
		ret = backtrace(address_array, capacity);
	}

	return ret;
}

/*
 * Read an ELF file header and do some basic validation.
 */
static BOOLEAN
elf_read_header(int fd, ElfW(Ehdr) *file_header)
{
	if (sizeof(*file_header) != read(fd, file_header, sizeof(*file_header))) {
		return FALSE;
	}

	/* check the expected magic number */
	if ((ELFMAG0 != file_header->e_ident[EI_MAG0])
	||  (ELFMAG1 != file_header->e_ident[EI_MAG1])
	||  (ELFMAG2 != file_header->e_ident[EI_MAG2])
	||  (ELFMAG3 != file_header->e_ident[EI_MAG3])
	) {
		return FALSE;
	}

	/* check that the class is appropriate for this process */
#if defined(OMR_ENV_DATA64)
	if (ELFCLASS64 != file_header->e_ident[EI_CLASS]) {
		return FALSE;
	}
#else /* defined(OMR_ENV_DATA64) */
	if (ELFCLASS32 != file_header->e_ident[EI_CLASS]) {
		return FALSE;
	}
#endif /* defined(OMR_ENV_DATA64) */

	/* check that data is ordered suitably for this process */
#if defined(OMR_ENV_LITTLE_ENDIAN)
	if (ELFDATA2LSB != file_header->e_ident[EI_DATA]) {
		return FALSE;
	}
#else /* defined(OMR_ENV_LITTLE_ENDIAN) */
	if (ELFDATA2MSB != file_header->e_ident[EI_DATA]) {
		return FALSE;
	}
#endif /* defined(OMR_ENV_LITTLE_ENDIAN) */

	/* check that the file is a shared library or an executable */
	if ((ET_DYN != file_header->e_type) && (ET_EXEC != file_header->e_type)) {
		return FALSE;
	}

	/* check that section headers have the expected size */
	if (sizeof(ElfW(Shdr)) != file_header->e_shentsize) {
		return FALSE;
	}

	/* everything appears to be ok */
	return TRUE;
}

struct ElfSymbolData {
	/*
	 * The offset of an interesting location. Initially, this is an offset
	 * in the virtual address space of the process (i.e. a virtual address).
	 * Once the program header containing that address is identified, it is
	 * updated to represent the offset in the file whose contents have been
	 * loaded at that virtual address. Finally, once a symbol is found that
	 * spans that file offset, it is modified to represent an offset from
	 * that symbol.
	 */
	uintptr_t offset;
	/*
	 * The name of the related symbol. This will be the empty string if no
	 * suitable symbol can be found.
	 */
	char name[256];
};

/*
 * Open the specified file and try to locate a symbol for a function which
 * includes the required offset. If found, the function name and instruction
 * offset are returned in *data.
 */
static void
elf_find_symbol(const char *fileName, struct ElfSymbolData *data)
{
	int fd = open(fileName, O_RDONLY, 0);
	ElfW(Ehdr) file_header;

	if (fd < 0) {
		/* cannot open file */
		return;
	}

	if (elf_read_header(fd, &file_header)) {
		uintptr_t section = 0;
		uintptr_t section_count = file_header.e_shnum;
		uintptr_t offset = data->offset;
		uintptr_t file_offset = 0;
		uintptr_t symbol_offset = 0;
		uintptr_t loaded_section_index = 0;
		ElfW(Shdr) loaded_section;
		ElfW(Shdr) symtab_section;
		ElfW(Shdr) strtab_section;
		ElfW(Sym) symbol;

		/* seek to the section header table */
		if ((off_t)-1 == lseek(fd, file_header.e_shoff, SEEK_SET)) {
			goto done;
		}

		/* find the loaded section that includes the offset of interest */
		for (loaded_section_index = 0;; ++loaded_section_index) {
			if (loaded_section_index >= section_count) {
				goto done;
			}

			/* read the next section header */
			if (sizeof(loaded_section) != read(fd, &loaded_section, sizeof(loaded_section))) {
				goto done;
			}

			if (OMR_ARE_NO_BITS_SET(loaded_section.sh_flags, SHF_ALLOC)) {
				/* we're only interested in sections that are in memory during exection */
				continue;
			}

			if ((loaded_section.sh_offset <= offset) && ((offset - loaded_section.sh_offset) < loaded_section.sh_size)) {
				/*
				 * this is the section we want; translate the offset to a virtual address
				 * as defined by the section header for consistency with symbol values
				 * found in a symbol table section
				 */
				offset = offset - loaded_section.sh_offset + loaded_section.sh_addr;
				break;
			}
		}

		/* seek to the section header table again */
		if ((off_t)-1 == lseek(fd, file_header.e_shoff, SEEK_SET)) {
			goto done;
		}

		for (section = 0; section < section_count; ++section) {
			/* read the next section header */
			if (sizeof(symtab_section) != read(fd, &symtab_section, sizeof(symtab_section))) {
				goto done;
			}

			if (SHT_SYMTAB != symtab_section.sh_type) {
				/* we're only interested in symbol table entries */
				continue;
			}

			/* check that the entry has the expected size */
			if (sizeof(symbol) != symtab_section.sh_entsize) {
				goto done;
			}

			/*
			 * the link in a symbol table entry makes reference to a
			 * string table section: make sure the index is reasonable
			 */
			if (symtab_section.sh_link >= section_count) {
				goto done;
			}

			/* seek to the related string table section */
			file_offset = file_header.e_shoff + (symtab_section.sh_link * sizeof(ElfW(Shdr)));
			if ((off_t)-1 == lseek(fd, file_offset, SEEK_SET)) {
				goto done;
			}

			/* read the string table section */
			if (sizeof(strtab_section) != read(fd, &strtab_section, sizeof(strtab_section))) {
				goto done;
			}

			/* check that the string table section has the expected type */
			if (SHT_STRTAB != strtab_section.sh_type) {
				goto done;
			}

			/* seek to the symbol table */
			if ((off_t)-1 == lseek(fd, symtab_section.sh_offset, SEEK_SET)) {
				goto done;
			}

			for (symbol_offset = sizeof(symbol); symbol_offset <= symtab_section.sh_size; symbol_offset += sizeof(symbol)) {
				if (sizeof(symbol) != read(fd, &symbol, sizeof(symbol))) {
					goto done;
				}

				/* skip entries that don't describe functions */
#if defined(OMR_ENV_DATA64)
				if (STT_FUNC != ELF64_ST_TYPE(symbol.st_info)) {
					continue;
				}
#else /* defined(OMR_ENV_DATA64) */
				if (STT_FUNC != ELF32_ST_TYPE(symbol.st_info)) {
					continue;
				}
#endif /* defined(OMR_ENV_DATA64) */

				/* skip entries unrelated to the loaded segment */
				if (symbol.st_shndx != loaded_section_index) {
					continue;
				}

				/* skip functions that don't include the offset of interest */
				if ((offset < symbol.st_value) || ((offset - symbol.st_value) >= symbol.st_size)) {
					continue;
				}

				/* we can't answer a name if the symbol doesn't have one */
				if (0 == symbol.st_name) {
					goto done;
				}

				/* check that the name is within the related string table */
				if (symbol.st_name >= strtab_section.sh_size) {
					goto done;
				}

				/* seek to the beginning of the symbol name */
				if ((off_t)-1 == lseek(fd, strtab_section.sh_offset + symbol.st_name, SEEK_SET)) {
					goto done;
				}

				/* read the symbol name */
				if (read(fd, data->name, sizeof(data->name)) <= 0) {
					/* ensure the name is empty to signal failure */
					data->name[0] = '\0';
				} else {
					/* names are NUL-terminated, but may be too long */
					data->name[sizeof(data->name) - 1] = '\0';
					data->offset = offset - symbol.st_value;
				}
				goto done;
			}
		}
	}

done:
	close(fd);
}

static int
elf_ph_handler(struct dl_phdr_info *info, size_t size, void *arg)
{
	struct ElfSymbolData *data = (struct ElfSymbolData *)arg;
	uintptr_t address = data->offset;
	const char *fileName = info->dlpi_name;
	ElfW(Half) ph_index = 0;

	if ('\0' == *fileName) {
		/* an empty string refers to the main program */
		fileName = "/proc/self/exe";
	} else if ('/' != *fileName) {
		/*
		 * We need an absolute path so we can open the file to read
		 * the symbol table. Skip this group of program headers.
		 */
		return 0;
	}

	for (ph_index = 0; ph_index < info->dlpi_phnum; ++ph_index) {
		const ElfW(Phdr) *ph = &info->dlpi_phdr[ph_index];

		if (PT_LOAD == ph->p_type) {
			uintptr_t load_start = info->dlpi_addr + ph->p_vaddr;

			if ((address >= load_start) && ((address - load_start) < ph->p_filesz)) {
				/* translate to an offset in the loaded file */
				data->offset = address - load_start + ph->p_offset;
				elf_find_symbol(fileName, data);
				/* a non-zero return value tells dl_iterate_phdr() to stop iterating */
				return 1;
			}
		}
	}

	return 0;
}

/* This function constructs a backtrace from a CPU context. Generally there are only one or two
 * values in the context that are actually used to construct the stack but these vary by platform
 * so aren't detailed here. If no heap is specified then this function will use malloc to allocate
 * the memory necessary for the stack frame structures which must be freed by the caller.
 *
 * @param portLbirary a pointer to an initialized port library
 * @param threadInfo the thread structure we want  to attach the backtrace to. Must not be NULL.
 * @param heap a heap from which to allocate any necessary memory. If NULL malloc is used instead.
 * @param signalInfo a platform signal context. If not null the context held in threadInfo is replaced
 *  	  with the signal context before the backtrace is generated.
 *
 * @return the number of frames in the backtrace.
 */
uintptr_t
omrintrospect_backtrace_thread_raw(struct OMRPortLibrary *portLibrary, J9PlatformThread *threadInfo, J9Heap *heap, void *signalInfo)
{
	void *addresses[50];
	J9PlatformStackFrame **nextFrame = NULL;
	J9PlatformStackFrame *junkFrames = NULL;
	J9PlatformStackFrame *prevFrame = NULL;
	OMRUnixSignalInfo *sigInfo = (OMRUnixSignalInfo *)signalInfo;
	uintptr_t i = 0;
	int discard = 0;
	uintptr_t ret = 0;
	const char *regName = "";
	void **faultingAddress = NULL;

	if ((NULL == threadInfo) || ((NULL == threadInfo->context) && (NULL == sigInfo))) {
		return 0;
	}

	/* if we've been passed a port library wrapped signal, then extract info from there */
	if (NULL != sigInfo) {
		threadInfo->context = sigInfo->platformSignalInfo.context;

		/* get the faulting address so we can discard frames that are part of the signal handling */
		infoForControl(portLibrary, sigInfo, 0, &regName, (void **)&faultingAddress);
	}

	ret = backtrace_sigprotect(portLibrary, threadInfo, addresses, sizeof(addresses) / sizeof(addresses[0]));

	nextFrame = &threadInfo->callstack;
	for (i = 0; i < ret; i++) {
		J9PlatformStackFrame *currentFrame = NULL;
		if (NULL != heap) {
			currentFrame = portLibrary->heap_allocate(portLibrary, heap, sizeof(*currentFrame));
		} else {
			currentFrame = portLibrary->mem_allocate_memory(portLibrary, sizeof(*currentFrame), OMR_GET_CALLSITE(), OMRMEM_CATEGORY_PORT_LIBRARY);
		}

		if (NULL == currentFrame) {
			if (0 == threadInfo->error) {
				threadInfo->error = ALLOCATION_FAILURE;
			}
			break;
		}

		memset(currentFrame, 0, sizeof(*currentFrame));
		currentFrame->instruction_pointer = (uintptr_t)addresses[i];

		*nextFrame = currentFrame;
		nextFrame = &currentFrame->parent_frame;

		/* check to see if we should truncate the stack trace to omit handler frames */
		if ((NULL != prevFrame) && (NULL != faultingAddress) && (addresses[i] == *faultingAddress)) {
			/* discard all frames up to this point */
			junkFrames = threadInfo->callstack;
			threadInfo->callstack = prevFrame->parent_frame;

			/* break the link so we can free the junk */
			prevFrame->parent_frame = NULL;

			/* correct the next frame */
			nextFrame = &threadInfo->callstack->parent_frame;
			/* record how many frames we discard */
			discard = i + 1;
		}

		/* save the frame we've just set up so that we can blank it if necessary */
		if (NULL == prevFrame) {
			prevFrame = threadInfo->callstack;
		} else {
			prevFrame = prevFrame->parent_frame;
		}
	}

	/* if we discarded any frames then free them */
	while (NULL != junkFrames) {
		J9PlatformStackFrame *tmp = junkFrames;
		junkFrames = tmp->parent_frame;

		if (NULL != heap) {
			portLibrary->heap_free(portLibrary, heap, tmp);
		} else {
			portLibrary->mem_free_memory(portLibrary, tmp);
		}
	}

	return i - discard;
}

/* This function takes a thread structure already populated with a backtrace by omrintrospect_backtrace_thread
 * and looks up the symbols for the frames. The format of the string generated is:
 * 		symbol_name (statement_id instruction_pointer [module+offset])
 * If it isn't possible to determine any of the items in the string then they are omitted. If no heap is specified
 * then this function will use malloc to allocate the memory necessary for the symbols which must be freed by the caller.
 *
 * @param portLbirary a pointer to an initialized port library
 * @param threadInfo a thread structure populated with a backtrace
 * @param heap a heap from which to allocate any necessary memory. If NULL malloc is used instead.
 * @param options controls how much effort is expended trying to resolve symbols
 *
 * @return the number of frames for which a symbol was constructed.
 */
uintptr_t
omrintrospect_backtrace_symbols_raw(struct OMRPortLibrary *portLibrary, J9PlatformThread *threadInfo, J9Heap *heap, uint32_t options)
{
	J9PlatformStackFrame *frame = threadInfo->callstack;
	struct ElfSymbolData data;
	uintptr_t frame_count = 0;

	for (; NULL != frame; frame = frame->parent_frame) {
		char output_buf[512];
		char *cursor = output_buf;
		char *cursor_end = cursor + sizeof(output_buf) - 1; /* leave room NUL-terminator */
		Dl_info dlInfo;
		uintptr_t length = 0;
		uintptr_t iar = frame->instruction_pointer;
		uintptr_t module_offset = 0;
		const char *symbol_name = "";
		const char *module_name = "<unknown>";

		memset(&dlInfo, 0, sizeof(dlInfo));

		/* do the symbol resolution while we're here */
		if (dladdr((void *)iar, &dlInfo)) {
			if (NULL != dlInfo.dli_fname) {
				/* strip off the full path if possible */
				module_name = strrchr(dlInfo.dli_fname, '/');
				if (NULL != module_name) {
					module_name += 1;
				} else {
					module_name = dlInfo.dli_fname;
				}
			}

			if (NULL != dlInfo.dli_fbase) {
				/* set the module offset */
				module_offset = iar - (uintptr_t)dlInfo.dli_fbase;
			}

			symbol_name = dlInfo.dli_sname;
			if (NULL != symbol_name) {
				data.offset = iar - (uintptr_t)dlInfo.dli_saddr;
			} else {
				memset(&data, 0, sizeof(data));
				if (OMR_ARE_NO_BITS_SET(options, OMR_BACKTRACE_SYMBOLS_BASIC)) {
					data.offset = iar;
					dl_iterate_phdr(elf_ph_handler, &data);
				}
				symbol_name = data.name;
			}
		}

		/* symbol_name+offset (instruction_pointer [module+offset]) */
		if ('\0' != *symbol_name) {
			cursor += omrstr_printf(portLibrary, cursor, cursor_end - cursor,
					"%s+0x%x", symbol_name, data.offset);
		}
		cursor += omrstr_printf(portLibrary, cursor, cursor_end - cursor,
				" (0x%p", frame->instruction_pointer);
		if ('\0' != *module_name) {
			cursor += omrstr_printf(portLibrary, cursor, cursor_end - cursor,
					" [%s+0x%x]", module_name, module_offset);
		}
		cursor += omrstr_printf(portLibrary, cursor, cursor_end - cursor,
				")", frame->instruction_pointer);
		*cursor = '\0';

		length = (cursor - output_buf) + 1;
		if (NULL != heap) {
			frame->symbol = portLibrary->heap_allocate(portLibrary, heap, length);
		} else {
			frame->symbol = portLibrary->mem_allocate_memory(portLibrary, length, OMR_GET_CALLSITE(), OMRMEM_CATEGORY_PORT_LIBRARY);
		}

		if (NULL != frame->symbol) {
			strncpy(frame->symbol, output_buf, length);
			frame_count += 1;
		} else {
			frame->symbol = NULL;
			if (0 == threadInfo->error) {
				threadInfo->error = ALLOCATION_FAILURE;
			}
		}
	}

	return frame_count;
}

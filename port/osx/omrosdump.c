/*******************************************************************************
 * Copyright (c) 2021, 2022 IBM Corp. and others
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
 * @brief User space system dump for OSX
 */

#include <errno.h>
#include <fcntl.h>
#include <mach/mach.h>
#include <mach/mach_vm.h>
#include <mach/machine.h>
#include <mach/thread_status.h>
#include <mach/vm_region.h>
#include <mach-o/loader.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/proc.h>
#include <sys/sysctl.h>
#include <sys/syslimits.h>
#include <sys/types.h>
#include <unistd.h>

#include "omrport.h"
#include "omrportpriv.h"
#include "omrosdump_helpers.h"

/* current thread flavors needed for OSX debugging */
struct thread_command_full_64 {
	uint32_t cmd;
	uint32_t cmdsize;
#if defined(OMR_ARCH_AARCH64)
	arm_thread_state64_t thread_state;
	arm_neon_state64_t float_state;
	arm_exception_state64_t exceptions;
#elif defined(OMR_ARCH_X86) /* defined(OMR_ARCH_AARCH64) */
	x86_thread_state_t thread_state;
	x86_float_state_t float_state;
	x86_exception_state_t exceptions;
#else /* defined(OMR_ARCH_AARCH64) */
#error Unsupported processor
#endif /* defined(OMR_ARCH_AARCH64) */
};

#if defined(OMR_ARCH_AARCH64)
#define OSDUMP_THREAD_STATE ARM_THREAD_STATE64
#define OSDUMP_THREAD_STATE_COUNT ARM_THREAD_STATE64_COUNT
#define OSDUMP_FLOAT_STATE ARM_NEON_STATE64
#define OSDUMP_FLOAT_STATE_COUNT ARM_NEON_STATE64_COUNT
#define OSDUMP_EXCEPTION_STATE ARM_EXCEPTION_STATE64
#define OSDUMP_EXCEPTION_STATE_COUNT ARM_EXCEPTION_STATE64_COUNT
#elif defined(OMR_ARCH_X86) /* defined(OMR_ARCH_AARCH64) */
#define OSDUMP_THREAD_STATE x86_THREAD_STATE
#define OSDUMP_THREAD_STATE_COUNT x86_THREAD_STATE_COUNT
#define OSDUMP_FLOAT_STATE x86_FLOAT_STATE
#define OSDUMP_FLOAT_STATE_COUNT x86_FLOAT_STATE_COUNT
#define OSDUMP_EXCEPTION_STATE x86_EXCEPTION_STATE
#define OSDUMP_EXCEPTION_STATE_COUNT x86_EXCEPTION_STATE_COUNT
#else /* defined(OMR_ARCH_AARCH64) */
#error Unsupported processor
#endif /* defined(OMR_ARCH_AARCH64) */

#define SEG_DATA_BUFFER_SIZE 65536
static uint8_t seg_data_buffer[SEG_DATA_BUFFER_SIZE];

static char corefile_name[PATH_MAX];
static int corefile_fd = -1;

static kern_return_t coredump_to_file(mach_port_t, pid_t);
static kern_return_t list_thread_commands(mach_port_t, struct thread_command_full_64 **, natural_t *);
static kern_return_t list_segment_commands(mach_port_t, struct segment_command_64 **, natural_t *);

static kern_return_t
coredump_to_file(mach_port_t task_port, pid_t pid)
{
	kern_return_t kr = KERN_SUCCESS;
	struct mach_header_64 mh64;
	struct segment_command_64 *segments = NULL;
	struct thread_command_full_64 *threads = NULL;
	natural_t segment_count = 0;
	natural_t thread_count = 0;
	natural_t cpu_count = 0;
	processor_basic_info_t proc_info_array;
	mach_msg_type_number_t info_count = 0;
	off_t file_off = 0;
	off_t seg_file_off = 0;
	ssize_t written = 0;
	natural_t i = 0;

	corefile_fd = open(corefile_name, O_RDWR | O_CREAT | O_EXCL, 0600);

	if (-1 == corefile_fd) {
		perror("open()");
		kr = KERN_FAILURE;
		goto done;
	}

	kr = host_processor_info(mach_host_self(), PROCESSOR_BASIC_INFO, &cpu_count,
		(processor_info_array_t *)&proc_info_array, &info_count);
	if (KERN_SUCCESS != kr) {
		mach_error("failed to get processor info:\n", kr);
		goto done;
	}

	mh64.magic = MH_MAGIC_64;
	/* host_processor_info does not return a value with the 64-bit flag set so
	 * set manually since we only deal with 64-bit on OSX
	 */
	mh64.cputype = proc_info_array[0].cpu_type | CPU_ARCH_ABI64;
	mh64.cpusubtype = proc_info_array[0].cpu_subtype;
	/* update ncmds and sizeofcmds as we process thread and segment commands
	 * and set initial values for them
	 */
	mh64.ncmds = 0;
	mh64.sizeofcmds = 0;
	mh64.filetype = MH_CORE;
	file_off += sizeof(struct mach_header_64);

	kr = list_thread_commands(task_port, &threads, &thread_count);
	if (KERN_SUCCESS != kr) {
		mach_error("error getting thread command data:\n", kr);
		goto done;
	}

	for (i = 0; i < thread_count; i++) {
		/* write each portion separately due to structure padding */
		written = pwrite(corefile_fd, &threads[i].cmd, sizeof(uint32_t), file_off);
		if (written < 0) {
			perror("pwrite() error writing threads:");
			kr = KERN_FAILURE;
			goto done;
		}
		file_off += sizeof(uint32_t);
		written = pwrite(corefile_fd, &threads[i].cmdsize, sizeof(uint32_t), file_off);
		if (written < 0) {
			perror("pwrite() error writing threads:");
			kr = KERN_FAILURE;
			goto done;
		}
		file_off += sizeof(uint32_t);
		written = pwrite(corefile_fd, &threads[i].thread_state, sizeof(threads[i].thread_state), file_off);
		if (written < 0) {
			perror("pwrite() error writing threads:");
			kr = KERN_FAILURE;
			goto done;
		}
		file_off += sizeof(threads[i].thread_state);
		written = pwrite(corefile_fd, &threads[i].float_state, sizeof(threads[i].float_state), file_off);
		if (written < 0) {
			perror("pwrite() error writing threads:");
			kr = KERN_FAILURE;
			goto done;
		}
		file_off += sizeof(threads[i].float_state);
		written = pwrite(corefile_fd, &threads[i].exceptions, sizeof(threads[i].exceptions), file_off);
		if (written < 0) {
			perror("pwrite() error writing threads:");
			kr = KERN_FAILURE;
			goto done;
		}
		file_off += sizeof(threads[i].exceptions);
		mh64.sizeofcmds += threads[i].cmdsize;
	}
	mh64.ncmds += thread_count;

	kr = list_segment_commands(task_port, &segments, &segment_count);
	if (KERN_SUCCESS != kr) {
		mach_error("error getting segment command data:\n", kr);
		goto done;
	}

	seg_file_off = file_off + segment_count * sizeof(struct segment_command_64);
	for (i = 0; i < segment_count; i++) {
		mach_vm_address_t data_read = (mach_vm_address_t)seg_data_buffer;
		mach_vm_size_t data_size = 0;
		mach_vm_size_t bytes_read = 0;

		for (; bytes_read < segments[i].vmsize; bytes_read += data_size) {
			mach_vm_size_t to_read = segments[i].vmsize - bytes_read;
			if (to_read > SEG_DATA_BUFFER_SIZE) {
				to_read = SEG_DATA_BUFFER_SIZE;
			}
			data_size = to_read;
			kr = mach_vm_read_overwrite(task_port, segments[i].vmaddr + bytes_read, to_read, data_read, &data_size);
			/* any memory segment that is unreadable will be added to the core as an empty segment */
			if (KERN_SUCCESS != kr) {
				break;
			}
			if (to_read != data_size) {
				break;
			} else {
				written = pwrite(corefile_fd, (void *)data_read, data_size, seg_file_off + bytes_read);
			}
			if (written < 0) {
				perror("pwrite() error writing segment data:");
				kr = KERN_FAILURE;
				goto done;
			}
		}

		segments[i].fileoff = seg_file_off;
		written = pwrite(corefile_fd, &segments[i], sizeof(struct segment_command_64), file_off);
		if (written < 0) {
			perror("pwrite() error writing segment commands:");
			kr = KERN_FAILURE;
			goto done;
		}

		seg_file_off += segments[i].vmsize;
		file_off += sizeof(struct segment_command_64);

		kr = KERN_SUCCESS; /* reset kr since a failure from mach_vm_read is not fatal */
	}

	mh64.ncmds += segment_count;
	mh64.sizeofcmds += segment_count * sizeof(struct segment_command_64);

	/* write mach header after all command number and size are known */
	written = pwrite(corefile_fd, &mh64, sizeof(struct mach_header_64), 0);
	if (written < 0) {
		perror("pwrite() error writing mach header:");
		kr = KERN_FAILURE;
		goto done;
	}

done:
	if (corefile_fd > 0) {
		close(corefile_fd);
		if (KERN_SUCCESS != kr) {
			remove(corefile_name);
		}
	}
	return kr;
}

static kern_return_t
list_thread_commands(mach_port_t task_port, struct thread_command_full_64 **thread_commands, natural_t *thread_count)
{
	kern_return_t kr = KERN_SUCCESS;
	thread_act_array_t thread_info;
	struct thread_command_full_64 *threads = NULL;
	natural_t i = 0;

	kr = task_threads(task_port, &thread_info, thread_count);

	if (KERN_SUCCESS != kr) {
		mach_error("task_thread failed with: ", kr);
		return kr;
	}

	threads = calloc(*thread_count, sizeof(struct thread_command_full_64));
	if (NULL == threads) {
		kr = KERN_NO_SPACE;
		goto done;
	}

	for (i = 0; i < *thread_count; i++) {
		uint32_t state_int_count = 0;
		threads[i].cmd = LC_THREAD;
		threads[i].cmdsize = sizeof(threads[i].cmd) + sizeof(threads[i].cmdsize);
		state_int_count = OSDUMP_THREAD_STATE_COUNT;
		kr = thread_get_state(thread_info[i], OSDUMP_THREAD_STATE,
			(thread_state_t)&threads[i].thread_state, &state_int_count);
		if (KERN_SUCCESS != kr) {
			goto done;
		}
		threads[i].cmdsize += state_int_count * sizeof(natural_t);
		state_int_count = OSDUMP_FLOAT_STATE_COUNT;
		kr = thread_get_state(thread_info[i], OSDUMP_FLOAT_STATE,
			(thread_state_t)&threads[i].float_state, &state_int_count);
		if (KERN_SUCCESS != kr) {
			goto done;
		}
		threads[i].cmdsize += state_int_count * sizeof(natural_t);
		state_int_count = OSDUMP_EXCEPTION_STATE_COUNT;
		kr = thread_get_state(thread_info[i], OSDUMP_EXCEPTION_STATE,
			(thread_state_t)&threads[i].exceptions, &state_int_count);
		if (KERN_SUCCESS != kr) {
			goto done;
		}
		threads[i].cmdsize += state_int_count * sizeof(natural_t);
	}

done:
	if (KERN_SUCCESS == kr) {
		*thread_commands = threads;
	} else {
		if (NULL != threads) {
			free(threads);
		}
	}
	for (i = 0; i < *thread_count; i++) {
		mach_port_deallocate(mach_task_self(), thread_info[i]);
	}
	mach_vm_deallocate(mach_task_self(), (mach_vm_address_t)thread_info, (*thread_count) * sizeof(thread_act_t));
	return kr;
}

static kern_return_t
list_segment_commands(mach_port_t task_port, struct segment_command_64 **segment_commands, natural_t *segment_count)
{
	kern_return_t kr = KERN_SUCCESS;
	struct segment_command_64 *segments = NULL;
	struct segment_command_64 *tmp_segments = NULL;
	vm_address_t address = 0;
	uint32_t depth = 0;
	vm_region_submap_info_data_64_t vm_info;
	natural_t segment_array_size = 128;

	*segment_count = 0;
	segments = calloc(segment_array_size, sizeof(struct segment_command_64));
	if (NULL == segments) {
		kr = KERN_NO_SPACE;
		goto done;
	}

	while (1) {
		vm_size_t size = 0;
		vm_address_t old_addr = address;
		mach_msg_type_number_t info_count = VM_REGION_SUBMAP_INFO_COUNT_64;

		if (*segment_count >= segment_array_size) {
			segment_array_size += segment_array_size / 2;
			tmp_segments = calloc(segment_array_size, sizeof(struct segment_command_64));
			if (NULL == tmp_segments) {
				kr = KERN_NO_SPACE;
				goto done;
			}
			memcpy(tmp_segments, segments, *segment_count * sizeof(struct segment_command_64));
			free(segments);
			segments = tmp_segments;
			tmp_segments = NULL;
		}

		kr = vm_region_recurse_64(task_port, &address, &size, &depth, (vm_region_info_t)&vm_info, &info_count);

		if (KERN_SUCCESS != kr) { /* end of memory segments or actual error */
			break;
		}

		if (vm_info.is_submap) {
			depth += 1;
			continue;
		}

		/* if we get the same region as previous, update previous region instead of creating a new one */
		if ((*segment_count > 0) && (old_addr == (address + segments[*segment_count - 1].vmsize))) {
			*segment_count -= 1;
			if (size < segments[*segment_count].vmsize) {
				size = segments[*segment_count].vmsize;
			}
		}

		segments[*segment_count].cmd        = LC_SEGMENT_64;
		segments[*segment_count].cmdsize    = sizeof(struct segment_command_64);
		segments[*segment_count].segname[0] = 0;
		segments[*segment_count].vmaddr     = address;
		segments[*segment_count].vmsize     = size;
		segments[*segment_count].fileoff    = 0; /* get correct offset while writing to file */
		segments[*segment_count].filesize   = size;
		segments[*segment_count].maxprot    = vm_info.max_protection;
		segments[*segment_count].initprot   = vm_info.protection;
		segments[*segment_count].nsects     = 0;

		*segment_count += 1;
		address += size;
		if (address <= old_addr) {
			address = old_addr + 1; /* so we don't endlessly loop on one region */
		}
	}
	if (KERN_INVALID_ADDRESS == kr) { /* from reaching end of memory in vm_region_recurse_64 */
		kr = KERN_SUCCESS;
	}

done:
	if (KERN_SUCCESS == kr) {
		tmp_segments = calloc(*segment_count, sizeof(struct segment_command_64));
		if (NULL == tmp_segments) {
			kr = KERN_NO_SPACE;
		} else {
			memcpy(tmp_segments, segments, *segment_count * sizeof(struct segment_command_64));
			*segment_commands = tmp_segments;
		}
		free(segments);
	} else {
		if (NULL != segments) {
			free(segments);
		}
		if (NULL != tmp_segments) {
			free(tmp_segments);
		}
	}
	return kr;
}


/**
 * Create a dump file of the OS state.
 *
 * @param[in] portLibrary The port library.
 * @param[in] filename Buffer for filename optionally containing the filename where dump is to be output.
 * @param[out] filename filename used for dump file or error message.
 * @param[in] dumpType Type of dump to perform. Unused on OSX
 * @param[in] userData Implementation specific data. Unused on OSX
 *
 * @return 0 on success, non-zero otherwise.
 *
 * @note filename buffer can not be NULL.
 * @note user allocates and frees filename buffer.
 * @note filename buffer length is platform dependent, assumed to be EsMaxPath/MAX_PATH
 *
 * @note if filename buffer is empty, a filename will be generated.
 * @note if J9UNIQUE_DUMPS is set, filename will be unique.
 */
uintptr_t
omrdump_create(struct OMRPortLibrary *portLibrary, char *filename, char *dumpType, void *userData)
{
	pid_t parent_pid = getpid();
	pid_t child_pid = 0;
	char *lastSep = NULL;
	kern_return_t kr = KERN_SUCCESS;
	mach_port_t pass_port = MACH_PORT_NULL;
	mach_port_t special_port = MACH_PORT_NULL;

	/* filename cannot be null */
	if (NULL == filename) {
		return 1;
	}
	/* set default file name if none given */
	if ('\0' == filename[0]) {
		snprintf(filename, PATH_MAX, "core.%u", parent_pid);
	}
	lastSep = strrchr(filename, DIR_SEPARATOR);
	if (NULL != lastSep) {
		strncpy(corefile_name, lastSep + 1, PATH_MAX);
	} else {
		strncpy(corefile_name, filename, PATH_MAX);
	}

	/* save the original special port so we can restore it after using it to pass the task port */
	kr = task_get_bootstrap_port(mach_task_self(), &special_port);
	if (KERN_SUCCESS != kr) {
		mach_error("failed get special port:\n", kr);
		goto done;
	}
	pass_port = mach_task_self();
	/* pass parent task port to child through special port inheritance */
	kr = task_set_bootstrap_port(mach_task_self(), pass_port);
	if (KERN_SUCCESS != kr) {
		mach_error("failed set special port:\n", kr);
		goto done;
	}

	child_pid = fork();
	if (0 == child_pid) { /* in child process */
		kr = task_get_bootstrap_port(mach_task_self(), &pass_port);
		if (KERN_SUCCESS != kr) {
			mach_error("failed get special port:\n", kr);
		} else {
			/* Move to specified folder before dumping */
			if (NULL != lastSep) {
				/* keep separator for cases such as when the path is '/' */
				lastSep[1] = '\0';
				if (0 != chdir(filename)) {
					perror("failed to change directories, attempting to create dump in current dir");
				}
			}
			kr = coredump_to_file(pass_port, parent_pid);
		}
		raise(SIGKILL); /* kill child process without running any exit procedures */
	} else if (child_pid < 0) { /* fork failed */
		perror("forking for core dump failed");
	} else { /* in parent process */
		waitpid(child_pid, NULL, 0);
		/* restore special port to original value */
		kr = task_set_bootstrap_port(mach_task_self(), special_port);
		if (KERN_SUCCESS != kr) {
			mach_error("failed set special port:\n", kr);
			goto done;
		}
	} /* 0 == child_pid */

done:
	if (KERN_SUCCESS == kr) {
		/* check if the core file has been created in the right directory */
		if (0 == access(filename, F_OK)) {
			return 0;
		}
		/* check for corefile in current working directory */
		if ((NULL != lastSep) && (0 == access(corefile_name, F_OK))) {
			return 0;
		}
	}
	return 1;
}

int32_t
omrdump_startup(struct OMRPortLibrary *portLibrary)
{
	return 0;
}

void
omrdump_shutdown(struct OMRPortLibrary *portLibrary)
{
}

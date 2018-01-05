/*******************************************************************************
 * Copyright (c) 2014, 2017 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at http://eclipse.org/legal/epl-2.0
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#ifndef ELFFILEGENERATOR_HPP
#define ELFFILEGENERATOR_HPP

#pragma once

#if defined(LINUX)

#include <elf.h>
#include <vector>
#include <string>
#include "env/TypedAllocator.hpp"
#include "env/RawAllocator.hpp"
#include "codegen/StaticRelocation.hpp"
#include "runtime/CodeCacheManager.hpp"
#include "codegen/ELFRelocationResolver.hpp"

class TR_Memory;

namespace TR {

/**
 * @class ELFObjectFileGenerator
 * @brief The ELFObjectFileGenerator class provides the ability to create an ELF object file from a section of generated code.
 */

class ELFObjectFileGenerator
   {
public:

   /**
    * @brief Constructs member fields and initializes the ELF file header.
    * @param rawAllocator Used for the memore allocations of internal containers.
    * @param codeStart Base address of the code area to be written out.
    * @param codeSize Size of the code area to be written out.
    * @param fileName Name of the object file to be written out.
    */
   ELFObjectFileGenerator(TR::RawAllocator rawAllocator, uint8_t const * codeStart, size_t codeSize, const char * fileName);

   ~ELFObjectFileGenerator() throw();

   /**
    * @brief registerCompiledMethod Registers a symbol for a method contained within the code area.
    * @param sig The exported name of the compiled method.
    * @param startPC The address of the desired entry point for functions linking to the compiled method.
    * @param codeSize The size of the compiled method in bytes.
    */
   void registerCompiledMethod(const char *sig, uint8_t *startPC, uint32_t codeSize);

   /**
    * @brief registerStaticRelocation Registers a relocation requirement against a location in the exported code area.
    * @param relocation An object containing the description of the requirements of the relocation.
    */
   void registerStaticRelocation(const TR::StaticRelocation &relocation);

   /**
    * @brief emitObjectFile Generates the section headers required and writes the completed object file to disk.
    */
   void emitObjectFile();

private:

#if defined(TR_HOST_64BIT)
   typedef Elf64_Ehdr ELFHeader;
   typedef Elf64_Shdr ELFSectionHeader;
   typedef Elf64_Phdr ELFProgramHeader;
   typedef Elf64_Addr ELFAddress;
   typedef Elf64_Sym ELFSymbol;
   typedef Elf64_Rela ELFRela;
   typedef Elf64_Off ELFOffset;
#define ELF_ST_INFO(bind, type) ELF64_ST_INFO(bind, type)
#define ELFClass ELFCLASS64;
#else
   typedef Elf32_Ehdr ELFHeader;
   typedef Elf32_Shdr ELFSectionHeader;
   typedef Elf32_Phdr ELFProgramHeader;
   typedef Elf32_Addr ELFAddress;
   typedef Elf32_Sym ELFSymbol;
   typedef Elf32_Rela ELFRela;
   typedef Elf32_Off ELFOffset;
#define ELF_ST_INFO(bind, type) ELF32_ST_INFO(bind, type)
#define ELFClass ELFCLASS32;
#endif

   struct ELFTrailer
      {
      ELFSectionHeader zeroSection;
      ELFSectionHeader textSection;
      ELFSectionHeader relaSection;
      ELFSectionHeader dynsymSection;
      ELFSectionHeader shstrtabSection;
      ELFSectionHeader dynstrSection;

      char zeroSectionName[1];
      char shstrtabSectionName[10];
      char textSectionName[6];
      char relaSectionName[11];
      char dynsymSectionName[8];
      char dynstrSectionName[8];

      // start of a variable sized region: an ELFSymbol structure per symbol + total size of elf symbol names
      ELFSymbol symbols[1];

      // followed by variable sized symbol names located only by computed offset

      // followed by rela entries located only by computed offset
      };

   // structure used to track regions of code cache that will become symbols
   struct ELFObjectFileSymbol
      {
      ELFObjectFileSymbol(const char *name, size_t nameLength, uint8_t *start, uint32_t size) :
         _name(name), _nameLength(nameLength), _start(start), _size(size) {}
      const char *_name;
      size_t _nameLength;
      uint8_t *_start;
      uint32_t _size;
      };

   struct ELFObjectFileRelocation
      {
      ELFObjectFileRelocation(uint8_t *location, uint32_t type, uint32_t symbol) :
         _location(location), _type(type), _symbol(symbol) {}
      uint8_t *_location;
      uint32_t _type;
      uint32_t _symbol;
      };

   void initializeELFHeader();
   void initializeELFHeaderForPlatform();
   void initializeELFTrailer();
   size_t numELFRelocations();
   size_t numELFSymbols();

   TR::RawAllocator _rawAllocator;
   uint8_t const * const _codeStart;
   size_t const _codeSize;
   size_t const _fileNameLength;
   char * const _fileNameChars;
   ELFHeader _elfHeader;
   ELFTrailer * _elfTrailer;
   size_t _elfTrailerSize;

   typedef TR::typed_allocator< ELFObjectFileSymbol, TR::RawAllocator > SymbolContainerAllocator;
   typedef std::vector< ELFObjectFileSymbol, SymbolContainerAllocator > SymbolContainer;
   SymbolContainer _symbols;

   typedef TR::typed_allocator< ELFObjectFileRelocation, TR::RawAllocator > RelocationContainerAllocator;
   typedef std::vector< ELFObjectFileRelocation, SymbolContainerAllocator > RelocationContainer;
   RelocationContainer _relocations;

   size_t _totalELFSymbolNamesLength;
   TR::ELFRelocationResolver _resolver;
   };

}

#endif /* defined(LINUX) */

#endif // ELFFILEGENERATOR_HPP

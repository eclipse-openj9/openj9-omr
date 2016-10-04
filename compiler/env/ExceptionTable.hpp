/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2000, 2016
 *
 *  This program and the accompanying materials are made available
 *  under the terms of the Eclipse Public License v1.0 and
 *  Apache License v2.0 which accompanies this distribution.
 *
 *      The Eclipse Public License is available at
 *      http://www.eclipse.org/legal/epl-v10.html
 *
 *      The Apache License v2.0 is available at
 *      http://www.opensource.org/licenses/apache2.0.php
 *
 * Contributors:
 *    Multiple authors (IBM Corp.) - initial implementation and documentation
 ******************************************************************************/

#ifndef EXCEPTIONTABLE_INCL
#define EXCEPTIONTABLE_INCL

#include "infra/Array.hpp"

#include <stdint.h>          // for uint32_t, uint16_t, int32_t
#include "env/TRMemory.hpp"  // for TR_Memory, etc
#include "env/jittypes.h"    // for TR_ByteCodeInfo
#include "infra/List.hpp"    // for List (ptr only), ListIterator

class TR_ResolvedMethod;
namespace TR { class Block; }
namespace TR { class Compilation; }
template <class T> class TR_Array;

struct TR_PCMapEntry
   {
   TR_ALLOC(TR_Memory::PCMapEntry)

   uint16_t _instructionStartPC;
   uint16_t _instructionEndPC;
   uint16_t _bytecodeStartPC;
   uint16_t _bytecodeEndPC;
   };

struct TR_ExceptionTableEntry
   {
   TR_ALLOC(TR_Memory::ExceptionTableEntry)

   TR_ResolvedMethod *   _method;
   uint32_t              _instructionStartPC;
   uint32_t              _instructionEndPC;
   uint32_t              _instructionHandlerPC;
   uint32_t              _catchType;
   TR_ByteCodeInfo       _byteCodeInfo;
   };

struct TR_ExceptionTableEntryIterator
   {
   TR_ALLOC(TR_Memory::ExceptionTableEntryIterator)

   TR_ExceptionTableEntryIterator(TR::Compilation *comp);

   TR_ExceptionTableEntry * getFirst();
   TR_ExceptionTableEntry * getNext();

   uint32_t                 size();
private:
   TR_ExceptionTableEntry * getCurrent();
   void                     addSnippetRanges(List<TR_ExceptionTableEntry> &, TR::Block *, TR::Block *, uint32_t, TR_ResolvedMethod *, TR::Compilation *);

   TR::Compilation *                          _compilation;
   TR_Array<List<TR_ExceptionTableEntry> > * _tableEntries;
   ListIterator<TR_ExceptionTableEntry>      _entryIterator;
   int32_t                                   _inlineDepth;
   uint32_t                                  _handlerIndex;
   };

#endif

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
 *******************************************************************************/

#ifndef LIVEREFERENCE_INCL
#define LIVEREFERENCE_INCL

#include <stddef.h>          // for NULL
#include <stdint.h>          // for uint32_t
#include "env/TRMemory.hpp"  // for TR_Memory, etc
#include "infra/List.hpp"    // for List

namespace TR { class Node; }

class TR_LiveReference
   {

private:

   TR::Node *       _referenceNode;
   List<TR::Node>   _parents;
   uint32_t         _numberOfParents;
   bool             _needSpillTemp;

public:

   TR_ALLOC(TR_Memory::LiveReference)

   TR_LiveReference(TR_Memory * m)
      : _referenceNode(NULL),
        _numberOfParents(0),
        _parents(m),
        _needSpillTemp(false)
      {}

   TR_LiveReference(TR::Node *rn, TR::Node *parent, TR_Memory * m)
      : _referenceNode(rn),
        _numberOfParents(0),
        _parents(m),
        _needSpillTemp(false)
      {
      addParentToList(parent);
      }

   List<TR::Node>& getParentList() { return _parents; }

   void addParentToList(TR::Node *parent)
      {
      ++_numberOfParents;
      _parents.add(parent);
      }

   uint32_t getNumberOfParents() { return _numberOfParents; }

   TR::Node *getReferenceNode()  { return _referenceNode; }

   bool needSpillTemp()          { return _needSpillTemp; }
   void setNeedSpillTemp(bool b) { _needSpillTemp = b; }

   };

#endif

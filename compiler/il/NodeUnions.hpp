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

#ifndef NODEUNIONS_INCL
#define NODEUNIONS_INCL

#include <stddef.h>          // for size_t
#include "il/NodeUtils.hpp"  // for CaseInfo, GlobalRegisterInfo, etc

/*
 * This file defines the unions picked up by TR::Node
 */

struct UnionedWithChildren
   {
   /*
    * To avoid bloating TR_Node, this struct should declare no
    * members outside the below union.
    *
    * Be very careful when adding members as well.
    * It must be ensured that whatever added is exclusive of not only
    * the members in this union, but also the members of the
    * TR::Node child union
    */

   union
      {
#ifdef SUPPORT_DFP
      // Used to hold long double constatants
      uint32_t                _constData[4];
#endif

      GlobalRegisterInfo                _globalRegisterInfo;

      // Offset to WCode literal pool for constants > 8 bytes
      size_t                  _offset;

      ChildUnionMembers::RelocationInfo _relocationInfo;
      ChildUnionMembers::CaseInfo       _caseInfo;
      ChildUnionMembers::MonitorInfo    _monitorInfo;
      };

   };
#endif


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

#ifndef TR_DELIMITER_INCL
#define TR_DELIMITER_INCL

#include <string.h>            // for NULL, strlen
#include "infra/Assert.hpp"    // for TR_ASSERT

namespace TR { class Compilation; }

namespace TR
{

class Delimiter
   {
   public:

   Delimiter(
         TR::Compilation * comp,
         bool trace,
         char * tag,
         char * comment0 = NULL,
         char * comment1 = NULL,
         char * comment2 = NULL) :
      _tag(tag),
      _comp(comp),
      _trace(trace)
      {
      TR_ASSERT(strlen(tag) < tagsize-1, "tag is too long");

      if (_trace)
         {
         if (!comment0)
            traceMsg(_comp,"<%s>\n",_tag);
         else
            {
            if (!comment1)
               traceMsg(_comp, "<%s %s>\n", _tag, comment0);
            else
               {
               traceMsg(_comp, "<%s\n", _tag);
               traceMsg(_comp, "\t%s\n", comment0);
               traceMsg(_comp, "\t%s", comment1);
               if (comment2)
                  traceMsg(_comp, "\n\t%s>\n", comment2);
               else
                  traceMsg(_comp, ">\n");
               }
            }
         }
      }

   ~Delimiter()
      {
      if (_trace)
         traceMsg(_comp,"</%s>\n",_tag);
      }

   protected:

   static const int tagsize = 32;

   // pin an address is good enough
   // char _buffer[tagsize];
   char * _tag;

   TR::Compilation *_comp;

   bool _trace;
   };

}

#endif

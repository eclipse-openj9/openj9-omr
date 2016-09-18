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

#include <stdint.h>        // for uint64_t, etc
#include "env/jittypes.h"  // for uintptrj_t

#ifdef TR_BIGEND_TO_LITEND
// byte order big to little long (8 bytes)
inline uint64_t bol(uint64_t arg)
   {
   char *x = (char *) &arg;
   for (int i = 0; i < 4; ++i)
      {
      char y = x[i];
      x[i] = x[7 - i];
      x[7 - i] = y;
      }
   return arg;
   }

// byte order big to little int (4 bytes)
inline uint32_t boi(uint32_t arg)
   {
   char *x = (char *) &arg;
   for (int i = 0; i < 2; ++i)
      {
      char y = x[i];
      x[i] = x[3 - i];
      x[3 - i] = y;
      }
   return arg;
   }

// byte order big to little address (4 or 8 bytes)
inline intptrj_t boa(intptrj_t arg)
   {
   if (sizeof(intptrj_t) == 4)
      return boi(arg);
   else
      return bol(arg);
   }

// byte order big to little short (2 bytes)
inline uint16_t bos(uint16_t arg)
   {
   char *x = (char *) &arg;
   int i = 0;
   char y = x[i];
   x[i] = x[1];
   x[1] = y;
   return arg;
   }
#else
inline uint64_t bol(uint64_t arg)   { return arg; }
inline uint32_t boi(uint32_t arg)   { return arg; }
inline intptrj_t boa(intptrj_t arg) { return arg; }
inline uint16_t bos(uint16_t arg)   { return arg; }
#endif

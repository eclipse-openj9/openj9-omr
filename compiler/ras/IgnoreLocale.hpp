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

#include <ctype.h>   // for tolower
#include <stddef.h>  // for size_t

// These functions should be used anywhere the locale should not influence how characters are compared
// e.g. option names are ascii strings that should not be compared differently in different locales

int tolower_ignore_locale(int c);
int toupper_ignore_locale(int c);
int stricmp_ignore_locale(const char *s1, const char *s2);
int strnicmp_ignore_locale(const char *s1, const char *s2, size_t n);

#if (J9ZOS390 || AIXPPC || LINUX || OSX)
#include <strings.h>
   #define STRICMP strcasecmp
   #define STRNICMP strncasecmp
#elif defined(WINDOWS)
   #define STRICMP _stricmp
   #define STRNICMP _strnicmp
#elif PILOT
   #define STRICMP StrCaselessCompare
   #define STRNICMP StrNCaselessCompare
#else
   inline int STRICMP(char *p1, char *p2)
   {
      while (1)
      {
         char c1 = *p1++, c2 = *p2++;
         int diff = tolower(c1) - tolower(c2);
         if (diff)
            return diff;
         if (!c1)
            break;
      }
      return 0;
   }
   inline int STRNICMP(char *p1, char *p2, int len)
   {
      while ((len--) > 0)
      {
         char c1 = *p1++, c2 = *p2++;
         int diff = tolower(c1) - tolower(c2);
         if (diff)
            return diff;
         if (!c1)
            break;
      }
      return 0;
   }
#endif

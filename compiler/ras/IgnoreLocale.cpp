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

#include "ras/IgnoreLocale.hpp"

#include <ctype.h>               // for tolower, toupper
#include <stddef.h>              // for NULL, size_t
#include "codegen/FrontEnd.hpp"  // for feGetEnv

#define   LOCALE_ENV_VAR   "TR_ProcessOptionsWithLocale"
#define   LOCALE_DEF   static const char *ignoreLocaleOption=feGetEnv(LOCALE_ENV_VAR)
#define USE_LOCALE   (ignoreLocaleOption != NULL)

// tolower that does not respect the locale: for ascii only
int
tolower_ignore_locale(int c)
   {
   LOCALE_DEF;
   if (USE_LOCALE)
      return tolower(c);

   return ((c >= 'A') && (c <= 'Z')) ? (c - 'A' + 'a') : c;
   }

// toupper that does not respect the locale: for ascii only
int
toupper_ignore_locale(int c)
   {
   LOCALE_DEF;
   if (USE_LOCALE)
      return toupper(c);

   return ((c >= 'a') && (c <= 'z')) ? (c - 'a' + 'A') : c;
   }

// stricmp that does not respect the locale: for ascii comparisons only
int
stricmp_ignore_locale(const char *s1, const char *s2)
   {
   LOCALE_DEF;
   if (USE_LOCALE)
      return STRICMP(s1, s2);

   while (1)
      {
      char c1 = *s1++;
      char c2 = *s2++;
      int diff = tolower_ignore_locale(c1) - tolower_ignore_locale(c2);
      if (diff != 0)
         {
         return diff;
         }
      else
         {
         if('\0' == c1)
            {
            return 0;
            }
         }
      }
      return 0;
   }


// strnicmp that does not respect the locale: for ascii comparisons only
int
strnicmp_ignore_locale(const char *s1, const char *s2, size_t length)
   {
   LOCALE_DEF;
   if (USE_LOCALE)
      return STRNICMP(s1, s2, length);

   while (length-- > 0)
      {
      char c1 = *s1++;
      char c2 = *s2++;
      int diff = tolower_ignore_locale(c1) - tolower_ignore_locale(c2);
      if (diff != 0)
         {
         return diff;
         }
      else
         {
         if('\0' == c1)
            {
            return 0;
            }
         }
      }
      return 0;
   }

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

#ifndef SIMPLEREGEX_INCL
#define SIMPLEREGEX_INCL

#include <stddef.h>          // for size_t
#include <stdint.h>          // for uint64_t, uint32_t
#include "env/TRMemory.hpp"  // for TR_Memory, etc
#include "il/DataTypes.hpp"  // for TR_YesNoMaybe

class TR_ResolvedMethod;

#define BITSPERUL 32
#if BITSPERUL == 32
#define BWORD(x)        ((x) >> 5)
#define BBIT(x)         ((x) & 0x1f)
#else
#define BWORD(x)        ((x) / (CHAR_BIT * sizeof(unsigned long int)))
#define BBIT(x)         ((x) % (CHAR_BIT * sizeof(unsigned long int)))
#endif

namespace TR
{

// Simple regular expression
//
class SimpleRegex
   {
   public:
   TR_ALLOC(TR_Memory::SimpleRegex)

   // Create a new regular expression
   //
   static SimpleRegex *create(char *&s);

   // Check whether a string matches this regular expression
   //
   bool match(const char *s, bool isCaseSensitive=true, bool useLocale=true);

   static bool match(TR::SimpleRegex *regex, const char *, bool isCaseSensitive=true);
   static bool match(TR::SimpleRegex *regex, int, bool isCaseSensitive=true);
   static bool match(TR::SimpleRegex *regex, TR_ResolvedMethod *, bool isCaseSensitive=true);
   static bool matchIgnoringLocale(TR::SimpleRegex *regex, const char *, bool isCaseSensitive=true);

   void print(bool negate);

   enum ComponentType{simple_string, wildcards, char_alternatives};

   struct Component
      {
      TR_ALLOC(TR_Memory::SimpleRegexComponent)
      void *operator new (size_t size, PERSISTENT_NEW_DECLARE, size_t numChars);

      ComponentType type;
      union Data {
         char     str[1]; /* really N */
         uint64_t bit_map[BWORD(256)];
         uint64_t counts;
         } data;
      };

   struct Simple
      {
      TR_ALLOC(TR_Memory::SimpleRegexSimple)
      bool match(const char *s, bool caseSensitive, bool useLocale);
      bool matchesRemainder(const char *s, bool caseSensitive, bool useLocale);
      void print();
      Component* component;
      Simple*    remainder;
      uint32_t   fixed_chars_right;
      };

   struct Regex
      {
      TR_ALLOC(TR_Memory::SimpleRegexRegex)
      bool match(const char *s, bool caseSensitive, bool useLocale);
      void print();
      Simple *simple;
      Regex  *remainder;
      };

   private:

   static Regex  *processRegex(char *&s, bool &foundError);
   static Simple *processSimple(char *&s, TR_YesNoMaybe allowAlternates, bool &foundError);

   Regex *_regex;
   bool   _negate;
   };

}

#endif

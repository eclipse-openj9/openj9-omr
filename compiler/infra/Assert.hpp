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

/**
 * \file Assert.hpp
 *
 * Defines a number of assert macros.
 *
 * \def TR_ASSERT_FATAL         an assertion that will always take down the VM.
 *                              Intended for the most egregious of errors.
 *
 * \def TR_ASSERT_SAFE_FATAL    an assertion that will take down the VM, unless
 *                              the non-fatal assert option is set (intended for
 *                              service purposes). The presumption is that the
 *                              user of this assert has ensured that the code
 *                              subsequent to the assert is safe to continue
 *                              executing.
 *
 *                              In production builds, only the line number and
 *                              file name are available to conserve DLL size.
 *
 * \def TR_ASSERT               a macro that defines an assertion which is compiled out
 *                              (including format strings) during production builds
 *
 * We also provide Expect and Ensure, based on the developing C++ Core guidelines [1].
 *
 * \def Expect                  Precondition macro, only defined for DEBUG and
 *                              EXPECT_BUILDs.
 *
 * \def Ensure                  Postcondition macro, only defined for DEBUG and
 *                              EXPECT_BUILDs.
 *
 *
 * \note While not standardized, this header relies on compiler behaviour that
 *       will trim the trailing comma from a variadic macro when provided no
 *       variadic arguments, to avoid TR_ASSERT(cond, "message") from
 *       complaining about a trailing comma.
 *
 * [1]: https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Ri-expects
 */


#ifndef TR_ASSERT_HPP
#define TR_ASSERT_HPP

#include "infra/Annotations.hpp"                // OMR_NORETURN
#include "compile/CompilationException.hpp"
namespace TR
   {
   // Don't use these directly.
   //
   // Use the TR_ASSERT* macros instead as they control the string
   // contents in production builds.
   void OMR_NORETURN fatal_assertion(const char *file, int line, const char *condition, const char *format, ...);

   // Non fatal assertions may in some circumstances return, so do not mark them as
   // no-return.
   void              assertion(const char *file, int line, const char *condition, const char *format, ...);

   /**
    * Assertion failure exception type.
    *
    * Thrown only by TR_ASSERT_SAFE_FATAL, when softFailOnAssumes is set.
    */
   struct AssertionFailure : public virtual CompilationException
      {
      virtual const char* what() const throw() { return "Assertion Failure"; }
      };


   }



// Macro Definitions

#define TR_ASSERT_FATAL(condition, format, ...) \
         do { (condition) ? (void)0 : TR::fatal_assertion(__FILE__, __LINE__, #condition, format, ##__VA_ARGS__); } while(0)

#if defined(DEBUG) || defined(PROD_WITH_ASSUMES)

   #define TR_ASSERT(condition, format, ...) \
         do { (condition) ? (void)0 : TR::assertion(__FILE__, __LINE__, #condition, format, ##__VA_ARGS__); } while(0)

   #define TR_ASSERT_SAFE_FATAL(condition, format, ...) \
         do { (condition) ? (void)0 : TR::assertion(__FILE__, __LINE__, #condition, format, ##__VA_ARGS__); } while(0)

#else

   #define TR_ASSERT(condition, format, ...) (void)0

   #define TR_ASSERT_SAFE_FATAL(condition, format, ...) \
         do { (condition) ? (void)0 : TR::assertion(__FILE__, __LINE__, NULL, NULL); } while(0)


#endif



#if defined(DEBUG) || defined(EXPECT_BUILD)
   #define Expect(x) TR_ASSERT((x), "Expectation Failure:")
   #define Ensure(x) TR_ASSERT((x), "Ensure Failure:")
#else
   #define Expect(x) (void)0
   #define Ensure(x) (void)0
#endif



#endif

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

#include "infra/Assert.hpp"

#include <assert.h>                            // for assert
#include <stdarg.h>                            // for va_list
#include <stdio.h>                             // for fprintf, stderr, etc
#include <stdlib.h>                            // for exit
#include "codegen/FrontEnd.hpp"                // for TR_FrontEnd, isJ9
#include "compile/Compilation.hpp"             // for Compilation, comp
#include "compile/ResolvedMethod.hpp"          // for TR_ResolvedMethod
#include "control/Options.hpp"
#include "control/Options_inlines.hpp"         // for TR::Options, etc
#include "control/Recompilation.hpp"           // for TR_Recompilation
#include "env/CompilerEnv.hpp"
#include "il/symbol/ResolvedMethodSymbol.hpp"  // for ResolvedMethodSymbol
#include "infra/Annotations.hpp"               // for OMR_NORETURN
#include "ras/Debug.hpp"                       // for TR_Debug
#include "stdarg.h"                            // for va_end, va_copy, etc

#ifdef J9_PROJECT_SPECIFIC
#include "env/VMJ9.h"
#endif

static void assumeDontCallMeDirectlyImpl( const char * file, int32_t line, const char *condition, const char *s, va_list ap, bool fatal)
   {
   TR::Compilation *comp = TR::comp();

   // if the ignoreAssert option is set, then we ignore failing assert
   if (comp && comp->getOption(TR_IgnoreAssert) && !fatal)
      return;

   if (!condition) condition = "";

#ifdef J9_PROJECT_SPECIFIC
   if (comp && comp->fej9()->traceIsEnabled())
      comp->fej9()->traceAssumeFailure(line, file);
#endif

   // Fatal assertions fire always, however, non fatal assertions
   // can be disabled via TR_SoftFailOnAssume.
   if (comp && comp->getOption(TR_SoftFailOnAssume) && !fatal)
      throw TR::AssertionFailure();

   // Default is to always print to screen
   bool printOnscreen = true;

   if (printOnscreen)
      {
      fprintf(stderr, "Assertion failed at %s:%d: %s\n", file, line, condition);

      if (comp && TR::isJ9())
        fprintf(stderr, "%s\n", TR::Compiler->debug.extraAssertMessage(comp));

      if (s)
         {
         fprintf(stderr, "\t");
         va_list copy;
         va_copy(copy, ap);
         vfprintf(stderr, s, copy);
         va_end(copy);
         fprintf(stderr, "\n");
         }

      if (comp)
         {
         const char *methodName =
               comp->signature();
         const char *hotness = comp->getHotnessName();
         bool profiling = false;
         if (comp->getRecompilationInfo() && comp->getRecompilationInfo()->isProfilingCompilation())
            profiling = true;

         fprintf(stderr, "compiling %s at level: %s%s\n", methodName, hotness, profiling?" (profiling)":"");
         }

      TR_Debug::printStackBacktrace();

      fprintf(stderr, "\n");

      fflush(stderr);
      }

   // this diagnostic call adds useful info to log file and flushes it, if we're tracing the current method
   if (comp)
      {
      comp->diagnosticImpl("Assertion failed at %s:%d:%s", file, line, condition);
      if (s)
         {
         comp->diagnosticImpl(":\n");
         va_list copy;
         va_copy(copy, ap);
         comp->diagnosticImplVA(s, copy);
         va_end(copy);
         }
      comp->diagnosticImpl("\n");
      }

   char *feGetEnv(const char *);
   static char * noDebug = feGetEnv("TR_NoDebuggerBreakPoint");
   if (noDebug)
      exit(1337);
   else
      {
      static char *crashLogOnAssume = feGetEnv("TR_crashLogOnAssume");
      if (crashLogOnAssume)
         {
         //FIXME: this doesn't work on z/OS
         *(volatile int*)(0) = 0; // let crashlog do its thing
         }

#ifdef _MSC_VER
      static char *revertToDebugbreakWin = feGetEnv("TR_revertToDebugbreakWin");
#ifdef DEBUG
      DebugBreak();
#else
      if(revertToDebugbreakWin)
         DebugBreak();
      else
         *(int*)(NULL) = 1;
#endif
#else // of _MSC_VER

      assert(0);

#endif // ifdef _MSC_VER
      }
   }

namespace TR
   {
   void assertion(const char *file, int line, const char *condition, const char *format, ...)
      {
      va_list ap;
      va_start(ap, format);
      assumeDontCallMeDirectlyImpl( file, line, condition, format, ap, false);
      va_end(ap);
      }

   void OMR_NORETURN fatal_assertion(const char *file, int line, const char *condition, const char *format, ...)
      {
      va_list ap;
      va_start(ap, format);
      assumeDontCallMeDirectlyImpl( file, line, condition, format, ap, true);
      va_end(ap);
      }

   }

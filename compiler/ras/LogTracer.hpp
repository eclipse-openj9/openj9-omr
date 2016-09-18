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

#ifndef TRLOGTRACER
#define TRLOGTRACER

#include <stdint.h>  // for uint8_t

namespace TR { class Compilation; }
namespace TR { class Optimization; }

#define heuristicTrace(r, ...) \
   if (true) { \
      if ((r)->heuristicLevel()) { (r)->alwaysTraceM(__VA_ARGS__); } \
   } else \
      (void)0


#define debugTrace(r, ...) \
      if (true) { \
         if ((r)->debugLevel())\
            {\
            (r)->alwaysTraceM(__VA_ARGS__);\
            }\
      } else \
         (void)0

#define alwaysTrace(r, ...) \
      (r)->alwaysTraceM(__VA_ARGS__);

class TR_LogTracer
   {
public:
   TR_LogTracer(TR::Compilation *comp, TR::Optimization *opt);

   TR::Compilation * comp()                   { return _comp; }

   // determine the tracing level

   void setTraceLevelToDebug()                     { _traceLevel = trace_debug;  }
   bool debugLevel()                              { return _traceLevel == trace_debug; }
   bool heuristicLevel()                          { return _traceLevel >= trace_heuristic; }      // the > ensures heuristic tracing gets turned on for debug as well

   // trace statements for specific tracing levels
   void alwaysTraceM (const char *fmt, ...);                      // NEVER call this method directly. Use macros defined above.

protected:

   enum traceLevel
       {
       trace_notrace,
       trace_full,            //traceFull option used in commandline
       trace_heuristic,       //traceInlining option used in commandline
       trace_debug            //debugInlining option used in commandline
       };

   TR::Compilation *        _comp;
   uint8_t                 _traceLevel;

   };

#endif

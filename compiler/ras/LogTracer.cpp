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

#include "ras/LogTracer.hpp"

#include <stdarg.h>                            // for va_list
#include "compile/Compilation.hpp"             // for Compilation
#include "optimizer/Optimization.hpp"          // for Optimization
#include "optimizer/Optimization_inlines.hpp"
#include "ras/Debug.hpp"                       // for TR_DebugBase

TR_LogTracer::TR_LogTracer(TR::Compilation *comp, TR::Optimization *opt) : _comp(comp), _traceLevel(trace_notrace)
   {

   if(opt)
      {
      if(opt->trace())
         _traceLevel = trace_heuristic;
      else if (comp->getDebug())
         _traceLevel = trace_full;
      }
   // Class should be extended to take advantage of trace_debug, or maybe more changes to tracing infrastructure

   }

//This method should never be called directly.  Should be called through Macros defined at top of LogTracer.hpp
void TR_LogTracer::alwaysTraceM ( const char * fmt, ...)
   {
   if( !comp()->getDebug() )
      return;
   va_list args;
   va_start(args,fmt);

   char buffer[2056];

   const char *str = comp()->getDebug()->formattedString(buffer,sizeof(buffer)/sizeof(buffer[0]),fmt,args);

   va_end(args);

   //traceMsg(comp(), "%s\n",str);
   comp()->getDebug()->traceLnFromLogTracer(str);

   return;
   }

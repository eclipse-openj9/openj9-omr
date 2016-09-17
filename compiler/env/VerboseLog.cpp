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

#include "env/VerboseLog.hpp"
#include "infra/Assert.hpp"

void *TR_VerboseLog::_config = 0;

const char * TR_VerboseLog::_vlogTable[] =
   {
   0,
   "#INFO:  ",
   "#SAMPLE:  ",
   "#IPROFILER:  ",
   "#CODECACHE:  ",
   "#JITSTATE:  ",
   "#PERF:  ",
   "#MEMORY:  ",
   "#FAILURE:  ",
   "#JITDUMP:  ",
   "#SCHINTS:  ",
   "#GCCYCLE:  ",
   "#J2I:  ",
   "#CR:  ",
   "#RA:  ",
   "#HK:  ",
   "#HD:  ",
   "#MH:   ", // extra space to line up with MHd
   "#MHd:  ",
   "#DLT:  ",
   "+ ",      // TR_Vlog_COMP
   " ",       // TR_Vlog_COMPSTART
   "! ",      // TR_Vlog_COMPFAIL
   "-precompileMethod ",
   " ",       // TR_Vlog_MMAP
   "#OSR:  ", // extra space to line up with OSRd
   "#OSRd: ",
   "#HWP: ",
   "#INL: ",
   "#GC: ",
   "[IBM GPU JIT]: ",  // to be consistent with JCL GPU output
   "#PATCH : ",
   "#DISPATCH: ",
   "#RECLAMATION: ",
   };

void TR_VerboseLog::writeLine(TR_VlogTag tag, const char *format, ...)
   {
   TR_ASSERT(tag != TR_Vlog_null, "TR_Vlog_null is not a valid Vlog tag");
   va_list args;
   va_start(args, format);
   write("\n");
   writeTimeStamp();
   write(_vlogTable[tag]);
   vwrite(format,args);
   va_end(args);
   }

void TR_VerboseLog::writeLineLocked(TR_VlogTag tag, const char *format, ...)
   {
   TR_ASSERT(tag != TR_Vlog_null, "TR_Vlog_null is not a valid Vlog tag");
   vlogAcquire();
   va_list args;
   va_start(args, format);
   write("\n");
   writeTimeStamp();
   write(_vlogTable[tag]);
   vwrite(format,args);
   va_end(args);
   vlogRelease();
   }

void TR_VerboseLog::write(const char *format, ...)
   {
   va_list args;
   va_start(args, format);
   vwrite(format,args);
   va_end(args);
   }

void TR_VerboseLog::initialize(void *config)
   {
   _config = config;
   }

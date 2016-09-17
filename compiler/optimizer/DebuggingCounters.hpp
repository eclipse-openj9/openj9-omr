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

#ifndef DEBUGGING_COUNTERS_INCL
#define DEBUGGING_COUNTERS_INCL

#include <stdint.h>          // for int32_t, etc
#include <stdio.h>           // for FILE
#include "env/TRMemory.hpp"  // for TR_Memory, etc
#include "env/jittypes.h"    // for TR_ByteCodeInfo, etc

class TR_Method;
namespace TR { class Compilation; }
namespace TR { class TreeTop; }

// This is the type of the debugging counters.
#define SMALL_COUNTER_TYPE uint32_t
#define LARGE_COUNTER_TYPE uint64_t

/**
 * Simple list to track already instrumented call sites.
 */
struct CountedCallSite
  {
  TR::TreeTop * callTreeTop;
  char name[64];
  bool flag;
  int32_t size;
  int32_t frequency;
  int32_t numLocals;
  TR_OpaqueMethodBlock *opaque_method;
  TR_Method *method;
  TR_ByteCodeInfo bcInfo;
  CountedCallSite * _next;
  };

/**
 *  Structure to hold all data for a given named counter.
 */
struct NamedCounterInfo
  {
  // Named of this counter
  char * counterName;

  // A small counter that is incremented without being corrupted by
  // races (may drop samples)
  SMALL_COUNTER_TYPE smallCount;

  // A large counter that won't overflow.  Updated periodically from
  // the interval count
  LARGE_COUNTER_TYPE totalCount;

  // Counter to track how many different locations in the code (at
  // compile time) increment this counter.
  SMALL_COUNTER_TYPE compilationCount;

  // Linked list of infos
  NamedCounterInfo * _next;

  int32_t delta;

  int32_t bucketSize;
  };

/**
 *  Class to contain functionality specific to debugging counters.
 *  For now, all methods and data are currently static.  Eventually
 *  the data could probably become instance-specific to support reentrant
 *  compilation.
 */
class TR_DebuggingCounters
  {
  public:
  TR_ALLOC(TR_Memory::TR_DebuggingCounters)
  static void initializeCompilation();
  static void transferSmallCountsToTotalCounts();
  static void report();

  private:
  // Linked list of counter infos
  static NamedCounterInfo * namedCounterInfos;

  // Methods
  static NamedCounterInfo* getOrCreateNamedCounter(TR::Compilation *comp, const char * name, int32_t d, int32_t bucketSize);
  static void insertCounter(const char * name, TR::Compilation * comp, TR::TreeTop * tt, int32_t d);
  static bool insertIfMissing(TR::Compilation * comp, const char *name, bool flag, TR::TreeTop * tt, int32_t size = 0, int32_t numLocals=0);

  /*
   *  The routines below are specific to inlining counters, but use
   *  the generic debugging counters above as the implementation
   *
   *  This could be done through inheritance, but it doesn't seem worth it at this point.
   */
  public:
  static void insertInliningCounter(const char * name, TR::Compilation * comp, TR::TreeTop * tt, int32_t size = 0, int32_t delta = 0, int32_t numLocals=0);
  static void insertInliningCounterOK(const char * name, TR::Compilation * comp, TR::TreeTop *origTT, TR::TreeTop * tt, int32_t size = 0, int32_t numLocals=0);
  static void inliningReportForMethod(TR::Compilation *comp);

  private:
  static CountedCallSite * countedCallSiteList;
  static FILE *output;
  };

#endif

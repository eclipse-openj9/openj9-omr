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

#ifndef OMR_OPTIONS_INLINE_INCL
#define OMR_OPTIONS_INLINE_INCL

#include "control/Options.hpp"
#include "control/Options_inlines.hpp"   // for Option

/*
 * Performance sensitive implementations are included here
 * to support inlining.
 *
 * Every function in this file needs to be marked as inline
 * or else there will be multiple copies.
 */
inline TR::Options *
OMR::Options::self()
   {
   return static_cast<TR::Options*>(this);
   }

inline const  TR::Options *
OMR::Options::self() const
   {
   return static_cast<const TR::Options*>(this);
   }

inline bool
OMR::Options::getOption(uint32_t mask)
   {
   return self()->getAnyOption(mask);
   }

inline bool
OMR::Options::dynamicDebugCounterIsEnabled(const char *name, int8_t fidelity)
   {
   return self()->counterIsEnabled(name, fidelity, _enabledDynamicCounterNames);
   }

inline bool
OMR::Options::staticDebugCounterIsEnabled(const char *name, int8_t fidelity)
   {
   return self()->counterIsEnabled(name, fidelity, _enabledStaticCounterNames);
   }

inline bool
OMR::Options::debugCounterHistogramIsEnabled(const char *name, int8_t fidelity)
   {
   return self()->counterIsEnabled(name, fidelity, _counterHistogramNames);
   }


inline int32_t
OMR::Options::getInlinerArgumentHeuristicFraction() const
   {
   return self()->getOptLevel() > warm ?
        _inlinerArgumentHeuristicFractionBeyondWarm
      : _inlinerArgumentHeuristicFractionUpToWarm;
   }

inline int32_t
OMR::Options::getNumLimitedGRARegsWithheld()
      {
      int32_t regsToWithhold = TR_MAX_AVAIL_LIMITED_GRA_REGS - self()->getMaxLimitedGRARegs();
      if (regsToWithhold < 0)
         return 0;
      else
         return regsToWithhold;
      }

inline bool
OMR::Options::needWriteBarriers()
   {
   return (self()->gcIsUsingConcurrentMark() || _gcMode == TR_WrtbarOldCheck);
   }


inline bool
OMR::Options::getTraceRAOption(uint32_t mask)
   {
   return self()->getRegisterAssignmentTraceOption(mask);
   }


inline bool
OMR::Options::isDisabledForAllMethods (OMR::Optimizations o)
   {
   return TR::Options::checkDisableFlagForAllMethods(o, false);
   }

inline bool
OMR::Options::isDisabledForAnyMethod (OMR::Optimizations o)
   {
   return TR::Options::checkDisableFlagForAllMethods(o, true);
   }

inline bool
OMR::Options::isAnyVerboseOptionSet(TR_VerboseFlags op1)
   {
   return TR::Options::getVerboseOption(op1);
   }

inline bool
OMR::Options::isAnyVerboseOptionSet(TR_VerboseFlags op1, TR_VerboseFlags op2)
   {
   return TR::Options::getVerboseOption(op1) || TR::Options::getVerboseOption(op2);
   }

inline bool
OMR::Options::isAnyVerboseOptionSet(TR_VerboseFlags op1, TR_VerboseFlags op2, TR_VerboseFlags op3)
   {
   return TR::Options::isAnyVerboseOptionSet(op1,op3) || TR::Options::getVerboseOption(op2);
   }

inline bool
OMR::Options::isAnyVerboseOptionSet(TR_VerboseFlags op1, TR_VerboseFlags op2, TR_VerboseFlags op3, TR_VerboseFlags op4)
   {
   return TR::Options::isAnyVerboseOptionSet(op1,op3) || TR::Options::isAnyVerboseOptionSet(op2,op4);
   }

inline bool
OMR::Options::isAnyVerboseOptionSet(TR_VerboseFlags op1, TR_VerboseFlags op2, TR_VerboseFlags op3, TR_VerboseFlags op4, TR_VerboseFlags op5)
   {
   return TR::Options::isAnyVerboseOptionSet(op1,op3,op5) || TR::Options::isAnyVerboseOptionSet(op2,op4);
   }

inline bool
OMR::Options::isAnyVerboseOptionSet(TR_VerboseFlags op1, TR_VerboseFlags op2, TR_VerboseFlags op3, TR_VerboseFlags op4, TR_VerboseFlags op5, TR_VerboseFlags op6)
   {
   return TR::Options::isAnyVerboseOptionSet(op1,op3,op5) || TR::Options::isAnyVerboseOptionSet(op2,op4,op6);
   }

inline void
OMR::Options::setCanJITCompile(bool f)
   {
   _canJITCompile = f;
   if (!f) TR::Options::disableSamplingThread();
   }

inline TR_Debug *
OMR::Options::findOrCreateDebug()
   {
   if (!_debug) TR::Options::createDebug();
   return _debug;
   }

#endif

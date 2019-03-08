#include "compile/Compilation.hpp"
#include "env/VerboseLog.hpp"
#include "optimizer/SelectInliner.hpp"
#include "optimizer/Inliner.hpp"

int32_t
OMR::SelectInliner::perform() {
   if (this->comp()->getOption(TR_EnableBenefitInliner))
   {
      return this->performBenefitInliner();
   }

   if (this->comp()->getMethodHotness() >= warm)
   {
      return this->performMultiTargetInliner();
   }

   return this->performTrivialInliner();
}

int32_t
OMR::SelectInliner::performTrivialInliner()
   {
   TR_TrivialInliner *inliner = (TR_TrivialInliner *)TR_TrivialInliner::create(this->_manager);
   return inliner->perform();
   }

int32_t
OMR::SelectInliner::performMultiTargetInliner()
   {
   TR_Inliner *inliner = (TR_Inliner *)TR_Inliner::create(this->_manager);
   return inliner->perform();
   }

int32_t
OMR::SelectInliner::performBenefitInliner()
   {
   TR_VerboseLog::vlogAcquire();
   TR_VerboseLog::writeLine(TR_Vlog_SIP, "Hello world");
   TR_VerboseLog::vlogRelease();
   return 0;
   }

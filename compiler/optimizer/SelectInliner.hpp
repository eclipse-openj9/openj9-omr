#ifndef SELECT_INLINER_INC
#define SELECT_INLINER_INC
#pragma once

#include "optimizer/Optimization.hpp"
#include "optimizer/OptimizationManager.hpp"

namespace OMR {
   class SelectInliner : public TR::Optimization {
      public:
         SelectInliner(TR::OptimizationManager *m) : TR::Optimization(m) {};
         static TR::Optimization *create(TR::OptimizationManager *m) {
            return new (m->allocator()) SelectInliner(m);
         };
         virtual int32_t perform();
         virtual const char * optDetailString() const throw()
         {
            return "O^O Select Inliner : ";
         }

        int32_t performTrivialInliner();
        int32_t performMultiTargetInliner();
        int32_t performBenefitInliner();
   };
};

#endif

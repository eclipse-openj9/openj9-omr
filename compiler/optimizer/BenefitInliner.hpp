#ifndef TR_BENEFIT_INLINER
#define TR_BENEFIT_INLINER
#pragma once

#include "infra/Cfg.hpp"
#include "optimizer/Inliner.hpp"
#include "optimizer/J9Inliner.hpp"
#include "optimizer/Optimization.hpp"
#include "optimizer/OptimizationManager.hpp"
#include "IDT.hpp"



namespace OMR {

   class BenefitInlinerWrapper : public TR::Optimization
   {
   public:
      BenefitInlinerWrapper(TR::OptimizationManager *manager) : TR::Optimization(manager) {};
      static TR::Optimization *create(TR::OptimizationManager *manager)
         {
            return new (manager->allocator()) BenefitInlinerWrapper(manager);
         }

      virtual int32_t perform();
      virtual const char * optDetailString() const throw()
         {
            return "O^O Beneft Inliner Wrapper: ";
         }
      int32_t getBudget(TR::ResolvedMethodSymbol *resolvedMethodSymbol);
   };


class AbsEnvInlinerUtil;
class BenefitInlinerBase: public TR_InlinerBase 
   {
   protected:
      virtual bool supportsMultipleTargetInlining () { return true; }
      BenefitInlinerBase(TR::Optimizer *optimizer, TR::Optimization *optimization);
   public:
      virtual void applyPolicyToTargets(TR_CallStack *, TR_CallSite *, TR::Block *block=NULL);
      int _callerIndex;
      int _nodes;
   private:
      AbsEnvInlinerUtil *_util2;
      void setAbsEnvUtil(AbsEnvInlinerUtil *u) { this->_util2 = u; }
      AbsEnvInlinerUtil *getAbsEnvUtil() { return this->_util2; }
   };


class BenefitInliner: public BenefitInlinerBase 
   {
   public:
      BenefitInliner(TR::Optimizer *, TR::Optimization *, uint32_t);
      virtual bool inlineCallTargets(TR::ResolvedMethodSymbol *, TR_CallStack *, TR_InnerPreexistenceInfo *)
         {
         return false;
         }
      void initIDT(TR::ResolvedMethodSymbol *root);
      void obtainIDT(TR::ResolvedMethodSymbol *resolvedMethodSymbol, int32_t budget);
      void obtainIDT(TR_CallSite*, int32_t budget, TR_ByteCodeInfo &info, int cpIndex);
      void obtainIDT(TR::ResolvedMethodSymbol *callerSymbol, TR::Block *block, int32_t budget);
      void traceIDT();
   private:
      TR::Region _callSitesRegion;
      TR::Region _callStacksRegion;
      TR_CallStack *_inliningCallStack;
      IDT *_idt;
      IDTNode *_currentIDTNode = NULL;
      
      const uint32_t _budget;
      inline const uint32_t budget() const;
      TR_CallSite *findCallSiteTarget(TR::ResolvedMethodSymbol *resolvedMethodSymbol, int bcIndex, int cpIndex, TR::MethodSymbol::Kinds kind, TR_ByteCodeInfo &info, TR::Block *block);
      void printTargets(TR::ResolvedMethodSymbol *resolvedMethodSymbol, int bcIndex, int cpIndex, TR::MethodSymbol::Kinds kind);
      void printInterfaceTargets(TR::ResolvedMethodSymbol *resolvedMethodSymbol, int bcIndex, int cpIndex);
      void printVirtualTargets(TR::ResolvedMethodSymbol *resolvedMethodSymbol, int bcIndex, int cpIndex);
      void printStaticTargets(TR::ResolvedMethodSymbol *resolvedMethodSymbol, int bcIndex, int cpIndex);
      void printSpecialTargets(TR::ResolvedMethodSymbol *resolvedMethodSymbol, int bcIndex, int cpIndex);
      TR::SymbolReference* getSymbolReference(TR::ResolvedMethodSymbol *callerSymbol, int cpIndex, TR::MethodSymbol::Kinds kind);
      TR_CallSite * getCallSite(TR::MethodSymbol::Kinds kind,
                                    TR_ResolvedMethod *callerResolvedMethod,
                                    TR::TreeTop *callNodeTreeTop,
                                    TR::Node *parent,
                                    TR::Node *callNode,
                                    TR_Method * interfaceMethod,
                                    TR_OpaqueClassBlock *receiverClass,
                                    int32_t vftSlot,
                                    int32_t cpIndex,
                                    TR_ResolvedMethod *initialCalleeMethod,
                                    TR::ResolvedMethodSymbol * initialCalleeSymbol,
                                    bool isIndirectCall,
                                    bool isInterface,
                                    TR_ByteCodeInfo & bcInfo,
                                    TR::Compilation *comp,
                                    int32_t depth=-1,
                                    bool allConsts=false);
   };

   class ByteCodeCFG : public TR::CFG
   {
   public:

   ByteCodeCFG(TR::Compilation *c, TR::ResolvedMethodSymbol *m) : TR::CFG(c, m) {};

   virtual int getStartBlockFrequency();
   virtual TR::CFGNode *getStartForReverseSnapshot();
   };

   class AbsEnvInlinerUtil : public TR_J9InlinerUtil
   {
      friend class TR_InlinerBase;
      friend class TR_MultipleCallTargetInliner;
      friend class BenefitInliner;
      public:
      AbsEnvInlinerUtil(TR::Compilation *comp);
      void computeMethodBranchProfileInfo2(TR::Block *, TR_CallTarget *, TR::ResolvedMethodSymbol*, int, TR::Block *);
   };
}

#endif

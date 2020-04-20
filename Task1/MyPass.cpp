#include <queue>          //bfs
#include <unordered_set>  //mark visited BBs

#include "llvm/IR/CFG.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

namespace {

class MyPass : public ModulePass {
public:
    static char ID;
    std::unordered_set<void *> st;
    MyPass() : ModulePass(ID) {}

    void visitBasicBlock(BasicBlock &bb) {
        if (st.count(&bb))
            return;
        else
            st.insert(&bb);
        errs() << "      # BasicBlock:" << &bb << '\n';
        errs() << "        > Successors:\n";
        auto termInst = bb.getTerminator();
        int numSucc = termInst->getNumSuccessors();
        for (int i = 0; i < numSucc; ++i) {
            BasicBlock &cur = *termInst->getSuccessor(i);
            errs() << "          :  " << &cur << '\n';
        }
        errs() << "        > Predecessors:\n";
        for (auto it = pred_begin(&bb), ed = pred_end(&bb); it != ed; ++it) {
            BasicBlock *cur = *it;
            errs() << "          :  " << cur << '\n';
        }
        errs() << "        > Instructions:\n";
        for (auto it = bb.begin(); it != bb.end(); ++it) {
            Instruction *ii = &*it;
            errs() << "          :" << *ii << "\n";
        }
    }

    void dfsBasicBlock(BasicBlock &bb) {
        visitBasicBlock(bb);
        auto termInst = bb.getTerminator();
        int numSucc = termInst->getNumSuccessors();
        for (int i = 0; i < numSucc; ++i) {
            BasicBlock &cur = *termInst->getSuccessor(i);
            dfsBasicBlock(cur);
        }
    }

    void dfsFunction(Function &f) {
        errs() << "  + Function(DFS):" << f.getName() << '\n';
        st.clear();
        if (f.empty()) {
            errs() << "    # Empty Function. Skipping.\n";
            return;
        }
        dfsBasicBlock(f.getEntryBlock());
    }
    void bfsFunction(Function &f) {
        errs() << "  + Function(BFS):" << f.getName() << '\n';
        if (f.empty()) {
            errs() << "    # Empty Function. Skipping.\n";
            return;
        }
        st.clear();
        std::queue<BasicBlock *> q;
        q.push(&f.getEntryBlock());
        while (!q.empty()) {
            if (st.count(q.front())) {
                q.pop();
                continue;
            }
            BasicBlock &cur = *q.front();
            q.pop();
            visitBasicBlock(cur);
            auto termInst = cur.getTerminator();
            int numSucc = termInst->getNumSuccessors();
            for (int i = 0; i < numSucc; ++i)
                q.push(termInst->getSuccessor(i));
        }
    }

    virtual bool runOnModule(Module &M) {
        errs() << "Module:" << M.getName() << '\n';
        for (auto iter = M.begin(); iter != M.end(); iter++) {
            Function &F = *iter;
            dfsFunction(F);
            bfsFunction(F);
        }
        return false;
    }
};
}  // namespace

char MyPass::ID = 0;

static void registerMyPass(const PassManagerBuilder &PMB, legacy::PassManagerBase &PM) {
    PM.add(new MyPass());
}

// works with "-O0" or no optimization options
static RegisterStandardPasses RegisterMyPass_OPT0(PassManagerBuilder::EP_EnabledOnOptLevel0, registerMyPass);

// works with "-O1", "-O2", ...
static RegisterStandardPasses RegisterMyPass_OPT(PassManagerBuilder::EP_ModuleOptimizerEarly, registerMyPass);

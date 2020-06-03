#include <queue>          //bfs
#include <unordered_set>  //mark visited BBs
#include <vector>

#include "llvm/IR/CFG.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

#include "SimpleDataDependenceGraph.h"
#include "calcu.h"
#include "apriori.h"
#include "rule.h"

using namespace llvm;
using std::vector ;
using namespace SPT_calc ;

namespace {

class MyPass : public ModulePass {
public:
    static char ID;
    std::unordered_set<void *> st;
    MyPass() : ModulePass(ID) {}

    virtual bool runOnModule(Module &M) {
        vector<SDDG*> sddg_graphs ; // this is needed when build the SupportInfo class

        errs() << "Module:" << M.getName() << '\n';
        for (auto iter = M.begin(); iter != M.end(); iter++) {
            Function &F = *iter;
            errs() << " + Function:" << F.getName() << '\n';
            if(F.empty()) {
                errs() << "   - Empty Function\n";
                continue ;
            }
            miner::SDDG* sddg = new miner::SDDG(&F);
            sddg->buildSDDG();
            sddg->flattenSDDG();
            sddg->dotify() ;
            sddg_graphs.push_back( sddg ) ;
        }
        int minFreqSupport = 10 , maxInfreqSupp = 5 ;
        double minConf = 0.85;
        SupportInfo *spt = new SupportInfo( sddg_graphs , minFreqSupport) ; // Instantiate SI class
        vector <string> sgfqc = spt->get_single_frequency() ; // this func returns a vector<string>
        AprioriAlgorithm solver(sgfqc, minFreqSupport, maxInfreqSupp, spt);
        GenRule generator(solver.getFreqItemSet(), solver.getInfreqItemSet(), minConf, spt);
        delete spt ;
        for( auto u : sddg_graphs ) delete u ;
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

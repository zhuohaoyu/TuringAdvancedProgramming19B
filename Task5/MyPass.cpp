#include <iostream>
#include <queue>          //bfs
#include <unordered_set>  //mark visited BBs
#include <vector>

#include "SimpleDataDependenceGraph.h"
#include "apriori.h"
#include "calcu.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "rule.h"

using namespace llvm;
using namespace std;
using namespace SPT_calc;

static cl::opt<int> MFS("mfs", cl::init(10), cl::desc("Minimum Frequent-itemset Support"));
static cl::opt<int> MIS("mis", cl::init(5), cl::desc("Maximum Infrequent-itemset Support"));
static cl::opt<double> MCF("min_conf", cl::init(0.85), cl::desc("Maximum Infrequent-itemset Support"));

namespace {

class MyPass : public ModulePass {
public:
    static char ID;
    std::unordered_set<void *> st;
    int minFreqSupport = 10, maxInfreqSupp = 5;
    double minConf = 0.85;
    MyPass() : ModulePass(ID) {}
    MyPass(int mfs, int mis, double mcf) : ModulePass(ID), minFreqSupport(mfs), maxInfreqSupp(mis), minConf(mcf / 100.0) {}
    virtual bool runOnModule(Module &M) {
        vector<SDDG *> sddg_graphs;  // this is needed when build the SupportInfo class
        outs() << "Module:" << (M.getName()) << '\n';
        outs().flush();
        for (auto iter = M.begin(); iter != M.end(); iter++) {
            Function &F = *iter;
            outs() << " + Function:" << (F.getName()) << '\n';
            outs().flush();
            if (F.empty()) {
                outs() << "   - Empty Function\n";
                outs().flush();
                continue;
            }
            miner::SDDG *sddg = new miner::SDDG(&F);
            sddg->buildSDDG();
            sddg->flattenSDDG();
            sddg->dotify();
            sddg_graphs.push_back(sddg);
        }
        outs() << '\n';
        outs().flush();
        SupportInfo *spt = new SupportInfo(sddg_graphs, minFreqSupport);  // Instantiate SI class
        vector<string> sgfqc = spt->get_single_frequency();               // this func returns a vector<string>
                                                                          //        for( auto u : sgfqc ) {
                                                                          //            outs() << "sgfqc : " << u  << "\n";
                                                                          //        }
        AprioriAlgorithm solver(sgfqc, minFreqSupport, maxInfreqSupp, spt);
        GenRule generator(solver.getFreqItemSet(), solver.getInfreqItemSet(), minConf, spt);

        DetectRule(generator, spt);

        delete spt;
        for (auto u : sddg_graphs) delete u;
        return false;
    }
};
}  // namespace

char MyPass::ID = 0;

static void registerMyPass(const PassManagerBuilder &PMB, legacy::PassManagerBase &PM) {
    PM.add(new MyPass(MFS, MIS, MCF));
}

// works with "-O0" or no optimization options
static RegisterStandardPasses RegisterMyPass_OPT0(PassManagerBuilder::EP_EnabledOnOptLevel0, registerMyPass);

// works with "-O1", "-O2", ...
static RegisterStandardPasses RegisterMyPass_OPT(PassManagerBuilder::EP_ModuleOptimizerEarly, registerMyPass);

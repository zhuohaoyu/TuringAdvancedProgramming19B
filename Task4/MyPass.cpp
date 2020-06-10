#include <queue>          //bfs
#include <unordered_set>  //mark visited BBs
#include <vector>
#include <iostream>

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
using namespace std;
using namespace SPT_calc ;

namespace {

class MyPass : public ModulePass {
public:
    static char ID;
    std::unordered_set<void *> st;
    MyPass() : ModulePass(ID) {}

    virtual bool runOnModule(Module &M) {
        vector<SDDG*> sddg_graphs ; // this is needed when build the SupportInfo class

        outs() << "Module:" << (M.getName()) << '\n';
        for (auto iter = M.begin(); iter != M.end(); iter++) {
            Function &F = *iter;
            outs() << " + Function:" << (F.getName()) << '\n';
            if(F.empty()) {
                outs() << "   - Empty Function\n";
                continue ;
            }
            miner::SDDG* sddg = new miner::SDDG(&F);
            sddg->buildSDDG();
            sddg->flattenSDDG();
            sddg->dotify() ;
            sddg_graphs.push_back( sddg ) ;
        }
        outs() << '\n';

        int minFreqSupport = 10 , maxInfreqSupp = 5 ;
        double minConf = 0.85;
        SupportInfo *spt = new SupportInfo( sddg_graphs , minFreqSupport) ; // Instantiate SI class
        vector <string> sgfqc = spt->get_single_frequency() ; // this func returns a vector<string>
//        for( auto u : sgfqc ) {
//            outs() << "sgfqc : " << u  << "\n";
//        }
        AprioriAlgorithm solver(sgfqc, minFreqSupport, maxInfreqSupp, spt);
        GenRule generator(solver.getFreqItemSet(), solver.getInfreqItemSet(), minConf, spt);

        DetectRule(generator, spt);
        /*
        vector<Rule> PARs = generator.getPARs() ;
        vector<Rule> NARs = generator.getNARs() ;

        for( auto u : PARs ){
            if( get<0>(u) >= 0.9999 ) continue ;
            vector<string> A = get<2>(u);
            vector<string> b{ get<3>(u) };
            for( auto v : A ) b.push_back( v ) ;
            outs() << "{ " ;
            for( auto v : A ) outs() << v << " " ;
            outs() << "}\n" ;
            outs() << "{ " ;
            for( auto v : b ) outs() << v << " " ;
            outs() << "}\n" ;

            vector< vector< vector<Instruction*> > > vios = spt->get_PARs_violations( &A , &b ) ;
            int qwq = 0 ;
            for( auto i : vios ){
                qwq ++ ;
                outs() << "the " << qwq << " func:\n" ;

                for( auto j : i ){
                    outs() << "  { " << "\n";
                    for( auto k : j ){
                        outs() << "    " << *k << "\n" ;
                    }
                    outs() << "  }\n" ;
                }
            } 
        }

        for( auto u : NARs ){
            if( get<0>(u) >= 0.9999 ) continue ;
            vector<string> A = get<2>(u);
            vector<string> b{ get<3>(u) };
            for( auto v : A ) b.push_back( v ) ;
            outs() << "{ " ;
            for( auto v : b ) outs() << v << " " ;
            outs() << "}\n" ;

            vector< vector< vector<Instruction*> > > vios = spt->get_NARs_violations( &b ) ;
            int qwq = 0 ;
            for( auto i : vios ){
                qwq ++ ;
                outs() << "the " << qwq << " func:\n" ;

                for( auto j : i ){
                    outs() << "  { " << "\n";
                    for( auto k : j ){
                        outs() << "    " << *k << "\n" ;
                    }
                    outs() << "  }\n" ;
                }
            } 
        }
        */


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

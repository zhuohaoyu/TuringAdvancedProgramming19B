#include "calcu.h"

#include <llvm/ADT/SCCIterator.h>
#include <llvm/IR/Argument.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Intrinsics.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/ModuleSlotTracker.h>
#include <llvm/IR/ValueSymbolTable.h>
#include <llvm/Support/Casting.h>
#include <llvm/Support/raw_ostream.h>

#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "SimpleDataDependenceGraph.h"

namespace SPT_calc {

using namespace llvm;
using std::map;
using std::pair;
using std::set;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;
using namespace miner;

sccNode::sccNode() {
    BBs.clear();
    edges.clear();
    Insts.clear();
}

sddgNode::sddgNode(Instruction *pI, SDDGNode *pSN, FuncInfo *FC_) {
    FC = FC_;
    Inst = pI, edges.clear();
    vector<SDDGNode *> &pred = pSN->getPredecessors(), &suc = pSN->getSuccessors();
    for (auto u : pred) edges.insert(FC->sddgNodes[u->getInst()]);
    for (auto u : suc) edges.insert(FC->sddgNodes[u->getInst()]);
}

void sddgNode::addedge(sddgNode *u) {
    edges.insert(u);
}

FuncInfo::FuncInfo(SDDG *S, SupportInfo *SP_) {
    Func = S->getFunc(), sddg = S, SP = SP_;
    SCC.clear();
    sccNodes.clear();
    ltNode.clear();
    sddgNodes.clear();
    SCCvis.clear();
    sddgvis.clear();
    _Calcu_SP_REM.clear();
    // process SCC and its information

    for (scc_iterator<Function *> SCCI = scc_begin(Func), SCCE = scc_end(Func); SCCI != SCCE; ++SCCI) {
        sccNode *nd = new sccNode();
        nd->BBs = *SCCI;
        sccNodes.push_back(nd);
        SCCvis[nd] = 0;
        for (BasicBlock *pB : nd->BBs) {
            SCC[pB] = nd;
            for (auto instIter = pB->begin(), instEnd = pB->end(); instIter != instEnd; instIter++) {
                Instruction *pI = &(*instIter);
                if (pI->getOpcode() != Instruction::Call) continue;
                if (SP->instStr.count(pI) == 0) {
                    SP->instStr[pI] = new string(normalizeInstruction(pI));  // match instructions to strings
                }
                nd->Insts.push_back(pI);  // gather the interesting instructions in SCC
            }
        }
    }
    // add edge in the SCC graph
    for (sccNode *nd : sccNodes) {
        for (BasicBlock *pB : nd->BBs) {
            auto *pTI = pB->getTerminator();
            for (int i = 0, totSucc = (int)((*pTI).getNumSuccessors()); i < totSucc; i++) {
                BasicBlock *v = pTI->getSuccessor((unsigned)i);
                if (SCC[v] != nd) nd->edges.insert(SCC[v]);
            }
        }
    }

    // add share & dependence edges into the vector"edges"
    DenseMap<Instruction *, SDDGNode *> &ItoNs = S->getItstNodes();
    for (auto u : ItoNs) sddgNodes[u.first] = new sddgNode();
    for (auto u : ItoNs) *sddgNodes[u.first] = sddgNode(u.first, u.second, this);

    set<pair<Instruction *, Instruction *>> Shares = S->getShares();
    for (auto u : Shares) {
        if ((u.first)->getOpcode() != Instruction::Call || (u.second)->getOpcode() != Instruction::Call) continue;
        sddgNodes[u.first]->addedge(sddgNodes[u.second]);
        sddgNodes[u.second]->addedge(sddgNodes[u.first]);
    }
}

FuncInfo::~FuncInfo() {
    for (auto u : sccNodes) delete u;
    for (auto u : sddgNodes) delete u.second;
}

void FuncInfo::dfs_sddg(sddgNode *u, set<string> &dfsset, vector<Instruction*> &vec , unordered_set<sddgNode*> lt ) {
    sddgvis[u] = 1 , vec.push_back( u->Inst ) ;
    //    cout << "     dfs_sddg: " << *(u->Inst) << "\n" ;
    dfsset.insert( *(SP->instStr[u->Inst]) );
    for (auto v : u->edges) {
        if (lt.count(v) && sddgvis[v] == 0) dfs_sddg(v, dfsset,vec,lt);
    }
}

bool FuncInfo::check_sddg_calsupp(unsigned siz1, unsigned siz2) {
    static set<string> dfsset;
    static vector<Instruction*> vec ;
    sddgvis.clear();
    for (auto u : ltNode) sddgvis[u] = 0;
    //    for( auto u : ltNode ) cout << *(u->Inst) << "\n" ;

    for (auto u : ltNode) {
        if (sddgvis[u] == 1) continue;
        dfsset.clear() , vec.clear() ;
        dfs_sddg(u, dfsset , vec , ltNode);
//        cout << dfsset.size() << " " << siz1 << " " << vec.size() << " " << " " << siz2 << "\n" ;
        if (dfsset.size() >= siz1 && vec.size() >= /*(int)*/siz2) return true;
    }
    return false;
}

void FuncInfo::check_sddg_PARvio( unsigned sizA1 , unsigned sizB1 , unsigned sizA2 , unsigned sizB2 , vector< vector<Instruction*> > *rt ){
    static set<string> dfsset ;
    static vector<Instruction*> vec ;
    sddgvis.clear() ;
    for (auto u : ltBB ) sddgvis[u] = 0 ;
    
    for( auto u : ltAA ){
        if( sddgvis[u] == 1 ) continue ;
        dfsset.clear() , vec.clear() ;
        sddgVisCnt = 0 ;
        dfs_sddg( u , dfsset , vec , ltBB ) ;
        if( dfsset.size() >= sizB1 && vec.size() >= /*(int)*/sizB2 ) continue ;
        if( dfsset.size() == sizA1 && vec.size() >= /*(int)*/sizA2 ){
            bool isok = true ;
            for( auto v : ltB_A ){
                if( dfsset.count( *(SP->instStr[v->Inst] ) ) ) { isok = false ; break ; }
            } if( !isok ) continue ;
            // if not continue here, a NAR vio is found
            rt->push_back( vec ) ; return ;
        }
    }
}

void FuncInfo::check_sddg_NARvio( unsigned int sizB1 , unsigned int sizB2 , vector< vector<Instruction*> > *rt ){
    static set<string> dfsset ;
    static vector<Instruction*> vec ;
    sddgvis.clear() ;
    for( auto u : ltBB ) sddgvis[u] = 0 ;
    
    for( auto u : ltBB ){
        if( sddgvis[u] == 1 ) continue ;
        dfsset.clear() , vec.clear() ;
        dfs_sddg( u , dfsset , vec , ltBB ) ;
        if( dfsset.size() >= sizB1 && vec.size() >= /*(int)*/sizB2 ){
            rt->push_back( vec ) ; return ;
        }
    }
}

int FuncInfo::dfs_SCC_calsupp(sccNode *u, unordered_set<string> *itemset, unsigned siz) {
    SCCvis[u] = 1;
    for (Instruction *pI : u->Insts) {
        if (itemset->count(*(SP->instStr[pI]))) ltNode.insert(sddgNodes[pI]);
    }
    int rt = 0;
    for (sccNode *v : u->edges) {
        if (SCCvis[v] == 1) continue;
        rt |= dfs_SCC_calsupp(v, itemset, siz);
        if (rt) {
            SCCvis[u] = 0;
            return rt;
        }
    }
    if ((u->edges).size() == 0) {
        //        cout << "enter leaf\n" ;
        rt |= check_sddg_calsupp(itemset->size(), siz);
    }
    SCCvis[u] = 0;
    for (Instruction *pI : u->Insts) {
        if (itemset->count(*(SP->instStr[pI]))) ltNode.erase(sddgNodes[pI]);
    }
    return rt;
}

void FuncInfo::dfs_SCC_PARs_violations( sccNode *u , unordered_set<string>* rA , unordered_set<string>* rB , int sizA , int sizB , vector< vector<Instruction*> > *rt ){
    SCCvis[u] = 1 ;
    for( Instruction *pI : u->Insts ) {
        if ( rB->count(*(SP->instStr[pI]))){
            ltBB.insert(sddgNodes[pI]);
            if ( rA->count(*(SP->instStr[pI]))) ltAA.insert(sddgNodes[pI]);
            else ltB_A.insert(sddgNodes[pI]);
        }
    }
    for( sccNode *v : u->edges ){
        if( SCCvis[v] == 1 ) continue ;
        dfs_SCC_PARs_violations( v , rA , rB , sizA , sizB , rt ) ;
    }
    if( (u->edges).size() == 0 ){
        check_sddg_PARvio( rA->size() , rB->size() , sizA , sizB , rt ) ;
    }
    SCCvis[u] = 0 ;
    for( Instruction *pI : u->Insts ) {
        if ( rB->count(*(SP->instStr[pI]))){
            ltBB.erase(sddgNodes[pI]);
            if ( rA->count(*(SP->instStr[pI]))) ltAA.erase(sddgNodes[pI]);
            else ltB_A.erase(sddgNodes[pI]);
        }
    }
}

void FuncInfo::dfs_SCC_NARs_violations( sccNode *u , unordered_set<string>* rB , int sizB , vector< vector<Instruction*> > *rt  ){
    SCCvis[u] = 1 ;
    for( Instruction *pI : u->Insts ) {
        if ( rB->count(*(SP->instStr[pI]))) ltBB.insert(sddgNodes[pI]);
    }
    for( sccNode *v : u->edges ){
        if( SCCvis[v] == 1 ) continue ;
        dfs_SCC_NARs_violations( v , rB , sizB , rt ) ;
    }
    if( (u->edges).size() == 0 ){
        check_sddg_NARvio( rB->size() , sizB , rt ) ;
    }
    SCCvis[u] = 0 ;
    for( Instruction *pI : u->Insts ) {
        if ( rB->count(*(SP->instStr[pI]))) ltBB.erase(sddgNodes[pI]);
    }
}

int FuncInfo::calcu_Func_support(vector<string> *itemset_) {
    // in this part, we check whether current Function support the itemset
    // idea:
    //   dfs in the SCC graph. dfs in a chain, and for those the interested instructions
    // which is also included the itemset, light corresponding interesting nodes in the
    // dependence graph according to the map<inst*,sddgnode*> mIn...
    //   When reach a node without outedge, check if there is a connected block in the
    // dependence graph, and it must include every item in the itemset at least once.

    // find the entry SCC, then start dfs

    if (_Calcu_SP_REM.count(*itemset_)) return _Calcu_SP_REM[*itemset_];

    //    cout << "Func : " << Func->getName() << " has " << sccNodes.size() << "SCCs" << "\n" ;
    unordered_set<string> itemset;
    itemset.clear();
    ltNode.clear();
    SCCvis.clear();
    for (int i = 0, ssize = SCCvis.size(); i < ssize; i++) SCCvis[sccNodes[i]] = 0;
    for (string u : *itemset_) itemset.insert(u);

    int rt = dfs_SCC_calsupp(SCC[&(Func->getEntryBlock())], &itemset, (*itemset_).size());
    _Calcu_SP_REM[*itemset_] = rt;
    //    cout << "rt = " << rt << "\n" ;
    return rt;
}

vector< vector<Instruction*> > FuncInfo::get_Func_PARs_violations( vector<string>* sA , vector<string>* sB ){
    static vector< vector<Instruction*> > rt ;
    rt.clear() ;
    unordered_set<string> rulesetA, rulesetB ;
    rulesetA.clear() ; rulesetA.insert( sA->begin() , sA->end() ) ;
    rulesetB.clear() ; rulesetB.insert( sB->begin() , sB->end() ) ;
    SCCvis.clear() ;
    ltAA.clear() , ltBB.clear() , ltB_A.clear() ;
    for( int i = 0, ssize = SCCvis.size(); i < ssize ; i ++ ) SCCvis[ sccNodes[i] ] = 0 ;

    dfs_SCC_PARs_violations( SCC[&(Func->getEntryBlock())] , &rulesetA , &rulesetB , (*sA).size() , (*sB).size() , &rt ) ;
    return rt ;
}

vector< vector<Instruction*> > FuncInfo::get_Func_NARs_violations( vector<string>* sB ){
    static vector< vector<Instruction*> > rt ;
    rt.clear() ;
    unordered_set<string> rulesetB ;
    rulesetB.clear() ; rulesetB.insert( sB->begin() , sB->end() ) ;
    SCCvis.clear() ;
    ltBB.clear() ;
    for( int i = 0 , ssize = SCCvis.size() ; i < ssize ; i ++ ) SCCvis[ sccNodes[i] ] = 0 ;

    dfs_SCC_NARs_violations( SCC[&(Func->getEntryBlock())] , &rulesetB , (*sB).size() , &rt ) ;
    return rt ;
}

vector< vector< vector<Instruction*> > > SupportInfo::get_PARs_violations( vector<string>* sA , vector<string>* sB ){
    static vector< vector< vector<Instruction*> > > rt ;
    rt.clear() ;
    for( int i = 0 ; i < sdgs_siz ; i ++ ){
        rt.push_back( sdgs[i]->get_Func_PARs_violations( sA , sB ) ) ;
    }
    return rt ;
}

vector< vector< vector<Instruction*> > > SupportInfo::get_NARs_violations( vector<string>* sB ){
    static vector< vector< vector<Instruction*> > > rt ;
    rt.clear() ;
    for( int i = 0 ; i < sdgs_siz ; i ++ ){
        rt.push_back( sdgs[i]->get_Func_NARs_violations( sB ) ) ;
    }
    return rt ;
}

int SupportInfo::calcu_support(vector<string> *itemset) {
    int support = 0;
    for (int i = 0; i < sdgs_siz; i++) {
        support += sdgs[i]->calcu_Func_support(itemset);
    }
    return support;
}

int SupportInfo::calcu_support(const string &x) {
    static vector<string> __cal_temp;
    __cal_temp.clear(), __cal_temp.push_back(x);
    return calcu_support(&__cal_temp);
}

SupportInfo::SupportInfo(vector<SDDG *> &sddg_graphs, int LIMIT_) : LIMIT(LIMIT_) {
    sdgs_siz = sddg_graphs.size();
    sgfqc.clear();
    for (int i = 0; i < sdgs_siz; i++) {
        sdgs.push_back(new FuncInfo(sddg_graphs[i], this));
    }

    vector<string> temp;
    int cnt = 0;
    for (auto ItoS : instStr) {
        temp.clear();
        temp.push_back(*ItoS.second);
        ++cnt;
        int spor = calcu_support(&temp);
        if (spor >= LIMIT) sgfqc.insert(*ItoS.second);
    }
}

SupportInfo::~SupportInfo() {
    for (auto u : sdgs) delete u;
    for (auto u : instStr) delete u.second;
}

vector<string> SupportInfo::get_single_frequency() {
    vector<string> __vec_sgfqc;
    __vec_sgfqc.insert(__vec_sgfqc.end(), sgfqc.begin(), sgfqc.end());
    return __vec_sgfqc;
}

}  // namespace SPT_calc
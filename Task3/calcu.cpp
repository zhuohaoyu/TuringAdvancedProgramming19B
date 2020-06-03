#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <string>

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
#include <llvm/ADT/SCCIterator.h>

#include "calcu.h"
#include "SimpleDataDependenceGraph.h"

namespace SPT_calc{

using namespace llvm;
using std::map;
using std::pair;
using std::set;
using std::vector;
using std::string;
using std::unordered_set;
using std::unordered_map;
using namespace miner ;

sccNode::sccNode() {
    BBs.clear() ;
    edges.clear() ;
    Insts.clear() ;
}

sddgNode::sddgNode( Instruction *pI , SDDGNode* pSN , FuncInfo* FC_ ){
    FC = FC_ ;
    Inst = pI , edges.clear() ;
    vector<SDDGNode *> &pred = pSN->getPredecessors() , &suc = pSN->getSuccessors() ;
    for( auto u : pred )edges.insert( FC->sddgNodes[ u->getInst() ] ) ;
    for( auto u : suc ) edges.insert( FC->sddgNodes[ u->getInst() ] ) ;
}

void sddgNode::addedge( sddgNode* u ){
    edges.insert( u ) ;
}

FuncInfo::FuncInfo( SDDG* S , SupportInfo *SP_ ){
    
    Func = S->getFunc() , sddg = S , SP = SP_ ;
    SCC.clear() ; sccNodes.clear() ; ltNode.clear() ; sddgNodes.clear() ; SCCvis.clear() ; sddgvis.clear() ;
    _Calcu_SP_REM.clear() ; 
    // process SCC and its information

    for( scc_iterator<Function *> SCCI = scc_begin(Func) , SCCE = scc_end(Func) ; SCCI != SCCE ; ++SCCI ){
        sccNode *nd = new sccNode() ;
        nd->BBs = *SCCI ;
        sccNodes.push_back( nd ) ;
        SCCvis[nd] = 0 ;
        for( BasicBlock* pB : nd->BBs ){
            SCC[pB] = nd ;
            for( auto instIter = pB->begin() , instEnd = pB->end() ; instIter != instEnd ; instIter ++ ){
                Instruction *pI = &(*instIter) ;
                if( pI->getOpcode() != Instruction::Call ) continue ;
                if( SP->instStr.count( pI ) == 0 ){
                    SP->instStr[pI] = new string( normalizeInstruction( pI ) ) ; // match instructions to strings
                }
                nd->Insts.push_back( pI ) ; // gather the interesting instructions in SCC
            }
        }
    }
    // add edge in the SCC graph
    for( sccNode *nd : sccNodes ){
        for( BasicBlock* pB : nd->BBs ){
            auto *pTI = pB->getTerminator() ;
            for( int i = 0 , totSucc = (int)( (*pTI).getNumSuccessors() ) ; i < totSucc ; i ++ ){
                BasicBlock *v = pTI->getSuccessor( (unsigned)i ) ;
                if( SCC[v] != nd ) nd->edges.insert( SCC[v] ) ;
            }
        }
    }
    
    // add share & dependence edges into the vector"edges"
    DenseMap<Instruction *, SDDGNode *> &ItoNs = S->getItstNodes() ;
    for( auto u : ItoNs ) sddgNodes[u.first] = new sddgNode() ;
    for( auto u : ItoNs )*sddgNodes[u.first] = sddgNode( u.first , u.second , this ) ;

    set<pair<Instruction *, Instruction *>> Shares = S->getShares() ;
    for( auto u : Shares ){
        if( (u.first)->getOpcode() != Instruction::Call || (u.second)->getOpcode() != Instruction::Call ) continue ;
        sddgNodes[u.first]->addedge( sddgNodes[u.second] ) ;
        sddgNodes[u.second]->addedge( sddgNodes[u.first] ) ;
    }

}

FuncInfo::~FuncInfo(){
    for( auto u : sccNodes ) delete u ;
    for( auto u : sddgNodes ) delete u.second ;
}

void FuncInfo::dfs_sddg( sddgNode *u , set<string*>& dfsset ){
    sddgvis[u] = 1 , sddgVisCnt ++ ;
    dfsset.insert( SP->instStr[u->Inst] ) ;
    for( auto v : u->edges ){
        if( ltNode.count( v ) && sddgvis[v] == 0 ) dfs_sddg( v , dfsset ) ;
    }
}

bool FuncInfo::check_sddg( unsigned siz ){
    static set<string*> dfsset ;
    sddgvis.clear() ;
    for( auto u : ltNode ) sddgvis[u] = 0 ;
//    for( auto u : ltNode ) errs() << *(u->Inst) << "\n" ;

    for( auto u : ltNode ){
        if( sddgvis[u] == 1 ) continue ;
        dfsset.clear() ; sddgVisCnt = 0 ;
        dfs_sddg( u , dfsset ) ;
    //    errs() << dfsset.size() << " " << siz << " " << sddgVisCnt << " " << " " << siz << "\n" ;
        if( dfsset.size() == siz && sddgVisCnt >= (int)siz ) return true ;
    } 
    return false ;
}

int FuncInfo::dfs_SCC( sccNode *u , unordered_set<string> *itemset , unsigned siz ) {
    SCCvis[u] = 1 ;
    for( Instruction* pI : u->Insts ){
        if( itemset->count( *(SP->instStr[pI]) ) ) ltNode.insert( sddgNodes[pI] ) ;
    }
    int rt = 0 ;
    for( sccNode* v : u->edges ){
        if( SCCvis[v] == 1 ) continue ;
        rt |= dfs_SCC( v , itemset , siz ) ;
        if( rt ){ SCCvis[u] = 0 ; return rt ; }
    }
    if( (u->edges).size() == 0 ){
//        errs() << "enter leaf\n" ; 
        rt |= check_sddg( siz ) ;
    }
    SCCvis[u] = 0 ;
    for( Instruction* pI : u->Insts ){
        if( itemset->count( *(SP->instStr[pI]) ) ) ltNode.erase( sddgNodes[pI] ) ;
    }
    return rt ;
}

int FuncInfo::calcu_Func_support( vector<string> *itemset_ ){
    // in this part, we check whether current Function support the itemset
    // idea:
    //   dfs in the SCC graph. dfs in a chain, and for those the interested instructions
    // which is also included the itemset, light corresponding interesting nodes in the 
    // dependence graph according to the map<inst*,sddgnode*> mIn...
    //   When reach a node without outedge, check if there is a connected block in the
    // dependence graph, and it must include every item in the itemset at least once.
    
    // find the entry SCC, then start dfs

    if( _Calcu_SP_REM.count( *itemset_) ) return _Calcu_SP_REM[*itemset_] ;

//    errs() << "Func : " << Func->getName() << "\n" ;
    unordered_set<string> itemset ; itemset.clear() ;
    ltNode.clear() ; SCCvis.clear() ;
    for( int i = 0 ; i < SCCvis.size() ; i++ ) SCCvis[ sccNodes[i] ] = 0 ; 
    for( string u : *itemset_ ) itemset.insert( u ) ;
    
    int rt = dfs_SCC( SCC[ &(Func->getEntryBlock()) ] , &itemset , (*itemset_).size() ) ;
    _Calcu_SP_REM[ *itemset_ ] = rt ;
//    errs() << "rt = " << rt << "\n" ;
    return rt ;
}

int SupportInfo::calcu_support( vector<string> *itemset ){
    int support = 0 ;
    for( int i = 0 ; i < sdgs_siz ; i ++ ){
        support += sdgs[i]->calcu_Func_support( itemset ) ; 
    } return support ;
}

int SupportInfo::calcu_support( const string &x ){
    static vector<string> __cal_temp ;
    __cal_temp.clear() , __cal_temp.push_back( x ) ;
    return calcu_support( &__cal_temp ) ;
}

SupportInfo::SupportInfo( vector<SDDG*> &sddg_graphs , int LIMIT_ ):LIMIT(LIMIT_) {
    sdgs_siz = sddg_graphs.size() ;
    sgfqc.clear() ;
    for( int i = 0 ; i < sdgs_siz ; i ++ ){
        sdgs.push_back( new FuncInfo( sddg_graphs[i] , this ) ) ;
    }

    vector<string> temp ;
    int cnt = 0 ;
    for( auto ItoS : instStr ){
        temp.clear() ; temp.push_back( *ItoS.second ) ;
        ++cnt ;
        int spor = calcu_support( &temp ) ;
        if( spor >= LIMIT ) sgfqc.insert( *ItoS.second ) ;
    }

}

SupportInfo::~SupportInfo(){
    for( auto u : sdgs ) delete u ;
    for( auto u : instStr ) delete u.second ;
}

vector<string> SupportInfo::get_single_frequency(){
    vector<string> __vec_sgfqc ;
    __vec_sgfqc.insert( __vec_sgfqc.end() , sgfqc.begin() , sgfqc.end() ) ;
    return __vec_sgfqc ;
}

}
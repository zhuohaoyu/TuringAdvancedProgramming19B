#ifndef MY_CALCU_H_
#define MY_CALCU_H_

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <set>
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

#include "SimpleDataDependenceGraph.h"
namespace SPT_calc{

using namespace llvm;
using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;
using std::unordered_set;
using std::unordered_map;
using namespace miner ;

class sccNode ;
class sddgNode ;
class FuncInfo ;
class SupportInfo ;

class sccNode{
public: 
    vector<BasicBlock*> BBs ;
    unordered_set<sccNode*> edges ;
    vector<Instruction*> Insts;
    sccNode() ;
    ~sccNode() = default ;
} ;

class sddgNode{
public :
    FuncInfo *FC ;
    unordered_set<sddgNode*> edges ;
    Instruction* Inst ;
    sddgNode() = default ;
    sddgNode( Instruction *pI , SDDGNode* pSN , FuncInfo* FC_ ) ;
    ~sddgNode() = default ;
    void addedge( sddgNode* u ) ;
} ;

class FuncInfo{
    friend class sddgNode ;
private:
    SupportInfo* SP ;
    Function* Func ;
    SDDG *sddg ;
    vector< sccNode* > sccNodes ;
    map< BasicBlock* , sccNode* > SCC ;
    unordered_set< sddgNode* > ltNode ; // lighted Node
    unordered_map< sccNode* , bool > SCCvis ;
    unordered_map< sddgNode* , bool > sddgvis ;
    int sddgVisCnt ;
    unordered_map< Instruction* , sddgNode* > sddgNodes ;
    map< vector<string> , int > _Calcu_SP_REM ;

public:
    FuncInfo( SDDG* S , SupportInfo* SP_) ;
    ~FuncInfo() ;
    int calcu_Func_support( vector<string> *itemset_ ) ;
    int dfs_SCC( sccNode *u , unordered_set<string> *itemset , unsigned siz ) ;
    bool check_sddg( unsigned siz1 , unsigned siz2 ) ;
    void dfs_sddg( sddgNode *u , set<string*>& dfsset ) ;
} ;

class SupportInfo{
    friend class FuncInfo ;
private:
    // BB* getParent
    vector<FuncInfo*> sdgs ;
    unordered_set<string> sgfqc ; // single_element_frequency 
    map< Instruction* , string* > instStr ;
    int sdgs_siz ;
    const int LIMIT ;

    int calcu_Func_support( vector<string> *itemset ) ;
    // for each sddg, calculate whether it support the itemset or not
    // the return value can only be 0 or 1 ;

    bool check_dfs_dpg( SDDGNode* u ) ;
    // for each SCC chain in the BB graph
    // check the related Nodes' connection on the dependence graph

public:
    SupportInfo( vector<SDDG*> &sddg_graphs , int LIMIT_ );
    // process the Funcs, SCC, sccInst, sccSuccessors and instStr
    // and also add the share edge to the mSuccessors & mPresuccessors
    ~SupportInfo() ;
    
    int calcu_support( vector<string> *itemset ) ;
    // main function we need

    int calcu_support( const string &x ) ;

    vector<string> get_single_frequency() ;

} ;

}


#endif 

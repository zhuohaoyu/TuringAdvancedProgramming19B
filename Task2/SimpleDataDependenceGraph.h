#ifndef SIMPLESIMPLEDATADEPENDENCE_H_
#define SIMPLESIMPLEDATADEPENDENCE_H_

#include <vector>
#include <set>
#include <map>
#include <string>

#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/DenseSet.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Function.h"

namespace miner
{

using namespace llvm;
using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;

/////////////////////////
// 以下的数据结构与方法实现，请根据自己的设计做出调整与改进。
// 若直接使用，请实现相应的成员函数，并添加可能需要用到的其他成员。
// 请自行理解已有各成员的功能与用途。
/////////////////////////

class SDDGNode
{
private:
    Instruction *mInst;
    vector<SDDGNode *> mSuccessors;
    vector<SDDGNode *> mPredecessors;

public:
    SDDGNode(Instruction *inst) : mInst(inst) {}
    ~SDDGNode();
    bool addSuccessor(SDDGNode *dst);
    bool addPredecessor(SDDGNode *dst);
    inline Instruction *getInst();
    vector<SDDGNode *> &getSuccessors();
};

class SDDG
{
private:
    Function *mFunc;
    DenseMap<Instruction *, SDDGNode *> mNodes;
    DenseMap<Instruction *, SDDGNode *> mInterestingNodes;
    set<pair<Instruction *, Instruction *>> mShares;
    bool share(Instruction *fst, Instruction *snd);

public:
    SDDG() {}
    SDDG(Function *func) : mFunc(func) {}
    ~SDDG() = default ;
    // 创建数据依赖图及数据共享关系
    void buildSDDG();
    // 将数据依赖图中的无关元素去除，仅保留所关注的元素
    void flattenSDDG();
    // 基于LLVM IR的信息，不做任何概念上的改变，创建数据依赖图
    //void buildLegacySDDG();
    // 提供将数据依赖图转化为dot文件的方法，对于将字符串映射为整数之后，如何转化，请自行设计实现。
    // 参数指示是否将“数据共享关系”输出到dot文件中。
    // 获得dot文件后（加上名为Test_func.dot，执行命令：dot -Tpng -o a.png Test_func.dot可生成相应
    // 的图形文件（可修改输出格式，生成其他格式如jpg、svg等的图形文件）【需安装Graphviz】。
    void dotify(bool showShareRelations = true);
};

} // namespace miner

#endif
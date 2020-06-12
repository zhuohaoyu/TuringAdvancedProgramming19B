#pragma once
#include <algorithm>
#include <string>
#include <vector>
#include <tuple>
#include "calcu.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/Metadata.h"

using namespace llvm;
using namespace std;

using ItemSet = vector<string>;
using Rule = tuple<double, ItemSet, ItemSet, string>;
using Info = tuple<unsigned int, string, string, unsigned int>;

class GenRule {
    friend class SupportInfo;
public:
    GenRule();
    GenRule(vector<ItemSet> FIs, vector<ItemSet> IIs, double min_conf, SPT_calc::SupportInfo *_supportInfoAddr);
    static bool cmp_Rule(Rule x, Rule y) { return get<0>(x) > get<0>(y); }
    vector<Rule>& getPARs();
    vector<Rule>& getNARs();
private:
    SPT_calc::SupportInfo *supportInfoAddr;
    vector<Rule> PARs, NARs;
};

Info &retrieveDebugInfo(Instruction* inst);

void DetectRule(GenRule &generator, SPT_calc::SupportInfo *spt);

void DisplayInst(Instruction* inst, char sep = '#');
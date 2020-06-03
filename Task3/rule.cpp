#include "rule.h"
#include "apriori.h"
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
#include <iostream>
using namespace llvm;

using namespace std;

//extern int support(ItemSet I); //后面对于这里的调用需要手动改成 SupportInfo 里面的 calc_func_support

GenRule::GenRule(vector<ItemSet> FIs, vector<ItemSet> IIs, double min_conf, SPT_calc::SupportInfo *_supportInfoAddr) {
    supportInfoAddr = _supportInfoAddr;
    for(auto I: FIs) {
        if(I.size() < 2) continue;
        for(int i = 0; i < I.size(); i++) {
            if(I.size() == 2 && I[0] == I[1]) continue; //check_same
            auto A = I;
            swap(A[i], A.back());
            A.pop_back();
            int v1 = supportInfoAddr -> calcu_support(&I), v2 = supportInfoAddr -> calcu_support(&A);
            double conf = 1.0 * v1 / v2;
            if(conf >= min_conf)
                PARs.push_back(make_tuple(conf, I, A, I[i]));
        }
    }
    sort(PARs.begin(), PARs.end(), cmp_Rule);

    //IIs
    for(auto I: IIs) {
        if(I.size() < 2) continue;
        //sort(I);
        Rule temp = make_tuple(1.14514, ItemSet(), ItemSet(), string());
        for(int i = 0; i < I.size(); i++) {
            auto A = I;
            swap(A[i], A.back());
            A.pop_back();
            int v1 = supportInfoAddr -> calcu_support(&I), v2 = supportInfoAddr -> calcu_support(&A);
            if(v2 == 0) continue;
            double conf = 1.0 - 1.0 * v1 / v2;
            if(conf <= get<0>(temp)) temp = make_tuple(conf, I, A, I[i]);
        }
        if(1.0 >= get<0>(temp) && get<0>(temp) >= min_conf) NARs.push_back(temp);
    }
    sort(NARs.begin(), NARs.end(), cmp_Rule);
    errs() << "PARs:\n";
    for(auto R: PARs) {
        //if(get<0>(R) == 1.0) continue;
        double conf = get<0>(R);
        auto A = get<2>(R);
        auto b = get<3>(R);
        errs() << "\tRule: { ";
        for(auto a: A) errs() << a << " ";
        errs() << "} -> { " << b << " }\n"; 
        errs() << "\tconfidence rate = " << conf << "\n\n";
    }

    errs() << "NARs:\n";
    for(auto R: NARs) {
        //if(get<0>(R) == 1.0) continue;
        double conf = get<0>(R);
        auto A = get<2>(R);
        auto b = get<3>(R);
        errs() << "\tRule: { ";
        for(auto a: A) errs() << a << " ";
        errs() << "} -> { " << b << " }\n"; 
        errs() << "\tconfidence rate = " << conf << "\n\n";
    }
}

vector<Rule>& GenRule::getPARs() { return PARs; }

vector<Rule>& GenRule::getNARs() { return NARs; }

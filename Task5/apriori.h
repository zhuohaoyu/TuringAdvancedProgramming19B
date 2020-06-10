#pragma once
#include <vector>
#include <string>
#include <unordered_set>
#include "calcu.h"
using namespace std;
using ItemSet = vector<string>;
using HashValue = unsigned long long;
class AprioriAlgorithm {
public:
    AprioriAlgorithm() = default;
    AprioriAlgorithm(ItemSet singleFreqItemSet, int minFreqSupport, int maxInfreqSupp, SPT_calc::SupportInfo *suptr);
    vector<ItemSet> getFreqItemSet();
    vector<ItemSet> getInfreqItemSet();
private:
    SPT_calc::SupportInfo *supportInfoAddr;
    vector<ItemSet> freqItemSet, infreqItemSet;
    vector<ItemSet> expandItemset(vector<ItemSet> s, int K);
    unordered_set<HashValue> infreqItemSetHash;
};
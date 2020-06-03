#pragma once
#include <algorithm>
#include <string>
#include <tuple>
#include <vector>

#include "calcu.h"

using namespace std;

using ItemSet = vector<string>;
using Rule = tuple<double, ItemSet, ItemSet, string>;

class GenRule {
    friend class SupportInfo;

public:
    GenRule();
    GenRule(vector<ItemSet> FIs, vector<ItemSet> IIs, double min_conf, SPT_calc::SupportInfo* _supportInfoAddr);
    static bool cmp_Rule(Rule x, Rule y) { return get<0>(x) > get<0>(y); }
    vector<Rule>& getPARs();
    vector<Rule>& getNARs();

private:
    SPT_calc::SupportInfo* supportInfoAddr;
    vector<Rule> PARs, NARs;
};
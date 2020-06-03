#pragma once
#include <algorithm>
#include <string>
#include <vector>
#include <tuple>
#include "calcu.h"

using namespace std;

using ItemSet = vector<string>;
using Rule = tuple<double, ItemSet, ItemSet>;

class GenRule {
    friend class SupportInfo;
public:
    GenRule();
    GenRule(vector<ItemSet> FIs, vector<ItemSet> IIs, double min_conf);
    static bool cmp_Rule(Rule x, Rule y) { return get<0>(x) > get<0>(y); }
    vector<Rule>& getPARs();
    vector<Rule>& getNARs();
private:
    vector<Rule> PARs, NARs;
};
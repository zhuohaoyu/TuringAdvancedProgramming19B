#include <vector>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <string>
#include "apriori.h"
#include "calcu.h"
#include <iostream>



using namespace std;

using ItemSet = vector<string>;
using HashValue = unsigned long long;
const int StringHashBase = 313;
const int ItemSetHashBase = 1013;
// extern int getItemsetSupport(ItemSet itemset);

unsigned int BKDRHash(string s) {
    unsigned int ret = 0;
    for(auto i: s)
        ret = ret * StringHashBase + i + 1;
    return ret;
}

HashValue ItemSetHash(ItemSet itemset, int ignorepos = -1) {
    HashValue ret = 0;
    for(unsigned int i = 0; i < itemset.size(); ++i) if(i != ignorepos) {
        ret = ret * ItemSetHashBase + BKDRHash(itemset[i]);
    }
    return ret;
}


vector<ItemSet> AprioriAlgorithm::getFreqItemSet() {
    return freqItemSet;
}


vector<ItemSet> AprioriAlgorithm::getInfreqItemSet() {
    return infreqItemSet;
}


vector<ItemSet> AprioriAlgorithm::expandItemset(vector<ItemSet> s, int K) {
    vector<ItemSet> ret;
    unordered_set<HashValue> expandItemsetHash;
    int siz = s.size();
    for(int i = 0; i < siz; ++i) sort(s[i].begin(), s[i].end());
    for(int i = 0; i < siz; ++i) {
        for(int j = i + 1; j < siz; ++j) {
            ItemSet diff1(K * 2), diff2(K * 2);
            auto diffa = set_difference(s[i].begin(), s[i].end(), s[j].begin(), s[j].end(), diff1.begin());
            auto diffb = set_difference(s[j].begin(), s[j].end(), s[i].begin(), s[i].end(), diff2.begin());
            diff1.resize(diffa - diff1.begin());
            diff2.resize(diffb - diff2.begin());
            if(diff1.size() == 1 && diff2.size() == 1) {
                ItemSet uni(K + 1);
                set_union(s[i].begin(), s[i].end(), s[j].begin(), s[j].end(), uni.begin());
                sort(uni.begin(), uni.end());
                bool requirePrune = false;
                for(int k = 0; k < K + 1; ++k) {
                    if(infreqItemSetHash.count(ItemSetHash(uni, k))) {
                        requirePrune = true;
                        break;
                    }
                }
                bool repeatElement = false;
                for(int k = 1; k < K; ++k) {
                    if(uni[k] == uni[k - 1]) {
                        repeatElement = true;
                        break;
                    }
                }
                if(repeatElement && K + 1 > 2) requirePrune = true;
                HashValue newItemsetHash = ItemSetHash(uni);
                if(!requirePrune && !expandItemsetHash.count(newItemsetHash)) {
                    ret.push_back(uni);
                    expandItemsetHash.insert(newItemsetHash);
                }
            }
        }
    }
    return ret;
}


AprioriAlgorithm::AprioriAlgorithm(ItemSet singleFreqItemSet, int minFreqSupp, int maxInfreqSupp, SPT_calc::SupportInfo *suptr) {
    supportInfoAddr = suptr;
    freqItemSet.clear(), infreqItemSet.clear();
    vector< vector<ItemSet> > L, N;
    L.resize(2);
    N.resize(2);
    for(auto i: singleFreqItemSet) L[1].push_back({i});
    for(int k = 2; !L[k - 1].empty(); ++k) {
        L.resize(k + 1), N.resize(k + 1);
        vector<ItemSet> S = expandItemset(L[k - 1], k - 1);
        if(k == 2) {
            for(auto i: singleFreqItemSet) 
                S.push_back({i, i});
        }
        for(auto itemset : S) {
            int supp = supportInfoAddr -> calcu_support(&itemset);
            if(supp >= minFreqSupp) L[k].push_back(itemset);
            else if(supp > 0 && supp < maxInfreqSupp) N[k].push_back(itemset);
        }
        if(!L[k].empty()) {
            for(auto its: L[k]) {
                freqItemSet.push_back(its);
            }
        }
        if(!N[k].empty()) {
            for(auto its: N[k]) {
                infreqItemSetHash.insert(ItemSetHash(its));
                infreqItemSet.push_back(its);
            }
        }
    }

    //Output Here

    cout << "Frequent Itemsets:\n";
    for(auto i: freqItemSet) {
        cout << "\t";
        for(auto j: i) cout << "\"" << j << "\"" << ", "; 
        cout << endl;
    }
    cout << "Infrequent Itemsets:\n";
    for(auto i: infreqItemSet) {
        cout << "\t";
        for(auto j: i) cout << "\"" << j << "\"" << ", "; 
        cout << endl;
    }
}

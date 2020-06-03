#include "rule.h"
#include "apriori.h"
#include "calcu.h"

using namespace std;

extern int support(ItemSet I); //后面对于这里的调用需要手动改成 SupportInfo 里面的 calc_func_support

GenRule::GenRule(vector<ItemSet> FIs, vector<ItemSet> IIs, double min_conf) {
    //FIs
    for(auto I: FIs) {
        if(I.size() < 2) continue;
        for(int i = 0; i < I.size(); i++) {
            if(I.size() == 2 && I[0] == I[1]) continue; //check_same
            auto A = I;
            swap(A[i], A.back());
            A.pop_back();
            double conf = 1.0 * support(I) / support(A);
            if(conf >= min_conf)
                PARs.push_back(make_tuple(conf, I, A));
        }
    }
    sort(PARs.begin(), PARs.end(), cmp_Rule);

    //IIs
    for(auto I: IIs) {
        if(I.size() < 2) continue;
        //sort(I);
        Rule temp = make_tuple(1.14514, ItemSet(), ItemSet());
        for(int i = 0; i < I.size(); i++) {
            auto A = I;
            swap(A[i], A.back());
            A.pop_back();
            double conf = 1.0 - 1.0 * support(I) / support(A);
            if(conf <= get<0>(temp)) temp = make_tuple(conf, I, A);
        }
        if(1.0 >= get<0>(temp) && get<0>(temp) >= min_conf) NARs.push_back(temp);
    }
    sort(NARs.begin(), NARs.end(), cmp_Rule);
}

vector<Rule>& GenRule::getPARs() { return PARs; }

vector<Rule>& GenRule::getNARs() { return NARs; }

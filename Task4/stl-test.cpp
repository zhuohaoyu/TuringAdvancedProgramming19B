#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <string>
using namespace std;
int main() {
    vector<string> a{"malloc", "haha", "malloc", "free"};
    vector<string> b{"malloc", "malloc", "free", "wawawa"};
    sort(a.begin(), a.end()), sort(b.begin(), b.end());
    int K = 4;
    vector<string> diff1(K * 2), diff2(K * 2);
    auto diffa = set_difference(a.begin(), a.end(), b.begin(), b.end(), diff1.begin());
    auto diffb = set_difference(b.begin(), b.end(), a.begin(), a.end(), diff2.begin());
    diff1.resize(diffa - diff1.begin());
    diff2.resize(diffb - diff2.begin());
    vector<string> uni(K + 1);
    set_union(a.begin(), a.end(), b.begin(), b.end(), uni.begin());
    sort(uni.begin(), uni.end());
    for(auto i:diff1) cout << i << endl;
    cout << endl;
    for(auto i:diff2) cout << i << endl;
    cout << endl;
    for(auto i:uni) cout << i << endl;
    cout << endl;
}
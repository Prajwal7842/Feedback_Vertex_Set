#include <bits/stdc++.h>
using namespace std;

int select_highest_degree_vertex(map<int, multiset<int>> g, set<int> f) {
  set<pair<int, int>> s;
  for(auto i: g) {
    if(f.count(i.first)==0){
      s.insert(make_pair(i.second.size(), i.first));
    }
  }
  if (!s.empty())
    return s.rbegin()->second;
  throw "No elements to branch on.";
}
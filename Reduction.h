#pragma once
#include <bits/stdc++.h>
using namespace std;

void reduce(Graph& graph);
bool pruningRule(map<int, multiset<int>>& g, const set<int>& f, int K);
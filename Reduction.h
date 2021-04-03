#pragma once
#include <bits/stdc++.h>
using namespace std;

bool rule1(map<int, multiset<int>>& g);
bool rule1_5(map<int, multiset<int>>& g, set<int> &f, set<int> &sol);
bool rule2(map<int, multiset<int>>& g, set<int> &f, set<int> &sol);
bool rule3(map<int, multiset<int>>& g);
bool rule4(map<int, multiset<int>>& g, set<int> &f, set<int> &sol);
bool rule5(map<int, multiset<int>>& g, set<int> &f, set<int> &sol);

void reduce(Graph& graph);
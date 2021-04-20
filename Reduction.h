#pragma once
#include <bits/stdc++.h>
#include "Timer.h"
using namespace std;

void reduce(Graph& graph, RRTimeLog& time);
bool pruningRule(map<int, multiset<int>>& g, const set<int>& f, int K);
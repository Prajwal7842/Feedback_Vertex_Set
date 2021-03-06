#pragma once
#include <bits/stdc++.h>
using namespace std;

class Graph {
public:
	int K;
	map<int, multiset<int>> adjList;
	vector<pair<string, string>> edgeList;
	map<string, int> strToInt;
	map<int, string> intToStr;
	set<int> undeletableVertices;
	set<int> solution;

	void addEdge(string u, string v);

	void printGraph();
};
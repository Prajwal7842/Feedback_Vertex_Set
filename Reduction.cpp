#include <bits/stdc++.h>
#include "Graph.h"
#include "Reduction.h"
using namespace std;


bool rule1(map<int, multiset<int>>& g) {
	vector<int> removeVertex;
	for(auto i : g) {
		if((int)(i.second.size()) == 0) {
			removeVertex.push_back(i.first);
		}
		if((int)(i.second.size()) == 1) {
			removeVertex.push_back(i.first);
		}
	}
	if((int)(removeVertex.size()) == 0) return 0;
	for(int i : removeVertex) {
		g.erase(i);
	}
	for(auto &i : g) {
		for(int v : removeVertex) {
			i.second.erase(v);
		}
	}
	return 1;
}

bool rule2(map<int, multiset<int>>& g) {
	return 0;
}

bool rule3(map<int, multiset<int>>& g) {
	map<int, pair<int, int>> merge;
	for(auto i : g) {
		if((int)(i.second.size()) == 2) {
			set<int> st(i.second.begin(), i.second.end());
			if((int)(st.size()) == 2) { // Check if neighbours of degree 2 vertex are different.
				pair<int, int> neighbours = {-1, -1};
				auto v = st.begin();
				neighbours.first = *v;
				v ++;
				neighbours.second = *v;
				merge[i.first] = neighbours;
			}
		}
	}
	if(merge.size() == 0) return 0;
	for(auto i : merge) {
		g.erase(i.first);
		int u = i.second.first;
		int v = i.second.second;
		g[u].insert(v);
		g[v].insert(u);
	}
	return 1;
}

void reduce(Graph& graph) {
	// Main function.
	map<int, multiset<int>> g;
	for(auto i : graph.adjList) {
		for(int j : i.second)
			g[i.first].emplace(j);
	} 

	bool running = true;
	while(running) {
		running = rule1(g);
	}

	running = true;
	while(running) {
		running = rule2(g);
	}

	running = true;
	while(running) {
		running = rule3(g);
	}



}
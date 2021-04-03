#include <bits/stdc++.h>
#include "Graph.h"
#include "Reduction.h"
using namespace std;


void delete_vertex(map<int, multiset<int>>& g, int u) {
	g.erase(u);
	for(auto i: g){
		i.second.erase(u);
	}
}

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
		delete_vertex(g, i);
	}
	return 1;
}

// If there exists a vertex u not in F, which has a double edge with w which is in F, then remove u and reduce k by 1.
bool rule1_5(map<int, multiset<int>>& g, set<int> &f, set<int> &sol) {
	vector<int> removeVertex;
	for(auto i: g) {
		if(!f.count(i.first)) {
			set<int> st(i.second.begin(), i.second.end());
			for(auto j: st) {
				if(i.second.count(j) == 2) {
					if(f.count(j)){
						removeVertex.push_back(i.first);
						break;
					}
				}
			}
		}
	}
	for(auto i: removeVertex) {
		sol.insert(i);
		delete_vertex(g, i);
	}
	return 0;
}

bool isCyclicUtil(map<int, multiset<int>> g, set<int> f, int v, vector<bool> &visited, int parent) {
	visited[v] = true;
	for(auto i: g[v]) {
		// If not visted and a vertex of F.
		if(!visited[i] && f.count(i)>0) {
			if (isCyclicUtil(g, f, i, visited, v))
				return true;
		} else if(i!=parent){
			return true;
		}
	}
	return false;
}


// If there exists a vertex u not in F such that G[F ∪ {u}] contains a cycle, delete u and decrease k by one.
bool rule2(map<int, multiset<int>>& g, set<int> &f, set<int> &sol) {
	vector<int> removeVertex;
	// For every vertex, it checks if there exists a cycle formed with vertices of F.
	for(auto i: g) {
		vector<bool> visited(g.size(), false);
		if (isCyclicUtil(g, f, i.first, visited, -1)) {
			removeVertex.push_back(i.first);
		}
	}
	if((int)(removeVertex.size()) == 0) return 0;
	for(int i : removeVertex) {
		delete_vertex(g, i);
	}
	return 1;
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


// If there exists an edge e of multiplicity more than two, reduce its multiplicity to two.
bool rule4(map<int, multiset<int>>& g) {
	map<int, pair<int, int>> edgeDelete;
	for(auto i: g){
		set<int> st(i.second.begin(), i.second.end());
		for(auto j: st) {
			int count = i.second.count(j);
			if(count > 2) {
				pair<int, int> edgeCount = {-1, -1};
				edgeCount.first = j;
				edgeCount.second = count;
				edgeDelete[i.first] = edgeCount;
			}
		}
	}
	if(edgeDelete.size() == 0) return 0;
	for(auto i: edgeDelete) {
		// i.second.second is the count of the number of edges, so -2, means at max two will remain.
		for(int j=0; j<i.second.second-2; j++){
			g[i.first].erase(i.second.first);
		}
	}
	// Since this is the structure of the graph is not changed.
	return 0;
}

// If there exists a vertex u not in F incident to a double edge uw with d(w) ≤ 3, delete u and decrease k by one.
bool rule5(map<int, multiset<int>>& g, set<int> &f, set<int> &sol) {
	vector<int> removeVertex;
	for(auto i: g) {
		if(!f.count(i.first)) {
			set<int> st(i.second.begin(), i.second.end());
			for(auto j: st) {
				if(i.second.count(j) == 2) {
					if(g[j].size()<=3){
						removeVertex.push_back(i.first);
						break;
					}
				}
			}
		}
	}
	if((int)(removeVertex.size()) == 0) return 0;
	for(auto i: removeVertex) {
		sol.insert(i);
		delete_vertex(g, i);
	}
	return 1;
}

void reduce(Graph& graph) {
	// Main function.
	map<int, multiset<int>> g;
	set<int> f, sol;
	for(auto i : graph.adjList) {
		for(int j : i.second)
			g[i.first].emplace(j);
	} 
	for(auto i: graph.undeletableVertices) {
		f.insert(i);
	}
	for(auto i: graph.solution) {
		sol.insert(i);
	}

	bool running = true;
	while(running) {
		running = rule1(g);
	}

	running = true;
	while(running) {
		running = rule1_5(g, f, sol);
	}

	running = true;
	while(running) {
		running = rule2(g, f, sol);
	}

	running = true;
	while(running) {
		running = rule3(g);
	}

	running = true;
	while(running) {
		running = rule4(g);
	}

	running = true;
	while(running) {
		running = rule5(g, f, sol);
	}
}
#include <bits/stdc++.h>
#include "Graph.h"
#include "selector.h"
using namespace std;

set<int> getNeighbours(map<int, multiset<int>>& g, int vertex) {
	// Helper function for returning neighbours of a vertex.
	set<int> neigh;
	for(auto i : g[vertex]) {
		neigh.emplace(i);
	}
	return neigh;
}	

set<int> getU(const set<int>& neigh, const set<int>& f) {
	// Returns the set of vertices to be contracted in second case of branching.
	set<int> U;
	for(auto i : neigh) {
		if(f.count(i)) {
			U.emplace(i);
		}
	}
	return U;
}

int getNewVertex(map<int, multiset<int>>& g) {
	// Returns  a new vertex to be used to represent the set U.
	int maxxV = 0;
	for(auto i : g) maxxV = max(maxxV, i.first);
	return maxxV + 1;
}

int contractGraph(map<int, multiset<int>>& g, set<int> U) {
	// Create a new graph by combining all vertices belonging to U into a new vertex u_dash. Perform necessary changes in F.
	int u_dash = getNewVertex(g);
	map<int, multiset<int>> g_dash;
	for(auto i : g) {
		for(auto j : i.second) {
			int u = i.first;
			int v = j;
			if(U.count(u) && U.count(v)) {
				continue;
			}
			if(U.count(u)) {
				g_dash[u_dash].insert(v);
				g_dash[v].insert(u_dash);
				continue;
			}
			if(U.count(v)) {
				g_dash[u_dash].insert(u);
				g_dash[u].insert(u_dash);
				continue;
			}
			g_dash[u].insert(v);
			g_dash[v].insert(u);
		}
	}
	g = g_dash;
	return u_dash;
}

set<int> branch(map<int, multiset<int>>& g, set<int> f, int k, bool& found) {
	set<int> solution;
	if(k >= 0) {
		if((int)(g.size()) == 0) {
			found = 1;
		}
		else {
			found = 0;
		}
		return solution;
	}
	int chosenVertex = select_highest_degree_vertex(g, f);
	set<int> neighbourU = getNeighbours(g, chosenVertex);
	set<int> U = getU(neighbourU, f);

	// 1st Branch : Assume chosenVertex is a part of solution.
	solution.emplace(chosenVertex);
	map<int, multiset<int>> copy_g = g;
	copy_g.erase(chosenVertex);
	for(auto &i : copy_g) {
		i.second.erase(chosenVertex);
	}
	// Maybe we have to perform reduction here again for safety. Can be added later.
	set<int> sol = branch(copy_g, f, k-1, found);
	if(found) {
		for(auto i : sol) solution.emplace(i);
		return solution;
	}
	else {
		solution.erase(chosenVertex);
	}

	// 2nd Branch : Assume chosenVertex is not part of solution.
	map<int, multiset<int>> g_dash = g;
	set<int> f_dash = f;
	U.emplace(chosenVertex);
	int u_dash = contractGraph(g_dash, U);
	for(auto i : U) f_dash.erase(i);
	f_dash.insert(u_dash);
	set<int> sol2 = branch(g_dash, f_dash, k, found);
	if(found) {
		for(auto i : sol) solution.emplace(i);
		return solution;
	}
	return solution;
}


void solve(Graph &graph) {
	bool solnFound = 0;
	set<int> solution = branch(graph.adjList, graph.undeletableVertices, graph.K, solnFound);
	if(solnFound) {
		printf("found sol\n");
		graph.printGraph();
		// Print Solution
	}
	else {
		printf("not found sol\n");
		// Print No Solution Found with current K. 
		graph.printGraph();
	}
}
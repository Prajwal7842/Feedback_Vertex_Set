#include <bits/stdc++.h>
#include "Graph.h"
#include "Reduction.h"
#include "selector.h"
#include "Timer.h"
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

void printset(set<int> s){
	// Utility function to print solution set.
	for(auto i: s){
		cout<<i<<" ";
	}
	cout<<endl;
}

bool branch(map<int, multiset<int>> g, set<int> f, int k, set<int>& solution, RRTimeLog &time2) {
	/* Main Branching function that generates a solution or determines that solution does not exists */

	auto now = high_resolution_clock::now();
	auto total_duration = duration_cast<minutes>(now - time2.start_time);
	if(total_duration.count() >= 30) {
		cout << "TIMEOUT\n";
		exit(0);
	}

	Graph temp;
	temp.adjList = g;
	temp.undeletableVertices = f;
	temp.solution = solution;
	temp.K = k;

	// Apply reduction rules
	reduce(temp, time2);

	g = temp.adjList;
	f = temp.undeletableVertices;
	solution = temp.solution;
	k = temp.K;

	if(pruningRule(g, f, k)) {
		return 0;
	}

	if(k >= 0) {
		if((int)(g.size()) == 0) {
			return 1;
		}
	} else {
		return 0;
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
	if(branch(copy_g, f, k-1, solution, time2)) {
		return 1;
	} else {
		// Back track changes, remove vertex from soln, and since we used a copy of G, no changes to it.
		solution.erase(chosenVertex);
	}

	// 2nd Branch : Assume chosenVertex is not part of solution.
	map<int, multiset<int>> g_dash = g;
	set<int> f_dash = f;
	U.emplace(chosenVertex);
	int u_dash = contractGraph(g_dash, U);
	for(auto i : U) f_dash.erase(i);
	f_dash.insert(u_dash);
	if(branch(g_dash, f_dash, k, solution, time2)) {
		return 1;
	}
	return 0;
}


bool solve(Graph &graph, RRTimeLog &time1, RRTimeLog &time2) { 
	/* Function to call branching function with initial params */
	reduce(graph, time1);
	return branch(graph.adjList, graph.undeletableVertices, graph.K, graph.solution, time2);
}
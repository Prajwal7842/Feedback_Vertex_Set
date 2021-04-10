#include <bits/stdc++.h>
#include "Graph.h"
#include "Reduction.h"
using namespace std;


void delete_vertex(map<int, multiset<int>>& g, int u) {
	g.erase(u);
	for(auto &i: g){
		i.second.erase(u);
	}
}

// If there exists a vertex u of degree at most one, delete u.
bool rule1(map<int, multiset<int>>& g) {
	// If there exists a vertex u of degree at most one, delete u.
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
bool rule1_5(map<int, multiset<int>>& g, set<int> &f, set<int> &sol, int &k) {
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
		k--;
		delete_vertex(g, i);
	}
	return 0;
}

bool isCyclicUtil(map<int, multiset<int>> g, set<int> f, int v, vector<bool> &visited, int parent, int start_vertex) {
	visited[v] = true;
	for(auto i: g[v]) {
		// If not visted and a vertex of F.
		if(!visited[i] && f.count(i)>0) {
			if (isCyclicUtil(g, f, i, visited, v, start_vertex))
				return true;
		} else if(i!=parent && i==start_vertex){
			return true;
		}
	}
	return false;
}


// If there exists a vertex u not in F such that G[F ∪ {u}] contains a cycle, delete u and decrease k by one.
bool rule2(map<int, multiset<int>>& g, set<int> &f, set<int> &sol, int &k) {
	vector<int> removeVertex;
	// For every vertex, it checks if there exists a cycle formed with vertices of F.
	for(auto i: g) {
		if(f.count(i.first)==0) {
			vector<bool> visited(g.size(), false);
			if (isCyclicUtil(g, f, i.first, visited, -1, i.first)) {
				removeVertex.push_back(i.first);
			}
		}
	}
	if((int)(removeVertex.size()) == 0) return 0;
	for(int i : removeVertex) {
		sol.insert(i);
		k--;
		delete_vertex(g, i);
	}
	return 1;
}

// If there exists a vertex u of degree two, delete u and add an edge connecting its two endpoints.
bool rule3(map<int, multiset<int>>& g) {
	// If there exists a vertex u of degree two, delete u and add an edge connecting its two endpoints.
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
				break;
			}
		}
	}
	if(merge.size() == 0) return 0;
	// for(auto i : merge) {
	// 	cout << i.first << " -> " << i.second.first << " " << i.second.second << endl;
	// }
	auto v = *merge.begin();
	g.erase(v.first);
	g[v.second.first].emplace(v.second.second);
	g[v.second.second].emplace(v.second.first);
	g[v.second.first].erase(v.first);
	g[v.second.second].erase(v.first);
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
			auto itr = g[i.first].find(i.second.first);
			if(itr!=g[i.first].end()){
					g[i.first].erase(itr);
			}
		}
	}
	// Since this is the structure of the graph is not changed.
	return 0;
}

// If there exists a vertex u not in F incident to a double edge uw with d(w) ≤ 3, delete u and decrease k by one.
bool rule5(map<int, multiset<int>>& g, set<int> &f, set<int> &sol, int &k) {
	set<int> removeVertex;
	for(auto i: g) {
		if(!f.count(i.first)) {
			set<int> st(i.second.begin(), i.second.end());
			for(auto j: st) {
				if(removeVertex.count(j) == 0) {
					if(i.second.count(j) == 2) {
						if(g[j].size()<=3){
							removeVertex.insert(i.first);
							break;
						}
					}
				}
			}
		}
	}
	if((int)(removeVertex.size()) == 0) return 0;
	for(auto i: removeVertex) {
		sol.insert(i);
		k--;
		delete_vertex(g, i);
	}
	return 1;
}

void print_reduced_graph(map<int, multiset<int>>& g, set<int> &f, set<int> &sol) {
	cout << "Reduced Graph\n";
	for(auto i : g) {
		for(auto j : i.second){
			cout << i.first << " " << j << "\n";
		}
	}
	cout << "\nF:\n";
	for(auto i : f){
		cout << i << "\n";
	}
	cout <<"\nSol:\n";
	for(auto i : sol){
		cout << i << "\n";
	}
}

int getMaxDegree(map<int, multiset<int>>& g, const set<int>& f) {
	// Returns the max degree among set of deletable vertices.
	set<int> vertex;
	for(auto i : g) {
		vertex.emplace(i.first);
	}
	for(auto i : f) {
		vertex.erase(i);
	}
	int D = 0;
	for(auto i : vertex) {
		D = max(D, (int)(g[i].size()));
	}
	return D;
}

bool checkReductionToMatroid(map<int, multiset<int>>& g, const set<int>& f) {
	int D = getMaxDegree(g, f);
	if(D <= 3) {
		return 1;
	}
	return 0;
}

bool pruningRule(map<int, multiset<int>>& g, const set<int>& f, int K) {
	// Each vertex has atleast degree 3. This rule is used for existence of a solution for a particular value of K(parameter).
	if(K < 0) return 0;
	int D = getMaxDegree(g, f);
	int sumOfDegree = 0;
	for(auto i : f) {
		sumOfDegree += ((int)(g[i].size()) - 2);
	}
	bool isPossible = (((K * D) - sumOfDegree) < 0);
	return isPossible;
}

bool solveMatroid(map<int, multiset<int>>& g, const set<int>& f, const set<int>& sol) {
	cout << "Solution using Matroid Matching : " << endl;
	for(auto i : g) {
		cout << i.first << endl;
	}
	cout << endl;
	cout << "F :" << endl;
	for(auto i : f) {
		cout << i << endl;
	}
	cout << endl;
	cout << "Sol :" << endl;
	for(auto i : sol) {
		cout << i << endl;
	} 
	return 1;
}

void reduce(Graph& graph) {

	bool running = true;
	while(running) {
		running = rule1(graph.adjList);
	}

	running = true;
	while(running) {
		running = rule1_5(graph.adjList, graph.undeletableVertices, graph.solution, graph.K);
	}

	running = true;
	while(running) {
		running = rule2(graph.adjList, graph.undeletableVertices, graph.solution, graph.K);
	}

	running = true;
	while(running) {
		running = rule3(graph.adjList);
	}
	// print_reduced_graph(g, f, sol);

	running = true;
	while(running) {
		running = rule4(graph.adjList);
	}

	running = true;
	while(running) {
		running = rule5(graph.adjList, graph.undeletableVertices, graph.solution, graph.K);
	}

	// print_reduced_graph(g, f, sol);

	bool possible = checkReductionToMatroid(graph.adjList, graph.undeletableVertices);
	if(possible == 1) {
		// Solve using Matroid matching and exit and program.
		cout << endl << "---------------------" << endl;
		solveMatroid(graph.adjList, graph.undeletableVertices, graph.solution);
		// Print and exit solution.
		exit(0);
	}

	bool solutionPossible = pruningRule(graph.adjList, graph.undeletableVertices, graph.K);
	if(solutionPossible == 0) {
		// Print Solution Not Possible and exit.
		printf("No solution.\n");
		exit(0);
	}
}

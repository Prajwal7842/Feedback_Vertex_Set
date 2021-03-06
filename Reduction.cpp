#include <bits/stdc++.h>
#include "Graph.h"
#include "Reduction.h"
#include "Timer.h"
using namespace std;


void delete_vertex(map<int, multiset<int>>& g, int u) {
	/* Function to delete a vertex from a graph and all edges assosciated with the given vertex */
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
	/* Check if the input graph contains a cycle or not */
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
			vector<bool> visited(g.rbegin()->first + 1, false);
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
	/* Utility function to print the graph */
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
	/* Checks if the graph can be solved in poly time using matroid matching */
	int D = getMaxDegree(g, f);
	if(D <= 3) {
		return 1;
	}
	return 0;
}

// If return True then no solution.
bool pruningRule(map<int, multiset<int>>& g, const set<int>& f, int K) {
	// Each vertex has atleast degree 3. This rule is used for existence of a solution for a particular value of K(parameter).
	if(K < 0) return true;
	int D = getMaxDegree(g, f);
	int sumOfDegree = 0;
	for(auto i : f) {
		sumOfDegree += ((int)(g[i].size()) - 2);
	}
	return (((K * D) - sumOfDegree) < 0);
}

int countEdges(map<int, multiset<int>> g) {
	/* Utility function to count the total number of edges */
	int count = 0;
	for(auto i: g) {
		count += i.second.size();
	}
	return count;
}

void reduce(Graph& graph, RRTimeLog &time) {
	/* Function to apply reduction rule to the graph exhaustively */

	bool changes_to_graph = true;
	while(changes_to_graph){ // Main Loop
		auto now = high_resolution_clock::now();
		auto total_duration = duration_cast<minutes>(now - time.start_time);
		if(total_duration.count() >= 30) {
			cout<<"TIMEOUT\n";
			exit(0);
		}
		changes_to_graph = false;
		bool running;
		std::chrono::_V2::system_clock::time_point start, end;
		int initialVertex, finalVertex, initialEdges, finalEdges;

		// RR1 begin
		running = true;
		start = high_resolution_clock::now();
		initialVertex = graph.adjList.size();
		initialEdges = countEdges(graph.adjList);
		while(running) {
			running = rule1(graph.adjList);
			changes_to_graph = changes_to_graph || running;
		}
		finalVertex = graph.adjList.size();
		finalEdges = countEdges(graph.adjList);
		end = high_resolution_clock::now();
		time.time_rr1 += duration_cast<milliseconds>(end - start);
		time.vertex_reduced_1 += initialVertex - finalVertex;
		time.edge_reduced_1 += initialEdges - finalEdges;
		// RR1 end

		// RR1_5 begin
		running = true;
		start = high_resolution_clock::now();
		initialVertex = graph.adjList.size();
		initialEdges = countEdges(graph.adjList);
		while(running) {
			running = rule1_5(graph.adjList, graph.undeletableVertices, graph.solution, graph.K);
			changes_to_graph = changes_to_graph || running;
		}
		finalVertex = graph.adjList.size();
		finalEdges = countEdges(graph.adjList);
		end = high_resolution_clock::now();
		time.time_rr1_5 += duration_cast<milliseconds>(end - start);
		time.vertex_reduced_1_5 += initialVertex - finalVertex;
		time.edge_reduced_1_5 += initialEdges - finalEdges;
		// RR1_5 end

		// RR2 begin
		running = true;
		start = high_resolution_clock::now();
		initialVertex = graph.adjList.size();
		initialEdges = countEdges(graph.adjList);
		while(running) {
			running = rule2(graph.adjList, graph.undeletableVertices, graph.solution, graph.K);
			changes_to_graph = changes_to_graph || running;
		}
		finalVertex = graph.adjList.size();
		finalEdges = countEdges(graph.adjList);
		end = high_resolution_clock::now();
		time.time_rr2 += duration_cast<milliseconds>(end - start);
		time.vertex_reduced_2 += initialVertex - finalVertex;
		time.edge_reduced_2 += initialEdges - finalEdges;
		// RR2 end

		// RR3 begin
		running = true;
		start = high_resolution_clock::now();
		initialVertex = graph.adjList.size();
		initialEdges = countEdges(graph.adjList);
		while(running) {
			running = rule3(graph.adjList);
			changes_to_graph = changes_to_graph || running;
		}
		finalVertex = graph.adjList.size();
		finalEdges = countEdges(graph.adjList);
		end = high_resolution_clock::now();
		time.time_rr3 += duration_cast<milliseconds>(end - start);
		time.vertex_reduced_3 += initialVertex - finalVertex;
		time.edge_reduced_3 += initialEdges - finalEdges;
		// RR3 end

		// RR4 begin
		running = true;
		start = high_resolution_clock::now();
		initialVertex = graph.adjList.size();
		initialEdges = countEdges(graph.adjList);
		while(running) {
			running = rule4(graph.adjList);
			changes_to_graph = changes_to_graph || running;
		}
		finalVertex = graph.adjList.size();
		finalEdges = countEdges(graph.adjList);
		end = high_resolution_clock::now();
		time.time_rr4 += duration_cast<milliseconds>(end - start);
		time.vertex_reduced_4 += initialVertex - finalVertex;
		time.edge_reduced_4 += initialEdges - finalEdges;
		// RR4 end

		//RR5 begin
		running = true;
		start = high_resolution_clock::now();
		initialVertex = graph.adjList.size();
		initialEdges = countEdges(graph.adjList);
		while(running) {
			running = rule5(graph.adjList, graph.undeletableVertices, graph.solution, graph.K);
			changes_to_graph = changes_to_graph || running;
		}
		finalVertex = graph.adjList.size();
		finalEdges = countEdges(graph.adjList);
		end = high_resolution_clock::now();
		time.time_rr5 += duration_cast<milliseconds>(end - start);
		time.vertex_reduced_5 += initialVertex - finalVertex;
		time.edge_reduced_5 += initialEdges - finalEdges;
		// RR5 end
	}

	if(!time.matroid_matching_completed) {
		bool possible = checkReductionToMatroid(graph.adjList, graph.undeletableVertices);
		if(possible == 1) {
			time.matroid_matching_time = high_resolution_clock::now();
			time.matroid_matching_completed = true;
		}
	}
}

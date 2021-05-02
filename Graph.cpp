#include <bits/stdc++.h>
#include "Graph.h"
using namespace std;

void Graph :: addEdge(string u, string v) {
	/* Function to add edge in the adjacency list of the graph */
	edgeList.push_back({u, v});
}

void Graph :: printGraph() {
	/* Helper function to print the graph and other essentials */
	for(auto i : adjList) { 
		cout << i.first << " -> ";
		for(auto j : i.second) {
			cout << j << " ";
		}
		cout << endl;
	}

	cout<<"F:\n";
	for(auto i: undeletableVertices) {
		cout<<i<<"\n";
	}

	cout<<"Sol:\n";
	for(auto i: solution) {
		cout<<i<<"\n";
	}

	cout<<"K:"<<K<<endl;
}
	
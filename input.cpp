#include <bits/stdc++.h>
#include "input.h"
using namespace std;

void readInput(Graph& graph, string inputFile) {
	ifstream file; 
	file.open(inputFile);

	string vertex1, vertex2;
	vector<pair<string, string>> edgeSet;
	// Make Edge Set.
	while(file >> vertex1 >> vertex2) {
		graph.addEdge(vertex1, vertex2);
	}

	file.close();

	// Assign a number to each unique vertex. Map string to int.
	set<string> done;
	int vertexNumber = 1;

	for(auto i : graph.edgeList) {
		if(done.find(i.first) == done.end()) {
			done.insert(i.first);
			graph.strToInt[i.first] = vertexNumber;
			graph.intToStr[vertexNumber] = i.first;
			vertexNumber ++;
		}
		if(done.find(i.second) == done.end()) {
			done.insert(i.second);
			graph.strToInt[i.second] = vertexNumber;
			graph.intToStr[vertexNumber] = i.second;
			vertexNumber ++;
		}
	}

	// Create Graph as adjancency list representation.
	for(auto i : graph.edgeList) {
		int u = graph.strToInt[i.first];
		int v = graph.strToInt[i.second];
		graph.adjList[u].push_back(v);
		graph.adjList[v].push_back(u);
	}
}
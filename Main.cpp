#include <bits/stdc++.h>
#include "input.h"
#include "Graph.h"
#include "Reduction.h"
using namespace std;



int32_t main(int argc, char*argv[]) {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);

	if(argc != 2) {
		cout << "Usage : " << argv[0] << " <InputFileName>" << endl;
		return 0;
	}

	Graph graph;
	readInput(graph, argv[1]);

	reduce(graph);

	return 0;
}
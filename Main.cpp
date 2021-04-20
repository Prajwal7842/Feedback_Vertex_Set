#include <bits/stdc++.h>
#include "input.h"
#include "Graph.h"
#include "Reduction.h"
#include "BranchingSolution.h"
#include "checker.h"
#include "Timer.h"
using namespace std;



int32_t main(int argc, char*argv[]) {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);


	if(argc != 3) {
		cout<<"Usage : "<<argv[0]<<" <InputFileName>"<<"<value K>"<<endl;
		return 0;
	}

	Graph graph;
	graph.K = atoi(argv[2]);;
	readInput(graph, argv[1]);

	auto start = high_resolution_clock::now();
	RRTimeLog time1, time2;
	time2.start_time = start;
	if(!solve(graph, time1, time2)) {
		cout<<"false"<<endl;
	} else {
		auto end = high_resolution_clock::now();
		if(!time2.matroid_matching_completed){
			time2.matroid_matching_time = end;
		}
		if(check(graph.adjList, graph.solution)) {
			auto duration = duration_cast<milliseconds>(end - start);
			cout<<duration.count()<<endl;
			duration = duration_cast<milliseconds>(time2.matroid_matching_time - time2.start_time);
			cout<<duration.count()<<endl;
			cout<<"Initial RR: "<<endl;
			time1.logMsg();
			cout<<"Branch RR: "<<endl;
			time2.logMsg();
		}
	}
	return 0;
}
#include <bits/stdc++.h>
using namespace std;

bool dfs(map<int, multiset<int>> g, int vertex, set<int>&visited, int parent) {
   visited.insert(vertex);
   for(auto v: g[vertex]) {
      if(v == parent)
        continue;
      if(visited.find(v) != visited.end())
        return true;
      if(dfs(g, v, visited, vertex))
        return true;
   }
   return false;
}

bool hasCycle(map<int, multiset<int>> g) {
   set<int> visited;      
   for(auto v: g) {
      if(visited.find(v.first) != visited.end())   
         continue;
      if(dfs(g, v.first, visited, -1)) {   
         return true;
      }
   }
   return false;
}

void check(map<int, multiset<int>> adjList, set<int> sol) {
  for(auto u: sol){
    adjList.erase(u);
    for(auto &i: adjList){
      i.second.erase(u);
    }
  }
  if(hasCycle(adjList)){
    printf("Wrong solution\n");
  } else {
    printf("Correct solution\n");
  }
}

// int32_t main(int argc, char*argv[]) {
// 	ios_base::sync_with_stdio(0);
// 	cin.tie(0);
// 	cout.tie(0);

// 	if(argc != 3) {
// 		cout << "Usage : " << argv[0] << " <GraphFileName>" << "<SolutionFileName>" << endl;
// 		return 0;
// 	}

// 	Graph graph;
// 	graph.K = 5;
// 	readInput(graph, argv[1]);

//   ifstream file; 
// 	file.open(argv[2]);
//   int vertex;
//   set<int> s;
//   while(file>>vertex) {
//     s.insert(vertex);
//   }
//   check(graph.adjList, s);
// 	return 0;
// }
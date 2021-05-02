#include <bits/stdc++.h>
#include "Timer.h"

using namespace std::chrono;
using namespace std;



void RRTimeLog :: logMsg() {
  // Function to log data into the file. 
  cout<<time_rr1.count()<<","<<vertex_reduced_1<<","<<edge_reduced_1<<",";
  cout<<time_rr1_5.count()<<","<<vertex_reduced_1_5<<","<<edge_reduced_1_5<<",";
  cout<<time_rr2.count()<<","<<vertex_reduced_2<<","<<edge_reduced_2<<",";
  cout<<time_rr3.count()<<","<<vertex_reduced_3<<","<<edge_reduced_3<<",";
  cout<<time_rr4.count()<<","<<vertex_reduced_4<<","<<edge_reduced_4<<",";
  cout<<time_rr5.count()<<","<<vertex_reduced_5<<","<<edge_reduced_5<<",";
}
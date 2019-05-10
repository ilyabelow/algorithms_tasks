#include <stack>

#include "Salesman.h"
#include <algorithm>
#include <numeric>

//Finds 2-approximation for polynomial time
const ListGraph Approximation2(const ListGraph &orig_graph) {
  //First, find MST
  ListGraph mst = orig_graph.FindMST();
  //then go through MST with DFS
  std::stack<int> dfs_stack;
  std::vector<char> color(orig_graph.VerticesCount(), 0);
  //sequence of vertices that is the answer to our problem
  std::vector<int> trace;
  dfs_stack.push(0);
  color[0] = 1;
  while (!dfs_stack.empty()) {
    int current = dfs_stack.top();
    dfs_stack.pop();
    trace.push_back(current);
    for (auto &edge : mst.GetNextEdges(current)) {
      if (color[edge.to] == 0) {
        dfs_stack.push(edge.to);
        color[edge.to] = 1;
      }
    }
  }
  //Constructing a graph from trace
  ListGraph path(orig_graph.VerticesCount());
  for (int i = 0; i < orig_graph.VerticesCount() - 1; ++i) {
    path.EmplaceEdge(trace[i], trace[i + 1], orig_graph.GetWeight(trace[i], trace[i + 1]));
  }
  //last to first
  path.EmplaceEdge(trace[orig_graph.VerticesCount() - 1],
                   trace[0],
                   orig_graph.GetWeight(trace[orig_graph.VerticesCount() - 1], trace[0]));
  return path;
}

//Finds 1.5-approximation for... O(n!). But the constant is lower!
const ListGraph Approximation1_5(const ListGraph &orig_graph) {
  return Optimum(orig_graph);
}



//Finds optimum for O(n!)
const ListGraph Optimum(const ListGraph &orig_graph) {
  //here optimum will be stored
  ListGraph optimum_path(orig_graph.VerticesCount());
  //making optimum a maximum
  for (int i = 0; i < optimum_path.VerticesCount(); ++i) {
    for (int j = 0; j < optimum_path.VerticesCount(); ++j) {
      if (i == j) {
        continue;
      }
      optimum_path.EmplaceEdge(i, j, 1000);  //TODO is 1000 enogh?
    }
  }
  //sequence of vertices that we are going along
  std::vector<int> trace(orig_graph.VerticesCount());
  std::iota(trace.begin(), trace.end(), 0);
  //trying all permutations
  do {
    ListGraph path(trace.size());
    for (int i = 0; i < trace.size() - 1; ++i) {
      path.EmplaceEdge(trace[i], trace[i + 1], orig_graph.GetWeight(trace[i], trace[i + 1]));
    }
    //last to first
    path.EmplaceEdge(trace[trace.size() - 1], trace[0], orig_graph.GetWeight(trace[trace.size() - 1], trace[0]));
    //find minimum
    if (path.GraphWeight() < optimum_path.GraphWeight()) {
      optimum_path = std::move(path);
    }
  } while (std::next_permutation(trace.begin(), trace.end()));
  return optimum_path;
}

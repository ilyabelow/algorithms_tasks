//
// Created by bumazhka on 28.04.19.
//

#include <stack>

#include "2Approximation.h"

const ListGraph Salesman2(const ListGraph &orig_graph) {
  ListGraph mst = orig_graph.FindMST();
  std::stack<int> dfs_stack;
  std::vector<char> color(orig_graph.VerticesCount(), 0);
  std::vector<int> trace;
  dfs_stack.push(0);
  while (!dfs_stack.empty()) {
    int current = dfs_stack.top();
    trace.push_back(current);
    dfs_stack.pop();
    for (auto &edge : mst.GetNextEdges(current)) {
      if (color[edge.to] == 0) {
        dfs_stack.push(edge.to);
        color[edge.to] = 1;
      }
    }
  }
  ListGraph path(orig_graph.VerticesCount());
  for (int i = 0; i < orig_graph.VerticesCount() - 1; ++i) {
    path.EmplaceEdge(trace[i], trace[i + 1], orig_graph.GetWeight(trace[i], trace[i + 1]));
  }
  path.EmplaceEdge(trace[orig_graph.VerticesCount() - 1],
                   trace[0],
                   orig_graph.GetWeight(trace[orig_graph.VerticesCount() - 1], trace[0]));
  return path;

}
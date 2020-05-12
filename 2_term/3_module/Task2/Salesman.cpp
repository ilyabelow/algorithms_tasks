#include <stack>

#include "Salesman.h"
#include <algorithm>
#include <numeric>
#include <iostream>

const ListGraph Approximation2(const ListGraph &orig_graph);
const ListGraph Approximation1_5(const ListGraph &orig_graph);
const ListGraph MinimumPerfectMatching(const ListGraph &orig_graph);
const ListGraph Optimum(const ListGraph &orig_graph);

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
    color[current] = 2;
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

//Finds 1.5-approximation for O(m!) m < n so it works significantly faster then Optimum solution
const ListGraph Approximation1_5(const ListGraph &orig_graph) {
  //Finding euler graph as adding MPM to MST
  ListGraph euler_graph = orig_graph.FindMST();
  euler_graph.AppendGraph(MinimumPerfectMatching(euler_graph));
  //Find euler cycle
  std::vector<int> trace;
  std::stack<int> dfs_stack;
  dfs_stack.push(0);
  while (!dfs_stack.empty()) {
    int current = dfs_stack.top();
    auto next = euler_graph.GetNextEdges(current);
    //Written from pseudo-code from https://neerc.ifmo.ru
    //(I don't really get it why this algorithm works)
    if (!next.empty()) {
      dfs_stack.push(next[0].to);
      euler_graph.DeleteEdge(next[0].to, current);
      euler_graph.DeleteEdge(current, next[0].to);
    } else {
      //Do not add vertex twice (constructing hamiltonian from euclidean on the fly)
      bool was = false;
      for (auto vertex: trace) {
        if (vertex == current) {
          was = true;
          break;
        }
      }
      if (!was) {
        trace.push_back(current);
      }
      dfs_stack.pop();
    }
  }
  //Constructing graph from euclidean cycle
  ListGraph path(trace.size());
  for (int j = 0; j < trace.size() - 1; ++j) {
    path.EmplaceEdge(trace[j], trace[j + 1], orig_graph.GetWeight(trace[j], trace[j + 1]));
  }
  path.EmplaceEdge(trace[trace.size() - 1], trace[0], orig_graph.GetWeight(trace[trace.size() - 1], trace[0]));
  return path;
}

//Finds minimum perfect matching for O(m!) where m < n
const ListGraph MinimumPerfectMatching(const ListGraph &orig_graph) {
  //Find odd vertices
  std::vector<int> subset;
  for (int i = 0; i < orig_graph.VerticesCount(); ++i) {
    if (orig_graph.GetNextEdges(i).size() % 2 == 1) {
      subset.push_back(i);
    }
  }
  //here minimum matching will be stored
  ListGraph minimum_matching(orig_graph.VerticesCount());
  if (subset.empty()) {
    return minimum_matching;
  }
  //making optimum a maximum
  for (int i = 0; i < minimum_matching.VerticesCount(); ++i) {
    for (int j = 0; j < minimum_matching.VerticesCount(); ++j) {
      if (i == j) {
        continue;
      }
      minimum_matching.EmplaceEdge(i, j, 1000);
    }
  }
  double minimum_length = minimum_matching.GraphWeight();
  //trying all permutations
  do {
    //counting length
    double length = 0;
    for (int i = 0; i < subset.size() / 2 - 1; ++i) {
      length += orig_graph.GetWeight(subset[2 * i], subset[2 * i + 1]);
    }
    //only if length is less that cirrent min length, construct actual graph (costly operation)
    if (length >= minimum_length) {
      continue;
    }
    ListGraph path(orig_graph.VerticesCount());
    for (int i = 0; i < subset.size() / 2; ++i) {
      //weights will be equal
      double weight = orig_graph.GetWeight(subset[2 * i], subset[2 * i + 1]);
      path.EmplaceEdge(subset[2 * i], subset[2 * i + 1], weight);
      path.EmplaceEdge(subset[2 * i + 1], subset[2 * i], weight);
    }
    minimum_matching = std::move(path);
    minimum_length = length;
  } while (std::next_permutation(subset.begin(), subset.end()));
  return minimum_matching;
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
  double optimum_length = optimum_path.GraphWeight();
  //sequence of vertices that we are going along
  std::vector<int> trace(orig_graph.VerticesCount());
  std::iota(trace.begin(), trace.end(), 0);
  //trying all permutations
  do {
    //counting length
    double length = 0;
    for (int i = 0; i < trace.size() - 1; ++i) {
      length += orig_graph.GetWeight(trace[i], trace[i + 1]);
    }
    length += orig_graph.GetWeight(trace[trace.size() - 1], trace[0]);

    if (length >= optimum_length) {
      continue;
    }
    //if length is less that current optimum, only then construct actual graph
    ListGraph path(orig_graph.VerticesCount());
    for (int i = 0; i < trace.size() - 1; ++i) {
      path.EmplaceEdge(trace[i], trace[i + 1], orig_graph.GetWeight(trace[i], trace[i + 1]));
    }
    path.EmplaceEdge(trace[trace.size() - 1], trace[0], orig_graph.GetWeight(trace[trace.size() - 1], trace[0]));
    optimum_path = std::move(path);
    optimum_length = length;
  } while (std::next_permutation(trace.begin(), trace.end()));
  return optimum_path;
}

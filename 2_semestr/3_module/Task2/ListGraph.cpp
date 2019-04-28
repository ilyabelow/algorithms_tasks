//
// Created by bumazhka on 28.04.19.
//
#include <tuple>

#include <vector>
#include <algorithm>
#include "ListGraph.h"
#include "DSU.h"

FullEdge::FullEdge(int from, int to, double weight) : from(from), to(to), weight(weight) {}

//Functor for sorting edges
bool FullEdgeLess::operator()(const FullEdge &first, const FullEdge &second) const {
  return std::tie(first.weight, first.from, first.to) < std::tie(second.weight, second.from, second.to);
}

const std::vector<FullEdge> &ListGraph::GetFullEdges() const {
  return edge_list;
}

//Creating new edge
void ListGraph::EmplaceFullEdge(int from, int to, double weight) {
  //here we assume that V is {0, 1, ..., n-1} so amount of vertices is max vertex + 1
  vetices_count = std::max(vetices_count, std::max(from, to) + 1);
  edge_list.emplace_back(from, to, weight);
}

int ListGraph::VerticesCount() const {
  return vetices_count;
}

//Pushing back existing edge
void ListGraph::AddFullEdge(const FullEdge &edge) {
  edge_list.push_back(edge);
}

//Standard Kruskal's algorithm
const ListGraph ListGraph::FindMST() const {
  DSU dsu(VerticesCount());
  ListGraph mst; //returning value

  std::vector<FullEdge> edges = GetFullEdges();
  std::sort(edges.begin(), edges.end(), FullEdgeLess());

  for (auto edge : edges) {
    //If u and v are in different sets, union sets
    if (dsu.FindSet(edge.from) != dsu.FindSet(edge.to)) {
      mst.AddFullEdge(edge);
      dsu.UnionSets(edge.from, edge.to);
    }
  }
  return mst;
}

//Calculate total weight of the graph
double ListGraph::GraphWeight() const {
  double weight = 0;
  //Just sum up all the weights
  for (auto edge : GetFullEdges()) {
    weight += edge.weight;
  }
  return weight;
}

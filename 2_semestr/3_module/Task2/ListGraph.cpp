//
// Created by bumazhka on 28.04.19.
//
#include <tuple>

#include <vector>
#include <algorithm>
#include "ListGraph.h"
#include "DSU.h"

FullEdge::FullEdge(int from, int to, double weight) : from(from), to(to), weight(weight) {}

FullEdge::FullEdge(int from, const Edge &edge) : from(from), to(edge.to), weight(edge.weight) {}

ListGraph::ListGraph(int vertices) : adjacency_list(vertices) {}

//Functor for sorting edges
bool FullEdgeLess::operator()(const FullEdge &first, const FullEdge &second) const {
  return std::tie(first.weight, first.from, first.to) < std::tie(second.weight, second.from, second.to);
}

const std::vector<FullEdge> ListGraph::GetFullEdges() const {
  std::vector<FullEdge> edge_list;
  for (int i = 0; i < VerticesCount(); ++i) {
    for (auto edge : GetNextEdges(i)) {
      edge_list.emplace_back(i, edge);
    }
  }
  return edge_list;
}

void ListGraph::EmplaceEdge(int from, int to, double weight) {
  adjacency_list[from].emplace_back(to, weight);
}
void ListGraph::AddFullEdge(const FullEdge &edge) {
  EmplaceEdge(edge.from, edge.to, edge.weight);
}

int ListGraph::VerticesCount() const {
  return adjacency_list.size();
}

//Standard Kruskal's algorithm
const ListGraph ListGraph::FindMST() const {
  DSU dsu(VerticesCount());
  ListGraph mst(VerticesCount()); //returning value

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

const std::vector<Edge> &ListGraph::GetNextEdges(int vertex) const {
  return adjacency_list[vertex];
}
Edge::Edge(int to, double weight) : to(to), weight(weight){

}

//
// Created by bumazhka on 28.04.19.
//
#include <vector>

#pragma once
struct Edge {
  int to;
  double weight;
  Edge(int to, double weight);

};

//Convenient struct for storing edges that hold full pair from -> to
struct FullEdge {
  int from;
  int to;
  double weight;
  FullEdge(int from, int to, double weight);
  FullEdge(int from, const Edge &edge);
};

//Functor for sorting edges
struct FullEdgeLess {
  bool operator()(const FullEdge &first, const FullEdge &second) const;
};

//Graph that stores list of edges
class ListGraph {
 private:
  std::vector<std::vector<Edge>> adjacency_list;
 public:
  explicit ListGraph(int vertices);
  int VerticesCount() const;
  void EmplaceEdge(int from, int to, double weight);
  void AddFullEdge(const FullEdge &edge);

  const std::vector<FullEdge> GetFullEdges() const;
  const std::vector<Edge> &GetNextEdges(int vertex) const;
  const ListGraph FindMST() const;
  double GraphWeight() const;

};
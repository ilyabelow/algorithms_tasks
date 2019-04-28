//
// Created by bumazhka on 28.04.19.
//
#include <vector>

#pragma once
//Convenient struct for storing edges that hold full pair from -> to
struct FullEdge {
  int from;
  int to;
  int weight;
  FullEdge(int from, int to, int weight);
};

//Functor for sorting edges
struct FullEdgeLess {
  bool operator()(const FullEdge &first, const FullEdge &second) const;
};

//Graph that stores list of edges
class ArcGraph {
 private:
  //List of edges
  std::vector<FullEdge> edge_list;
  //For fast vertices amount checks
  int vetices_count = 0;
 public:
  ArcGraph() = default;
  int VerticesCount() const;
  void EmplaceFullEdge(int from, int to, int weight);
  void AddFullEdge(const FullEdge &edge);

  const std::vector<FullEdge> &GetFullEdges() const;
  const ArcGraph FindMST() const;
  int GraphWeight() const;

};
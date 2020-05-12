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
  FullEdge Reversed();
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
  void AddEdge(int from, const Edge &edge);
  void DeleteEdge(int from, int to);
  void AppendGraph(const ListGraph &other);

  const std::vector<FullEdge> GetFullEdges() const;
  const std::vector<Edge> &GetNextEdges(int vertex) const;
  const ListGraph FindMST() const;
  double GraphWeight() const;
  double GetWeight(int from, int to) const;
};
//
// Created by bumazhka on 28.04.19.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <tuple>

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
};

//Improved realisation from e-maxx. Uses heuristic of path shorting
class DSU {
 private:
  //Tree
  std::vector<int> parent;
  //Rank for heuristics of union by rank
  std::vector<int> rank;
 public:
  explicit DSU(int vertices);
  int FindSet(int element);
  void UnionSets(int first, int second);
};

//Find MST
const ArcGraph FindMST(const ArcGraph &graph);

//Counts weight of graph
int GraphWeight(const ArcGraph &graph);

int main() {
  int vertices, edges;
  std::cin >> vertices >> edges;
  //Reading the graph
  ArcGraph graph;
  for (int i = 0; i < edges; ++i) {
    int from, to, weight;
    std::cin >> from >> to >> weight;
    graph.EmplaceFullEdge(from - 1, to - 1, weight);
  }
  //Finding MST
  auto MST = FindMST(graph);
  std::cout << GraphWeight(MST);
  return 0;
}

//Standard Kruskal's algorithm
const ArcGraph FindMST(const ArcGraph &graph) {
  DSU dsu(graph.VerticesCount());
  ArcGraph mst; //returning value

  std::vector<FullEdge> edges = graph.GetFullEdges();
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
int GraphWeight(const ArcGraph &graph) {
  int weight = 0;
  //Just sum up all the weights
  for (auto edge : graph.GetFullEdges()) {
    weight += edge.weight;
  }
  return weight;
}

DSU::DSU(int vertices) : parent(vertices, -1), rank(vertices, 0) {} //-1 means root

//Finding set using trees
int DSU::FindSet(int element) {
  if (parent[element] == -1) {
    return element;
  }
  //Heuristic of path shorting
  parent[element] = FindSet(parent[element]);
  return parent[element];
}

//Union using trees
void DSU::UnionSets(int first, int second) {
  int left_tree = FindSet(first);
  int right_tree = FindSet(second);
  if (left_tree != right_tree) {
    //Shorter tree is tied up to higher tree
    if (rank[left_tree] < rank[right_tree]) {
      parent[left_tree] = right_tree;
    } else {
      parent[right_tree] = left_tree;
      if (rank[left_tree] == rank[right_tree]) {
        ++rank[left_tree];
      }
    }
  }
}

FullEdge::FullEdge(int from, int to, int weight) : from(from), to(to), weight(weight) {}

//Functor for sorting edges
bool FullEdgeLess::operator()(const FullEdge &first, const FullEdge &second) const {
  return std::tie(first.weight, first.from, first.to) < std::tie(second.weight, second.from, second.to);
}

const std::vector<FullEdge> &ArcGraph::GetFullEdges() const {
  return edge_list;
}

//Creating new edge
void ArcGraph::EmplaceFullEdge(int from, int to, int weight) {
  //here we assume that V is {0, 1, ..., n-1} so amount of vertices is max vertex + 1
  vetices_count = std::max(vetices_count, std::max(from, to) + 1);
  edge_list.emplace_back(from, to, weight);
}

int ArcGraph::VerticesCount() const {
  return vetices_count;
}

//Pushing back existing edge
void ArcGraph::AddFullEdge(const FullEdge &edge) {
  edge_list.push_back(edge);
}

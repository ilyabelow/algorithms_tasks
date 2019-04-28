#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <tuple>
#include <memory>

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

//Graph interface
struct IGraph {
  virtual ~IGraph() {}
  //Add new edge to graph
  virtual void EmplaceFullEdge(int from, int to, int weight) = 0;
  virtual void AddFullEdge(const FullEdge &edge) = 0;
  //Return amount of vertices
  virtual int VerticesCount() const = 0;
  //Return list of all edges
  virtual const std::vector<FullEdge> &GetFullEdges() const = 0;
};

//Graph that stores list of edges
class ArcGraph : public IGraph {
 private:
  //List of edges
  std::vector<FullEdge> edge_list;
  //For fast vertices amount checks
  int vetices_count = 0;
 public:
  ArcGraph() = default;
  int VerticesCount() const override;
  void EmplaceFullEdge(int from, int to, int weight) override;
  void AddFullEdge(const FullEdge &edge) override;

  const std::vector<FullEdge> &GetFullEdges() const override;
};

//Interface for DSU
struct IDSU {
  virtual ~IDSU() = default;
  //Determine set which holds an element
  virtual int FindSet(int element) = 0;
  //Union to sets that holds first and second
  virtual void UnionSets(int first, int second) = 0;
};

//Simple realisation of DSU that first came op to me. Check for O(1), union for O(V)
class NaiveDSU : public IDSU {
 private:
  //Which element corresponds to which set
  std::vector<int> element_to_set;
 public:
  explicit NaiveDSU(int vertices);
  int FindSet(int element) override;
  void UnionSets(int first, int second) override;
};

//Improved realisation from e-maxx. Uses heuristic of path shorting
class ImprovedDSU : public IDSU {
 private:
  //Tree
  std::vector<int> parent;
  //Rank for heuristics of union by rank
  std::vector<int> rank;
 public:
  ImprovedDSU(int vertices);
  int FindSet(int element) override;
  void UnionSets(int first, int second) override;
};

//Find MST
const std::shared_ptr<IGraph> FindMST(const IGraph &graph);

//Counts weight of graph
int GraphWeight(const IGraph &graph);

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
  std::cout << GraphWeight(*MST);
  return 0;
}

//Standard Kruskal's algorithm
const std::shared_ptr<IGraph> FindMST(const IGraph &graph) {
  auto dsu = std::make_shared<ImprovedDSU>(graph.VerticesCount());
  auto mst = std::make_shared<ArcGraph>(); //returning value

  std::vector<FullEdge> edges = graph.GetFullEdges();
  std::sort(edges.begin(), edges.end(), FullEdgeLess());

  for (auto edge : edges) {
    //If u and v are in different sets, union sets
    if (dsu->FindSet(edge.from) != dsu->FindSet(edge.to)) {
      mst->AddFullEdge(edge);
      dsu->UnionSets(edge.from, edge.to);
    }
  }
  return mst;
}

//Calculate total weight of the graph
int GraphWeight(const IGraph &graph) {
  int weight = 0;
  //Just sum up all the weights
  for (auto edge : graph.GetFullEdges()) {
    weight += edge.weight;
  }
  return weight;
}

ImprovedDSU::ImprovedDSU(int vertices) : parent(vertices, -1), rank(vertices, 0) {} //-1 means root

//Finding set using trees
int ImprovedDSU::FindSet(int element) {
  if (parent[element] == -1) {
    return element;
  }
  //Heuristic of path shorting
  parent[element] = FindSet(parent[element]);
  return parent[element];
}

//Union using trees
void ImprovedDSU::UnionSets(int first, int second) {
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

NaiveDSU::NaiveDSU(int vertices) : element_to_set(vertices) {
  //All vertices are placed in their own union
  std::iota(element_to_set.begin(), element_to_set.end(), 0);
}

int NaiveDSU::FindSet(int element) {
  return element_to_set[element];
}

//Naive union sets
void NaiveDSU::UnionSets(int first, int second) {
  int will_be_unioned = element_to_set[second];
  //Just move all vertices from second union to first union
  for (int &vertex : element_to_set) {
    if (vertex == will_be_unioned) {
      vertex = element_to_set[first];
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

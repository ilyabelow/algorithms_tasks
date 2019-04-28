#include <iostream>
#include <vector>
#include <list>
#include <set>

//Convenient struct for edges (substitute for std::pair)
struct Edge {
  int to;
  double weight;
  Edge(int to, double weight);
  ~Edge() = default;
};

//Upgraded List Graph with an ability to store edge weights
class ListGraph {
 private:

  std::vector<std::list<Edge>> adjacency_list_outcome;

 public:

  explicit ListGraph(int vertices_count);

  void EmplaceEdge(int from, int to, double weight);

  int VerticesCount() const;

  std::vector<Edge> GetNextEdges(int vertex) const;

};


bool findArbitrage(ListGraph &graph);

int main() {
  int currencies;
  std::cin >> currencies;
  ListGraph graph(currencies);
  //filling the graph
  double weight;
  for (int i = 0; i < currencies; ++i) {
    for (int j = 0; j < currencies; ++j) {
      if (j == i) {
        continue;
      }
      std::cin >> weight;
      if (weight == -1) {
        continue;
      }
      graph.EmplaceEdge(i, j, weight);
    }
  }
  std::cout << (findArbitrage(graph) ? "YES" : "NO");
  return 0;
}

Edge::Edge(int to, double weight) : to(to), weight(weight) {}

//Constructor for an empty graph
ListGraph::ListGraph(int vertices_count) : adjacency_list_outcome(vertices_count) {}

//Add new weighted edge to the graph
void ListGraph::EmplaceEdge(int from, int to, double weight) {
  adjacency_list_outcome[from].emplace_back(to, weight);
}

//Get amount of vertices in the graph
int ListGraph::VerticesCount() const {
  return adjacency_list_outcome.size();
}

//Get next vertices and weights of edges to them
std::vector<Edge> ListGraph::GetNextEdges(int vertex) const {
  return std::vector<Edge>(adjacency_list_outcome[vertex].begin(), adjacency_list_outcome[vertex].end());
}


//Find arbitrage using Ford-Bellman algorithm
bool findArbitrage(ListGraph &graph) {
  //Weight of each vertex
  std::vector<double> price(graph.VerticesCount(), 1);

  //Relaxed on previous iteration
  std::set<int> relaxed;
  //Relaxed on current iteration
  std::set<int> next_relaxed;
  std::vector<std::pair<int, double>> next_edges; //buffer
  //start from every vertex
  for (int i = 0; i < graph.VerticesCount(); ++i) {
    relaxed.insert(i);
  }
  //standart Ford-Bellman
  for (int i = 0; i <= graph.VerticesCount(); ++i) {
    next_relaxed = std::set<int>();
    for (auto vertex : relaxed) {
      for (auto next_edge : graph.GetNextEdges(vertex)) {
        if (price[vertex] * next_edge.weight > price[next_edge.to]) {
          price[next_edge.to] = price[vertex] * next_edge.weight;
          next_relaxed.insert(next_edge.to);
        }
      }
    }
    //moving relaxed
    relaxed = std::move(next_relaxed);
  }
  //we still have algorithm going => we have a "negative" cycle
  return !relaxed.empty();
}

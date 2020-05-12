#include <iostream>
#include <vector>

//Graph with adjacency matrix
class MatrixGraph {
 private:
  //Adjacency matrix
  std::vector<std::vector<int>> adjacency_matrix;
 public:
  explicit MatrixGraph(int vertices_count);

  ~MatrixGraph() = default;

  void AddEdge(int from, int to, int weight);

  int VerticesCount() const;

  std::vector<int> GetNextEdges(int vertex) const;

};

std::vector<std::vector<int>> Floyd(const MatrixGraph &graph);

int main() {
  int vertices, weight;
  std::cin >> vertices;
  //Input graph
  MatrixGraph graph(vertices);
  for (int i = 0; i < vertices; ++i) {
    for (int j = 0; j < vertices; ++j) {
      std::cin >> weight;
      graph.AddEdge(i, j, weight);
    }
  }
  //Getting distances
  std::vector<std::vector<int>> distances = Floyd(graph);
  //Output graph
  for (int i = 0; i < vertices; ++i) {
    for (int j = 0; j < vertices; ++j) {
      std::cout << distances[i][j] << ' ';
    }
    std::cout << std::endl;
  }
  return 0;
}

//Constructor of graph with n vertices and no edges
MatrixGraph::MatrixGraph(int vertices_count) : adjacency_matrix(vertices_count,
                                                                std::vector<int>(vertices_count, 0)) {}

//Add new weight of an edge
void MatrixGraph::AddEdge(int from, int to, int weight) {
  adjacency_matrix[from][to] = weight;
}

int MatrixGraph::VerticesCount() const {
  return adjacency_matrix.size();
}

//Returns vector of all the children of the vertex
std::vector<int> MatrixGraph::GetNextEdges(int vertex) const {
  return adjacency_matrix[vertex];
}

//Standard Floyd algorithm
std::vector<std::vector<int>> Floyd(const MatrixGraph &graph) {
  std::vector<std::vector<int>> distances;
  distances.reserve(graph.VerticesCount());
  //Extracting initial adjacency matrix
  for (int i = 0; i < graph.VerticesCount(); ++i) {
    distances.push_back(graph.GetNextEdges(i));
  }
  //Finding new best paths
  for (int k = 0; k < graph.VerticesCount(); ++k) {
    for (int i = 0; i < graph.VerticesCount(); ++i) {
      for (int j = 0; j < graph.VerticesCount(); ++j) {
        distances[i][j] = std::min(distances[i][j], distances[i][k] + distances[k][j]);
      }
    }
  }
  return distances;
}
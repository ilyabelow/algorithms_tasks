#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

//Convenient struct for storing edges
struct Edge {
  int to;
  int capacity;
  Edge(int to, int capacity);
};

//Graph with adjacency lists (graphs)
class ListGraph {
 private:
  std::vector<std::vector<Edge>> adjacency_list_outcome;
 public:
  //Empty graph
  explicit ListGraph(int vertex_count);
  //Copy constructor
  ListGraph(const ListGraph &other);
  //Emplce new edge
  void EmplaceEdge(int from, int to, int capacity);
  //Find edge and lower it's capacity by flow
  void LowerCapacity(int from, int to, int lower_by);
  //Amount of vertices
  int VerticesCount() const;
  //Vertices adjacent to given vertex
  std::vector<Edge> &GetNextEdges(int vertex);
  const std::vector<Edge> &GetNextEdges(int vertex) const;
};

//Find max flow through network
int FindMaxFlow(const ListGraph &graph, int source, int sink);
//Find shortest path. Returns empty vector if there is no valid path
std::vector<Edge> FindPath(const ListGraph &graph, int source, int sink);
//Find bottleneck of the path
int FindBottleneck(const std::vector<Edge> &path);
//Lower capacity of edges on the path in graph by value
void UpdateNetwork(ListGraph &graph, const std::vector<Edge> &path, int value);
//Count flow though graph
int CountFlow(const ListGraph &graph, const ListGraph &residual_network, int source);

int main() {
  int vertices, edges;
  std::cin >> vertices >> edges;
  //Reading the graph
  ListGraph graph(vertices);
  for (int i = 0; i < edges; ++i) {
    int from, to, weight;
    std::cin >> from >> to >> weight;
    //Adding forward and backward edge (with capacity 0)
    graph.EmplaceEdge(from - 1, to - 1, weight);
    graph.EmplaceEdge(to - 1, from - 1, 0);
  }
  std::cout << FindMaxFlow(graph, 0, vertices - 1);
  return 0;
}

int CountFlow(const ListGraph &graph, const ListGraph &residual_network, int source) {
  //Counting max flow -> flow from source
  int flow = 0;
  const auto &from_source_graph = graph.GetNextEdges(source);
  const auto &from_source_residual = residual_network.GetNextEdges(source);
  for (int i = 0; i < from_source_residual.size(); ++i) {
    flow += from_source_graph[i].capacity - from_source_residual[i].capacity;
  }
  return flow;
}

void UpdateNetwork(ListGraph &graph, const std::vector<Edge> &path, int value) {
  for (int i = 1; i < path.size(); ++i) {
    //Forward and backward edges are updated
    graph.LowerCapacity(path[i - 1].to, path[i].to, value);
    graph.LowerCapacity(path[i].to, path[i - 1].to, -value);
  }
}

int FindBottleneck(const std::vector<Edge> &path) {
  int bottleneck = INT32_MAX; //+inf
  for (auto edge : path) {
    bottleneck = edge.capacity < bottleneck ? edge.capacity : bottleneck;
  }
  return bottleneck;
}

std::vector<Edge> FindPath(const ListGraph &graph, int source, int sink) {
  std::vector<int> parent(graph.VerticesCount(), -1); //for path restoring
  std::vector<char> visited(graph.VerticesCount(), 0); //for not going backwards
  std::vector<int> residual_capacity(graph.VerticesCount(), 0); //for  finding bottleneck
  std::queue<int> bfs_queue; //for bfs

  bfs_queue.push(source);
  visited[source] = 1;
  while (!bfs_queue.empty()) { //standard bfs
    int current_vertex = bfs_queue.front();
    if (current_vertex == sink) { //got to destination
      break;
    }
    bfs_queue.pop();
    for (const auto edge : graph.GetNextEdges(current_vertex)) {
      if (edge.capacity > 0 && visited[edge.to] == 0) {
        bfs_queue.push(edge.to);
        visited[edge.to] = 1;
        parent[edge.to] = current_vertex;
        residual_capacity[edge.to] = edge.capacity;
      }
    }
  }
  if (bfs_queue.empty()) {
    return {}; //no path -> return empty vector
  }
  //Restoring path from parents vector
  std::vector<Edge> path;
  int current = sink;
  while (parent[current] != -1) {
    path.emplace_back(current, residual_capacity[current]);
    current = parent[current];
  }
  path.emplace_back(source, INT32_MAX); //fictitious edge to the source
  std::reverse(path.begin(), path.end());
  return path;
}

//Using Edmonds–Karp algorithm
int FindMaxFlow(const ListGraph &graph, int source, int sink) {
  ListGraph residual_network(graph); //Residual network
  //find shortest path while there are paths
  while (true) {
    auto path = FindPath(residual_network, source, sink);
    if (path.empty()) {
      break; //there is no more path -> stop the algorithm
    }
    //Updating residual network
    UpdateNetwork(residual_network, path, FindBottleneck(path));
  }
  return CountFlow(graph, residual_network, source);
}

Edge::Edge(int to, int capacity) : to(to), capacity(capacity) {}

ListGraph::ListGraph(int vertex_count) : adjacency_list_outcome(vertex_count) {}

//Constructor if empty graph
int ListGraph::VerticesCount() const {
  return adjacency_list_outcome.size();
}

//Copy constructor
ListGraph::ListGraph(const ListGraph &other) : adjacency_list_outcome(other.VerticesCount()) {
  for (int i = 0; i < other.VerticesCount(); ++i) {
    adjacency_list_outcome[i] = other.GetNextEdges(i);
  }
}

//Emplace new edge
void ListGraph::EmplaceEdge(int from, int to, int capacity) {
  adjacency_list_outcome[from].emplace_back(to, capacity);
}

//Find vertex with O(V)
void ListGraph::LowerCapacity(int from, int to, int lower_by) {
  for (auto &edge : GetNextEdges(from)) {
    if (edge.to == to) {
      edge.capacity -= lower_by;
      break;
    }
  }
}

std::vector<Edge> &ListGraph::GetNextEdges(int vertex) {
  return adjacency_list_outcome[vertex];
}

const std::vector<Edge> &ListGraph::GetNextEdges(int vertex) const {
  return adjacency_list_outcome[vertex];
}

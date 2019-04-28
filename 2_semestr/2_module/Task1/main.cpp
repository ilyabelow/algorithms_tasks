#include <iostream>
#include <vector>
#include <list>
#include <set>

//Convenient struct for edges (substitute for std::pair)
struct Edge {
  int to;
  int weight;
  Edge(int to, int weight);
  ~Edge() = default;
};

//Upgraded List Graph with an ability to store edge weights
class ListGraph {
 private:

  std::vector<std::list<Edge>> adjacency_list_outcome;

 public:

  explicit ListGraph(int vertices_count);

  void EmplaceEdge(int from, int to, int weight);

  int VerticesCount() const;

  std::vector<Edge> GetNextEdges(int vertex) const;

};

//Upgraded Priority Queue with an ability to access any element
class PriorityQueue {
 private:

  //Convenient struct for vertices with priority (substitute for std::pair)
  struct PrioritizedVertex {
    int vertex;
    int priority;
    PrioritizedVertex(int vertex, int priority);
    ~PrioritizedVertex() = default;
  };

  //Comparator for PrioritizedVertex
  struct PrioritizedVertexLess {
    bool operator()(const PrioritizedVertex &first, const PrioritizedVertex &second) const;
  };

  std::vector<int> priority; //priorities sorted by elements

  std::set<PrioritizedVertex, PrioritizedVertexLess> bst; //elements sorted by priorities

 public:

  explicit PriorityQueue(int size);

  void Update(int element, int new_priority);

  int GetNext();

  int GetPriority(int element) const;

  bool Empty() const;
};

int Dijkstra(const ListGraph &graph, int start, int finish);

int main() {
  int cities_count, roads_count;
  std::cin >> cities_count >> roads_count;
  ListGraph graph(cities_count);
  //filling the graph
  int from, to, weight;
  for (int i = 0; i < roads_count; ++i) {
    std::cin >> from >> to >> weight;
    graph.EmplaceEdge(from, to, weight);
    graph.EmplaceEdge(to, from, weight);
  }
  int start, finish;
  std::cin >> start >> finish;
  std::cout << Dijkstra(graph, start, finish);
  return 0;
}

Edge::Edge(int to, int weight) : to(to), weight(weight) {}

//Constructor for an empty graph
ListGraph::ListGraph(int vertices_count) : adjacency_list_outcome(vertices_count) {}

//Add new weighted edge to the graph
void ListGraph::EmplaceEdge(int from, int to, int weight) {
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

//Convenient struct for vertices with priority (substitute for std::pair)
PriorityQueue::PrioritizedVertex::PrioritizedVertex(int vertex, int priority)
    : priority(priority), vertex(vertex) {}

//Sort by priority first, then by vertex number (=arbitrary)
bool PriorityQueue::PrioritizedVertexLess::operator()(const PriorityQueue::PrioritizedVertex &first,
                                                      const PriorityQueue::PrioritizedVertex &second) const {
  if (first.priority < second.priority) {
    return true;
  }
  if (first.priority == second.priority) {
    return first.vertex < second.vertex;
  }
  return false;
}

//Sets every priority as "+infinity"
PriorityQueue::PriorityQueue(int size) : priority(size, INT32_MAX) {}

//Pops an element with the least priority and returns its value
int PriorityQueue::GetNext() {
  auto top = bst.begin();
  int top_vertex = (*top).vertex;
  bst.erase(top);
  return top_vertex;
}

//Weather queue is empty or not
bool PriorityQueue::Empty() const {
  return bst.empty();
}

//Updates priority of an element. Just pushes it in the queue if there was no such element
void PriorityQueue::Update(int element, int new_priority) {
  auto element_in_bst = bst.find(PrioritizedVertex(element, priority[element]));
  //tree nodes are constant, so we just erase instead of editing
  if (element_in_bst != bst.end()) {
    bst.erase(element_in_bst);
  }
  bst.emplace(element, new_priority);
  priority[element] = new_priority;
}

//Get priority for one element
int PriorityQueue::GetPriority(int element) const {
  return priority[element];
}

//Standard Dijkstra's algorith
int Dijkstra(const ListGraph &graph, int start, int finish) {
  PriorityQueue queue(graph.VerticesCount());
  std::vector<Edge> next_edges; //buffer for GetNextEdges
  queue.Update(start, 0);
  while (!queue.Empty()) {
    int current_vertex = queue.GetNext();
    for (auto edge : graph.GetNextEdges(current_vertex)) { //for every outcoming edge - calculating new distance
      int current_distance = queue.GetPriority(edge.to);
      int new_distance = queue.GetPriority(current_vertex) + edge.weight;
      if (current_distance > new_distance) { //relaxation
        queue.Update(edge.to, new_distance);
      }
    }
  }
  return queue.GetPriority(finish);
}

#include <iostream>
#include <vector>
#include <list>
#include <iostream>
#include <queue>

//graph with adjacency lists
class ListGraph {
private:
    //we have to store two vectors to be able to find and children and parents for O(n)
    std::vector<std::list<int>> adjacency_list_outcome;
public:
    ListGraph(int vertices_count);

    ~ListGraph() = default;

    void AddEdge(int from, int to);

    int VerticesCount() const;

    void GetNextVertices(int vertex, std::vector<int> &vertices) const;
};

int findPathCount(const ListGraph &graph, int start, int finish);

int main() {
    //Filling the graph
    int vertices_count, edges_count;
    std::cin >> vertices_count >> edges_count;
    ListGraph graph(vertices_count);
    for (int i = 0; i < edges_count; ++i) {
        int from, to;
        std::cin >> from >> to;
        //The graph is undirected => we should add edges in both directions
        graph.AddEdge(from, to);
        graph.AddEdge(to, from);
    }
    int u, w;
    std::cin >> u >> w;
    std::cout << findPathCount(graph, u, w);
    return 0;
}


//Constructor of graph with n vertices and no edges
ListGraph::ListGraph(int vertices_count) : adjacency_list_outcome(vertices_count){}

//Add new edge to the graph
void ListGraph::AddEdge(int from, int to) {
    adjacency_list_outcome[from].push_back(to);
}

int ListGraph::VerticesCount() const {
    return adjacency_list_outcome.size();
}

//Puts into input vector list of all the children of the vertex
void ListGraph::GetNextVertices(int vertex, std::vector<int> &vertices) const {
    vertices.clear();
    //just copy list to vector
    for (auto item : adjacency_list_outcome[vertex]) {
        vertices.push_back(item);
    }
}


//Find an amount of shortest paths from start to finish in a graph
int findPathCount(const ListGraph &graph, int start, int finish) {
    std::queue<int> bfs_queue;
    std::vector<int> next_vertices;
    //stores distance to the starting vertex
    std::vector<int> distance(graph.VerticesCount(), -1);
    distance[start] = 0;
    //stores amount of shortest paths for every vertex
    std::vector<int> path_count(graph.VerticesCount(), 0);
    path_count[start] = 1;

    bool finish_vertex_found = false;
    bfs_queue.push(start);
    //standard bfs
    while (!bfs_queue.empty()) {
        int current = bfs_queue.front();
        bfs_queue.pop();
        graph.GetNextVertices(current, next_vertices);
        for (int j = 0; j < next_vertices.size(); ++j) {
            if(next_vertices[j] == finish){
                finish_vertex_found = true;
            }
            if (distance[next_vertices[j]] == -1) {
                //we can stop pushing into a queue once we've found target vertex
                if(!finish_vertex_found){
                    bfs_queue.push(next_vertices[j]);
                }
                distance[next_vertices[j]] = distance[current] + 1;
                //paths to new vertex equal to path to current vertex
                path_count[next_vertices[j]] = path_count[current];
            } else if  (distance[current] + 1 == distance[next_vertices[j]]){
                //adding path count only if distances are same (they can be only bigger)
                path_count[next_vertices[j]] += path_count[current];
            }
        }

    }
    return path_count[finish];
}



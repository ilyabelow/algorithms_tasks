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


bool isBipartite(const ListGraph &graph);


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
    std::cout << (isBipartite(graph) ? "YES" : "NO");
    return 0;
}

//Constructor of graph with n vertices and no edges
ListGraph::ListGraph(int vertices_count) : adjacency_list_outcome(vertices_count) {}

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

//Checks if a graph is bipartite or not
bool isBipartite(const ListGraph &graph) {
    std::queue<int> bfs_queue;
    std::vector<int> next_vertices;
    //stores color of each vertex = its part
    std::vector<int> color(graph.VerticesCount(), 0);
    //check every сonnected component
    for (int i = 0; i < color.size(); ++i) {
        if (color[i] != 0) {
            continue;
        }
        color[i] = 1;
        bfs_queue.push(i);
        //standard bfs
        while (!bfs_queue.empty()) {
            int current = bfs_queue.front();
            bfs_queue.pop();
            graph.GetNextVertices(current, next_vertices);
            for (int j = 0; j < next_vertices.size(); ++j) {
                if (color[next_vertices[j]] == 0) {
                    bfs_queue.push(next_vertices[j]);
                    //color next vertex in an alternate color
                    color[next_vertices[j]] = color[current] % 2 + 1;
                } else if (color[next_vertices[j]] == color[current]) {
                    //we've found an edge to the same part
                    return false;
                }
            }
        }
    }
    return true;
}


#include <iostream>
#include <vector>
#include <unordered_set>
#include <iostream>
#include <queue>


//graph with adjacency sets
class SetGraph {
private:
    //we have to store two vectors to be able to find and children and parents fast
    std::vector<std::unordered_set<int>> adjacency_list_income;
    std::vector<std::unordered_set<int>> adjacency_list_outcome;
public:
    SetGraph(int vertices_count);

    ~SetGraph() = default;

    void AddEdge(int from, int to);

    int VerticesCount() const;

    void GetNextVertices(int vertex, std::vector<int> &vertices) const;

    void GetPrevVertices(int vertex, std::vector<int> &vertices) const;
};

int findCycle(SetGraph &graph);



int main() {
    //Filling the graph
    int vertices_count, edges_count;
    std::cin >> vertices_count >> edges_count;
    SetGraph graph(vertices_count);
    for (int i = 0; i < edges_count; ++i) {
        int from, to;
        std::cin >> from >> to;
        //The graph is undirected => we should add edges in both directions
        graph.AddEdge(from, to);
        graph.AddEdge(to, from);
    }
    std::cout << findCycle(graph);
    return 0;
}

//Constructor of graph with n vertices and no edges
SetGraph::SetGraph(int vertices_count) : adjacency_list_outcome(vertices_count),
                                         adjacency_list_income(vertices_count) {}

//Add new edge to the graph
void SetGraph::AddEdge(int from, int to) {
    adjacency_list_outcome[from].insert(to);
    adjacency_list_income[to].insert(from);
}

int SetGraph::VerticesCount() const {
    return adjacency_list_outcome.size();
}

//Puts into input vector list of all the children of the vertex
void SetGraph::GetNextVertices(int vertex, std::vector<int> &vertices) const {
    vertices.clear();
    //just copy list to vector
    for (auto item : adjacency_list_outcome[vertex]) {
        vertices.push_back(item);
    }
}

//Puts into input vector list of all the parents of the vertex
void SetGraph::GetPrevVertices(int vertex, std::vector<int> &vertices) const {
    vertices.clear();
    //same thing but from another set of lists
    for (auto item : adjacency_list_income[vertex]) {
        vertices.push_back(item);
    }

}

//Find a length of the smallest cycle in a graph
int findCycle(SetGraph &graph) {
    int min_cycle = 200001;
    //start BFS from every vertex
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        std::queue<int> bfs_queue;
        std::vector<int> next_vertices;
        //stores distance to the starting vertex
        std::vector<int> distance(graph.VerticesCount(), -1);
        distance[i] = 0;
        bool cycle_is_found = false;
        bfs_queue.push(i);
        //standard bfs
        while (!bfs_queue.empty() && !cycle_is_found) {
            int current = bfs_queue.front();
            bfs_queue.pop();
            graph.GetNextVertices(current, next_vertices);
            for (int j = 0; j < next_vertices.size(); ++j) {
                if (distance[next_vertices[j]] == -1) {
                    //pushing next vertex into a queue
                    bfs_queue.push(next_vertices[j]);
                    distance[next_vertices[j]] = distance[current] + 1;
                } else {
                    //don't go backwards
                    if (distance[next_vertices[j]] + 1 == distance[current]) {
                        continue;
                    }
                    min_cycle = std::min(min_cycle, distance[current] + distance[next_vertices[j]] + 1);
                    //we need to find only the first cycle because every other cycle will be bigger (see pdf)
                    cycle_is_found = true;
                    break;
                }
            }
        }
    }
    return min_cycle == 200001 ? -1 : min_cycle;
}



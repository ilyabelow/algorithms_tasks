#include <iostream>
#include <vector>
#include <list>
#include <iostream>
#include <stack>
#include <algorithm>

//graph with adjacency lists
class ListGraph {
private:
    //we have to store two vectors to be able to find and children and parents for O(n)
    std::vector<std::list<int>> adjacency_list_outcome;
    std::vector<std::list<int>> adjacency_list_income;
public:
    ListGraph();

    ListGraph(int vertices_count);

    ~ListGraph() = default;

    void AddEdge(int from, int to);

    int AddVertex();

    int VerticesCount() const;

    void GetNextVertices(int vertex, std::vector<int> &vertices) const;

    void GetPrevVertices(int vertex, std::vector<int> &vertices) const;

    void Condense();
};

int countSinks(const ListGraph &graph);

int countSources(const ListGraph &graph);

int main() {
    //Filling the graph
    int vertices_count, edges_count;
    std::cin >> vertices_count >> edges_count;
    ListGraph graph(vertices_count);
    for (int i = 0; i < edges_count; ++i) {
        int from, to;
        std::cin >> from >> to;
        graph.AddEdge(--from, --to); //for some reason in this task counting starts from 1
    }
    graph.Condense();
    //Special case id there is only one component
    if (graph.VerticesCount() == 1){
        std::cout << 0;
        return 0;
    }
    std::cout << std::max(countSinks(graph), countSources(graph));
    return 0;
}

//Empty graph
ListGraph::ListGraph() : ListGraph(0) {}

//Constructor of graph with n vertices and no edges
ListGraph::ListGraph(int vertices_count) : adjacency_list_outcome(vertices_count),
                                           adjacency_list_income(vertices_count) {}

//Add new edge to the graph
void ListGraph::AddEdge(int from, int to) {
    adjacency_list_outcome[from].push_back(to);
    adjacency_list_income[to].push_back(from);

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


//Puts into input vector list of all the parents of the vertex
void ListGraph::GetPrevVertices(int vertex, std::vector<int> &vertices) const {
    vertices.clear();
    //same thing but from another set of lists
    for (auto item : adjacency_list_income[vertex]) {
        vertices.push_back(item);
    }
}

//Adds new vertex to the graph
int ListGraph::AddVertex() {
    adjacency_list_outcome.emplace_back();
    adjacency_list_income.emplace_back();
    return VerticesCount()-1;
}

//Turn a graph into its condensed version
void ListGraph::Condense() {
    //common DFS tools
    std::stack<int> dfs_stack;
    std::vector<int> next_vertices;
    //
    // Topology Sort
    //
    std::vector<int> dfs_order;
    std::vector<int> color(VerticesCount(), 0);
    dfs_order.reserve(VerticesCount());
    //dfs from each vertex
    for (int i = 0; i < color.size(); ++i) {
        if (color[i] != 0) {
            continue;
        }
        color[i] = 1;
        dfs_stack.push(i);
        //standart dfs
        while (!dfs_stack.empty()) {
            int current = dfs_stack.top();
            if (color[current] == 2) {
                dfs_stack.pop();
                dfs_order.push_back(current);
                continue;
            }
            GetNextVertices(current, next_vertices);
            //pushing all the vertices at once to save time
            for (int j = 0; j < next_vertices.size(); ++j) {
                if (color[next_vertices[j]] == 0) {
                    dfs_stack.push(next_vertices[j]);
                    color[next_vertices[j]] = 1;
                }
            }
            color[current] = 2;
        }
    }
    std::reverse(dfs_order.begin(), dfs_order.end());
    //
    // Transpone
    //
    std::swap(adjacency_list_income, adjacency_list_outcome);
    //
    // Condense
    //
    ListGraph condensed; //result graph
    std::vector<int> components(VerticesCount(), -1); //which vertex belong to which component
    int current_component = -1;
    //starting dfs from vertices in the order of topology sort
    for (auto start : dfs_order) {
        if (components[start] != -1) {
            continue;
        }
        current_component = condensed.AddVertex();
        components[start] = current_component;
        dfs_stack.push(start);
        while (!dfs_stack.empty()) {
            int current = dfs_stack.top();
            dfs_stack.pop();
            GetNextVertices(current, next_vertices);
            for (int j = 0; j < next_vertices.size(); ++j) {
                if (components[next_vertices[j]] == -1) { //expanding component
                    dfs_stack.push(next_vertices[j]);
                    components[next_vertices[j]] = current_component;
                } else if (components[next_vertices[j]] != current_component) { //stumble upon traversed component
                    //adding reversed edge because now we actually work with already reversed edges, so
                    //we need to reverse them back
                    condensed.AddEdge(components[next_vertices[j]], current_component);
                }
            }

        }
    }
    //overwrite graph
    *this = condensed;
}

//Count vertices which don't have children
int countSinks(const ListGraph &graph){
    int sinks = 0;
    std::vector<int> next_vertices;
    for (int i = 0;  i < graph.VerticesCount(); ++i){
        graph.GetNextVertices(i, next_vertices);
        if (next_vertices.size() == 0) {
            ++sinks;
        }
    }
    return sinks;
}

//Count vertices which don't have parents
int countSources(const ListGraph &graph){
    int sources = 0;
    std::vector<int> prev_vertices;
    for (int i = 0;  i < graph.VerticesCount(); ++i){
        graph.GetPrevVertices(i, prev_vertices);
        if (prev_vertices.size() == 0) {
            ++sources;
        }
    }
    return sources;
}

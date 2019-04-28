#include "ListGraph.h"
#include <iostream>

//Constructor of graph with n vertices and no edges
ListGraph::ListGraph(int vertices_count) : adjacency_list_outcome(vertices_count),
                                           adjacency_list_income(vertices_count) {}

//Copy constructor
ListGraph::ListGraph(const IGraph *graph) : adjacency_list_outcome(graph->VerticesCount()),
                                            adjacency_list_income(graph->VerticesCount()) {
    std::vector<int> vertices(0); //buffer
    //extracting all the edges from source graph
    for (int i = 0; i < graph->VerticesCount(); ++i) {
        graph->GetNextVertices(i, vertices);
        for (int j = 0; j < vertices.size(); ++j) {
            AddEdge(i, vertices[j]);
        }
    }
}

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
#include "MatrixGraph.h"
#include <vector>
#include <iostream>

//Constructor of graph with n vertices and no edges
MatrixGraph::MatrixGraph(int vertices_count) : adjacency_matrix(vertices_count,
                                                                std::vector<int>(vertices_count, 0)) {}

//Copy constructor
MatrixGraph::MatrixGraph(const IGraph *graph) : adjacency_matrix(graph->VerticesCount(),
                                                                 std::vector<int>(graph->VerticesCount(), 0)) {
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
void MatrixGraph::AddEdge(int from, int to) {
    adjacency_matrix[from][to] = 1;
}

int MatrixGraph::VerticesCount() const {
    return adjacency_matrix.size();
}

//Puts into input vector list of all the children of the vertex
void MatrixGraph::GetNextVertices(int vertex, std::vector<int> &vertices) const {
    vertices.clear(); //we don't know if input vector is empty
    for (int i = 0; i < adjacency_matrix.size(); ++i) {
        if (adjacency_matrix[vertex][i] == 1) { //rows - from which, colums - to which
            vertices.push_back(i);
        }
    }
}

//Puts into input vector list of all the parents of the vertex
void MatrixGraph::GetPrevVertices(int vertex, std::vector<int> &vertices) const {
    vertices.clear();
    for (int i = 0; i < adjacency_matrix.size(); ++i) {
        if (adjacency_matrix[i][vertex] == 1) { //just like next vertices but in an opposite order
            vertices.push_back(i);
        }
    }
}
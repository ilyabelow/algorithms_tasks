#include "ArcGraph.h"
#include <iostream>

//Constructor of graph with n vertices and no edges
ArcGraph::ArcGraph(int vertices_count) : vertices_count(vertices_count) {}

//Copy constructor
ArcGraph::ArcGraph(const IGraph *graph) : vertices_count(graph->VerticesCount()) {
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
void ArcGraph::AddEdge(int from, int to) {
    edges.emplace_back(from, to);
}

int ArcGraph::VerticesCount() const {
    return vertices_count;
}

//Puts into input vector list of all the children of the vertex
void ArcGraph::GetNextVertices(int vertex, std::vector<int> &vertices) const {
    vertices.clear();
    //find edges with corresponding outcome vertex, returning income one
    for (auto item : edges) {
        if (item.first == vertex) {
            vertices.push_back(item.second);
        }
    }
}

//Puts into input vector list of all the parents of the vertex
void ArcGraph::GetPrevVertices(int vertex, std::vector<int> &vertices) const {
    vertices.clear();
    //the same but in the other direction
    for (auto item : edges) {
        if (item.second == vertex) {
            vertices.push_back(item.first);
        }
    }
}
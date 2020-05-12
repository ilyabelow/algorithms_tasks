#pragma once

#include "IGraph.h"
#include <vector>

//Graph with adjacency matrix
class MatrixGraph : public IGraph {
private:
    //Adjacency matrix
    std::vector<std::vector<int>> adjacency_matrix;
public:
    MatrixGraph(int vertices_count);

    MatrixGraph(const IGraph *graph);

    ~MatrixGraph() = default;

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    void GetNextVertices(int vertex, std::vector<int> &vertices) const override;

    void GetPrevVertices(int vertex, std::vector<int> &vertices) const override;
};


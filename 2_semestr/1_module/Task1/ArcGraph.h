#pragma once

#include "IGraph.h"
#include <vector>
#include <utility>


//graph with vector of its adjacency_matrix
class ArcGraph : public IGraph {
private:
    std::vector<std::pair<int, int>> edges;
    //in this graph size of the container doesn't correspond with vertices count so we need to store another integer
    int vertices_count;
public:
    ArcGraph(int vertices_count);

    ArcGraph(const IGraph *graph);

    ~ArcGraph() = default;

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    void GetNextVertices(int vertex, std::vector<int> &vertices) const override;

    void GetPrevVertices(int vertex, std::vector<int> &vertices) const override;
};


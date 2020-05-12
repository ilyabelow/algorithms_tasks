#pragma once

#include "IGraph.h"
#include <vector>
#include <list>

//graph with adjacency lists
class ListGraph : public IGraph {
private:
    //we have to store two vectors to be able to find and children and parents for O(n)
    std::vector<std::list<int>> adjacency_list_income;
    std::vector<std::list<int>> adjacency_list_outcome;
public:
    ListGraph(int vertices_count);

    ListGraph(const IGraph *graph);

    ~ListGraph() = default;

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    void GetNextVertices(int vertex, std::vector<int> &vertices) const override;

    void GetPrevVertices(int vertex, std::vector<int> &vertices) const override;
};


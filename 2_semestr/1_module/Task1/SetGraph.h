#pragma once

#include "IGraph.h"
#include <vector>
#include <unordered_set>


//graph with adjacency sets
class SetGraph : public IGraph {
private:
    //we have to store two vectors to be able to find and children and parents fast 
    std::vector<std::unordered_set<int>> adjacency_list_income;
    std::vector<std::unordered_set<int>> adjacency_list_outcome;
public:
    SetGraph(int vertices_count);

    SetGraph(const IGraph *graph);

    ~SetGraph() = default;

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    void GetNextVertices(int vertex, std::vector<int> &vertices) const override;

    void GetPrevVertices(int vertex, std::vector<int> &vertices) const override;
};


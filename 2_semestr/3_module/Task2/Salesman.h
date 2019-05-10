#pragma once
#include "ListGraph.h"

//Finds 2-approximation
const ListGraph Approximation2(const ListGraph &orig_graph);
//Finds 1.5-approximation
const ListGraph Approximation1_5(const ListGraph &orig_graph);
//Finds optimum
const ListGraph Optimum(const ListGraph &orig_graph);

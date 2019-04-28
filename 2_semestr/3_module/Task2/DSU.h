//
// Created by bumazhka on 28.04.19.
//

#include <vector>
#pragma once

//Improved realisation from e-maxx. Uses heuristic of path shorting
class DSU {
 private:
  //Tree
  std::vector<int> parent;
  //Rank for heuristics of union by rank
  std::vector<int> rank;
 public:
  explicit DSU(int vertices);
  int FindSet(int element);
  void UnionSets(int first, int second);
};

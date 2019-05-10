#include "DSU.h"

DSU::DSU(int vertices) : parent(vertices, -1), rank(vertices, 0) {} //-1 means root

//Finding set using trees
int DSU::FindSet(int element) {
  if (parent[element] == -1) {
    return element;
  }
  //Heuristic of path shorting
  parent[element] = FindSet(parent[element]);
  return parent[element];
}

//Union using trees
void DSU::UnionSets(int first, int second) {
  int left_tree = FindSet(first);
  int right_tree = FindSet(second);
  if (left_tree != right_tree) {
    //Shorter tree is tied up to higher tree
    if (rank[left_tree] < rank[right_tree]) {
      parent[left_tree] = right_tree;
    } else {
      parent[right_tree] = left_tree;
      if (rank[left_tree] == rank[right_tree]) {
        ++rank[left_tree];
      }
    }
  }
}

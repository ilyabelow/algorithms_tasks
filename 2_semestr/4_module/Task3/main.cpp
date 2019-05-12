#include <iostream>
#include <vector>
#include <cmath>
#include <stack>

//Class that answers LCA requests
class LCA {
 private:
  std::vector<std::vector<int>> parents;
  std::vector<int> depth;
 public:
  explicit LCA(const std::vector<int> &parents);
  int Request(int left_vertex, int right_vertex) const;
};

int main() {
  //Reading the tree
  int vertex_count, request_count;
  std::cin >> vertex_count >> request_count;
  std::vector<int> parents(vertex_count);
  parents[0] = 0;
  for (int v = 1; v < vertex_count; ++v) {
    std::cin >> parents[v];
  }
  LCA lca(parents);
  //Reading request info
  long long left_vertex, right_vertex;
  int x, y, z, prev_answer;
  std::cin >> left_vertex >> right_vertex;
  std::cin >> x >> y >> z;
  long long sum = 0;
  //First request is handled seperately
  sum += prev_answer = lca.Request(left_vertex, right_vertex);
  for (int r = 1; r < request_count; ++r) {
    //Complex calculations of next request
    left_vertex = (x * left_vertex + y * right_vertex + z) % vertex_count;
    right_vertex = (x * right_vertex + y * left_vertex + z) % vertex_count;
    sum += prev_answer = lca.Request((left_vertex + prev_answer) % vertex_count, right_vertex);
  }
  //Writing hashed answer
  std::cout << sum;
  return 0;
}

//LCA preprocessing is here
LCA::LCA(const std::vector<int> &init_parents) : parents(init_parents.size(),
                                                         std::vector<int>(std::ceil(std::log2(std::max(2ul,
                                                                                                       init_parents.size()))),
                                                                          0)),
                                                 depth(init_parents.size(), -1) {
  //Filling parents table according to the recursive formula in conspects
  for (int v = 0; v < parents.size(); ++v) {
    parents[v][0] = init_parents[v];
  }
  for (int i = 1; i < parents[0].size(); ++i) {
    for (int v = 0; v < parents.size(); ++v) {
      parents[v][i] = parents[parents[v][i - 1]][i - 1];
    }
  }
  //Depth calculation via sly DFS
  depth[0] = 0;
  for (int j = 1; j < depth.size(); ++j) {
    if (depth[j] == -1) {
      std::stack<int> dfs_stack;
      dfs_stack.push(j);
      //Pushing unvisited vertices to a stack up to the point when we find visited vertex
      while (depth[parents[dfs_stack.top()][0]] == -1) {
        dfs_stack.push(parents[dfs_stack.top()][0]);
      }
      //Calculate depth in a reversed order
      while (!dfs_stack.empty()) {
        depth[dfs_stack.top()] = depth[parents[dfs_stack.top()][0]] + 1;
        dfs_stack.pop();
      }
    }
  }
}

//Make request to LCA problem
int LCA::Request(int left_vertex, int right_vertex) const {
  //Decide which vertex is lower and needs pulling up
  int upper_vertex, lower_vertex;
  if (depth[left_vertex] > depth[right_vertex]) {
    upper_vertex = right_vertex;
    lower_vertex = left_vertex;
  } else {
    lower_vertex = right_vertex;
    upper_vertex = left_vertex;
  }
  //Pulling up lower vertex
  for (int i = parents[0].size() - 1; i >= 0; --i) {
    if (depth[parents[lower_vertex][i]] >= depth[upper_vertex]) {
      lower_vertex = parents[lower_vertex][i];
    }
  }
  //Pulling up both vertices
  int vertex_x = lower_vertex, vertex_y = upper_vertex;
  if (vertex_x == vertex_y) {
    return vertex_x;
  }
  for (int i = parents[0].size() - 1; i >= 0; --i) {
    //Going up until x and y have identical first order parent
    if (parents[vertex_x][i] != parents[vertex_y][i]) {
      vertex_x = parents[vertex_x][i];
      vertex_y = parents[vertex_y][i];
    }
  }
  return parents[vertex_x][0];
}

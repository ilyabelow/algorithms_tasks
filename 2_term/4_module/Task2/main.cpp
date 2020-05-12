#include <iostream>
#include <vector>

//Tree node with information we need for calculation and with pointers to children
struct Node {
  int length;
  int prefix;
  int suffix;
  int sequence;
  Node(const Node &left, const Node &right);
  explicit Node(bool state);
  explicit Node();
};

//Segment tree which calculates max length of sequence made of 1
class SegmentTree {
 private:
  //Tree vector
  std::vector<Node> tree;
  //Recursive function for calculating answer in a range
  int getSequence(int index, int left, int right) const;
 public:
  //Tree preprocessing here
  explicit SegmentTree(const std::vector<int> &array);
  //"Client" function for calculating answer
  int Request(int left, int right) const;
};

int main() {
  //Reading initial array
  int array_size, request_count;
  std::cin >> array_size >> request_count;
  std::vector<int> initial(array_size);
  for (int i = 0; i < array_size; ++i) {
    std::cin >> initial[i];
  }
  SegmentTree tree(initial);
  //Handling requests
  int left, right;
  for (int i = 0; i < request_count; ++i) {
    std::cin >> left >> right;
    std::cout << tree.Request(left, right) << std::endl;
  }
  return 0;
}

//Here tree is build layer by layer starting with leafs and ascending to root
SegmentTree::SegmentTree(const std::vector<int> &array) {
  // Size of the bottom level is 2^(log_2_(array.size())
  int size = 1;
  while (size < array.size()) {
    size *= 2;
  }
  tree = std::vector<Node>(size * 2 - 1);
  //Filling bottom layer
  for (int i = 0; i < size; ++i) {
    tree[i + size - 1] = Node(array[i]);
  }
  //Filling tre rest from top to bottom
  size /= 2;
  while (size > 0) {
    for (int i = size - 1; i < size * 2 - 1; ++i) {
      tree[i] = Node(tree[2 * i + 1], tree[2 * i + 2]);
    }
    size /= 2;
  }
}

//Just redirect call to main calculating function
int SegmentTree::Request(int left, int right) const {
  return getSequence(0, left, right);
}

int SegmentTree::getSequence(int index, int left, int right) const {
  int left_child = 2 * index + 1;
  int right_child = 2 * index + 2;
  //The whole segment lies on the node
  if (left == 0 && right == tree[index].length - 1) {
    return tree[index].sequence;
  }
  //The whole segment is in left child
  if (right < tree[left_child].length) {
    return getSequence(left_child, left, right);
  }
  //The whole segment is in right child
  if (left >= tree[left_child].length) {
    return getSequence(right_child, left - tree[left_child].length, right - tree[left_child].length);
  }
  //The segment is in both children
  return std::max(std::max(
      getSequence(left_child, left, tree[left_child].length - 1),
      getSequence(right_child, 0, right - tree[left_child].length)),
      //Concatenation of sequence on the border between children
                  std::min(tree[left_child].suffix, tree[left_child].length - left) +
                      std::min(tree[right_child].prefix, right - tree[left_child].length + 1));
}

//Constructor for node with only both children

Node::Node(const Node &left, const Node &right) {
  //Calculations are made according to formulas in description
  length = left.length + right.length;
  if (left.prefix == left.length) {
    prefix = left.length + right.prefix;
  } else {
    prefix = left.prefix;
  }
  if (right.suffix == right.length) {
    suffix = right.length + left.suffix;
  } else {
    suffix = right.suffix;
  }
  sequence = std::max(left.sequence, std::max(right.sequence, left.suffix + right.prefix));
}

//Constructor for bottom layer
Node::Node(bool state) :
    length(1),
    prefix(state),
    suffix(state),
    sequence(state) {}

//Constructor for neutral element
Node::Node() : length(0),
               prefix(0),
               suffix(0),
               sequence(0) {}

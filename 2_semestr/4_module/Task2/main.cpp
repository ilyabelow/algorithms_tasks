#include <iostream>
#include <vector>

//Tree node with information we need for calculation and with pointers to children
struct Node {
  int length;
  int prefix;
  int suffix;
  int sequence;
  Node *left;
  Node *right;
  Node(Node *left, Node *right);
  explicit Node(Node *left);
  explicit Node(bool state);
};

//Segment tree which calculates max length of sequence made of 1
class SegmentTree {
 private:
  //Tree root
  Node *root;
  //Recursive function for calculating answer in a range
  int getSequence(const Node *node, int left, int right) const;
 public:
  //Tree preprocessing here
  explicit SegmentTree(const std::vector<int> &array);
  //"Client" function for calculating answer
  int Request(int left, int right) const;
};

int main() {
  //Reading initial array
  int n, m;
  std::cin >> n >> m;
  std::vector<int> initial(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> initial[i];
  }
  SegmentTree tree(initial);
  //Recording answer
  std::vector<int> answer;
  for (int i = 0; i < m; ++i) {
    int left, right;
    std::cin >> left >> right;
    answer.push_back(tree.Request(left, right));
  }
  //Writing answer
  for (int i = 0; i < m; ++i) {
    std::cout << answer[i] << std::endl;
  }
  return 0;
}

//Here tree is build layer by layer starting with leafs and ascending to root
SegmentTree::SegmentTree(const std::vector<int> &array) {
  std::vector<Node *> bottom;
  std::vector<Node *> top;
  //Bottom layer
  for (auto i : array) {
    bottom.push_back(new Node(i));
  }
  //While tree is not width of 1
  while (bottom.size() > 1) {
    for (int i = 0; i < bottom.size() / 2; ++i) {
      top.push_back(new Node(bottom[2 * i], bottom[2 * i + 1]));
    }
    //Dealing with excess node
    if (bottom.size() % 2 == 1) {
      top.push_back(new Node(bottom[bottom.size() - 1]));
    }
    bottom = std::move(top);
    top = std::vector<Node *>();
  }
  root = bottom[0];
}

//Just redirect call to main calculating function
int SegmentTree::Request(int left, int right) const {
  return getSequence(root, left, right);
}


int SegmentTree::getSequence(const Node *node, int left, int right) const {
  //The whole segment lies on the node
  if (left == 0 && right == node->length - 1) {
    return node->sequence;
  }
  //The whole segment is in left child
  if (right < node->left->length) {
    return getSequence(node->left, left, right);
  }
  //The whole segment is in right child
  if (left >= node->left->length) {
    return getSequence(node->right, left - node->left->length, right - node->left->length);
  }
  //The segment is in both children
  return std::max(std::max(
      getSequence(node->left, left, node->left->length - 1),
      getSequence(node->right, 0, right - node->left->length)),
                  //Concatenation of sequence on the border between children
                  std::min(node->left->suffix, node->left->length - left) +
                      std::min(node->right->prefix, right - node->left->length+1));
}

//Constructor for node with only both children
Node::Node(Node *left, Node *right) : left(left), right(right) {
  //Calculations are made according to formulas in description
  length = left->length + right->length;
  if (left->prefix == left->length) {
    prefix = left->length + right->prefix;
  } else {
    prefix = left->prefix;
  }
  if (right->suffix == right->length) {
    suffix = right->length + left->suffix;
  } else {
    suffix = right->suffix;
  }
  sequence = std::max(left->sequence, std::max(right->sequence, left->suffix + right->prefix));
}

//Constructor for bottom layer
Node::Node(bool state)
    : left(nullptr),
      right(nullptr),
      length(1),
      prefix(state),
      suffix(state),
      sequence(state) {}
//Constructor for node with only one child
Node::Node(Node *left)
    : left(left),
      right(nullptr),
      length(left->length),
      prefix(left->prefix),
      suffix(left->suffix),
      sequence(left->sequence) {}

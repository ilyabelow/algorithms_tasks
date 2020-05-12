#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <utility>
#include <cstdio>

const int ALPHABET = 128;

struct Node {
    // Bounds of a substring that correspont to the edge from parent
    int start;
    int end;

    int suff_link;
    std::vector<int> children;

    Node(int start, int end): children(ALPHABET, -1),
                              start(start),
                              end(end),
                              suff_link(-1) {}

    bool has_child(char trans) const;
    void add_child(char trans, int node) ;
    int get_child(char trans) const;
};

class SuffixTree {
    // Tree nodes
    std::vector<Node> nodes;

    // String itself
    std::string str;
    int i;

    // Current position is the tree
    int active_node;
    int active_edge;
    int active_len;

    int remainder;
    int need_suff_link;

    // Suplimental functions
    Node& active();
    char active_edge_char();

    void build_tree();
    void add_leaf_to_node();
    void split_edge();
    void jump_to_the_next_location();
    void add_suff_link(int to);
    bool do_we_overshoot();
    int get_edge_length(int node);
    int add_leaf(int start);
    int add_node(int start, int end);

public:
    SuffixTree(const std::string &str);
    SuffixTree(std::string &&str);

    // Outside interface
    const Node& get_node(int node) const;
    int node_count() const;
};


void Node::add_child(char trans, int node) {
    children[trans] = node;
}

int Node::get_child(char trans) const {
    return children[trans];
}

bool Node::has_child(char trans) const {
    return children[trans] != -1;
}

int SuffixTree::node_count() const {
    return nodes.size();
}

const Node& SuffixTree::get_node(int node) const {
    return nodes[node];
}

int SuffixTree::get_edge_length(int node) {
    // -1 = end, so real end is current suffix length
    if (nodes[node].end == -1) {
        return i - nodes[node].start + 1;
    }
    return nodes[node].end - nodes[node].start;
}

// Checks if active len overshoots active edge and chops it if it does overshoot
bool SuffixTree::do_we_overshoot() {
    int next = active().get_child(active_edge_char());
    int edge_len = get_edge_length(next);
    if (active_len >= edge_len) {
        active_len -= edge_len;
        active_edge += edge_len;
        active_node = next;
        return true;
    }
    return false;
}

// Add pending suffix link to current edge
void SuffixTree::add_suff_link(int to) {
    if (need_suff_link != -1) {
        nodes[need_suff_link].suff_link = to;
    }
    need_suff_link = to;
}

int SuffixTree::add_leaf(int start) {
    nodes.emplace_back(start, -1);
    return nodes.size() - 1;
}

int SuffixTree::add_node(int start, int end) {
    nodes.emplace_back(start, end);
    return nodes.size() - 1;
}

char SuffixTree::active_edge_char() {
    return str[active_edge];
}

Node& SuffixTree::active() {
    return nodes[active_node];
}

// Jmp to the next point of insertion
void SuffixTree::jump_to_the_next_location() {
    --remainder;
    if (active_node == 0 && active_len > 0) {
        active_edge = i - remainder + 1;
        --active_len;
    } else {
        if (active().suff_link == -1) {
            active_node = 0;
        } else {
            active_node = active().suff_link;
        }
    }
}

// Add leaf node to active node
void SuffixTree::add_leaf_to_node() {
    int new_node = add_leaf(i);
    active().add_child(active_edge_char(), new_node);
    add_suff_link(active_node);
}

// Split active edge and add a leaf to it
void SuffixTree::split_edge() {
    int next = active().get_child(active_edge_char());
    int split = add_node(nodes[next].start, nodes[next].start + active_len);
    active().add_child(active_edge_char(), split);
    // Add leaf node
    int leaf = add_leaf(i);
    nodes[split].add_child(str[i], leaf);
    // Add second half of the edge
    nodes[next].start += active_len;
    nodes[split].add_child(str[nodes[next].start], next);
    // Add pending suffix link
    add_suff_link(split);
}

void SuffixTree::build_tree() {
    i = 0;
    remainder = 0;
    active_node = 0;
    active_len = 0;
    active_edge = 0;
    // Add root
    add_node(0, 0);
    // Go through the string
    for (; i < str.size(); ++i) {
        need_suff_link = -1;
        ++remainder;
        while (remainder > 0) {
            // WE ARE IN A NODE
            if (active_len == 0) {
                // We want to go to the edge with current symbol
                active_edge = i;
                if (!active().has_child(str[i])) {
                    // No such edge? Create one!
                    add_leaf_to_node();
                    jump_to_the_next_location();
                    continue;
                }
            }
            // WE ARE ALONG AN EDGE

            // 1. If we overshoot, chop down active edge until we have a valid value
            if (do_we_overshoot()) {
                continue;
            }
            int next = active().get_child(active_edge_char());
            // 2. Can we go further on the same edge?
            if (str[nodes[next].start + active_len] == str[i]) {
                // 3. Yes! Then go there and move to the next symbol without inserting anything
                ++active_len;
                add_suff_link(active_node);
                break;
            }
            // 4. No! Split the edge
            split_edge();
            jump_to_the_next_location();
        }
    }
}

SuffixTree::SuffixTree(std::string &&str): str(str) {
    build_tree();
}

SuffixTree::SuffixTree(const std::string &str): str(str) {
    build_tree();
}

void output_two_tree(const std::string &str1, const std::string &str2) {

    SuffixTree tree(str1 + str2);
    printf("%d\n", tree.node_count());
    // Store pairs (parent, node)
    std::stack<std::pair<int, int>> dfs;
    dfs.push(std::make_pair(-1, 0));
    int iteration = 0;

    while (!dfs.empty()) {
        auto current = dfs.top();
        dfs.pop();
        const Node& node = tree.get_node(current.second);
        // Pre order
        if (current.first != -1) {
            printf("%d ", current.first);
            if (node.start < str1.size()) {
                printf("0 %d ", node.start);
                if (node.end == -1) {
                    printf("%d\n", str1.size());
                } else {
                    printf("%d\n", node.end);
                }
            } else {
                printf("1 %d ", node.start - str1.size());
                if (node.end == -1) {
                    printf("%d\n", str2.size());
                } else {
                    printf("%d\n", node.end - str1.size());
                }
            }
        }
        // Continue descend
        for (int i = ALPHABET - 1; i >= 0; --i) {
            if (node.has_child(i)) {
                dfs.push(std::make_pair(iteration, node.get_child(i)));
            }
        }
        ++iteration;
    }
}

int main() {
    // Input
    std::string str1, str2;
    std::cin >> str1 >> str2;
    // Output
    output_two_tree(str1, str2);
    return 0;
}


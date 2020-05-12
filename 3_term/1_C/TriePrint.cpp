#include "Trie.h"


void Trie::print() {
    internal_print(0, 0, false, "");
    std::cout << std::endl;
}

void Trie::internal_print(int node, int depth, bool shift, std::string padding) {
    if (shift) {
        std::cout << std::endl << padding << "└";
        padding += "    ";
    }
    int children = 0;
    if (node == 0) {
        std::cout << "@";
    } else {
        std::cout << "-" << static_cast<char>(nodes[node].to_parent + first) << "-";
        if (nodes[node].is_terminal) {
            std::cout << '#';
        } else {
            std::cout << '*';
        }
    }
   for (int i = 0; i < alphabet; ++i) {
        children += nodes[node].has_child(i+first);
    }
    shift = false;
    for (int i = 0; i < alphabet; ++i) {
        if (nodes[node].has_child(i)) {
            if (shift) {
                internal_print(nodes[node].get_child(i), depth + 1, shift, padding);
            } else if (children > 1) {
                internal_print(nodes[node].get_child(i), depth + 1, shift, padding + "|   ");
            } else {
                internal_print(nodes[node].get_child(i), depth + 1, shift, padding + "    ");
            }
            --children;
            shift = true;
        }
    }
}


#include "Trie.h"

//Temporal solution (that grown to be permanents)
static const int alphabet = 26;
static const char first = 'a';

//Root constructor
Trie::Node::Node(): Node(-1, -1) {
    transversed = true;
    suff_link = 0;
}

//Normal node constructor
Trie::Node::Node(int parent, char to_parent): transitions(alphabet, -1),
                                              parent(parent),
                                              to_parent(to_parent),
                                              suff_link(-1),
                                              is_terminal(false),
                                              transversed(false),
                                              string_ids(0) {}

bool Trie::Node::has_transition(char chr) {
    return transitions[chr] != -1;
}

bool Trie::Node::has_suff_link() {
    return suff_link != -1;
}

int Trie::Node::get_transition(char chr) {
    return transitions[chr];
}

void Trie::Node::add_transition(char chr, int transition) {
    transitions[chr] = transition;
}

//Emply trie starts with one node
Trie::Trie() : nodes(1), current_node(0) {}

//Calcuate transiction function only with the first call (lazy computation)
int Trie::get_transition(int node, char chr) {
    if (!nodes[node].has_transition(chr)) {
        if (node == 0) {
            nodes[node].add_transition(chr, 0);
        } else {
            nodes[node].add_transition(chr, get_transition(get_suff_link(node), chr));
        }
    }
    return nodes[node].transitions[chr];
}

//Same but with siffix links
int Trie::get_suff_link(int node) {
    if (!nodes[node].has_suff_link()) {
        if (node == 0 || nodes[node].parent == 0){
            nodes[node].suff_link = 0;
        } else {
            nodes[node].suff_link = get_transition(get_suff_link(nodes[node].parent), nodes[node].to_parent);
        }
    }
    return nodes[node].suff_link;
}

//Same but with terminal links
int Trie::get_terminal_link(int node) {
    int suff_link = get_suff_link(node);
    if (nodes[suff_link].is_terminal) {
        return suff_link;
    } else if (suff_link == 0) {
        return 0;
    }
    return get_terminal_link(suff_link);
}

//Recursively collect string ids from nodes from terminal links
//This optimisation allowes not jumping term links every time and get the answer right away
void Trie::transverse(int node) {
    if (!nodes[node].transversed) {
        int terminal_link = get_terminal_link(node);
        transverse(terminal_link);
        //Copy string ids
        for (int string_id : nodes[terminal_link].string_ids) {
            nodes[node].string_ids.push_back(string_id);
        }
        //Ofc we do this only once
        nodes[node].transversed = true;
    }
}

//Make a transition to the next node
std::vector<int>& Trie::to_next_symbol(char chr) {
    current_node = get_transition(current_node, chr - first);
    transverse(current_node);
    //Client will get the desired result of ids of strings that end in the current symbol
    return nodes[current_node].string_ids;
}

//Fill the graph. Implimentation is standart
void Trie::add_string(std::string pattern, int number) {
    int node = 0;
    for (int i = 0; i < pattern.size(); ++i) {
        char chr = pattern[i] - first;
        if (nodes[node].has_transition(chr)) {
            node = nodes[node].get_transition(chr);
        } else {
            nodes.emplace_back(node, chr);
            nodes[node].add_transition(chr, nodes.size() - 1);
            node = nodes.size() - 1;
        }
   }
    nodes[node].string_ids.push_back(number);
    nodes[node].is_terminal = true;
}


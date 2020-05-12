






#include <iostream>
#include <vector>
#include <string>

//Trie data structure for pattern finding
class Trie{
private:
    struct Node{
        //This bit differs from a standart implimentation:
        //I don't store children apart from transitions because it's actually redundant
        std::vector<int> transitions;

        int parent;
        char to_parent;

        int suff_link;
        bool is_terminal;
        //This bit differs from a standart implimentation too:
        //string_ids stores not only strings ending in the terminal,
        //but also all the string in all the terminals if you go up
        //terminal link recursively
        //transversed - string_ids was already collected
        bool transversed;
        std::vector<int> string_ids;

        //Standart constructor
        Node(int parent, char to_parent);
        //Root constructor
        Node();

        //Functions for simple checks
        bool has_transition(char chr);
        bool has_suff_link();
        int get_transition(char chr);
        void add_transition(char chr, int transition);
    };

    int current_node;
    std::vector<Node> nodes;

    int get_transition(int node, char chr);
    int get_suff_link(int node);
    int get_terminal_link(int node);
    void transverse(int node);

    //Fancy printing. Not used by the algorithm
    void internal_print(int node, int depth, bool shift, std::string padding);
public:

    //Constuctor can only construct empty tries
    Trie();

    //Use this function to fill p the trie
    void add_string(std::string pattern, int number);
    //Change state of the trie. Returns strings ids ending in a new node
    std::vector<int>& to_next_symbol(char chr);

    //Public entry point for fancy printing
    void print();
};


//Algorithm for finding patterns with masks using trie
class CorasickMachine {
private:
    Trie trie;
    //List of no-mask chunks
    std::vector<std::string> pattern;
    //and their positions witing the pattern
    std::vector<int> positions;

    //Used for spotting the pattern
    std::vector<int> matches;
    //Where we store the answer
    std::vector<int> entries;

    int index;
    int head;
    int length;
public:
    //Constructs from original pattern
    CorasickMachine(std::string pattern_string);

    //Online mode (efficient but slow)
    void test_next_symbol(char chr);
    //Offline mode (VERY costly but fast)
    void test_whole_text(const std::string &text);

    //Get the result (disabled)
    std::vector<int>& result();
};

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


CorasickMachine::CorasickMachine(std::string pattern_string): index(0),
                                                              head(0),
                                                              entries(0),
                                                              pattern(0),
                                                              positions(0),
                                                              matches(pattern_string.size(), 0),
                                                              length(pattern_string.size()),
                                                              trie() {
    bool chunk = false;
    for (int i = 0; i < pattern_string.size(); ++i) {
        if (pattern_string[i] != '?') {
            if (!chunk) {
                positions.push_back(i);
                pattern.push_back("");
                chunk = true;
            }
            pattern[pattern.size()-1] += pattern_string[i];
        } else {
            chunk = false;
        }
    }
    for (int i = 0; i < pattern.size(); ++i) {
        trie.add_string(pattern[i], i);
    }
}

//Slightly less time efficient (for some reason)
//Really memory efficient
void CorasickMachine::test_next_symbol(char chr) {
    for (int id : trie.to_next_symbol(chr)) {
        int pattern_pos = head - pattern[id].size() - positions[id] + 1;
        if (pattern_pos < 0) {
            pattern_pos += length;
        }
        ++matches[pattern_pos];
    }
    ++index;
    ++head;
    if (head == length) {
        head = 0;
    }
    if (matches[head] == pattern.size() && index >= length) {
        std::cout << index - length << ' ';
        //result.push_back(index - length);
    }
    matches[head] = 0;
}

//Slightly more time efficient (for some reason)
//EXTRIMELY memory inefficient
void CorasickMachine::test_whole_text(const std::string &text) {
    matches.resize(text.size());
    for (int index = 0; index < text.size(); ++index) {
        std::vector<int> &string_ids = trie.to_next_symbol(text[index]);
        for (int id : string_ids) {
            int pattern_pos = index - pattern[id].size() - positions[id] + 1;
            if (pattern_pos >= 0) {
                ++matches[pattern_pos];
            }
        }
        if (index >= length - 1 && matches[index - length + 1] == pattern.size()) {
            std::cout << index - length + 1 << ' ';
            //result.push_back(index - length + 1);
        }
    }
}

std::vector<int>& CorasickMachine::result() {
    return entries;
}

int main() {
    //SPEED
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    //Pattern finding machine init
    std::string pattern;
    std::cin >> pattern;
    CorasickMachine corasick(pattern);

    //Online text processing
    //char chr = 0;
    //while (std::cin >> chr) {
    //    corasick.test_next_symbol(chr);
    //}

    //Not as online as it used to be
    std::string text;
    std::cin >> text;
    corasick.test_whole_text(text);

    //Result (shutdown for the sake of SPEED)
    //for (int entrie : corasick.result()) {
    //    std::cout << entrie << ' ';
    //}
    return 0;
}


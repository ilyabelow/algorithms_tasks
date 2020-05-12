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


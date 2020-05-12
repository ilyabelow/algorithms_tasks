#include "Trie.h"
#include <vector>
#include <string>

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

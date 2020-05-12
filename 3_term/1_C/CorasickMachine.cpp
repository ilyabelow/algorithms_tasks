#include "CorasickMachine.h"
#include <iostream>

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

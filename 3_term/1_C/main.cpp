#include "CorasickMachine.h"
#include <iostream>

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


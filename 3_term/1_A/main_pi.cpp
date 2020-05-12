#include <iostream>
#include <vector>
#include <string>

//Machine that searches pattern entries in online text
class KMP_machine_PI{
private:
    //Index of the current symbol
    int index;
    //Index where prefix will be extended
    int extention;
    //Pattern that the machine will find
    std::string pattern;
    //Prefix function for pattern
    std::vector<int> pi_func;
    //List of pattern entries into text
    std::vector<int> entries;
public:
    KMP_machine_PI(std::string pattern);
    //Send next symbol to test
    void test_next_symbol(char symbol);
    //Get the answer (probably not the best solution)
    std::vector<int> result();
};

//Calculate prefix function for pattern
KMP_machine_PI::KMP_machine_PI(std::string pattern): pattern(pattern),
                                                     pi_func(pattern.size()),
                                                     entries(0),
                                                     extention(0),
                                                     index(0){
    for (int i = 1; i < pattern.size(); ++i){
        extention = pi_func[i - 1];
        while (pattern[extention] != pattern[i] && extention > 0){
            extention = pi_func[extention-1];
        }
        //Bodge for the case when prefix function should be zero
        if (extention == 0 && pattern[0] != pattern[i]){
            extention--;
        }
        pi_func[i] = extention + 1;
    }
    //= prefix func for # symbol between pattern and text is 0
    extention = 0;
}

//Calculate prefix function online
void KMP_machine_PI::test_next_symbol(char symbol){
    //If we cannot extend prefix or bump into #,
    if (extention == pattern.size() || pattern[extention] != symbol){
        //we go jumping backwards as usual
        extention = pi_func[extention - 1];
        while (pattern[extention] != symbol && extention > 0){
            extention = pi_func[extention - 1];
        }
        //Same bodge as in constructor, but in reverse
        if (extention != 0 || pattern[0] == symbol){
            extention++;
        }
    }else{ //else - we can extend current prefix
        extention++;
    }
    //Check if we've found a match
    if (extention == pattern.size()){
        entries.push_back(index - pattern.size() + 1);
    }
    ++index;
}

std::vector<int> KMP_machine_PI::result(){
    return entries;
}

int main(){
    //SPEED
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    //Setup
    std::string pattern;
    char symbol = 0;
    std::cin >> pattern;
    KMP_machine_PI kmp(pattern);

    //Read text by symbols and send it to KMP machine
    while (std::cin >> symbol){
        kmp.test_next_symbol(symbol);
    }
    //Output the answer
    for (int i : kmp.result()){
        std::cout << i << ' ';
    }
}

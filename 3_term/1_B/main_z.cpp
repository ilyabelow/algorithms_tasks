#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

//Restores minimal string from a valid z-functiob
std::string restore_string_from_z(const std::vector<int> &z_func){
    std::string result = "a";
    //Current index in z-function
    int i = 1;
    //Letters that are forbidden for use in case we need to add a new letter
    std::vector<char> forbidden_letters(26, 0);
    forbidden_letters[0] = 1;
    int  max_forb_letter = 0;
    while (i < z_func.size()){
        //If z-function is not zero, we copy the prefix
        if (z_func[i] != 0){
            //Reset forbidden letters
            for (int j = 1; j <= max_forb_letter; ++j){
                forbidden_letters[j] = 0;
            }
            max_forb_letter = 0;
            int z_block_start = i;
            int z_block_end = i + z_func[i];
            //We copy until z-block end or until we find a bigger
            //z-block inside the current z-block
            while (i < z_block_end && z_func[i] <= z_block_end - i){
                //For all z-blocks that ends in one place we forbid letters
                //that would expand them if we were to add a new letter after
                //the blocks
                if (z_func[i] == z_block_end - i){
                    forbidden_letters[result[z_func[i]] - 'a'] = 1;
                    max_forb_letter = std::max(max_forb_letter, result[z_func[i]] - 'a');
                }
                //Copy string
                result += result[i - z_block_start];
                ++i;
            }
        } else {
            //If z-function is zero, we try to add a new letter
            //First non-forbidden letter
            int allowed_letter = 0;
            while (forbidden_letters[allowed_letter] == 1){
                ++allowed_letter;
            }
            result += allowed_letter + 'a';
            ++i;
            //Reset forbidden letters
            for (int j = 1; j <= max_forb_letter; ++j){
                forbidden_letters[j] = 0;
            }
            max_forb_letter = 0;
        }
    }
    return result;
}

int main(){
    //Setup
    std::vector<int> z_func(0);
    int next_z = 0;
    while (std::cin >> next_z){
        z_func.push_back(next_z);
    }
    //Calculate the answer
    std::cout << restore_string_from_z(z_func);
    return 0;
}

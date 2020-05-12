#include <iostream>
#include <string>
#include <vector>

// Cardinality of the alphabet
const int alphabet = 26;

// Calculate suffix array for a given string for O(n log n)
std::vector<int> build_suffix_array(const std::string& str) {
    const int alphabet_dollar = alphabet + 1;
    std::string str_dollar = str + "`";
    int len = str_dollar.size();
    std::vector<int> suff(len);
    // Intermidiate sorting
    std::vector<int> inter_suff(len);
    // Equivalence classes
    std::vector<int> classes(len);
    // Intermidiate classes
    std::vector<int> inter_classes(len);
    // Used for count sort
    std::vector<int> count(alphabet_dollar);

    // PHASE 0
    // Initiate suffix array
    // TODO merge count sort into seperate nice function
    for (int i = 0; i < len; ++i) {
        ++count[str_dollar[i]-'`'];
    }
    for (int i = 1; i < alphabet_dollar; ++i) {
        count[i] += count[i - 1];
    }
    for (int i = len - 1; i >= 0; --i) {
        --count[str_dollar[i]-'`'];
        suff[count[str_dollar[i]-'`']] = i;
    }
    // Initiate classes
    int current_class = 0;
    classes[suff[0]] = 0;
    for (int i = 1; i < len; ++i) {
        if (str_dollar[suff[i]] != str_dollar[suff[i-1]]){
            current_class += 1;
        }
        classes[suff[i]] = current_class;
    }

    // PHASE 1 --> logn
    for (int k = 1; (1<<(k-1)) < len; ++k) {
        // Shifting all the strings 2^(k-1) to the right
        for (int i = 0; i < len; ++i) {
            inter_suff[i] = suff[i] - (1<<(k-1));
            if (inter_suff[i] < 0) {
                inter_suff[i] += len;
            }
        }
        // Count sort first halves of suffixes
        for (int i = 0; i < current_class+1; ++i) {
            count[i] = 0;
        }
        for (int i = 0; i < len; ++i) {
            ++count[classes[inter_suff[i]]];
        }
        for (int i = 1; i < current_class+1; ++i) {
            count[i] += count[i - 1];
        }
        for (int i = len - 1; i >= 0; --i) {
            --count[classes[inter_suff[i]]];
            suff[count[classes[inter_suff[i]]]] = inter_suff[i];
        }
        // Updating classes
        current_class = 0;
        inter_classes[suff[0]] = 0;
        for (int i = 1; i < len; ++i) {
            int shifted_i = (suff[i] + (1<<(k-1))) % len;
            int shifted_j = (suff[i-1] + (1<<(k-1))) % len;
            if (classes[suff[i]] != classes[suff[i-1]] ||
                classes[shifted_i] != classes[shifted_j] ) {
                ++current_class;
            }
            inter_classes[suff[i]] = current_class;
        }
        // Amount of classes = amount of suffixes => no more sorting is required
        if (current_class == len-1) {
            break;
        }
        classes = inter_classes;
    }
    // Delete $
    suff.erase(suff.begin());
    return suff;
}

// Calculate lcp using Kasai algorithm
// Let x be index in suffix array
// Let i be index in string
// suff:    x ---> i
// suff^-1: i ---> x
std::vector<int> build_lcp(const std::string str, const std::vector<int> suff) {
    int len = suff.size();
    //Inverse suffix array
    std::vector<int> suff_inv(len);
    for (int x = 0; x < len; ++x) {
        suff_inv[suff[x]] = x;
    }
    // Calculate lcp
    std::vector<int> lcp(len);
    for (int i = 0; i < len; ++i) {
        int x = suff_inv[i];
        // Exception: lcp[0] = -1
        if (x == 0) {
            lcp[0] = -1;
            continue;
        }
        int k = 0;
        // Exception: when i = 0, we calculate lcp explicitely
        if (i != 0) {
            k = lcp[suff_inv[i-1]] - 1;
        }
        // Exeption: if lcp[suff_inv[i-1]] == 0, we calc lcp explicitely
        if (k < 0) {
            k = 0;
        }
        // Compare suffixes
        while (k + suff[x] < len && k + suff[x-1] < len && str[k+suff[x]] == str[k+suff[x-1]]) {
            ++k;
        }
        lcp[x] = k;
    }
    return lcp;
}

// Count different substrins in a string
int count_substrings(const std::string str) {
    // Find suffiox array and lcp
    std::vector<int> suff = build_suffix_array(str);
    std::vector<int> lcp = build_lcp(str, suff);
    // Calculate answer
    int len = str.size();
    int answer = len - suff[0];
    for (int x = 1; x < len; ++x) {
        answer += len - suff[x] - lcp[x];
    }
    return answer;
}

int main() {
    //Input
    std::string str;
    std::cin >> str;
    // Output
    std::cout << count_substrings(str);
    return 0;
}

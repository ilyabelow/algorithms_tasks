#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// Cardinality of the alphabet
const int alphabet = 27;

// Calculate suffix array for a given string for O(n log n)
std::vector<int> build_suffix_array(const std::string& str) {
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
    std::vector<int> count(std::max(alphabet+1, len));

    // PHASE 0
    // Initiate suffix array
    // TODO merge count sort into seperate nice function
    for (int i = 0; i < len; ++i) {
        ++count[str_dollar[i]-'`'];
    }
    for (int i = 1; i < count.size(); ++i) {
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
            //break;
        }
        std::swap(classes, inter_classes);
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
std::vector<int> build_lcp(const std::string &str, const std::vector<int> &suff) {
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

std::string find_kth_common_substring(const std::string &str1, const std::string &str2, long long k) {
    std::string str = str1 + "{" + str2;
    auto suff = build_suffix_array(str);
    auto lcp = build_lcp(str, suff);
    int len1 = str1.size();
    int len_wo_brace = str.size()-1; // { is the last symbol
    long long count = 0; // Amount of processed substrings
    int len = 0; // Current substing length
    int substr_start = -1; // Start of the desired substring
    for (int x = 1; x < len_wo_brace; ++x) {
        // 1: reset pos if lcp > pos
        if (len > lcp[x]) {
            len = lcp[x];
        }
        // 2: check if suffix ownership has changed
        if ((suff[x] < len1) ^ (suff[x-1] < len1)) {
            // 3: if it did, continue coiunting substrings
            if (count + lcp[x] - len >= k) {
                // 4: if we've count up k substrings, stop and record result
                len += k - count;
                substr_start = suff[x];
                break;
            }
            count += lcp[x] - len;
            len = lcp[x];
        }
    }
    if (substr_start == -1) {
        return "";
    }
    return str.substr(substr_start, len);
}

int main() {
    //Input
    long long k;
    std::string str1, str2;
    std::cin >> str1 >> str2 >> k;
    // Output
    std::string answer = find_kth_common_substring(str1, str2, k);
    if (answer.empty()) {
        std::cout << -1;
    } else {
        std::cout << answer;
    }
    return 0;
}

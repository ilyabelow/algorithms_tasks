#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <set>

//Shortcuts to POW and LOG2 functions from stl, casted to integer
int int_pow(int base, int exp);
int int_log2(int number);

//Class with sparse table
class SparseTable {
 private:
  //Sparse table itself with indexes to original array
  std::vector<std::vector<std::pair<int, int>>> table;
  //Original array
  std::vector<int> array;
  //Preprocessed logarithms
  std::vector<int> logs;
  //Takes two pairs and returns pair of first and second minimums
  std::pair<int, int> getMinPair(const std::pair<int, int> &first, const std::pair<int, int> &second);
 public:
  //Construct table
  explicit SparseTable(std::vector<int> &&init);
  //Get second minimum from range
  int Request(int left, int right);
};

int main() {
  //Reading incoming sequence
  int array_size, requests_count;
  std::cin >> array_size >> requests_count;
  std::vector<int> input_array(array_size);
  for (int i = 0; i < array_size; ++i) {
    std::cin >> input_array[i];
  }
  //Constructing table
  SparseTable table(std::move(input_array));
  //Writing answer
  for (int j = 0; j < requests_count; ++j) {
    int left, right;
    std::cin >> left >> right;
    std::cout << table.Request(left - 1, right - 1) << std::endl;
  }
  return 0;
}

SparseTable::SparseTable(std::vector<int> &&init) :
    table(int_log2(init.size()) + 1),
    array(init), logs(init.size()) {
  for (int k = 0; k < init.size(); ++k) {
    logs[k] = int_log2(k + 1);
  }
  //First layer just points to original array
  for (int i = 0; i < init.size(); ++i) {
    table[0].emplace_back(i, i);
  }
  for (int j = 1; j < table.size(); ++j) {
    //Every next column is shrieked
    table[j] = std::vector<std::pair<int, int>>(init.size() - int_pow(2, j) + 1);
    for (int i = 0; i < table[j].size(); ++i) {
      //Calculating every other table element according to formula from description
      table[j][i] = getMinPair(table[j - 1][i], table[j - 1][i + int_pow(2, j - 1)]);
    }
  }
}

int SparseTable::Request(int left, int right) {
  //Returning value according to formula
  int j = logs[right - left];  //shift to -1
  //getMinPair returns index, we need index of second min
  return array[getMinPair(table[j][left], table[j][right - int_pow(2, j) + 1]).second];
}

//Static cast is bad, but what can we do?
int int_pow(int base, int exp) {
  return static_cast<int>(std::pow(base, exp));
}

int int_log2(int number) {
  return static_cast<int>(std::log2(number));
}

std::pair<int, int> SparseTable::getMinPair(const std::pair<int, int> &first, const std::pair<int, int> &second) {
  //This function mixes two pairs together to find a global min1 and min2
  //
  //The whole point of indexes instead of values in SparseTable::table is this:
  //we can't tell apart these two situations:
  //we have two identical numbers because of we have two copies of it in the original vector
  //or because of an overlap?
  //Let me explain:
  //100 100 0 50 100 100 on (1, 6)
  //will return 0 because we have windows 100 100 0 50 and 0 50 100 100
  //we mix and sort them and get 0 0 50 50 100 100 100 100. Second min is, surprise, 0
  //0 100 100 100 100 0 on (1, 6)
  //will return 0 and it will be the correct answer. So if we kill duplicates, we
  //will lose second zero
  //
  //this is avoided if we store indexes! Woohoo!
  std::set<int> all_elems = {first.first, first.second, second.second, second.first}; //set will kill duplications
  int first_min = -1;
  int second_min = -1;
  for (auto i : all_elems) {
    //If min is not filled, FILL IT
    //we always have at least 2 elements in all_elem
    if (first_min == -1 || array[i] < array[first_min]) {
      second_min = first_min;
      first_min = i;
    } else if (second_min == -1 || array[i] < array[second_min]) {
      second_min = i;
    }
  }
  return std::make_pair(first_min, second_min);
}

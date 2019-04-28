#include <iostream>
#include <cstdint>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>
#include <algorithm>

//Struct that compactly stores state of the game. S is board sidelength
template<uint8_t S>
class State {
 private:
  uint64_t state;
  uint8_t zero;

  void SetNumber(uint64_t number, uint8_t position);
  uint64_t GetNumber(uint8_t position) const;
  void Move(uint8_t next_zero);

 public:
  explicit State(std::vector<uint64_t> initial_state);
  explicit State(uint64_t initial_state);
  State();

  uint64_t GetInt() const;

  State GetLeft() const;
  State GetRight() const;
  State GetUp() const;
  State GetDown() const;

  void MoveUp();
  void MoveDown();
  void MoveLeft();
  void MoveRight();

  const std::vector<State> GetNext() const;
  const std::vector<uint64_t> ToVector() const;

  void PrintState() const;
  void PrintBinary() const;
};

template<uint8_t S>
std::string FindSolution(State<S> initial_state, State<S> final_state);
template<uint8_t S>

bool CheckIfSolvable(State<S> initial_state);

int main() {
  //Reading initial state
  std::vector<uint64_t> initial_state(16);
  for (int i = 0; i < 16; ++i) {
    std::cin >> initial_state[i];
  }
  //Find solution
  std::string solution =
      FindSolution(State<4>(initial_state), State<4>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0}));
  //Solution output
  if (solution == "No solution") {
    std::cout << -1;
  } else {
    std::cout << solution.size() << std::endl << solution;
  }
  return 0;
}

template<uint8_t S>
std::string FindSolution(State<S> initial_state, State<S> final_state) {
  if (!CheckIfSolvable(initial_state)) {
    return "No solution";
  }
  //Fool proofing
  if (initial_state.GetInt() == final_state.GetInt()) {
    return "";
  }
  std::queue<uint64_t> queue; //bfs queue
  std::unordered_map<uint64_t, uint8_t> path; //record moves
  //initializing
  queue.push(initial_state.GetInt());
  path.emplace(initial_state.GetInt(), 4);

  bool found = false;
  while (!found) {
    State<S> current_state(queue.front());
    queue.pop();
    auto next = current_state.GetNext();
    //next position processing. i represents move (up/down/left/right)
    for (int i = 0; i < 4; ++i) {
      if (next[i].GetInt() == final_state.GetInt()) { //we've solved it, hurray!
        found = true;
        path[next[i].GetInt()] = i;
        break;
      } else {
        //such a move exists AND such a state is not visited
        if (next[i].GetInt() != 0 && path.find(next[i].GetInt()) == path.end()) {
          path[next[i].GetInt()] = i;
          queue.push(next[i].GetInt());
        }
      }
    }
  }
  //resolving path
  std::string solution;
  State<S> current_state = final_state;
  while (true) {
    if (path[current_state.GetInt()] == 0) {
      solution += 'U';
      current_state.MoveDown(); //inverted because we are going back
    } else if (path[current_state.GetInt()] == 1) {
      solution += 'D';
      current_state.MoveUp();
    } else if (path[current_state.GetInt()] == 2) {
      solution += 'L';
      current_state.MoveRight();
    } else if (path[current_state.GetInt()] == 3) {
      solution += 'R';
      current_state.MoveLeft();
    } else { // = 4 => initial state
      break;
    }
  }
  std::reverse(solution.begin(), solution.end());
  return solution;
}

//Checks if a oard is solvable. TODO works because of bodge
template<uint8_t S>
bool CheckIfSolvable(State<S> initial_state) {
  auto state = initial_state.ToVector();
  int inversions = 0;
  //counting inversions
  for (int i = 0; i < S * S; ++i) {
    if (state[i] != 0) {
      for (int j = 0; j < i; ++j) {
        if (state[j] > state[i]) {
          ++inversions;
        }
      }
    } else if (S % 2 == 0) {
      inversions += 1 + i / S; //idk y this works
    }
  }
  //if even then solvable
  return inversions % 2 == 0;
}

//Empty thus not valid state
template<uint8_t S>
State<S>::State(): state(0), zero(0) {}

//Initializing from another state
template<uint8_t S>
State<S>::State(uint64_t initial_state):state(initial_state) {
  for (uint8_t i = 0; i < S * S; ++i) {
    if (GetNumber(i) == 0) {
      zero = i;
      break;
    }
  }
}

//Initializing from human-readable vector
template<uint8_t S>
State<S>::State(std::vector<uint64_t> initial_state) {
  state = 0;
  for (uint8_t i = 0; i < S * S; ++i) {
    SetNumber(initial_state[i], i);
    if (initial_state[i] == 0) {
      zero = i;
    }

  }
}
//Set number on a particular position
template<uint8_t S>
void State<S>::SetNumber(uint64_t number, uint8_t position) {
  state &= UINT64_MAX - (15ull << (position * 4));
  if (number != 0) {
    state |= number << (position * 4);
  }
}

//Get number from a particular position
template<uint8_t S>
uint64_t State<S>::GetNumber(uint8_t position) const {
  return ((15ull << (position * 4)) & state) >> (position * 4);
}

//Get raw state
template<uint8_t S>
uint64_t State<S>::GetInt() const {
  return state;
}

//Returns reachable states
template<uint8_t S>
const std::vector<State<S>> State<S>::GetNext() const {
  std::vector<State<S>> next(4);
  if (zero >= S) {
    next[0] = GetUp();
  }
  if (zero < S * (S - 1)) {
    next[1] = GetDown();
  }
  if (zero % S != 0) {
    next[2] = GetLeft();
  }
  if (zero % S != S - 1) {
    next[3] = GetRight();
  }
  return next;
}

//Return state in which 0 tile is moved
template<uint8_t S>
State<S> State<S>::GetLeft() const {
  State left(*this);
  left.MoveLeft();
  return left;
}

template<uint8_t S>
State<S> State<S>::GetRight() const {
  State right(*this);
  right.MoveRight();
  return right;
}

template<uint8_t S>
State<S> State<S>::GetUp() const {
  State up(*this);
  up.MoveUp();
  return up;
}

template<uint8_t S>
State<S> State<S>::GetDown() const {
  State down(*this);
  down.MoveDown();
  return down;
}

//Move 0 tile
template<uint8_t S>
void State<S>::MoveUp() {
  Move(zero - S);
}

template<uint8_t S>
void State<S>::MoveDown() {
  Move(zero + S);
}

template<uint8_t S>
void State<S>::MoveLeft() {
  Move(zero - 1);
}

template<uint8_t S>
void State<S>::MoveRight() {
  Move(zero + 1);
}

template<uint8_t S>
void State<S>::Move(uint8_t next_zero) {
  SetNumber(GetNumber(next_zero), zero);
  SetNumber(0, next_zero);
  zero = next_zero;
}

//Get vector representation of state
template<uint8_t S>
const std::vector<uint64_t> State<S>::ToVector() const {
  std::vector<uint64_t> to_vector(S * S);
  for (uint8_t i = 0; i < S * S; ++i) {
    to_vector[i] = GetNumber(i);
  }
  return to_vector;
}
//Debug and stuf
template<uint8_t S>
void State<S>::PrintState() const {
  for (int i = 0; i < S * S; ++i) {
    std::cout << GetNumber(i) << ' ';
    if (i % S == S - 1) {
      std::cout << std::endl;
    }
  }
  std::cout << std::endl;
}

template<uint8_t S>
void State<S>::PrintBinary() const {
  uint64_t number = state;
  for (uint8_t i = 0; i < 32; ++i) {
    std::cout << (number & 1);
    number >>= 1;
    if (i % 4 == 3) {
      std::cout << ' ';
    }
  }
  std::cout << std::endl;
}


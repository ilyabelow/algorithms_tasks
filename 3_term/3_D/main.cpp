#include "Hull2.hpp"
#include <iostream>
#include <algorithm>
#include <iomanip>

int main() {
    Hull2 hello;
    std::cin >> hello;
    std::cout <<std::setprecision(6) <<  hello.calculate_avg_edges();
    return 0;
}

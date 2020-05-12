#include "Hull3.hpp"
#include <iostream>

int main() {
    int m;
    std::cin >> m;
    for (int i = 0; i < m; ++i) {
        Hull3 hull;
        std::cin >> hull;
        hull.rotate({.001, .001, .001});
        hull.build_hull();
        std::sort(hull.faces.begin(), hull.faces.end(), LessFace());
        std::cout << hull;
    }
    return 0;
}

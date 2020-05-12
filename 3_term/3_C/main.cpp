#include "Vector2.hpp"
#include "Polygon2.hpp"
#include <iostream>


int main() {
    int n;
    std::cin >> n;
    std::vector<Vector2> for_p1(n);
    for (int i = n-1; i >= 0; --i) {
        std::cin >> for_p1[i];
    }
    int m;
    std::cin >> m;
    std::vector<Vector2> for_p2(m);
    for (int i = m-1; i >= 0; --i) {
        std::cin >> for_p2[i];
    }
    Polygon p1(std::move(for_p1));
    Polygon p2(std::move(for_p2));
    Polygon diff = p1 - p2;
    std::cout << (diff.is_zero_inside() ? "YES" : "NO");
    return 0;
}

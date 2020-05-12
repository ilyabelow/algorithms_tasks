#include "Vector3.hpp"
#include "Segment3.hpp"
#include <iostream>
#include <stdio.h>

int main() {
    double x1, y1, z1;
    double x2, y2, z2;
    std::cin >> x1 >> y1 >> z1;
    std::cin >> x2 >> y2 >> z2;
    Segment3 s1(x1, y1, z1, x2, y2, z2);
    double x3, y3, z3;
    double x4, y4, z4;
    std::cin >> x3 >> y3 >> z3;
    std::cin >> x4 >> y4 >> z4;
    Segment3 s2(x3, y3, z3, x4, y4, z4);
    printf("%.12lf", s1.distance_to_segment(s2));
}

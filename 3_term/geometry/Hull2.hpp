#ifndef HULL2_HPP
#define HULL2_HPP

#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include "Vector2.hpp"
#include "Face.hpp"
#include "Hull3.hpp"

class Hull3;

class Hull2 {
private:
    Hull3 parabaloid_projection();
    void delaunay_triangulation();
public:
    std::vector<Vector2> points;
    std::vector<Face> faces;
    std::unordered_set<int> hull_2d;

    double calculate_avg_edges();
    friend std::istream& operator>>(std::istream& in, Hull2& v);
    friend std::ostream& operator<<(std::ostream& out, const Hull2& v);
};

#endif

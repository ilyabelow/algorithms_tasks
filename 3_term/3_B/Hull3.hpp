#ifndef HULL3_HPP
#define HULL3_HPP

#include <vector>
#include <unordered_set>
#include "Vector3.hpp"
#include "Face.hpp"
#include "Hull2.hpp"

struct Point {
    double x;
    double y;
    double z;
    Point* prev;
    Point* next;

    Point() = default;
    Point(const Vector3& vector);
    Point(double x, double y, double z, Point* prev, Point* next);

    bool act();
};

class Hull2;

class Hull3 {
private:
    inline double time_of_turn(Point* a, Point* b, Point* c);
    inline double turn_on_inf(Point* a, Point* b, Point* c);
    Point* merge_sort(Point list[], int n);
    void hull(Point* list, int n, Point** out, Point** buffer);
public:
    Hull3() = default;
    Hull3(const Hull2& hull);

    std::vector<Vector3> points;
    std::vector<Face> faces;

    std::unordered_set<int> build_whole_hull();
    void rotate(const Vector3& n);
    friend std::istream& operator>>(std::istream& in, Hull3& v);
    friend std::ostream& operator<<(std::ostream& out, const Hull3& v);
};


#endif

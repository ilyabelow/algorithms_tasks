#include "Segment3.hpp"
#include <cmath>

#define EPSILON 1.e-6

Segment3::Segment3 (const Vector3& start, const Vector3& end): start(start), end(end) {}
Segment3::Segment3 (double x1, double y1, double z1, double x2, double y2, double z2):
                                                 start(Vector3(x1, y1, z1)),
                                                 end(Vector3(x2, y2, z2)) {}

Vector3 Segment3::to_vector() const {
    return end - start;
}

double Segment3::magnitude() const {
    return to_vector().magnitude();
}

double Segment3::distance_to_point(const Vector3& point) const {
    Vector3 direction = to_vector()/3;
    Vector3 cur_start = start;
    double min_dist = 0;
    double new_dist = 0;
    do {
        min_dist = new_dist;
        double distance1 = point.distance_to(cur_start + direction);
        double distance2 = point.distance_to(cur_start + direction*2);
        if (distance1 > distance2) {
            cur_start += direction;
            new_dist = distance2;
        } else {
            new_dist = distance1;
        }
        direction *= 2/3.;
    } while (fabs(new_dist - min_dist) > EPSILON);
    return min_dist;
}

double Segment3::distance_to_segment(const Segment3& another) const {
    Vector3 direction = to_vector()/3;
    Vector3 cur_start = start;
    double min_dist = 0;
    double new_dist = 0;
    do {
        min_dist = new_dist;
        double distance1 = another.distance_to_point(cur_start + direction);
        double distance2 = another.distance_to_point(cur_start + direction*2);
        if (distance1 > distance2) {
            cur_start += direction;
            new_dist = distance2;
        } else {
            new_dist = distance1;
        }
        direction *= 2/3.;
    } while (fabs(new_dist - min_dist) > EPSILON);
    return min_dist;
}

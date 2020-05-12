#ifndef SEGMENT3_HPP
#define SEGMENT3_HPP

#include "Vector3.hpp"



class Segment3 {
private:
    Vector3 start;
    Vector3 end;
public:
    Segment3 (const Vector3& start, const Vector3& end);
    Segment3 (double x1, double y1, double z1, double x2, double y2, double z2);
    ~Segment3() = default;
    Segment3(const Segment3& another) = default;

    double distance_to_point(const Vector3& point) const;
    double distance_to_segment(const Segment3& another) const;

    Vector3 to_vector() const;
    double magnitude() const;
};

#endif

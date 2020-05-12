#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include "Vector2.hpp"

class Polygon {
private:
    std::vector<Vector2> vertices;
    int leftmost;
    int size;

    void calculate_leftmost_point();
public:
    Polygon(std::vector<Vector2>&& vertices);
    Polygon(const std::vector<Vector2>& vertices);
    Polygon();
    ~Polygon() = default;
    Polygon(const Polygon& another) = default;
    Polygon(Polygon&& another) = default;

    Polygon& operator-=(const Vector2& point) &;
    Polygon operator-(const Vector2& point) const;
    bool is_point_inside(const Vector2& point) const;
    bool is_zero_inside() const;

    Polygon& invert() &;
    Polygon operator-() const;

    Polygon operator+(const Polygon& another) const;
    Polygon operator-(const Polygon& another) const;

    int get_size() const;
    const Vector2& operator[](int i) const;

    friend std::ostream& operator<<(std::ostream& out, const Polygon& polygon);
};

#endif

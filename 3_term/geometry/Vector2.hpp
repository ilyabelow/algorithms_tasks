#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <istream>
#include <ostream>
#include <initializer_list>

class Vector2 {
private:
    double x;
    double y;
public:
    Vector2(std::initializer_list<double> list);
    Vector2(double x, double y);
    Vector2();
    ~Vector2() = default;
    Vector2(const Vector2& another) = default;

    Vector2& operator=(const Vector2& another) &;

    Vector2& operator+=(const Vector2& another) &;
    Vector2 operator+(const Vector2& another) const;
    Vector2& operator-=(const Vector2& another) &;
    Vector2 operator-(const Vector2& another) const;

    double dot_product(const Vector2& another) const;
    double cross_product(const Vector2& another) const;

    Vector2& operator*=(double scalar) &;
    Vector2 operator*(double scalar) const;
    Vector2& operator/=(double scalar) &;
    Vector2 operator/(double scalar) const;

    bool operator==(const Vector2& another) const;
    bool operator!=(const Vector2& another) const;
    bool operator<(const Vector2& another) const;

    Vector2 operator-() const;
    Vector2 operator+() const;
    Vector2& invert() &;

    double magnitude() const;
    Vector2& normalize() &;
    double distance_to(const Vector2& another) const;
    double operator^(const Vector2& another) const;

    double get_x() const;
    double get_y() const;

    friend std::ostream& operator<<(std::ostream& out, const Vector2& v);
    friend std::istream& operator>>(std::istream& in, Vector2& v);
};
#endif

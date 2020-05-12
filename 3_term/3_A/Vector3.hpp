#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <iostream>
#include <initializer_list>
#include "Vector2.hpp"

class Vector3 {
private:
    double x;
    double y;
    double z;
public:
    Vector3(std::initializer_list<double> list);
    Vector3(double x, double y, double z);
    Vector3(double x, double y);
    Vector3(const Vector2& v);
    Vector3();
    ~Vector3() = default;
    Vector3(const Vector3& another) = default;

    Vector3& operator=(const Vector3& another) &;

    Vector3& operator+=(const Vector3& another) &;
    Vector3 operator+(const Vector3& another) const;
    Vector3& operator-=(const Vector3& another) &;
    Vector3 operator-(const Vector3& another) const;

    double dot_product(const Vector3& another) const;
    Vector3 cross_product(const Vector3& another) const;

    Vector3& operator*=(double scalar) &;
    Vector3 operator*(double scalar) const;
    Vector3& operator/=(double scalar) &;
    Vector3 operator/(double scalar) const;

    bool operator==(const Vector3& another) const;
    bool operator!=(const Vector3& another) const;

    Vector3 operator-() const;
    Vector3 operator+() const;
    Vector3& invert() &;

    double magnitude() const;
    Vector3& normalize() &;
    double distance_to(const Vector3& another) const;
    Vector3 project(const Vector3& victum) const;


    double get_x() const;
    double get_y() const;
    double get_z() const;
    void project_on_paraboloid();

    friend std::ostream& operator<<(std::ostream& out, const Vector3& v);
    friend std::istream& operator>>(std::istream& in, Vector3& v);
};

#endif

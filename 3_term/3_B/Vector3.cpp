#include "Vector3.hpp"
#include <cmath>
#include <iostream>

#define EPSILON 1.e-6

Vector3::Vector3(std::initializer_list<double> list): x(*list.begin()), y(*(list.begin()+1)), z(*(list.begin()+2)) {}
Vector3::Vector3(double x, double y, double z): x(x), y(y), z(z) {}
Vector3::Vector3(double x, double y): x(x), y(y), z(0) {}
Vector3::Vector3(): x(0), y(0), z(0) {}
Vector3::Vector3(const Vector2& v): x(v.get_x()), y(v.get_y()), z(0) {}

Vector3& Vector3::operator=(const Vector3& another) & {
    x = another.x;
    y = another.y;
    z = another.z;
    return *this;
}

Vector3& Vector3::operator+=(const Vector3& another) & {
    x += another.x;
    y += another.y;
    z += another.z;
    return *this;
}

Vector3 Vector3::operator+(const Vector3& another) const {
    Vector3 result(*this);
    result += another;
    return result;
}

Vector3& Vector3::operator-=(const Vector3& another) & {
    x -= another.x;
    y -= another.y;
    z -= another.z;
    return *this;
}

Vector3 Vector3::operator-(const Vector3& another) const {
    Vector3 result(*this);
    result -= another;
    return result;
}

double Vector3::dot_product(const Vector3& another) const {
    return x*another.x + y*another.y + z*another.z;
}

Vector3 Vector3::cross_product(const Vector3& another) const {
    Vector3 result;
    result.x = y*another.z - z*another.y;
    result.y = -x*another.z + z*another.x;
    result.z = x*another.y - y*another.x;
    return result;
}

Vector3& Vector3::operator*=(double scalar) & {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vector3 Vector3::operator*(double scalar) const {
    Vector3 result(*this);
    result *= scalar;
    return result;
}

Vector3& Vector3::operator/=(double scalar) & {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}

Vector3 Vector3::operator/(double scalar) const {
    Vector3 result(*this);
    result /= scalar;
    return result;
}

bool Vector3::operator==(const Vector3& another) const {
    return (abs(x - another.x) < EPSILON)
        && (abs(y - another.y) < EPSILON)
        && (abs(z - another.z) < EPSILON);
}

bool Vector3::operator!=(const Vector3& another) const {
    return !(*this != another);
}

Vector3 Vector3::operator-() const {
    Vector3 result(*this);
    result.invert();
    return result;
}

Vector3 Vector3::operator+() const {
    Vector3 result(*this);
    return result;
}

Vector3& Vector3::invert() & {
    x = -x;
    y = -y;
    z = -z;
    return *this;
}

double Vector3::magnitude() const {
    return sqrt(x*x + y*y + z*z);
}

Vector3& Vector3::normalize() & {
    operator/=(magnitude());
    return *this;
}

double Vector3::distance_to(const Vector3& another) const {
    return (another - *this).magnitude();
}

Vector3 Vector3::project(const Vector3& victum) const {
    Vector3 result = *this;
    result.normalize();
    result *= dot_product(victum);
    return result;
}

std::ostream& operator<<(std::ostream& out, const Vector3& v) {
    out << v.x << ' ' << v.y << ' ' << v.z << '\n';
    return out;
}

std::istream& operator>>(std::istream& in, Vector3& v) {
    in >> v.x >> v.y >> v.z;
    return in;
}

double Vector3::get_x() const {
    return x;
}

double Vector3::get_y() const {
    return y;
}

double Vector3::get_z() const {
    return z;
}

void Vector3::project_on_paraboloid() {
    z = x*x + y*y;
}

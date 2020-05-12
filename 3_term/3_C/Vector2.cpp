#include "Vector2.hpp"
#include <cmath>
#include <ostream>
#include <istream>
#define EPSILON 1.e-6

Vector2::Vector2(std::initializer_list<double> list): x(*list.begin()), y(*(list.begin()+1)) {}
Vector2::Vector2(double x, double y): x(x), y(y) {}
Vector2::Vector2(): x(0), y(0) {}


Vector2& Vector2::operator=(const Vector2& another) & {
    x = another.x;
    y = another.y;
    return *this;
}

Vector2& Vector2::operator+=(const Vector2& another) & {
    x += another.x;
    y += another.y;
    return *this;
}

Vector2 Vector2::operator+(const Vector2& another) const {
    Vector2 result(*this);
    result += another;
    return result;
}

Vector2& Vector2::operator-=(const Vector2& another) & {
    x -= another.x;
    y -= another.y;
    return *this;
}

Vector2 Vector2::operator-(const Vector2& another) const {
    Vector2 result(*this);
    result -= another;
    return result;
}

double Vector2::dot_product(const Vector2& another) const {
    return x*another.x + y*another.y;
}

double Vector2::cross_product(const Vector2& another) const {
    return x*another.y - y*another.x;
}

Vector2& Vector2::operator*=(double scalar) & {
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector2 Vector2::operator*(double scalar) const {
    Vector2 result(*this);
    result *= scalar;
    return result;
}

Vector2& Vector2::operator/=(double scalar) & {
    x /= scalar;
    y /= scalar;
    return *this;
}

Vector2 Vector2::operator/(double scalar) const {
    Vector2 result(*this);
    result /= scalar;
    return result;
}

bool Vector2::operator==(const Vector2& another) const {
    return (abs(x - another.x) < EPSILON)
        && (abs(y - another.y) < EPSILON);
}

bool Vector2::operator!=(const Vector2& another) const {
    return !(*this != another);
}

bool Vector2::operator<(const Vector2& another) const {
    return y < another.y || (y == another.y && x < another.x); // epsilon?..
}

Vector2 Vector2::operator-() const {
    Vector2 result(*this);
    result.invert();
    return result;
}

Vector2 Vector2::operator+() const {
    Vector2 result(*this);
    return result;
}

Vector2& Vector2::invert() & {
    x = -x;
    y = -y;
    return *this;
}

double Vector2::magnitude() const {
    return sqrt(x*x + y*y);
}

Vector2& Vector2::normalize() & {
    operator/=(magnitude());
    return *this;
}

double Vector2::distance_to(const Vector2& another) const {
    return (another - *this).magnitude();
}

double Vector2::operator^(const Vector2& another) const {
    return dot_product(another) / (magnitude() * another.magnitude());
}

std::ostream& operator<<(std::ostream& out, const Vector2& v) {
    out << v.x << ' ' << v.y << '\n';
    return out;
}

std::istream& operator>>(std::istream& in, Vector2& v) {
    in >> v.x >> v.y;
    return in;
}
double Vector2::get_x() const {
    return x;
}

double Vector2::get_y() const {
    return y;
}

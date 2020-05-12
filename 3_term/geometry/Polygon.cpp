#include "Polygon.hpp"

Polygon::Polygon(std::vector<Vector2>&& vertices): vertices(std::move(vertices)) {
    size = this->vertices.size();
    calculate_leftmost_point();
}

Polygon::Polygon(const std::vector<Vector2>& vertices): vertices(vertices) {
    size = this->vertices.size();
    calculate_leftmost_point();
}

Polygon::Polygon(): vertices(0), leftmost(-1) {}

Polygon& Polygon::operator-=(const Vector2& point) & {
    for (Vector2& vertex : vertices) {
        vertex -= point;
    }
    return *this;
}

void Polygon::calculate_leftmost_point() {
    leftmost = 0;
    for (int i = 1; i < size; ++i) {
        if (vertices[i] < vertices[leftmost]) {
            leftmost = i;
        }
    }
}

Polygon Polygon::operator-(const Vector2& point) const {
    Polygon result = *this;
    result -= point;
    return result;
}

bool Polygon::is_point_inside(const Vector2& point) const {
    return (*this - point).is_zero_inside();
}

bool Polygon::is_zero_inside() const {
    for (int i = 0; i < size - 1; ++i) {
        if (vertices[i].cross_product(vertices[i+1]) < 0) {
            return false;
        }
    }
    if(vertices[size-1].cross_product(vertices[0]) < 0) {
        return false;
    }
    return true;
}

Polygon& Polygon::invert() & {
    for (Vector2& vertex : vertices) {
        vertex.invert();
    }
    calculate_leftmost_point();
    return *this;
}

Polygon Polygon::operator-() const {
    Polygon result = *this;
    result.invert();
    return result;
}

int Polygon::get_size() const {
    return size;
}

const Vector2& Polygon::operator[](int i) const {
    return vertices[(i+leftmost)%size];
}

std::ostream& operator<<(std::ostream& out, const Polygon& polygon) {
    out << "leftmost=" << polygon.leftmost << '\n';
    out << "size=" << polygon.size << '\n';
    for (const Vector2& vertex : polygon.vertices) {
        out << vertex;
    }
    return out;
}

Polygon Polygon::operator-(const Polygon& another) const {
    return *this + -another;
}

Polygon Polygon::operator+(const Polygon& another) const {
    std::vector<Vector2> result;
    int i = 0;
    int j = 0;
    while (i < get_size() || j < another.get_size()) {
        result.push_back((*this)[i] + another[j]);
        Vector2 this_next_edge = (*this)[i+1] - (*this)[i];
        Vector2 another_next_edge = another[j+1] - another[j];
        double cross = (another_next_edge).cross_product(this_next_edge);
        if (cross < 0) {
            ++i;
        } else if (cross > 0) {
            ++j;
        } else {
            ++i;
            ++j;
        }
    }
    return Polygon(std::move(result));
}


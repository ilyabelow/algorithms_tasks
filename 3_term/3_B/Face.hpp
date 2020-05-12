#ifndef FACE_HPP
#define FACE_HPP

#include <array>
#include <ostream>

struct Face {
    std::array<int, 3> vertices;

    Face();
    Face(int v1, int v2, int v3);
    void invert();
    friend std::ostream& operator<<(std::ostream& out, const Face& f);
};

struct LessFace {
    bool operator()(const Face& one, const Face& another) {
        if (one.vertices[0] < another.vertices[0]) {
            return true;
        }
        if (one.vertices[0] > another.vertices[0]) {
            return false;
        }
        if (one.vertices[1] < another.vertices[1]) {
            return true;
        }
        if (one.vertices[1] > another.vertices[1]) {
            return false;
        }
        if (one.vertices[2] < another.vertices[2]) {
            return true;
        }
        if (one.vertices[2] > another.vertices[2]) {
            return false;
        }
        return false;
    }
};


#endif

#include "Face.hpp"


Face::Face() {
    vertices[0] = -1;
    vertices[1] = -1;
    vertices[2] = -1;
}

Face::Face(int v1, int v2, int v3) {
    int min = std::min(std::min(v1, v2), v3);
    if (min == v1) {
        vertices[0] = v1;
        vertices[1] = v2;
        vertices[2] = v3;
    }
    if (min == v2) {
        vertices[0] = v2;
        vertices[1] = v3;
        vertices[2] = v1;
    }
    if (min == v3) {
        vertices[0] = v3;
        vertices[1] = v1;
        vertices[2] = v2;
    }
}


void Face::invert() {
    std::swap(vertices[1], vertices[2]);
}

std::ostream& operator<<(std::ostream& out, const Face& f) {
    out << 3 << ' ' << f.vertices[0] << ' ' << f.vertices[1] << ' ' << f.vertices[2] << '\n';
    return out;
}

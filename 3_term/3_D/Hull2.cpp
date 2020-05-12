#include "Hull2.hpp"


std::istream& operator>>(std::istream& in, Hull2& v) {
    Vector2 p;
    while (in >> p) {
        v.points.push_back(p);
    }
    return in;
}

//std::ostream& operator<<(std::ostream& out, const Hull2& v) {
//    for (const Vector2& p : v.points) {
//        out << p;
//    }
//    return out;
//}

std::ostream& operator<<(std::ostream& out, const Hull2& v) {
    for (const Face& f : v.faces) {
        out << f;
    }
    return out;
}

Hull3 Hull2::parabaloid_projection() {
    Hull3 answer(*this);
    for (Vector3& v : answer.points) {
        v.project_on_paraboloid();
    }
    return answer;
}

void Hull2::delaunay_triangulation() {
    Hull3 projection = parabaloid_projection();
    hull_2d = projection.build_whole_hull();
    faces = std::move(projection.faces);
}

double Hull2::calculate_avg_edges() {
    delaunay_triangulation();
    std::unordered_map<int, int> count;
    for (const Face& f: faces) {
        for (int i = 0; i < 3; ++i) {
            int vert = f.vertices[i];
            if (hull_2d.count(vert) == 0) {
                if (count.count(vert) == 0) {
                    count[vert] = 0;
                }
                ++count[vert];
            }
        }
    }
    int sites = 0;
    double sum = .0;
    for (auto it = count.begin(); it != count.end(); ++it) {
        ++sites;
        sum += it->second;
    }
    return sum / sites;
}

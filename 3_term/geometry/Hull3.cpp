#include "Hull3.hpp"
#include <algorithm>

static double INF = 1e228;
static Point NaP_object(INF, INF, INF, nullptr, nullptr);
static Point* NaP = &NaP_object; // Not a Point

Point::Point(const Vector3& vector): x(vector.get_x()),
                                     y(vector.get_y()),
                                     z(vector.get_z()),
                                     prev(nullptr),
                                     next(nullptr) {}

Point::Point(double x, double y, double z, Point* prev, Point* next): x(x),
                                                               y(y),
                                                               z(z),
                                                               prev(prev),
                                                               next(next) {}


bool Point::act() {
    if (prev->next != this) {
        // Insert
        next->prev = this;
        prev->next = this;
        return true;
    }
    // Delete
    prev->next = next;
    next->prev = prev;
    return false;
}

std::istream& operator>>(std::istream& in, Hull3& v) {
    int n;
    in >> n;
    v.points = std::vector<Vector3>(n);
    for (int j = 0; j < n; ++j) {
        in >> v.points[j];
    }
    return in;
}

Hull3::Hull3(const Hull2& hull): points(hull.points.size()) {
    int n = points.size();
    for (int i = 0; i < n; ++i) {
        points[i] = Vector3(hull.points[i]);
    }
}

//std::ostream& operator<<(std::ostream& out, const Hull3& v) {
//    out << v.faces.size() << '\n';
//    for (const Face& face : v.faces) {
//        out << face;
//    }
//    out << '\n';
//    return out;
//}

std::ostream& operator<<(std::ostream& out, const Hull3& v) {
    for (const Vector3& p: v.points) {
        out << p;
    }
    return out;
}

void Hull3::rotate(const Vector3& n) {
    for (Vector3& point : points) {
        double prev_len = point.magnitude();
        if (prev_len == 0) {
            continue;
        }
        point += n.cross_product(point - n.project(point));
        double new_len = point.magnitude();
        point *= prev_len / new_len;
    }
}

// Calculate time when turn of three projecred points changes = their cross product is zero
inline double Hull3::time_of_turn(Point* a, Point* b, Point* c) {
    if (a == NaP || b == NaP || c == NaP) {
        return INF;
    }
    // One can easily show that it is a ratio of two vector products
    return ((b->x-a->x)*(c->z-b->z) - (b->z-a->z)*(c->x-b->x))
         / ((b->x-a->x)*(c->y-b->y) - (b->y-a->y)*(c->x-b->x));
}

// Calcutale sign of turn of thee projected points when time is equal to -Inf
inline double Hull3::turn_on_inf(Point* a, Point* b, Point* c) {
    if (a == NaP || b == NaP || c == NaP) {
        return 1;
    }
    // One can easily show that it is a sing cross product projected on OY
    return (b->x-a->x)*(c->y-b->y) - (b->y-a->y)*(c->x-b->x);
}

// Sort points by X coordinate
Point* Hull3::merge_sort(Point list[], int n) {
    if (n == 1) {
        list[0].next = NaP;
        return list;
    }
    Point buffer;
    Point* left = merge_sort(list, n/2);
    Point* right = merge_sort(list + n/2, n - n/2);
    Point* head = &buffer;

    do {
        if (left->x < right->x) {
            head->next = left;
            head = head->next;
            left = left->next;
        } else {
            head->next = right;
            head = head->next;
            right = right->next;
        }
    } while (head != NaP);
    return buffer.next;
}

// Build lower hull
void Hull3::hull(Point* list, int n, Point** out, Point** buffer) {
    // Bottom of the recursive descend is hull consisting of 1 point
    if (n == 1) {
        list->next = NaP;
        list->prev = NaP;
        out[0] = NaP;
        return;
    }
    // Devide list into 2 and fild hull for halves

    // Find midpoint
    Point* mid = list;
    for (int i = 0; i < n/2-1; ++i) {
        mid = mid->next;
    }
    Point* u = mid;
    mid = mid->next;
    Point* v = mid;
    // Movies for left hull and right hull
    Point** left = buffer;
    Point** right = buffer+n/2*2;
    hull(list, n/2, left, out); // Use out as buffer because why not
    hull(mid, n-n/2, right, out+n/2*2);

    // Find initial bridge
    while (true) {
        if (turn_on_inf(u, v, v->next) < 0) {
            v = v->next;
        } else if (turn_on_inf(u->prev, u, v) < 0) {
            u = u->prev;
        } else {
            break;
        }
    }

    // Merge two movies
    int i = 0; // in left hull
    int j = 0; // in right hull
    int k = 0; // in resilting hull
    double prev_t = -INF;
    double t[6];
    while (true) {
        // Determine which event is next
        t[0] = time_of_turn(left[i]->prev, left[i], left[i]->next);
        t[1] = time_of_turn(right[j]->prev, right[j], right[j]->next);
        t[2] = time_of_turn(u, u->next, v);
        t[3] = time_of_turn(u->prev, u, v);
        t[4] = time_of_turn(u, v->prev, v);
        t[5] = time_of_turn(u, v, v->next);
        double next_t = INF;
        int next_event = -1;
        for(int event = 0; event < 6; ++event) {
            if (t[event] > prev_t && t[event] < next_t) {
                next_event = event;
                next_t = t[event];
            }
        }
        if (next_t == INF) {
            break;
        }
        prev_t = next_t;
        // Execute next event

        // Events that do not change bridge
        if (next_event == 0) {
            if (left[i]->x < u->x) {
                out[k] = left[i];
                ++k;
            }
            left[i]->act();
            ++i;
            continue;
        }
        if (next_event == 1) {
            if (right[j]->x > v->x) {
                out[k] = right[j];
                ++k;
            }
            right[j]->act();
            ++j;
            continue;
        }

        // Events that change bridge
        if (next_event == 2) {
            out[k] = u->next;
            u = u->next;
            ++k;
            continue;
        }
        if (next_event == 3) {
            out[k] = u;
            u = u->prev;
            ++k;
            continue;
        }
        if (next_event == 4) {
            out[k] = v->prev;
            v = v->prev;
            ++k;
            continue;
        }
        if (next_event == 5) {
            out[k] = v;
            v = v->next;
            ++k;
            continue;
        }
    }
    out[k] = NaP;

    // Delete points between bridge
    u->next = v;
    v->prev = u;
    // Restore hull to -Inf
    --k;
    while (k >= 0) {
        Point* event = out[k];
        // event does not affect the bridge
        if (event->x <= u->x || event->x >= v->x) {
            event->act();
            // Deletion of bridge ends
            if (event == u) {
                u = u -> prev;
            } else if (event == v) {
                v = v -> next;
            }
        } else {
            // Effect (addition) is between the bridge
            u->next = event;
            v->prev = event;
            event->prev = u;
            event->next = v;
            if (event->x < mid->x) {
                u = event;
            } else {
                v = event;
            }
        }
        --k;
    }
}

std::unordered_set<int> Hull3::build_whole_hull() {
    std::unordered_set<int> hull_2d;
    faces = std::vector<Face>(0);

    // Convert points to ordered points
    int n = points.size();
    Point *list = new Point[n];
    for (int i = 0; i < n; ++i) {
        list[i] = Point(points[i]);
    }

    Point** out = new Point *[2*n];
    Point** buffer = new Point *[2*n];

    //Lower hull
    Point* sorted = merge_sort(list, n);
    hull(sorted, n, out, buffer);
    // t = -inf => we have lower 2d hull
    Point* ext = sorted;
    for (; ext != NaP; ext=ext->next) {
        hull_2d.emplace(ext-list);
    }
    for (int i = 0; out[i] != NaP; ++i) {
        faces.emplace_back(out[i]->prev-list, out[i]-list, out[i]->next-list);
        bool was_inserted = out[i]->act();
        // Orientation of normal vector depends on if the event was an insertion event
        // or deletion event. If it was deletion event, n is flipped
        if (!was_inserted) {
            faces[faces.size()-1].invert();
        }
    }

    // t = -inf => we have upper 2d hull
    ext = sorted;
    for (; ext != NaP; ext=ext->next) {
        hull_2d.emplace(ext-list);
    }
    // Upper hull
    for (int i = 0; i < n; ++i) {
        list[i].z *= -1; // Invert the points so upper hull is a lower hull
    }
    sorted = merge_sort(list, n);
    hull(sorted, n, out, buffer);
    ext = sorted;
    for (; ext != NaP; ext=ext->next) {
        hull_2d.emplace(ext-list);
    }
    for (int i = 0; out[i] != NaP; ++i) {
        faces.emplace_back(out[i]->prev-list, out[i]-list, out[i]->next-list);
        bool was_inserted = out[i]->act();
        // Here the rule is opposite
        if (was_inserted) {
            faces[faces.size()-1].invert();
        }
    }
    ext = sorted;
    for (; ext != NaP; ext=ext->next) {
        hull_2d.emplace(ext-list);
    }
    // Clean up
    delete[] out;
    delete[] buffer;
    delete[] list;

    return hull_2d;
}


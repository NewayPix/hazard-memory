#include <cassert>
#include <string>

#include "math/Vector2.hpp"

int main(void) {
    Vector2 u(1, 0);
    Vector2 v(0, 1);
    // test linear algebra methods
    assert(u.dot(v) == 0);
    assert(u.norm() == 1);
    assert(u.cos(v) == 0);
    assert(u.distance(v) > 0);
    // test operators
    assert((u + v) == Vector2(1, 1));
    assert((u - v) == Vector2(1, -1));
    assert((u + 1) == Vector2(2, 1));
    assert((u - 1) == Vector2(0, -1));
    assert((u * 2) == Vector2(2, 0));
    assert((u / 2) == Vector2(0.5, 0));
    // test mutating operators
    u += v;
    assert(u.get_x() == 1 && u.get_y() == 1);
    u -= 1;
    assert(u.get_x() == 0 && u.get_y() == 0);
    u -= v;
    assert(u.get_x() == 0 && u.get_y() == -1);
    u *= 2;
    assert(u.get_x() == 0 && u.get_y() == -2);
    u /= 2;
    assert(u.get_x() == 0 && u.get_y() == -1);
    // test extra methods
    assert(Vector2(u) == u);
    assert(std::string(u) == "<0, -1>");
    return 0;
}

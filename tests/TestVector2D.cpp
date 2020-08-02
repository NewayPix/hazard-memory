#include <Vector2D.hpp>
#include <cassert>

int main(void) {
    Vector2D u(1, 0);
    Vector2D v(0, 1);
    // test linear algebra methods
    assert(u.dot(v) == 0);
    assert(u.norm() == 1);
    assert(u.cos(v) == 0);
    assert(u.distance(v) > 0);
    // test operators
    assert((u + v) == Vector2D(1, 1));
    assert((u - v) == Vector2D(1, -1));
    assert((u + 1) == Vector2D(2, 1));
    assert((u - 1) == Vector2D(0, -1));
    assert((u * 2) == Vector2D(2, 0));
    assert((u / 2) == Vector2D(0.5, 0));
    // test mutating operators
    assert((u += v) == Vector2D(1, 1));
    assert((u -= 1) == Vector2D(0, 0));
    assert((u -= v) == Vector2D(0, -1));
    assert((u *= 2) == Vector2D(0, -2));
    assert((u /= 2) == Vector2D(0, -1));
    return 0;
}

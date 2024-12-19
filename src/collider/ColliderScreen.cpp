#include "collider/ColliderScreen.hpp"
#include "collider/Collider.hpp"
#include "math/Vector2.hpp"

ColliderScreen::ColliderScreen(int width, int height)
    : top(0, -1, width, 1),         // top
      left(-1, 0, 1, height),       // left
      right(width, 0, 1, height),   // right
      bottom(0, height, width, 1) { // bottom
    this->width = width;
    this->height = height;
    this->center = Vector2(width / 2, height / 2);
}
bool ColliderScreen::collide(Collider* c) {
    return bottom.collide(c) || left.collide(c) || right.collide(c) ||
           top.collide(c);
}
Vector2 ColliderScreen::radius() { return Vector2(width / 2, height / 2); }

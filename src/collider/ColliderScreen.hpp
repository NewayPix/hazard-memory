#ifndef COLLIDER_SCREEN_HPP
#define COLLIDER_SCREEN_HPP

#include "collider/Collider.hpp"
#include "collider/ColliderRect.hpp"
#include "math/Vector2.hpp"

/**
 * @brief detects the collision with the screen boundaries
 */
class ColliderScreen : public Collider {
  public:
    ColliderScreen(int width, int height);

    bool collide(Collider* c);
    Vector2 radius();

    int width;
    int height;
    ColliderRect top;
    ColliderRect left;
    ColliderRect right;
    ColliderRect bottom;
};

#endif // COLLIDER_SCREEN_HPP

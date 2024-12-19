#ifndef COLLIDER_CIRCLE_H
#define COLLIDER_CIRCLE_H

#include "collider/Collider.hpp"
#include "gfx/Circle.hpp"
#include "math/Vector2.hpp"

class ColliderCircle : public Collider {
  public:
    Circle circle;
    ColliderCircle(Circle const& c);
    ColliderCircle(Vector2 center, int radius);

    /**
     * @brief checks if the current collider is intersecting another one.
     */
    bool collide(Collider* c);

    Vector2 radius();
};
#endif // COLLIDER_RECT_H

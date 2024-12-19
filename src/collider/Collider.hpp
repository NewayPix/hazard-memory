#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "math/Vector2.hpp"
#include <cmath>

class Collider {
  public:
    // The center of the collider polygon, it must be calculated at the
    // constructor
    Vector2 center;

    /**
     * @brief abstract collide method
     */
    virtual bool collide(Collider* c) = 0;

    /**
     * @brief return the object radius
     */
    virtual Vector2 radius() = 0;
};

// return the nearest value of c in (min, max) range.
inline float range(float c, float min, float max) {
    if (c < min) {
        return min;
    } else if (c > max) {
        return max;
    } else {
        return c;
    }
}

#endif // COLLISION_HPP

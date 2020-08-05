#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <cmath>
#include "math/Vector2.hpp"

class Collider {
public:
    // The center of the collider polygon, it must be calculated at the
    // constructor
    Vector2 center;

    /**
     * @brief abstract collide method
     */
    virtual bool collide(const Collider &c) = 0;

    /**
     * @brief return the object radius
     */
    virtual Vector2 radius() const = 0;
};

#endif //COLLISION_HPP

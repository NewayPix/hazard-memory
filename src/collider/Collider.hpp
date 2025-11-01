#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "math/Vector2.hpp"
#include <cmath>
#include <concepts>

class Collider {
public:
    /**
     * @brief abstract collide method
     */
    virtual bool collide(Collider* c) = 0;

    /**
     * @brief return the object radius
     */
    virtual Vector2 radius() = 0;

    Vector2 get_center() const { return center; }

protected:
    // The center of the collider polygon, it must be calculated at the
    // constructor
    Vector2 center;
};

template <typename T>
concept Collidable = requires(T a, Collider* c) {
    {
        a.collide(c)
    } -> std::same_as<bool>;
    {
        a.radius()
    } -> std::same_as<Vector2>;
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

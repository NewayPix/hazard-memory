#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <cmath>

#include <SDL2/SDL.h>

#include "math/Vector2.hpp"

class Collider {
public:
    // The center of the collider polygon, it must be calculated at the
    // constructor
    Vector2 center;
    // The polygon is the surface on which
    SDL_Rect polygon;

    /**
     * @brief abstract collide method
     */
    virtual bool collide(const Collider &c) = 0;
};

#endif //COLLISION_HPP

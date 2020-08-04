#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <cmath>
#include <vector>

#include <SDL2/SDL.h>

#include "math/Vector2.hpp"

class Collider {
public:
    // The center of the collider polygon, it must be calculated at the
    // constructor
    Vector2 center;
    // The polygon is the surface on which
    SDL_Rect *polygon;

public:
    Collider(SDL_Rect *r);
    Collider(int x, int y, int w, int h);
    virtual ~Collider();

    /**
     * @brief
     */
    virtual bool collide(const Collider &c) = 0;
};

#endif //COLLISION_HPP
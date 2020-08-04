#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <cmath>

#include <SDL2/SDL.h>

#include "math/Vector2.hpp"

class Collider {
public:
    Collider(SDL_Rect *r);
    Collider(int x, int y, int w, int h);
    virtual ~Collider();

    virtual bool collide(Collider &c) = 0;

    //
    SDL_Rect *polygon;
    //
    Vector2 center;
};

#endif //COLLISION_HPP

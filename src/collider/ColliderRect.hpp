#ifndef COLLIDER_RECT_H
#define COLLIDER_RECT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>

#include "collider/Collider.hpp"
#include "math/Vector2.hpp"

class ColliderRect : public Collider {
public:
    SDL_Rect polygon;
    ColliderRect(SDL_Rect const &r);
    ColliderRect(int x, int y, int w, int h);

    /**
     * @brief checks if the current collider is intersecting another one.
     */
    bool collide(Collider *c);

    /**
      * @brief checks if the current collide is on top of another one
      */
    bool on_top(Collider *c);

    Vector2 radius();
};
#endif //COLLIDER_RECT_H

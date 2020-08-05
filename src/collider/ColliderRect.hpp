#ifndef COLLIDER_RECT_H
#define COLLIDER_RECT_H

#include <SDL2/SDL.h>

#include "collider/Collider.hpp"

class ColliderRect : public Collider {
public:
    SDL_Rect polygon;
    ColliderRect(SDL_Rect const &r);
    ColliderRect(int x, int y, int w, int h);

    /**
     * @brief checks if the current collider is intersecting another one.
     */
    bool collide(const Collider &c);

    /**
      * @brief checks if the current collide is on top of another one
      */
    bool on_top(const ColliderRect &c);

    Vector2 radius() const;
};
#endif //COLLIDER_RECT_H

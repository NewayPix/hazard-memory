#ifndef COLLIDER_RECT_H
#define COLLIDER_RECT_H

#include <SDL2/SDL.h>

#include "Collider.hpp"

class ColliderRect : public Collider {
private:

public:
    ColliderRect(SDL_Rect *r);
    ColliderRect(int x, int y, int w, int h);
    ~ColliderRect();

    /**
     * @brief checks if the current collider is intersecting another one.
     */
    bool collide(Collider &c) override {

        float x_max = (this->polygon->w + c.polygon->w) / 2;
        float y_max = (this->polygon->h + c.polygon->h) / 2;

        Vector2 diff = this->center - c.center;

        return abs(diff.x) < x_max && abs(diff.y) < y_max;

    }

    /**
      * @brief checks if the current collide is on top of another one
      */
    bool on_top(const Collider &c) {
        float rect_distance = c.polygon->y - \
                              (this->polygon->y + this->polygon->h);

        int x_min = c.polygon->x - this->polygon->w;
        int x_max = c.polygon->x + this->polygon->w;

        bool cond = (this->polygon->x >= x_min && \
                     this->polygon->x <= x_max && rect_distance == 0);

        return cond;
    }
};

#endif //COLLIDER_RECT_H
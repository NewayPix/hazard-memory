#ifndef COLLIDER_RECT_H
#define COLLIDER_RECT_H

#include <SDL2/SDL.h>

#include "Collider.hpp"

class ColliderRect : public Collider {
public:
    ColliderRect(SDL_Rect *r);
    ColliderRect(int x, int y, int w, int h);
    ~ColliderRect();

    /**
     * @brief checks if the current collider is intersecting another one.
     */
    bool collide(const Collider &c) {
        // FIXME: Probable type conversion inside (?)

        float x_max = (this->polygon->w + c.polygon->w) / 2;
        float y_max = (this->polygon->h + c.polygon->h) / 2;

        Vector2 diff = this->center - c.center;

        return abs(diff.x) < x_max && abs(diff.y) < y_max;

    }

    /**
      * @brief checks if the current collide is on top of another one
      */
    bool on_top(const ColliderRect &c);

    /**
     * @brief
     */
    std::pair<int, int> get_bounds() {
        auto p = std::make_pair(polygon->x, polygon->y);
        return p;
    }

    /**
     * @brief
     */
    void set_bounds(int x, int y) {
        polygon->x = x;
        polygon->y = y;
    }

    /**
     * @brief
     */
    std::pair<int, int> get_dimension() {
        return std::make_pair(polygon->w, polygon->h);
    }

    /**
     * @brief
     */
    void set_dimension(int w, int h) {
        polygon->w = w;
        polygon->h = h;
    }
};

#endif //COLLIDER_RECT_H
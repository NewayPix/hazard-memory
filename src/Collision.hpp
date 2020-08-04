#ifndef COLLISION_HPP
#define COLLISION_HPP
#include <cmath>
#include <SDL2/SDL.h>
#include "Vector2D.hpp"

class Collision {
private:
    static Vector2D center(SDL_Rect &r) {
        return Vector2D(r.x + r.w/2, r.y + r.h/2);
    }

public:

    /*
     * rect check if two rects collides
     */
    static bool rect(SDL_Rect &r1, SDL_Rect &r2) {
        Vector2D r1_center = center(r1);
        Vector2D r2_center = center(r2);
        float x_max = (r1.w + r2.w) / 2;
        float y_max = (r1.h + r2.h) / 2;
        Vector2D diff = r1_center - r2_center;
        return abs(diff.x) < x_max && abs(diff.y) < y_max;

    }

    /*
     * rect_on_top checks if r1 is on top of r2.
     */
    static bool rect_on_top(SDL_Rect &r1, SDL_Rect &r2) {
        float rect_distance = r2.y - (r1.y + r1.h);
        int x_min = r2.x - r1.w;
        int x_max = r2.x + r2.w;
        return (r1.x >= x_min && r1.x <= x_max && rect_distance == 0);
    }
};

#endif //COLLISION_HPP

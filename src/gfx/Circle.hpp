#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "math/Vector2.hpp"

class Circle {
    public:
    explicit Circle(Vector2 center, int radius) : center(center), radius(radius) {
    }

    Vector2 get_center() const {
        return center;
    }
    int get_radius() const {
        return radius;
    }

    private:
    Vector2 center;
    int radius;
};

#endif // CIRCLE_HPP

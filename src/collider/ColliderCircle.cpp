#include "collider/ColliderCircle.hpp"
#include "collider/ColliderRect.hpp"
#include <vector>

ColliderCircle::ColliderCircle(Circle const &c): ColliderCircle(c.center, c.radius) {}

ColliderCircle::ColliderCircle(Vector2 center, int radius) {
    this->center = center;
    this->circle.center = center;
    this->circle.radius = radius;
}

Vector2 ColliderCircle::radius() {
    return Vector2(circle.radius, circle.radius);
}
// return the nearest value of c in (min, max) range.
float range(float c, float min, float max) {
    if (c < min) {
        return min;
    } else if (c > max) {
        return max;
    } else {
        return c;
    }
}

bool ColliderCircle::collide(Collider *c) {
    // special colllision if Collider it's actually ColliderRect
    if (dynamic_cast<ColliderRect*>(c)) {
        Vector2 ct = c->center;
        Vector2 r  = c->radius();
        Vector2 max = ct + r;
        Vector2 min = ct - r;
        float nearest_x = range(this->center.x, min.x, max.x);
        float nearest_y = range(this->center.y, min.y, max.y);
        std::vector<Vector2> boundaries = {
            Vector2(nearest_x, max.y), // bottom
            Vector2(nearest_x, min.y), // top
            Vector2(max.x, nearest_y), // right
            Vector2(min.x, nearest_y), // left
        };
        for (auto &boundary: boundaries) {
            float distance = this->center.distance(boundary);
            if (distance < this->radius().max()) {
                return true;
            }
        }
        return false;
    } else {
        float distance = this->center.distance(c->center);
        float radius_sum = this->radius().max() + c->radius().max();
        return distance < radius_sum;
    }
}

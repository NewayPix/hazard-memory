#include "collider/ColliderCircle.hpp"
#include "collider/Collider.hpp"
#include "collider/ColliderRect.hpp"
#include "gfx/Circle.hpp"
#include "math/Vector2.hpp"
#include <vector>

ColliderCircle::ColliderCircle(Circle const& c) : ColliderCircle(c.center, c.radius) {
}

ColliderCircle::ColliderCircle(Vector2 center, int radius) {
    this->center        = center;
    this->circle.center = center;
    this->circle.radius = radius;
}

Vector2 ColliderCircle::radius() {
    return Vector2(circle.radius, circle.radius);
}

bool ColliderCircle::collide(Collider* c) {
    // special colllision if Collider it's actually ColliderRect
    bool cond = false;
    if (dynamic_cast<ColliderRect*>(c)) {
        Vector2 ct                      = c->center;
        Vector2 r                       = c->radius();
        Vector2 max                     = ct + r;
        Vector2 min                     = ct - r;
        float nearest_x                 = range(this->center.x, min.x, max.x);
        float nearest_y                 = range(this->center.y, min.y, max.y);
        std::vector<Vector2> boundaries = {
            Vector2(nearest_x, max.y), // bottom
            Vector2(nearest_x, min.y), // top
            Vector2(max.x, nearest_y), // right
            Vector2(min.x, nearest_y), // left
        };
        for (auto& boundary : boundaries) {
            float distance = this->center.distance(boundary);
            if (distance < this->radius().max()) {
                cond = true;
            }
        }
    } else {
        float distance   = this->center.distance(c->center);
        float radius_sum = this->radius().max() + c->radius().max();
        cond             = distance < radius_sum;
    }
    return cond;
}

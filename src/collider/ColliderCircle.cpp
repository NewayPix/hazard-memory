#include "collider/ColliderCircle.hpp"
#include "collider/Collider.hpp"
#include "collider/ColliderRect.hpp"
#include "gfx/Circle.hpp"
#include "math/Vector2.hpp"
#include <vector>

ColliderCircle::ColliderCircle(Circle c) : circle(c) {
    this->center = circle.get_center();
}

ColliderCircle::ColliderCircle(Vector2 center, int radius) : circle(Circle(center, radius)) {
    this->center = center;
}

Vector2 ColliderCircle::radius() {
    return Vector2(circle.get_radius(), circle.get_radius());
}

bool ColliderCircle::collide(Collider* c) {
    // special colllision if Collider it's actually ColliderRect
    bool cond = false;
    if (dynamic_cast<ColliderRect*>(c)) {
        Vector2 ct                      = c->get_center();
        Vector2 r                       = c->radius();
        Vector2 max                     = ct + r;
        Vector2 min                     = ct - r;
        float nearest_x                 = range(this->center.get_x(), min.get_x(), max.get_x());
        float nearest_y                 = range(this->center.get_y(), min.get_y(), max.get_y());
        std::vector<Vector2> boundaries = {
            Vector2(nearest_x, max.get_y()), // bottom
            Vector2(nearest_x, min.get_y()), // top
            Vector2(max.get_x(), nearest_y), // right
            Vector2(min.get_x(), nearest_y), // left
        };
        for (auto& boundary : boundaries) {
            float distance = this->center.distance(boundary);
            if (distance < this->radius().max()) {
                cond = true;
            }
        }
    } else {
        float distance   = this->center.distance(c->get_center());
        float radius_sum = this->radius().max() + c->radius().max();
        cond             = distance < radius_sum;
    }
    return cond;
}

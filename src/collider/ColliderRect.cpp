#include "collider/ColliderRect.hpp"
#include "collider/Collider.hpp"
#include "collider/ColliderCircle.hpp"
#include "collider/ColliderScreen.hpp"
#include "math/Vector2.hpp"
#include <SDL2/SDL_rect.h>
#include <cstdlib>

ColliderRect::ColliderRect(SDL_Rect const& r) : ColliderRect(r.x, r.y, r.w, r.h) {
}

ColliderRect::ColliderRect(int x, int y, int w, int h) {
    polygon.x = x;
    polygon.y = y;
    polygon.w = w;
    polygon.h = h;

    center = Vector2(x + static_cast<float>(w) / 2, y + static_cast<float>(h) / 2);
}

bool ColliderRect::on_top(Collider* c) {
    // special logic for ColliderCircle
    bool cond = false;
    if (dynamic_cast<ColliderCircle*>(c)) {
        Vector2 ct       = this->center;
        Vector2 r        = this->radius();
        Vector2 min      = ct - r;
        Vector2 max      = ct + r;
        float nearest_x  = range(c->get_center().get_x(), min.get_x(), max.get_x());
        Vector2 boundary = Vector2(nearest_x, max.get_y()); // bottom;
        float distance   = c->get_center().distance(boundary);
        cond             = (distance - c->radius().max()) < 1;
    }
    if (auto screen = dynamic_cast<ColliderScreen*>(c)) {
        cond = this->on_top(&screen->bottom);
    } else { // default on_top behavior
        Vector2 r1     = this->radius();
        Vector2 v1     = this->center - r1;
        Vector2 r2     = c->radius();
        Vector2 v2     = c->get_center() - r2;
        float distance = v2.get_y() - (v1.get_y() + 2 * r1.get_y());

        int x_min = v2.get_x() - 2 * r1.get_x();
        int x_max = v2.get_x() + 2 * r2.get_x();

        cond = (v1.get_x() >= x_min && v1.get_x() <= x_max && distance == 0);
    }

    return cond;
}

/*
 * Equation: abs(c1 - c2) < (r1 + r2)
 */
bool ColliderRect::collide(Collider* c) {
    Vector2 radius = this->radius() + c->radius();
    Vector2 diff   = this->center - c->get_center();

    return abs(diff.get_x()) < radius.get_x() && abs(diff.get_y()) < radius.get_y();
}

Vector2 ColliderRect::radius() {
    return Vector2(static_cast<float>(polygon.w) / 2, static_cast<float>(polygon.h) / 2);
}

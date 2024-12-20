#include "collider/ColliderRect.hpp"
#include "collider/Collider.hpp"
#include "collider/ColliderCircle.hpp"
#include "collider/ColliderScreen.hpp"
#include "math/Vector2.hpp"
#include <SDL2/SDL_rect.h>
#include <cstdlib>

ColliderRect::ColliderRect(SDL_Rect const& r)
: ColliderRect(r.x, r.y, r.w, r.h) {
}

ColliderRect::ColliderRect(int x, int y, int w, int h) {
    polygon.x = x;
    polygon.y = y;
    polygon.w = w;
    polygon.h = h;

    center = Vector2(x + w / 2, y + h / 2);
}

bool ColliderRect::on_top(Collider* c) {
    // special logic for ColliderCircle
    bool cond = false;
    if (dynamic_cast<ColliderCircle*>(c)) {
        Vector2 ct       = this->center;
        Vector2 r        = this->radius();
        Vector2 min      = ct - r;
        Vector2 max      = ct + r;
        float nearest_x  = range(c->center.x, min.x, max.x);
        Vector2 boundary = Vector2(nearest_x, max.y); // bottom;
        float distance   = c->center.distance(boundary);
        cond             = (distance - c->radius().max()) < 1;
    }
    if (auto screen = dynamic_cast<ColliderScreen*>(c)) {
        cond = this->on_top(&screen->bottom);
    } else { // default on_top behavior
        Vector2 r1     = this->radius();
        Vector2 v1     = this->center - r1;
        Vector2 r2     = c->radius();
        Vector2 v2     = c->center - r2;
        float distance = v2.y - (v1.y + 2 * r1.y);

        int x_min = v2.x - 2 * r1.x;
        int x_max = v2.x + 2 * r2.x;

        cond = (v1.x >= x_min && v1.x <= x_max && distance == 0);
    }

    return cond;
}

/*
 * Equation: abs(c1 - c2) < (r1 + r2)
 */
bool ColliderRect::collide(Collider* c) {
    Vector2 radius = this->radius() + c->radius();
    Vector2 diff   = this->center - c->center;

    return abs(diff.x) < radius.x && abs(diff.y) < radius.y;
}

Vector2 ColliderRect::radius() {
    return Vector2(polygon.w / 2, polygon.h / 2);
}

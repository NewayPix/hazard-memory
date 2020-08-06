#include "collider/ColliderRect.hpp"
ColliderRect::ColliderRect(SDL_Rect const &r): ColliderRect(r.x, r.y, r.w, r.h) {}

ColliderRect::ColliderRect(int x, int y, int w, int h) {
    polygon.x = x;
    polygon.y = y;
    polygon.w = w;
    polygon.h = h;

    center = Vector2(x + w/2, y + h/2);
}


bool ColliderRect::on_top(Collider *c) {
    Vector2 r1 = this->radius();
    Vector2 v1 = this->center - r1;
    Vector2 r2 = c->radius();
    Vector2 v2 = c->center - r2;
    float distance = v2.y - (v1.y + 2 * r1.y);

    int x_min = v2.x - 2 * r1.x;
    int x_max = v2.x +  2 * r2.x;

    bool cond = (v1.x >= x_min && \
                 v1.x <= x_max && \
                 distance == 0);

    return cond;
}

/*
 * Equation: abs(c1 - c2) < (r1 + r2)
 */
bool ColliderRect::collide(Collider *c) {
    Vector2 radius = this->radius() + c->radius();
    Vector2 diff = this->center - c->center;

    return abs(diff.x) < radius.x && abs(diff.y) < radius.y;
}


Vector2 ColliderRect::radius() {
    return Vector2(polygon.w / 2, polygon.h / 2);
}

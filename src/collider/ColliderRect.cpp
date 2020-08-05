#include "collider/ColliderRect.hpp"
ColliderRect::ColliderRect(SDL_Rect const &r): ColliderRect(r.x, r.y, r.w, r.h) {}

ColliderRect::ColliderRect(int x, int y, int w, int h) {
    polygon.x = x;
    polygon.y = y;
    polygon.w = w;
    polygon.h = h;

    center = Vector2(x + w/2, y + h/2);
}


bool ColliderRect::on_top(const ColliderRect &c) {
    float rect_distance = c.polygon.y - (this->polygon.y + this->polygon.h);

    int x_min = c.polygon.x - this->polygon.w;
    int x_max = c.polygon.x + c.polygon.w;

    bool cond = (this->polygon.x >= x_min && \
                 this->polygon.x <= x_max && \
                 rect_distance == 0);

    return cond;
}

bool ColliderRect::collide(const Collider &c) {
    Vector2 radius = this->radius() + c.radius();
    Vector2 diff = this->center - c.center;

    return abs(diff.x) < radius.x && abs(diff.y) < radius.y;
}


Vector2 ColliderRect::radius() const {
    return Vector2(polygon.w / 2, polygon.h / 2);
}

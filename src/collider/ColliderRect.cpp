#include "collider/ColliderRect.hpp"
ColliderRect::ColliderRect(SDL_Rect const &r): ColliderRect(r.x, r.y, r.w, r.h) {}

ColliderRect::ColliderRect(int x, int y, int w, int h) {
    polygon.x = x;
    polygon.y = y;
    polygon.w = w;
    polygon.h = h;

    center = Vector2(x + w/2, round(y + h/2));
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

    float x_max = (this->polygon.w + c.polygon.w) / 2;
    float y_max = (this->polygon.h + c.polygon.h) / 2;

    Vector2 diff = this->center - c.center;

    return abs(diff.x) < x_max && abs(diff.y) < y_max;
}

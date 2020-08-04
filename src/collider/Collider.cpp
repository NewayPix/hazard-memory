#include "Collider.hpp"

Collider::Collider(SDL_Rect *r) {
    polygon = r;
    center = Vector2(r->x + r->w/2, r->y + r->h/2);
}

Collider::Collider(int x, int y, int w, int h) {
    polygon->w = w;
    polygon->h = h;
    polygon->x = x;
    polygon->y = y;

    center = Vector2(x + w/2, y + h/2);
}

Collider::~Collider() {
    delete polygon;
}
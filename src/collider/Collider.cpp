#include "Collider.hpp"

Collider::Collider(SDL_Rect *r): Collider(r->x, r->y, r->w, r->h) {}

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
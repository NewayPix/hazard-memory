#include "ColliderRect.hpp"

ColliderRect::ColliderRect(SDL_Rect *r): Collider(r) {

}

ColliderRect::ColliderRect(int x, int y, int w, int h): Collider(x, y, w, h) {

}

ColliderRect::~ColliderRect() {}


bool ColliderRect::on_top(const ColliderRect &c) {
    float rect_distance = c.polygon->y - \
                            (this->polygon->y + this->polygon->h);

    int x_min = c.polygon->x - this->polygon->w;
    int x_max = c.polygon->x + this->polygon->w;

    bool cond = (this->polygon->x >= x_min && \
                    this->polygon->x <= x_max && rect_distance == 0);

    return cond;
}


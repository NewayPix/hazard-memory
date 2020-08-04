#include "ColliderRect.hpp"

bool ColliderRect::on_top(const ColliderRect &c) {
    float rect_distance = c.polygon->y - \
                            (this->polygon->y + this->polygon->h);

    int x_min = c.polygon->x - this->polygon->w;
    int x_max = c.polygon->x + this->polygon->w;

    bool cond = (this->polygon->x >= x_min && \
                    this->polygon->x <= x_max && rect_distance == 0);

    return cond;
}


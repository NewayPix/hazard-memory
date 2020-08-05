#include "collider/ColliderCircle.hpp"

ColliderCircle::ColliderCircle(Circle const &c): ColliderCircle(c.center, c.radius) {}

ColliderCircle::ColliderCircle(Vector2 center, int radius) {
    this->center = center;
    this->circle.center = center;
    this->circle.radius = radius;
}


Vector2 ColliderCircle::radius() const {
    return Vector2(circle.radius, circle.radius);
}

bool ColliderCircle::collide(const Collider &c) {
    float distance = this->center.distance(c.center);
    float radius_sum = (this->radius() + c.radius()).norm();
    return distance < radius_sum;
}

#include "ColliderRect.hpp"

ColliderRect::ColliderRect(SDL_Rect *r): Collider(r) {}
ColliderRect::ColliderRect(int x, int y, int w, int h): Collider(x, y, w, h) {}

ColliderRect::~ColliderRect() {}
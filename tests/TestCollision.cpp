#include <cassert>
#include <string>

#include <SDL2/SDL_rect.h>

#include "collider/ColliderRect.hpp"

int main(void) {
    /*

      Brief visual explanation of testing:

              SCREEN
      .(0, 0) -----------------------+
      |+-------+                     |
      ||  r1   |                     |
      ||       |                     |
      |+-------+                     |
      ||  r2   |                     |
      ||       |----+                |
      |+-------+    |                |
      |      |  r3  |                |
      |      |      |                |
      |      +------+                |
      +------------------------------+


      Facts:
      - r1 is on top of r2
      - r1 is not on top of r3
      - r2 is not on top of r1
      - r2 is not on top of r3
      - r3 is not on top of r1
      - r3 is not on top of r2
      - r2 do collides with r3
      - r1 do not collides with r2
      - r1 do not collides with r3
    */


    SDL_Rect r1 = {0, 0, 20, 20};
    SDL_Rect r2 = {0, 20, 20, 20};
    SDL_Rect r3 = {10, 30, 20, 20};

    ColliderRect c1(r1);
    ColliderRect c2(r2);
    ColliderRect c3(r3);

    // Test on_top method from ColliderRect
    assert(c1.on_top(&c2));
    assert(!c1.on_top(&c3));
    assert(!c2.on_top(&c1));
    assert(!c2.on_top(&c3));
    assert(!c3.on_top(&c1));
    assert(!c3.on_top(&c2));

    // Test collision between two rects
    assert(c2.collide(&c3));
    assert(!c1.collide(&c2));
    assert(!c1.collide(&c3));

    return 0;
}

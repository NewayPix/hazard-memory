#include <cassert>
#include <string>
#include <SDL2/SDL_rect.h>
#include "Collision.hpp"

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
    // test Collision::rect_on_top method
    assert(Collision::rect_on_top(r1, r2));
    assert(!Collision::rect_on_top(r1, r3));
    assert(!Collision::rect_on_top(r2, r1));
    assert(!Collision::rect_on_top(r2, r3));
    assert(!Collision::rect_on_top(r3, r1));
    assert(!Collision::rect_on_top(r3, r2));

    // test Collision::rect method
    assert(Collision::rect(r2, r3));
    assert(!Collision::rect(r1, r2));
    assert(!Collision::rect(r1, r3));
    return 0;
}

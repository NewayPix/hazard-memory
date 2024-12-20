#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <cmath>
#include <cstdlib>
#include <map>
#include <vector>

#define TITLE "BreakOut"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define BLOCK_SIZE 20
#define BALL_RADIUS 10
#define BLOCKS 5

#include "Game.hpp"
#include "InputHandler.hpp"
#include "collider/ColliderCircle.hpp"
#include "collider/ColliderRect.hpp"
#include "collider/ColliderScreen.hpp"
#include "gfx/Circle.hpp"
#include "math/Vector2.hpp"

// keys to InputHandler observe
std::map<const char*, SDL_Keycode> input_config = {
    { "left", SDLK_LEFT },
    { "right", SDLK_RIGHT },
    { "quit", SDLK_ESCAPE },
    { "double_size", SDLK_SPACE },
    { "run", SDLK_LSHIFT },
};

struct Keyboard {
    bool left;
    bool right;
    bool run;
    bool double_size;

    bool valid () {
        return !(left && right);
    }
};

struct Player {
    Vector2 position = { SCREEN_WIDTH / 2, SCREEN_HEIGHT - BLOCK_SIZE };
    Vector2 velocity = { 500, 0 };
    Vector2 direction;
    bool running;
    bool double_size;
    ColliderRect collider = { (int)(position.x), (int)position.y, BLOCK_SIZE * 3, BLOCK_SIZE };

    void update (float dt) {
        position.x += (this->running + 1) * velocity.x * direction.x * dt;
    }

    void read_keyboard (Keyboard& k) {
        this->direction.x = k.left ? -1 : k.right ? 1 : 0;
        this->double_size = k.double_size;
        this->running     = k.run;
    }

    void reset_position () {
        position.x = collider.polygon.x;
    }
    ColliderRect current_collider () {
        return ColliderRect (round (position.x), round (position.y),
        BLOCK_SIZE * 3 * (this->double_size + 1), BLOCK_SIZE);
    }

    void render (SDL_Renderer* renderer) {
        collider = current_collider ();
        SDL_SetRenderDrawColor (renderer, 0, 255, 255, 255);
        SDL_RenderFillRect (renderer, &collider.polygon);
        SDL_SetRenderDrawColor (renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect (renderer, &collider.polygon);
    };
};

struct Ball {
    Vector2 position;
    Vector2 velocity;
    ColliderCircle collider;

    Ball (Vector2 v) : collider (v, BALL_RADIUS) {
        this->position = v;
    }

    void update (float dt) {
        position.x += dt * velocity.x;
        position.y += dt * velocity.y;
    }

    ColliderCircle current_collider () {
        return ColliderCircle (position, BALL_RADIUS);
    }

    void reset_position () {
        position.x = collider.center.x;
        position.y = collider.center.y;
    }

    void render (SDL_Renderer* renderer) {
        collider = current_collider ();
        filledCircleRGBA (renderer, collider.center.x, collider.center.y,
        collider.circle.radius, 255, 30, 30, 255);
        circleRGBA (renderer, collider.center.x, collider.center.y,
        collider.circle.radius, 0, 0, 0, 255);
    };
};

struct Block {
    ColliderRect collider = ColliderRect (0, 0, 2 * BLOCK_SIZE, BLOCK_SIZE);
    bool alive            = true;
    SDL_Color color       = { 0, 0, 0, 255 };

    Block (Vector2 v) {
        this->collider = ColliderRect (v.x, v.y, 2 * BLOCK_SIZE, BLOCK_SIZE);
    }

    void set_color (int i) {
        int level = (0x0f | (1 << i));
        color.r   = 0.2 * level;
        color.g   = 0.4 * level;
        color.b   = 0.9 * level;
    }

    void render (SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor (renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect (renderer, &collider.polygon);
        SDL_SetRenderDrawColor (renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect (renderer, &collider.polygon);
    };
};

ColliderScreen screen_collider = ColliderScreen (SCREEN_WIDTH, SCREEN_HEIGHT);
InputHandler input_handler     = InputHandler (input_config);
Keyboard keyboard              = { .left = false, .right = false };
Ball ball     = Ball (Vector2 (SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
Player player = Player ();
std::vector<Block> blocks;

void start () {
    ball.velocity = { 150, 300 };
    for (int i = 0; i < BLOCKS; ++i) {
        for (int j = 0; j < SCREEN_WIDTH / BLOCK_SIZE / 2; j++) {
            float x = 2 * j * BLOCK_SIZE;
            float y = i * BLOCK_SIZE;
            Block b (Vector2 (x, y));
            b.set_color (i + 3);
            blocks.push_back (b);
        }
    }
}

void Game::event () {
    static SDL_Event e;
    input_handler.process (e);
    keyboard.left        = input_handler.read ("left");
    keyboard.right       = input_handler.read ("right");
    keyboard.run         = input_handler.read ("run");
    keyboard.double_size = input_handler.read ("double_size");
    running = !(input_handler.read ("quit") || input_handler.read (SDL_QUIT));
}

void Game::update (float dt) {
    // update player
    player.read_keyboard (keyboard);
    player.update (dt);
    ball.update (dt);

    auto player_collider = player.current_collider ();
    if (screen_collider.collide (&player_collider)) {
        player.reset_position ();
    }

    auto ball_collider = ball.current_collider ();
    if (screen_collider.collide (&ball_collider)) {
        auto& s = screen_collider;
        ball.reset_position ();
        bool x_collide = s.left.collide (&ball_collider) || s.right.collide (&ball_collider);
        bool y_collide = s.top.collide (&ball_collider) || s.bottom.collide (&ball_collider);
        if (y_collide) {
            ball.velocity.y *= -1;
        }
        if (x_collide) {
            ball.velocity.x *= -1;
        }
    } else if (ball_collider.collide (&player_collider)) {
        // FIXME: set ball position to a safer position that don't collides
        // anymore with player
        ball.reset_position ();
        ball.velocity.y *= -1;
        if (player.direction.x != 0) {
            ball.velocity.x = abs (ball.velocity.x) * player.direction.x;
        }
    }

    for (auto& b : blocks) {
        if (b.alive && ball_collider.collide (&b.collider)) {
            b.alive = false;
            ball.reset_position ();
            ball.velocity.y *= -1;
            // ball.velocity *= 1.1;
            break;
        }
    }
}

void Game::draw () {
    SDL_SetRenderDrawColor (renderer, 255, 255, 255, 255);
    SDL_RenderClear (renderer);

    player.render (renderer);
    ball.render (renderer);

    for (auto& b : blocks) {
        if (b.alive) {
            b.render (renderer);
        }
    }

    // render everything
    SDL_RenderPresent (renderer);
}

int main () {
    start ();
    Game game (TITLE, SCREEN_WIDTH, SCREEN_HEIGHT);
    game.set_max_frame_rate (60);
    return game.run ();
}

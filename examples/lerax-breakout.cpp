#include <map>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#define TITLE "BreakOut"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define BLOCK_SIZE 20
#define BALL_RADIUS 5
#define BLOCKS 5

#include "math/Vector2.hpp"
#include "InputHandler.hpp"
#include "collider/ColliderRect.hpp"
#include "collider/ColliderCircle.hpp"
#include "collider/ColliderScreen.hpp"
#include "gfx/Circle.hpp"
#include "GameLoop.hpp"

std::map<const char*, SDL_Keycode> input_config = {
    {"left", SDLK_LEFT},
    {"right", SDLK_RIGHT},
    {"quit", SDLK_ESCAPE},
};

struct Keyboard {
    bool left;
    bool right;

    bool valid() {
        return !(left && right);
    }
};

struct Player {
    Vector2 position = {SCREEN_WIDTH / 2, SCREEN_HEIGHT - BLOCK_SIZE};
    Vector2 velocity = {300, 0};
    Vector2 direction;
    ColliderRect collider = {(int)(position.x), (int)position.y,
                             BLOCK_SIZE * 3, BLOCK_SIZE};

    void update(float dt) {
        position.x += velocity.x * direction.x * dt;
    }

    void reset_position() {
        position.x = collider.polygon.x;
    }
    ColliderRect current_collider() {
        return ColliderRect(
               round(position.x),
               round(position.y),
               BLOCK_SIZE * 3,
               BLOCK_SIZE
            );
    }

    void render(SDL_Renderer *renderer) {
        collider.polygon.x = round(position.x); // IT'S A GOOD IDEA?
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &collider.polygon);
    };
};

struct Ball {
    Vector2 position;
    Vector2 velocity;
    ColliderCircle collider;

    Ball(Vector2 v): collider(v, BALL_RADIUS) {
        this->position = v;
    }

    void update(float dt) {
        position.x += dt * velocity.x;
        position.y += dt * velocity.y;
    }

    ColliderCircle current_collider() {
        return ColliderCircle(position, BALL_RADIUS);
    }

    void reset_position() {
        position.x = collider.center.x;
        position.y = collider.center.y;
    }

    void render(SDL_Renderer *renderer) {
        collider = current_collider();
        filledCircleRGBA(renderer,\
                         collider.center.x, \
                         collider.center.y, \
                         collider.circle.radius,
                         0, 0, 0, 255);
    };
};


struct Block {
    ColliderRect collider = ColliderRect(0, 0, BLOCK_SIZE, BLOCK_SIZE);
    bool alive = true;

    Block(Vector2 v) {
        this->collider = ColliderRect(v.x, v.y, BLOCK_SIZE, BLOCK_SIZE);
    }

    void render(SDL_Renderer *renderer) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &collider.polygon);
    };
};

class Game: public GameLoop {
    using GameLoop::GameLoop;

    ColliderScreen screen_collider = ColliderScreen(SCREEN_WIDTH, SCREEN_HEIGHT);
    InputHandler input_handler = InputHandler(input_config);
    Keyboard keyboard = {.left=false, .right=false};
    Ball ball = Ball(Vector2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
    Player player = Player();
    std::vector<Block> blocks;

    void start() {
        // initialize blocks
        ball.velocity = {150, 300};
        for (int i = 1; i <= BLOCKS; ++i) {
            for (int j = 1; j <= SCREEN_WIDTH / BLOCK_SIZE / 2  ; j++) {
                float x = 2 * j * BLOCK_SIZE;
                float y = 2 * i * BLOCK_SIZE;
                blocks.push_back(Block(Vector2(x, y)));
            }
        }
    }

    void event() {
        static SDL_Event e;
        input_handler.process(e);
        keyboard.left = input_handler.read("left");
        keyboard.right = input_handler.read("right");
        running = !(input_handler.read("quit") || input_handler.read(SDL_QUIT));
    }

    void update(float dt) {
        // update player
        player.direction.x = keyboard.left? -1: keyboard.right? 1: 0;
        player.update(dt);
        ball.update(dt);

        auto player_collider = player.current_collider();
        if (screen_collider.collide(&player_collider)) {
            player.reset_position();
        }

        auto ball_collider = ball.current_collider();
        if (screen_collider.collide(&ball_collider)) {
            ball.reset_position();
            bool x_collide = screen_collider.left.collide(&ball_collider) || screen_collider.right.collide(&ball_collider);
            bool y_collide = screen_collider.top.collide(&ball_collider) || screen_collider.bottom.collide(&ball_collider);
            if (y_collide) {
                ball.velocity.y *= -1;
            } else if (x_collide) {
                ball.velocity.x *= -1;
            }
        } else if (ball_collider.collide(&player_collider)) {
            ball.reset_position();
            ball.velocity.y *= -1;
            if (player.direction.x != 0) {
                ball.velocity.x = abs(ball.velocity.x) * player.direction.x;
            }
        }


        for (auto &b: blocks) {
            if (b.alive && ball_collider.collide(&b.collider)) {
                b.alive = false;
                ball.reset_position();
                ball.velocity.y *= -1;
                break;
            }
        }
    }

    void render() {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        player.render(renderer);
        ball.render(renderer);

        for(auto &b: blocks) {
            if (b.alive) {
                b.render(renderer);
            }
        }

        // render everything
        SDL_RenderPresent(renderer);
    }
};

int main() {
    Game game(TITLE, SCREEN_WIDTH, SCREEN_HEIGHT);
    return game.run();
}

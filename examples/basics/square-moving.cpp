#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <cmath>
#include <iostream>
#include <map>

#include <SDL2/SDL.h>

#include "Game.hpp"
#include "InputHandler.hpp"
#include "math/Vector2.hpp"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

using namespace std;

// keys to InputHandler observe
std::map<const char*, SDL_Keycode> input_config = {
    { "quit", SDLK_ESCAPE },
    { "up", SDLK_UP },
    { "left", SDLK_LEFT },
    { "right", SDLK_RIGHT },
    { "down", SDLK_DOWN },
    { "velocity_up", SDLK_w },
    { "velocity_down", SDLK_s },
};

struct KeyboardState {
    bool up;
    bool left;
    bool right;
    bool down;
    bool velocity_up;
    bool velocity_down;

    bool valid_move () {
        return (up || down || left || right) && !((up && down) || (left && right));
    }
};

struct Player {
    Vector2 position  = { 0, 0 };
    Vector2 direction = { 0, 0 };
    int size          = 50;
    SDL_Rect rect     = { 0, 0, size, size };
    float velocity    = 300;

    void set_direction (struct KeyboardState* k) {
        direction.x = k->right ? 1 : k->left ? -1 : 0;
        direction.y = k->up ? 1 : k->down ? -1 : 0;
    }

    float cos_direction () {
        Vector2 unit_vector (1, 0);
        return direction.cos (unit_vector);
    }

    float sin_direction () {
        float cos = cos_direction ();
        float sin = sqrt (1 - cos * cos);
        return -direction.y * sin;
    }

    void move (float dt) {
        position.x += cos_direction () * velocity * dt;
        position.y += sin_direction () * velocity * dt;
    }
};

struct Player player          = {};
struct KeyboardState keyboard = {};
InputHandler input_handler    = InputHandler (input_config);

void Game::event () {
    static SDL_Event e;
    input_handler.process (e);
    keyboard.up            = input_handler.read ("up");
    keyboard.down          = input_handler.read ("down");
    keyboard.left          = input_handler.read ("left");
    keyboard.right         = input_handler.read ("right");
    keyboard.velocity_up   = input_handler.read ("velocity_up");
    keyboard.velocity_down = input_handler.read ("velocity_down");
    this->running = !(input_handler.read (SDL_QUIT) || input_handler.read ("quit"));
}

void Game::update (float dt) {
    if (keyboard.velocity_up) {
        player.velocity += 50;
        input_handler.write ("velocity_up", false);
    }
    if (keyboard.velocity_down) {
        player.velocity -= 50;
        if (player.velocity <= 50)
            player.velocity = 50;
        input_handler.write ("velocity_down", false);
    }

    if (keyboard.valid_move ()) {
        player.set_direction (&keyboard);
        player.move (dt);
    }

    if (player.position.x < 0) {
        player.position.x = 0;
    } else if (player.position.x + player.size > SCREEN_WIDTH) {
        player.position.x = SCREEN_WIDTH - player.size;
    }
    if (player.position.y < 0) {
        player.position.y = 0;
    } else if (player.position.y + player.size > SCREEN_HEIGHT) {
        player.position.y = SCREEN_HEIGHT - player.size;
    }

    player.rect.x = round (player.position.x);
    player.rect.y = round (player.position.y);
}

void Game::draw () {
    // background
    SDL_SetRenderDrawColor (this->renderer, 255, 255, 255, 255);
    SDL_RenderClear (this->renderer);

    // player
    SDL_SetRenderDrawColor (this->renderer, 0, 255, 255, 255);
    SDL_RenderFillRect (this->renderer, &player.rect);

    // render everything
    SDL_RenderPresent (this->renderer);
}

void start () {
    cout << ":: Game initialization!" << endl;
    player.position.x = SCREEN_WIDTH / 2;
    player.position.y = SCREEN_HEIGHT / 2;
}

int main (void) {
    start ();
    Game game ("Square Moving", SCREEN_WIDTH, SCREEN_HEIGHT);
    game.set_max_frame_rate (60);

    return game.run ();
}

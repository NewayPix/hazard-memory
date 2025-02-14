/*
 *  Author: WesleyCSJ
 *  Description: Render four colors on screen with 2s delay
 *
 *
 */
#include "Game.hpp"
#include "InputHandler.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <map>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

std::map<const char*, SDL_Keycode> input_config = {
    { "quit", SDLK_ESCAPE },
};

InputHandler input_handler = InputHandler(input_config);
float screen_counter       = 0;
int color_cursor           = 0;
int colors[4][4]           = {
              { 225, 0, 0, 0 },
              { 247, 227, 0, 97 },
              { 25, 235, 0, 42 },
              { 0, 1, 224, 255 },
};

void Game::event() {
    static SDL_Event e;
    input_handler.process(e);
    // This is obligatory, if not, window will run forever and be blocked by OS
    // when tried to close.
    this->running = !(input_handler.read("quit") || input_handler.read(SDL_QUIT));
}

void Game::update(float dt) {
    screen_counter = screen_counter + dt;

    if (screen_counter > 2.0f) {
        screen_counter = 0;
        if (color_cursor < 3) {
            color_cursor = color_cursor + 1;
        } else {
            color_cursor = 0;
        }
    }
}

void Game::draw() {
    // Sets a color in a renderer and render on screen
    int* color = colors[color_cursor];
    SDL_SetRenderDrawColor(this->renderer, color[0], color[1], color[2], color[3]);
    SDL_RenderClear(this->renderer);
    SDL_RenderPresent(this->renderer);
}

int main(void) {
    Game game("Hello World", SCREEN_WIDTH, SCREEN_HEIGHT);
    game.set_max_frame_rate(60);
    return game.run();
}

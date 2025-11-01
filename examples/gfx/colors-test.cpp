/*
 *  Author: WesleyCSJ
 *  Description: Render four colors on screen with 2s delay
 *
 *
 */
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <array>
#include <unordered_map>
#include <vector>

#include "Game.hpp"
#include "InputHandler.hpp"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// keys to InputHandler observe
std::unordered_map<std::string, SDL_Keycode> input_config = {
    { "quit", SDLK_ESCAPE },
};

InputHandler input_handler = InputHandler(input_config);

class MyGame : public Game {
public:
    MyGame(std::string_view title, int width, int height) : Game(title, width, height) {}

    void event() override {
        static SDL_Event e;
        input_handler.process(e);
        this->running = !(input_handler.read("quit") || input_handler.read(SDL_QUIT));
    }

    void update(float dt) override {
        static float time = 0;
        if (time > 0.1) {
            color_idx = (color_idx + 1) % (sizeof(colors) / sizeof(colors[0]));
            time = 0;
        }
        time += dt;
    }

    void draw() override {
        // Sets a color in a renderer and render on screen
        auto color = colors[color_idx];
        SDL_SetRenderDrawColor(this->renderer.get(), color[0], color[1], color[2], color[3]);
        SDL_RenderClear(this->renderer.get());
        SDL_RenderPresent(this->renderer.get());
    }

private:
    int color_idx = 0;
    int colors[4][4]           = {
              { 225, 0, 0, 0 },
              { 247, 227, 0, 97 },
              { 25, 235, 0, 42 },
              { 0, 1, 224, 255 },
    };
};

int main(void) {
    MyGame game("Hello World", SCREEN_WIDTH, SCREEN_HEIGHT);
    game.set_max_frame_rate(60);
    return game.run();
}

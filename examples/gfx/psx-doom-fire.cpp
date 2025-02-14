/*
 *  Author: WesleyCSJ
 *  Description: Doom Fire PSX Implementation
 *
 *
 */
#include "Game.hpp"
#include "InputHandler.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <array>
#include <map>
#include <vector>

#include <cmath>
#include <cstdlib>
#include <ctime>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TILESIZE 4

std::map<const char*, SDL_Keycode> input_config = {
    { "quit", SDLK_ESCAPE },
};

InputHandler input_handler = InputHandler(input_config);
// Game variables
int widthQuantity  = SCREEN_WIDTH / TILESIZE;
int heightQuantity = 96;
int offsetX        = 0;
int offsetY        = SCREEN_HEIGHT - (heightQuantity * TILESIZE) + 5;
// int tiles[widthQuantity][heightQuantity];
std::vector<std::array<int, 3>> colors = {
    { 0, 0, 0 },       { 31, 7, 7 },     { 47, 15, 7 },     { 71, 15, 7 },     { 87, 23, 7 },
    { 103, 31, 7 },    { 119, 31, 7 },   { 143, 39, 7 },    { 159, 47, 7 },    { 175, 63, 7 },
    { 191, 71, 7 },    { 199, 71, 7 },   { 223, 79, 7 },    { 223, 87, 7 },    { 223, 87, 7 },
    { 215, 95, 7 },    { 215, 103, 15 }, { 207, 111, 15 },  { 207, 119, 15 },  { 207, 127, 15 },
    { 207, 137, 23 },  { 199, 135, 23 }, { 199, 143, 23 },  { 199, 151, 31 },  { 191, 159, 31 },
    { 191, 159, 31 },  { 191, 167, 39 }, { 191, 167, 39 },  { 191, 175, 47 },  { 183, 175, 47 },
    { 183, 183, 47 },  { 183, 183, 55 }, { 207, 207, 111 }, { 223, 223, 159 }, { 239, 239, 199 },
    { 255, 255, 255 },
};
std::vector<int> tiles;

void start() {
    for (int i = 0; i < widthQuantity; i++) {
        for (int j = 0; j < heightQuantity; j++) {
            if (j == (heightQuantity - 1)) {
                tiles.push_back(35);
            } else {
                tiles.push_back(0);
            }
        }
    }
}

int getTileValue(int x, int y) {
    return tiles.at((x * heightQuantity) + y);
}

double sortNumber() {
    return (std::rand() / (double)RAND_MAX);
}

void setTileValue(int x, int y, int value) {
    tiles.at((x * heightQuantity) + y) = value;
}

void Game::event() {
    static SDL_Event e;
    input_handler.process(e);
    this->running = !(input_handler.read("quit") || input_handler.read(SDL_QUIT));
}


void Game::update([[maybe_unused]] float dt) {
    for (int i = 1; i < (widthQuantity - 1); i++) {
        for (int j = 1; j < (heightQuantity - 1); j++) {
            int value         = getTileValue(i, (j + 1));
            double randNumber = sortNumber();
            int coeficient    = value - floor(randNumber * 3);
            value             = (coeficient > 0) ? coeficient : 0;
            if (randNumber < 0.4) {
                setTileValue((i - 1), j, value);
            } else if (randNumber > 0.4 && randNumber <= 0.8) {
                setTileValue((i + 1), j, value);
            } else {
                setTileValue(i, (j - 1), value);
            }
        }
    }
}

void Game::draw() {
    // Sets a color in a renderer and render on screen
    SDL_RenderClear(this->renderer);
    for (int i = 0; i < widthQuantity; i++) {
        for (int j = 0; j < heightQuantity; j++) {
            int tileEl = getTileValue(i, j);
            SDL_SetRenderDrawColor(this->renderer, colors[tileEl][0], colors[tileEl][1], colors[tileEl][2], 255);
            SDL_Rect rectangle;

            rectangle.x = offsetX + (TILESIZE * i);
            rectangle.y = offsetY + (TILESIZE * j);
            rectangle.w = TILESIZE;
            rectangle.h = TILESIZE;
            SDL_RenderFillRect(this->renderer, &rectangle);
        }
    }
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
    SDL_RenderPresent(this->renderer);
}

int main(void) {
    std::srand(std::time(nullptr));
    start();
    Game game("Doom Fire", SCREEN_WIDTH, SCREEN_HEIGHT);
    game.set_max_frame_rate(60);
    return game.run();
}

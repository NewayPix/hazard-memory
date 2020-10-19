/*
 *  Author: WesleyCSJ
 *  Description: Render four colors on screen with 2s delay
 *
 *
 */
#include <SDL2/SDL.h>
#include "GameLoop.hpp"
#include "InputHandler.hpp"

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

std::map<const char*, SDL_Keycode> input_config = {
    {"quit", SDLK_ESCAPE},
};

class Game: public GameLoop {
    using GameLoop::GameLoop;
    InputHandler input_handler = InputHandler(input_config);
    float screenCounter = 0;
    int colorCursor = 0;
    int colors[4][4] = {
        {225,  0,  0,0},
        {247,227,  0, 97},
        {25 ,235,  0, 42},
        {0  ,1  ,224,255}
    };

    void event(){
        static SDL_Event e;
        input_handler.process(e);
        //This is obligatory, if not, window will run forever and be blocked by OS when tried to close.
        running = !(input_handler.read("quit") || input_handler.read(SDL_QUIT));

    }

    void update(float dt) {
        screenCounter = screenCounter + dt;
        int *color = colors[colorCursor];

        if (screenCounter > 2.0f){
            screenCounter = 0;
            if (colorCursor < 3){
                colorCursor = colorCursor + 1;
            } else {
                colorCursor = 0;
            }
        }

    }

    void render(){
        //Sets a color in a renderer and render on screen
        int *color = colors[colorCursor];
        SDL_SetRenderDrawColor(renderer, color[0] , color[1], color[2], color[3]);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }
};

int main(int argc, char** argv){
    Game game("Hello World", SCREEN_WIDTH, SCREEN_HEIGHT);
    game.set_max_frame_rate(60);
    return game.run();
}

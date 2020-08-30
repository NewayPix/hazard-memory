#ifndef GAME_LOOP_HPP
#define GAME_LOOP_HPP

#include <string>
#include <iostream>

#include <SDL2/SDL.h>

#include "Ticker.hpp"

class GameLoop {
protected:
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    bool running = true;
    Ticker timer;
    virtual void event() = 0;
    virtual void update(float dt) = 0;
    virtual void render() = 0;
    virtual void start() {};
    virtual void stop() {};
public:
    GameLoop(const char* title, int width, int height) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            throw std::string("SDL could not initialize: ") + SDL_GetError();
        }

        window = SDL_CreateWindow(title,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  width,
                                  height,
                                  SDL_WINDOW_SHOWN);

        if (window == nullptr) {
            throw std::string("window could not be created: ") + SDL_GetError();
        }


        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            throw std::string("renderer could not be created: ") + SDL_GetError();
        }
    }

    ~GameLoop() {
        std::cout << ":: Game being destroyed!" << std::endl;
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;

        SDL_DestroyWindow(window);
        window = nullptr;

        SDL_Quit();
    }

    void loop() {
        do {
            event();
            update(timer.dt());
            render();
        } while(running);
    }

    int run() {
        try {
            start();
            loop();
            stop();
        } catch (std::string e) {
            std::cerr << "[error] " << e << std::endl;
            return 1;
        }

        return 0;
    }

};

#endif // GAME_LOOP_HPP
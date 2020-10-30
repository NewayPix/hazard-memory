#ifndef GAME_LOOP_HPP
#define GAME_LOOP_HPP

#include <string>
#include <iostream>

#include <SDL2/SDL.h>

#include "Ticker.hpp"

class Game {
public:
    int run() {
        try {
            loop();
        } catch (std::string e) {
            std::cerr << "[error] " << e << std::endl;
            return 1;
        }

        return 0;
    }

    Game(const char* title, int width, int height) {
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


        renderer = SDL_CreateRenderer(window, -1, renderer_flags);
        if (!renderer) {
            throw std::string("renderer could not be created: ") + SDL_GetError();
        }
    }

    ~Game() {
        std::cout << ":: Game being destroyed!" << std::endl;
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;

        SDL_DestroyWindow(window);
        window = nullptr;

        SDL_Quit();
    }

    void set_max_frame_rate(int fps) {
        this->fps_target = fps;
    }

private:
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    // int renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    int renderer_flags = SDL_RENDERER_ACCELERATED;
    bool running = true;
    Ticker timer;
    void event();
    void update(float dt);
    void draw();
    int fps_target = 240;

    void fps_lock(float dt) {
        if (fps_target > 0) {
            float ideal_frame_time_ms = 1000.0f/fps_target;
            float frame_time_ms = 1000 * dt;
            int delay_time = ideal_frame_time_ms - frame_time_ms;
            if (delay_time > 0) {
                SDL_Delay(delay_time);
            }
        }
    }

    void loop() {
        float dt;
        do {
            event();
            update(dt = timer.dt());
            draw();
            fps_lock(dt);
        } while(running);
    }

};

#endif // GAME_LOOP_HPP

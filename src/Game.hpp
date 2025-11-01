#ifndef GAME_LOOP_HPP
#define GAME_LOOP_HPP

#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <thread>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>

#include "Exception.hpp"
#include "Ticker.hpp"

// Custom deleters for SDL resources
struct SDLWindowDeleter {
    void operator()(SDL_Window* window) const {
        SDL_DestroyWindow(window);
    }
};

struct SDLRendererDeleter {
    void operator()(SDL_Renderer* renderer) const {
        SDL_DestroyRenderer(renderer);
    }
};

class Game {
    public:
    int run() {
        try {
            loop();
        } catch (const Exception& e) {
            std::cerr << "[error] " << e.what() << std::endl;
            return 1;
        }

        return 0;
    }

    Game(std::string_view title, int width, int height) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            throw Exception("SDL could not initialize: " + std::string(SDL_GetError()));
        }

        window = std::unique_ptr<SDL_Window, SDLWindowDeleter>(SDL_CreateWindow(title.data(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN));

        if (window == nullptr) {
            throw Exception("window could not be created: " + std::string(SDL_GetError()));
        }

        renderer = std::unique_ptr<SDL_Renderer, SDLRendererDeleter>(SDL_CreateRenderer(window.get(), -1, renderer_flags));
        if (!renderer) {
            throw Exception("renderer could not be created: " + std::string(SDL_GetError()));
        }
    }

    ~Game() {
        std::cout << ":: Game being destroyed!" << std::endl;
        SDL_Quit();
    }

    void set_max_frame_rate(int fps) {
        this->fps_target = fps;
    }

    protected:
    std::unique_ptr<SDL_Window, SDLWindowDeleter> window;
    std::unique_ptr<SDL_Renderer, SDLRendererDeleter> renderer;
    // int renderer_flags = SDL_RENDERER_ACCELERATED |
    // SDL_RENDERER_PRESENTVSYNC;
    int renderer_flags = SDL_RENDERER_ACCELERATED;
    bool running       = true;
    Ticker timer;
    virtual void event();
    virtual void update(float dt);
    virtual void draw();
    int fps_target = 240;

    void fps_lock() {
        if (fps_target > 0) {
            const auto frame_duration = std::chrono::milliseconds(1000) / fps_target;
            const auto frame_end = timer.get_last_tick() + frame_duration;
            std::this_thread::sleep_until(frame_end);
        }
    }

    void loop() {
        float dt;
        do {
            dt = timer.dt();
            event();
            update(dt);
            draw();
            fps_lock();
        } while (running);
    }
};

#endif // GAME_LOOP_HPP

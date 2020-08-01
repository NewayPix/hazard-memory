#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

struct KeyboardState {
    bool up;
    bool left;
    bool right;
    bool down;
    bool velocity_up;
    bool velocity_down;
    float angle() {
        if (up && right)        return 45;
        else if (up && left)    return 135;
        else if (down && left)  return 235;
        else if (down && right) return 315;
        else if (right)         return 0;
        else if (up)            return 90;
        else if (left)          return 180;
        else if (down)          return 270;
        throw std::string("KeyboardState.angle() needs at least one direction available");
    }

    bool valid_move() {
        return !((up && down) || (left && right));
    }

    bool moving() {
        return up || down || left || right;
    }
};

struct Player {
    float x;
    float y;

    void move(float angle, float ds) {
        x += cos(- angle * (M_PI / 180)) * ds;
        y += sin(- angle * (M_PI / 180)) * ds;
    }
};

class Game {
private:
    //The window we'll be rendering to
    SDL_Window *window = NULL;

    // Renderer object
    SDL_Renderer *renderer = NULL;

    // player
    const int block_size = 50;
    SDL_Rect playerRect = {0, 0, block_size, block_size};
    struct Player player = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};

    const char *title = "Square Moving";
    const float sqrt2 = sqrt(2);

    bool running = true;

    struct KeyboardState keyboard;

    float velocity = 150;

    void event(SDL_Event *e) {
        while (SDL_PollEvent(e)) {
            switch (e->type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                switch(e->key.keysym.sym) {
                case SDLK_ESCAPE:
                    running = false;
                    break;
                case SDLK_UP:
                    keyboard.up = true;
                    break;
                case SDLK_DOWN:
                    keyboard.down = true;
                    break;
                case SDLK_LEFT:
                    keyboard.left = true;
                    break;
                case SDLK_RIGHT:
                    keyboard.right = true;
                    break;
                case SDLK_w:
                    keyboard.velocity_up = true;
                    break;
                case SDLK_s:
                    keyboard.velocity_down = true;
                    break;
                }
                break;
            case SDL_KEYUP:
                switch(e->key.keysym.sym) {
                case SDLK_UP:
                    keyboard.up = false;
                    break;
                case SDLK_DOWN:
                    keyboard.down = false;
                    break;
                case SDLK_LEFT:
                    keyboard.left = false;
                    break;
                case SDLK_RIGHT:
                    keyboard.right = false;
                    break;
                }
                break;
            }
        }
    }

    void update(float dt) {
        if (keyboard.velocity_up) {
            velocity += 50;
            keyboard.velocity_up = false;
        }
        if (keyboard.velocity_down) {
            velocity -= 50;
            if (velocity <= 50) velocity = 50;
            keyboard.velocity_down = false;
        }

        if (keyboard.moving() && keyboard.valid_move()) {
            player.move(keyboard.angle(), velocity * dt);
        }

        if (player.x < 0) {
            player.x = 0;
        }
        if (player.y < 0) {
            player.y = 0;
        }
        if (player.x + block_size > SCREEN_WIDTH) {
            player.x = SCREEN_WIDTH - block_size;
        }
        if (player.y + block_size > SCREEN_HEIGHT) {
            player.y = SCREEN_HEIGHT - block_size;
        }

        playerRect.x = round(player.x);
        playerRect.y = round(player.y);
    }

    void render() {
        // background
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // player
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
        SDL_RenderFillRect(renderer, &playerRect);

        // render everything
        SDL_RenderPresent(renderer);
    }

public:
    // Initialization flag
    bool success = true;


    Game() {
        std::cout << ":: Game initialization!" << std::endl;
        //Initialize SDL
        if(SDL_Init(SDL_INIT_VIDEO) < 0) {
            printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else {
            //Create window
            window = SDL_CreateWindow(title,
                                      SDL_WINDOWPOS_UNDEFINED,
                                      SDL_WINDOWPOS_UNDEFINED,
                                      SCREEN_WIDTH,
                                      SCREEN_HEIGHT,
                                      SDL_WINDOW_SHOWN);
            if(window == NULL) {
                printf("Window could not be created! SDL_Error: %s\n",
                       SDL_GetError() );
                success = false;
            }
            else {
                renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
                if (!renderer) {
                    printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
                    success = false;
                }

            }
        }
    }

    ~Game() {
        std::cout << ":: Game being destroyed!" << std::endl;
        SDL_DestroyRenderer(renderer);
        renderer = NULL;

        SDL_DestroyWindow(window);
        window = NULL;

        SDL_Quit();
    }

    float dt(Uint32 clock) {
        return (SDL_GetTicks() - clock) / 1000.0f;
    }

    void run() {
        SDL_Event e;
        do {
            Uint32 clock = SDL_GetTicks();
            event(&e);
            render();
            update(dt(clock));
        } while(running);
    }
};


int main(void) {
    Game *g = new Game();
    int error = g->success? 0: 1;
    if (!error) {
        g->run();
    }
    delete g;
    return error;
}

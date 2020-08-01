#include <iostream>
#include <cmath>
#include <string>

#include <SDL2/SDL.h>

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
    const int size = 50;
    SDL_Rect rect = {0, 0, size, size};
    float velocity = 300;


    void move(float angle, float dt) {
        x += cos(- angle * (M_PI / 180)) * velocity * dt;
        y += sin(- angle * (M_PI / 180)) * velocity * dt;
    }
};

class Game {
private:
    //The window we'll be rendering to
    SDL_Window *window = NULL;

    // Renderer object
    SDL_Renderer *renderer = NULL;

    // player
    struct Player player = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};

    const char *title = "Square Moving";

    bool running = true;

    struct KeyboardState keyboard;

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
            player.velocity += 50;
            keyboard.velocity_up = false;
        }
        if (keyboard.velocity_down) {
            player.velocity -= 50;
            if (player.velocity <= 50) player.velocity = 50;
            keyboard.velocity_down = false;
        }

        if (keyboard.moving() && keyboard.valid_move()) {
            player.move(keyboard.angle(), dt);
        }

        if (player.x < 0) {
            player.x = 0;
        }
        if (player.y < 0) {
            player.y = 0;
        }
        if (player.x + player.size > SCREEN_WIDTH) {
            player.x = SCREEN_WIDTH - player.size;
        }
        if (player.y + player.size > SCREEN_HEIGHT) {
            player.y = SCREEN_HEIGHT - player.size;
        }

        player.rect.x = round(player.x);
        player.rect.y = round(player.y);
    }

    void render() {
        // background
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // player
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
        SDL_RenderFillRect(renderer, &player.rect);

        // render everything
        SDL_RenderPresent(renderer);
    }

public:
    Game() {
        std::cout << ":: Game initialization!" << std::endl;
        //Initialize SDL
        if(SDL_Init(SDL_INIT_VIDEO) < 0) {
            throw std::string("SDL could not initialize: ") + SDL_GetError();
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
                throw std::string("window could not be created: ") + SDL_GetError();
            }
            else {
                renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
                if (!renderer) {
                    throw std::string("renderer could not be created: ") + SDL_GetError();
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
    try {
        Game *g = new Game();
        g->run();
        delete g;
    } catch (std::string s) {
        std::cerr << "[error] " << s << std::endl;
        return 1;
    }

    return 0;
}

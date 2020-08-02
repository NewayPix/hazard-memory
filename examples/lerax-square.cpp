#include <iostream>
#include <cmath>
#include <string>

#include <SDL2/SDL.h>
#include <Vector2D.hpp>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define G 9.8

using namespace std;

struct KeyboardState {
    bool up;
    bool left;
    bool right;
    bool down;
    bool velocity_up;
    bool velocity_down;
    bool run;

    bool valid_move() {
        return (up || down || left || right) && !((up && down) || (left && right));
    }
};

struct Player {
    Vector2D position = {0, 0};
    Vector2D direction = {0, 0};
    Vector2D velocity = {300, 0};
    int size = 50;
    SDL_Rect rect = {0, 0, size, size};


    void set_direction(struct KeyboardState *k) {
        direction.x = k->right? 1: k->left? -1: 0;
        direction.y = k->up? 1: k->down? -1: 0;
    }

    float cos_direction() {
        Vector2D unit_vector(1, 0);
        return direction.cos(unit_vector);
    }

    float sin_direction() {
        float cos = cos_direction();
        float sin = sqrt(1 - cos*cos);
        return -direction.y * sin;
    }

    void try_jump(float dt) {
        if (direction.y == 1) {
            velocity.y = -100;
            position.y += velocity.y * dt;
        }
    }

    void move(float dt) {
        move(dt, velocity);
    }

    void move(float dt, Vector2D velocity) {
        position.x += cos_direction() * velocity.x * dt;
        // position.y += sin_direction() * velocity * dt;
    }

    void move(float dt, bool run) {
        if (run) {
            move(dt, velocity * 2);
        } else {
            move(dt);
        }
    }
};

class Game {
private:
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    const char *title = "Square Moving";
    bool running = true;
    struct Player player;
    struct KeyboardState keyboard = {};

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
                case SDLK_LSHIFT:
                    keyboard.run = true;
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
                case SDLK_LSHIFT:
                    keyboard.run = false;
                    break;
                }
                break;
            }
        }
    }

    void update(float dt) {
        if (keyboard.velocity_up) {
            player.velocity.x += 50;
            keyboard.velocity_up = false;
        }
        if (keyboard.velocity_down) {
            player.velocity.x -= 50;
            if (player.velocity.x <= 50) player.velocity.x = 50;
            keyboard.velocity_down = false;
        }

        if (keyboard.valid_move()) {
            player.set_direction(&keyboard);
            player.move(dt, keyboard.run);
        }

        // gravity velocity
        if (player.position.y + player.size < SCREEN_HEIGHT) {
            player.velocity.y += 15 * G * dt;
            player.position.y += 4 * player.velocity.y * dt;
        } else {
            player.velocity.y = 0;
            player.try_jump(dt);
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

        player.rect.x = round(player.position.x);
        player.rect.y = round(player.position.y);
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
        cout << ":: Game initialization!" << endl;
        player.position.x = SCREEN_WIDTH / 2;
        player.position.y = SCREEN_HEIGHT - player.size;

        if(SDL_Init(SDL_INIT_VIDEO) < 0) {
            throw string("SDL could not initialize: ") + SDL_GetError();
        }
        else {
            window = SDL_CreateWindow(title,
                                      SDL_WINDOWPOS_UNDEFINED,
                                      SDL_WINDOWPOS_UNDEFINED,
                                      SCREEN_WIDTH,
                                      SCREEN_HEIGHT,
                                      SDL_WINDOW_SHOWN);
            if(window == NULL) {
                throw string("window could not be created: ") + SDL_GetError();
            }
            else {
                renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
                if (!renderer) {
                    throw string("renderer could not be created: ") + SDL_GetError();
                }

            }
        }
    }

    ~Game() {
        cout << ":: Game being destroyed!" << endl;
        SDL_DestroyRenderer(renderer);
        renderer = NULL;

        SDL_DestroyWindow(window);
        window = NULL;

        SDL_Quit();
    }

    float dt(Uint32 clock) {
        return (SDL_GetTicks() - clock) / 1000.0f;
    }

    void debug() {
        cout << "Player position: " << string(player.position) << endl;
    }

    void run() {
        SDL_Event e;
        Uint32 clock = 0;
        float dt = 0, debugTime = 0;
        do {
            dt = this->dt(clock);
            clock = SDL_GetTicks();
            event(&e);
            render();
            update(dt);
            debugTime += dt;
            if (debugTime > 1) {
                debug();
                debugTime = 0;
            }
        } while(running);
    }
};


int main(void) {
    try {
        Game game;
        game.run();
    } catch (string s) {
        cerr << "[error] " << s << endl;
        return 1;
    }

    return 0;
}

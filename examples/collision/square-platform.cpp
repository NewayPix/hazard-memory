#include <cmath>
#include <deque>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "Game.hpp"
#include "InputHandler.hpp"
#include "collider/ColliderCircle.hpp"
#include "collider/ColliderRect.hpp"
#include "collider/ColliderScreen.hpp"
#include "math/Vector2.hpp"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define G 9.8

using namespace std;

// keys to InputHandler observe
std::map<const char*, SDL_Keycode> input_config = {
    {"quit", SDLK_ESCAPE},     {"up", SDLK_UP},     {"left", SDLK_LEFT},
    {"right", SDLK_RIGHT},     {"down", SDLK_DOWN}, {"velocity_up", SDLK_w},
    {"velocity_down", SDLK_s}, {"run", SDLK_LSHIFT}};

struct KeyboardState {
    bool up;
    bool left;
    bool right;
    bool down;
    bool velocity_up;
    bool velocity_down;
    bool run;

    bool valid_move() {
        return (up || down || left || right) &&
               !((up && down) || (left && right));
    }
};

struct Player {
    Vector2 position = {0, 0};
    Vector2 direction = {0, 0};
    Vector2 velocity = {300, 0};
    int size = 50;

    ColliderRect collider = ColliderRect(0, 0, size, size);

    void set_size(int size) {
        position.x += (this->size - size);
        position.y += (this->size - size);
        this->size = size;

        // update player's collider
        collider = ColliderRect(position.x, position.y, size, size);
    }

    ColliderRect current_collider() {
        return ColliderRect(round(position.x), round(position.y),
                            collider.polygon.w, collider.polygon.h);
    }

    /*
     * Check if the player is on top of some block
     */
    bool on_ground(vector<Collider*>& colliders) {
        ColliderRect player_collider = current_collider();
        for (auto collider : colliders) {
            if (player_collider.on_top(collider)) {
                return true;
            }
        }

        return false;
    }

    /*
     * Check if the player collides with some block
     */
    bool collision(vector<Collider*>& colliders) {
        ColliderRect player_collider = current_collider();
        for (auto collider : colliders) {
            if (collider->collide(&player_collider)) {
                return true;
            }
        }

        return false;
    }

    void reset_to_last_position() {
        position.x = collider.polygon.x;
        position.y = collider.polygon.y;
        velocity.y = 0;
    }

    void update_rect() { collider = current_collider(); }

    void set_direction(struct KeyboardState* k) {
        direction.x = k->right ? 1 : k->left ? -1 : 0;
        direction.y = k->up ? 1 : k->down ? -1 : 0;
    }

    float cos_direction() {
        Vector2 unit_vector(1, 0);
        return direction.cos(unit_vector);
    }

    float sin_direction() {
        float cos = cos_direction();
        float sin = sqrt(1 - cos * cos);
        return -direction.y * sin;
    }

    void try_jump(float dt) {
        if (direction.y == 1) {
            velocity.y = -100;
            position.y += velocity.y * dt;
            direction.y = 0;
        }
    }

    void move(float dt, Vector2 velocity) {
        position.x += cos_direction() * velocity.x * dt;
        // position.y += sin_direction() * velocity.x * dt;
    }

    void move(float dt, bool run) {
        if (run) {
            move(dt, velocity * 2);
        } else {
            move(dt, velocity);
        }
    }
};

struct Player player = {};
struct KeyboardState keyboard = {};
ColliderScreen collider_screen = ColliderScreen(SCREEN_WIDTH, SCREEN_HEIGHT);
deque<SDL_Rect> squares_shadow;
vector<ColliderRect> blocks;
vector<ColliderCircle> circles;
vector<Collider*> colliders;
unsigned int squares_max = 50; /* Max size of squares_shadow container */
unsigned long frames = 0;
InputHandler input_handler = InputHandler(input_config);

void Game::event() {
    static SDL_Event e;
    input_handler.process(e);
    keyboard.up = input_handler.read("up");
    keyboard.down = input_handler.read("down");
    keyboard.left = input_handler.read("left");
    keyboard.right = input_handler.read("right");
    keyboard.velocity_up = input_handler.read("velocity_up");
    keyboard.velocity_down = input_handler.read("velocity_down");
    keyboard.run = input_handler.read("run");
    running = !(input_handler.read(SDL_QUIT) || input_handler.read("quit"));
}

void debug(float dt) {
    static float debugTime = 0;
    if (debugTime > 1) {
        static unsigned long lastFrames = frames;
        cout << "--- DEBUG ---" << endl;
        cout << "Player position: \t" << string(player.position) << endl;
        cout << "Player velocity: \t" << string(player.velocity) << endl;
        cout << "            FPS: \t" << frames - lastFrames << endl;
        cout << "      on ground: \t" << player.on_ground(colliders) << endl;
        cout << "      collision: \t" << player.collision(colliders) << endl;
        cout << fixed;
        cout.precision(7);
        cout << "             dt: \t" << dt << "s" << endl;
        cout << endl;
        lastFrames = frames;
        debugTime = 0;
    }
    debugTime += dt;
    frames++;
}

void Game::update(float dt) {
    // velocity change
    if (keyboard.velocity_up) {
        player.velocity.x += 50;
        player.set_size(player.size + 10);
        input_handler.write("velocity_up", false);
    }

    if (keyboard.velocity_down) {
        player.velocity.x -= 50;
        if (player.velocity.x <= 50)
            player.velocity.x = 50;
        player.set_size(player.size - 10);
        if (player.size <= 10)
            player.set_size(10);
        input_handler.write("velocity_down", false);
    }
    // gravity velocity
    if (player.on_ground(colliders)) {
        player.velocity.y = 0;
        player.try_jump(dt);
    } else {
        player.velocity.y += 15 * G * dt;
        player.position.y += 4 * player.velocity.y * dt;
    }

    // collision detection
    if (player.collision(colliders)) {
        player.reset_to_last_position();
    } else {
        // player movement
        Vector2 copy_position(player.position);
        if (keyboard.valid_move()) {
            player.set_direction(&keyboard);
            player.move(dt, keyboard.run);
        }
        if (player.collision(colliders)) {
            player.position = copy_position;
        }

        // valid state
        player.update_rect();
    }

    static float shadow_interval = 0;
    if (shadow_interval <= 0) {
        shadow_interval = 0.005;
        if (squares_shadow.size() > squares_max) {
            squares_shadow.pop_front();
        } else {
            squares_shadow.push_back(player.collider.polygon);
        }
    }

    shadow_interval -= dt;
    debug(dt);
}

void Game::draw() {
    // background
    SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
    SDL_RenderClear(this->renderer);

    // player
    int size = squares_shadow.size();
    for (int i = 0; i < size; ++i) {
        int factor = round(255 * ((float)(i + 1) / size));
        SDL_SetRenderDrawColor(this->renderer, 0, factor, 255, 255);
        SDL_RenderFillRect(this->renderer, &squares_shadow[i]);
    }

    // platform blocks
    for (auto b : blocks) {
        SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(this->renderer, &b.polygon);
    }

    for (auto c : circles) {
        filledCircleRGBA(this->renderer, c.center.x, c.center.y,
                         c.circle.radius, 0, 0, 0, 255);
    }

    // render everything
    SDL_RenderPresent(this->renderer);
}

void start() {
    cout << ":: Game initialization!" << endl;
    player.position.x = SCREEN_WIDTH / 2;
    player.position.y = SCREEN_HEIGHT - player.size;
    player.update_rect();

    int block_size = 50;
    colliders.push_back(&collider_screen);
    SDL_Rect block1 = {.x = 0,
                       .y = SCREEN_HEIGHT - block_size,
                       .w = 2 * block_size,
                       .h = block_size};
    SDL_Rect block2 = {.x = SCREEN_WIDTH / 4,
                       .y = SCREEN_HEIGHT - 3 * block_size,
                       .w = 2 * block_size,
                       .h = block_size};
    SDL_Rect block3 = {.x = SCREEN_WIDTH / 2,
                       .y = SCREEN_HEIGHT - 4 * block_size,
                       .w = 2 * block_size,
                       .h = block_size};
    SDL_Rect block4 = {.x = SCREEN_WIDTH / 6,
                       .y = SCREEN_HEIGHT - 6 * block_size,
                       .w = 2 * block_size,
                       .h = block_size};
    SDL_Rect block5 = {.x = SCREEN_WIDTH / 2,
                       .y = SCREEN_HEIGHT - 8 * block_size,
                       .w = 2 * block_size,
                       .h = block_size};
    SDL_Rect block6 = {.x = SCREEN_WIDTH - 2 * block_size,
                       .y = SCREEN_HEIGHT - 10 * block_size,
                       .w = 2 * block_size,
                       .h = block_size};

    blocks.push_back(ColliderRect(block1));
    blocks.push_back(ColliderRect(block2));
    blocks.push_back(ColliderRect(block3));
    blocks.push_back(ColliderRect(block4));
    blocks.push_back(ColliderRect(block5));
    blocks.push_back(ColliderRect(block6));

    float radius = 50;
    circles.push_back(ColliderCircle(Vector2(200, 150), radius));
    circles.push_back(ColliderCircle(Vector2(600, 350), radius));

    for (auto& b : blocks) {
        colliders.push_back(&b);
    }

    for (auto& c : circles) {
        colliders.push_back(&c);
    }
    blocks.push_back(
        ColliderRect(200 - radius, 150 - radius, radius * 2, radius * 2));
    blocks.push_back(
        ColliderRect(600 - radius, 350 - radius, radius * 2, radius * 2));
}

int main(void) {
    start();
    Game game("Square Platform", SCREEN_WIDTH, SCREEN_HEIGHT);
    game.set_max_frame_rate(120);
    return game.run();
}

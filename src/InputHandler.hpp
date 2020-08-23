#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include <map>
#include <vector>
#include <string>

#include <SDL2/SDL_events.h>
#include <SDL2/SDL.h>

class InputHandler {
public:
    InputHandler(std::map<const char*, SDL_Keycode> input_config) {
        this->input_config = input_config;
    };

    /*
     * @brief Process event SDL_Event and stores state on internal map
     */
    void process(SDL_Event &e) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                this->input_state[SDL_QUIT] = true;
            }
            for (auto &input : input_state) {
                auto k = input.first;
                switch (e.type) {
                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == k) {
                        this->input_state[k] = true;
                    }
                    break;
                case SDL_KEYUP:
                    if (e.key.keysym.sym == k) {
                        this->input_state[k] = false;
                    }
                    break;
                }
            }
        }
    }

    /*
     *  @brief read state of input_key
     */
    bool read(SDL_Keycode input_key) {
        return input_state[input_key];
    }

    bool read(const char* alias) {
        return read(input_config[alias]);
    }

    /*
     *  @brief write state of input_key
     */
    void write(SDL_Keycode input_key, bool state) {
        input_state[input_key] = state;
    }

    void write(const char* alias, bool state) {
        write(input_config[alias], state);
    }

private:
    std::map<SDL_Keycode, bool> input_state;
    std::map<const char*, SDL_Keycode> input_config;
};

#endif // INPUT_HANDLER_HPP

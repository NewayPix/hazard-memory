#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include <unordered_map>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>

class InputHandler {
public:
    InputHandler(std::unordered_map<std::string, SDL_Keycode> input_config) : input_config(input_config) {
        for (const auto& [key, value] : input_config) {
            input_state[value] = false;
        }
    };

    /*
     * @brief Process event SDL_Event and stores state on internal map
     */
    void process(SDL_Event& e) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                input_state[SDL_QUIT] = true;
            }
            if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
                if (input_state.count(e.key.keysym.sym)) {
                    input_state[e.key.keysym.sym] = (e.type == SDL_KEYDOWN);
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

    bool read(const std::string& alias) {
        return read(input_config.at(alias));
    }

    /*
     *  @brief write state of input_key
     */
    void write(SDL_Keycode input_key, bool state) {
        input_state[input_key] = state;
    }

    void write(const std::string& alias, bool state) {
        write(input_config.at(alias), state);
    }

private:
    std::unordered_map<SDL_Keycode, bool> input_state;
    std::unordered_map<std::string, SDL_Keycode> input_config;
};

#endif // INPUT_HANDLER_HPP
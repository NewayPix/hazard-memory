#ifndef GAME_HPP
#define GAME_HPP

#include "GameLoop.hpp"

class Game: public GameLoop {
private:
    void start();
    void event();
    void update(float dt);
    void render();
public:
    using GameLoop::GameLoop;
};

#endif // GAME_HPP

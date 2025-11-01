#ifndef TICKER_HPP
#define TICKER_HPP

#include <chrono>

/**
 * @brief control the time: tick and delta-time
 */
class Ticker {
public:
    using clock = std::chrono::high_resolution_clock;

    Ticker() : last_tick(clock::now()) {}

    // tick
    clock::time_point tick() {
        return clock::now();
    }

    // delta time in seconds
    float dt() {
        auto t = clock::now();
        std::chrono::duration<float> delta = t - last_tick;
        last_tick = t;
        return delta.count();
    }

    clock::time_point get_last_tick() const {
        return last_tick;
    }

private:
    clock::time_point last_tick;
};

#endif // TICKER_HPP

#ifndef TICKER_HPP
#define TICKER_HPP

#include <chrono>
#include <cstdint>

/**
 * @brief control the time: tick and delta-time
 */
class Ticker {
public:

    Ticker(): last_tick(tick()) {}

    // tick in microseconds
    uint64_t tick() {
        using namespace std::chrono;
        auto now = high_resolution_clock::now();
        return duration_cast<microseconds>(now.time_since_epoch()).count();
    }

    // delta time in seconds
    float dt() {
        auto t = tick();
        float delta = (t - last_tick) / 1000000.0f;
        last_tick = t;
        return delta;
    }

private:
    uint64_t last_tick;
};

#endif // TICKER_HPP

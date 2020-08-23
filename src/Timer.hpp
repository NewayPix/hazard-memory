#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

/**
 * @brief control the time: tick and delta-time
 */
class Timer {
public:

    Timer(): last_tick(tick()) {}

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

#endif // TIMER_HPP

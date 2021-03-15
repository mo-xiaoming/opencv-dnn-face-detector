#pragma once

#include <chrono>

namespace utils {
struct Frame_counter {
    Frame_counter(int frame_range, void (*fps_func)(double fps)) : frame_range_(frame_range), fps_func_(fps_func) {}
    void tick();

private:
    int const frame_range_ = 100;
    int cur_frame_ = 0;
    void (*const fps_func_)(double) = nullptr;
    std::chrono::time_point<std::chrono::steady_clock> start_ = std::chrono::steady_clock::now();
};
} // namespace utils

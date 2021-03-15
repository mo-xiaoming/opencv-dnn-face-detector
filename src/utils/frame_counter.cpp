#include "frame_counter.hpp"

namespace utils {
void Frame_counter::tick() {
    if (++cur_frame_ == frame_range_) {
        auto const d = std::chrono::duration<double>(std::chrono::steady_clock::now() - start_).count();
        fps_func_(frame_range_ / d);
        cur_frame_ = 0;
        start_ = std::chrono::steady_clock::now();
    }
}

} // namespace utils

module;

#include <chrono>

#include "flick.hpp"

export module ttime;

namespace ttime {
  export using point = std::chrono::time_point<std::chrono::system_clock>;

  export fn to_epoch(const point& time) -> int64_t {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
      time.time_since_epoch()
    ).count();
  }

  export fn from_epoch(int64_t since) -> point {
    return point(std::chrono::milliseconds(since));
  }

  export fn now() -> point {
    return std::chrono::system_clock::now();
  }

  export fn diff(const point& start, const point& end) -> int64_t {
    return std::chrono::duration_cast<std::chrono::milliseconds>(start - end).count();
  }
}

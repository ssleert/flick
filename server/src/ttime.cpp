module;

#include <chrono>

#include "flick.hpp"

export module ttime;

namespace ttime {
  export using point = std::chrono::time_point<std::chrono::system_clock>;

  export fn to_epoch(point time) -> int64_t {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
      time.time_since_epoch()
    ).count();
  }

  export fn now() -> point {
    return std::chrono::system_clock::now();
  }
}

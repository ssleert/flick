module;

#include <chrono>

#include <nlohmann/json.hpp>

#include "flick.hpp"

export module ttime;

using json = nlohmann::json;

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

namespace nlohmann {
  export template<> class adl_serializer<ttime::point> {
    public:
      static fn to_json(json& j, const ttime::point& t) -> void {
        j = ttime::to_epoch(t);
      }

      static fn from_json(const json& j, ttime::point& t) -> void {
        t = ttime::from_epoch(j.template get<int64_t>());
      }
  };
}

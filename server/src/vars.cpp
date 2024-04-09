module;

#include <string>

#include "flick.hpp"

export module vars;

import get_env;

using get_env::getenv;
using namespace std::string_literals;

namespace vars {
  export {
    constexpr size_t map_prealloc_size = 32;
    constexpr size_t registration_strings_max_size = 256;
    constexpr size_t registration_strings_min_size = 7;
    constexpr const char* default_avatar_img = "default_avatar";
  }

  export class env {
    public:
      std::string pg_connection_string = getenv("PG_CONNECTION_STRING", "null"s);
      std::string global_pow = getenv("GLOBAL_POW", "null"s);
  };

  static bool inited_env = false;
  env env_instance;

  export fn getenv() -> env& {
    if (inited_env == false) {
      env_instance = env();
      inited_env = true;
    }
    return env_instance;
  }
}

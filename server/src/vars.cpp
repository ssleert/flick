module;

#include <string>

#include "flick.hpp"

export module vars;

import get_env;

using get_env::getenv;
using namespace std::literals;

namespace vars {
  export {
    constexpr size_t map_prealloc_size = 32;
    constexpr size_t registration_strings_max_size = 256;
    constexpr size_t registration_strings_min_size = 5;
    constexpr const char* default_avatar_img = "default_avatar";

    constexpr const char* auth_header_name = "HTTP_AUTH_TOKEN";
  }

  export class env {
    public:
      std::string pg_connection_string = getenv("PG_CONNECTION_STRING", ""s);
      std::string global_pow = getenv("GLOBAL_POW", "null"s);
      int64_t access_token_life_seconds = getenv("ACCESS_TOKEN_LIFE_SECONDS", int64_t(3600));
      int64_t refresh_token_life_seconds = getenv("REFRESH_TOKEN_LIFE_SECONDS", int64_t(1814400));
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

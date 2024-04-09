module;

#include <concepts>
#include <cstdlib>
#include <string>

#include "flick.hpp"

#include "logger_macro.hpp"

export module get_env;

import logger;

namespace get_env {
  template <typename T>
  concept Value = std::is_same_v<std::string, std::decay_t<T>> ||
                  std::is_integral_v<std::decay_t<T>> ||
                  std::is_floating_point_v<std::decay_t<T>>;

  export fn getenv(const char *var_name, Value auto default_value) -> std::decay_t<decltype(default_value)> {
    const char *tmp_env_var = std::getenv(var_name);
    if (tmp_env_var == nullptr) {
      return default_value;
    }

    const auto env_var = std::string(tmp_env_var);
    if (env_var.empty()) {
      return default_value;
    }

    if constexpr (std::is_same_v<std::string, std::decay_t<decltype(default_value)>>) {
      return env_var;
    } else if constexpr (std::is_floating_point_v<std::decay_t<decltype(default_value)>>) {
      return static_cast<decltype(default_value)>(
        std::stod(env_var)
      );
    } else if constexpr (std::is_integral_v<std::decay_t<decltype(default_value)>>) {
      return static_cast<decltype(default_value)>(
        std::stoll(env_var)
      );
    } else {
      static_assert(false, "Unsupported type.");
    }
  }
}

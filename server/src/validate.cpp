module;

#include <string_view>

#include "flick.hpp"

export module validate;

import vars;
import exceptions;

namespace validate {
  export template<
    class Function,
    class... Args
  > fn validate_iterable(Function func, Args&... args) -> void {
    for (const auto& e : {args...}) {
      if (func(e)) {
        throw exceptions::validation_error();
      }
    }
  }

  export fn registration_param(const std::string_view param) -> bool {
    const auto size = param.size();
    return (
      size > vars::registration_strings_max_size ||
      size < vars::registration_strings_min_size 
    );
  }
}

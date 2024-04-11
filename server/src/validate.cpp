module;

#include <string_view>
#include <algorithm>

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

  export fn registration_param_size(const std::string_view param) -> bool {
    const auto size = param.size();
    return (
      size > vars::registration_strings_max_size ||
      size < vars::registration_strings_min_size 
    );
  }

  export fn email(const std::string_view str) -> void {
    if (str.size() > 150) {
      throw exceptions::malformed_email();
    }

    const auto at = std::find(str.cbegin(), str.cend(), '@');
    const auto dot = std::find(at, str.cend(), '.');

    if((at == str.cend()) || (dot == str.cend())) {
      throw exceptions::malformed_email();
    }

    if (std::distance(str.cbegin(), at) < 1) {
      throw exceptions::malformed_email();
    }

    if(std::distance(at, str.cend()) < 5) {
      throw exceptions::malformed_email();
    }

    if (std::distance(at, dot) < 1) {
      throw exceptions::malformed_email();
    }
  }
}

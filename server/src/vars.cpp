module;

#include "flick.hpp"

export module vars;

namespace vars {
  export {
    constexpr size_t map_prealloc_size = 32;
    constexpr size_t registration_strings_max_size = 2048;
    constexpr size_t registration_strings_min_size = 7;
  }
}

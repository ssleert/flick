module;

#include <cstdio>
#include "flick.hpp"

export module printer;

namespace printer {
  export fn print() -> void {
    std::puts("from printer");
  }
}

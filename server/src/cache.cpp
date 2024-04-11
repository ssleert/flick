module;

#include <string>

#include "flick.hpp"

export module cache;

import ttime;

namespace cache {
  export class token {
    public:
      int32_t user_id;
      ttime::point date;
  };
}

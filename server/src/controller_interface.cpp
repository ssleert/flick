module;

#include <fcgiapp.h>

#include "flick.hpp"

export module controller_interface;

namespace controller_interface {
  export class controller {
    public:
      virtual fn operator()(FCGX_Request& req) -> void = 0;
  };
}

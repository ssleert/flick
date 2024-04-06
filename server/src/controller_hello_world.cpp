module;

#include <fcgiapp.h>

#include "logger_macro.hpp"
#include "flick.hpp"

export module controller_hello_world;

import logger;
import controller_interface;

namespace controller_hello_world {
  export class controller : public controller_interface::controller {
    public:
      fn operator()(FCGX_Request& req) -> void override {
        log_trace("hello world connected");

        FCGX_PutS("Content-type: text/html\r\n", req.out); 
        FCGX_PutS("\r\n", req.out); 
        FCGX_PutS("hello world\n", req.out);

        FCGX_Finish_r(&req);
      }
  };
}

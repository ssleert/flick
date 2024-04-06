module;

#include <fcgiapp.h>

#include <format>

#include "logger_macro.hpp"
#include "flick.hpp"

export module controller_register_user;

import logger;
import controller_interface;
import utils;

namespace controller_register_user {
  export template<
    class add_user_service
  > class controller : public controller_interface::controller {
      public:
        controller() 
          : add_user(add_user_service())
        {}

        fn operator()(FCGX_Request& req) -> void override {
          log_trace("register_user connected");

          const auto input_data = add_user_service::input_data(req);
          log_trace("input_data = {}", input_data.to_string());

          const auto output_data = add_user.invoke(input_data);

          utils::add_custom_headers(output_data.headers);
          utils::write_http_headers(req, output_data.headers);

          utils::write_bytes_to_http_request(output_data.to_bytes());
          utils::finish_http_request(req);
        }

      private:
        add_user_service add_user;
  };
}

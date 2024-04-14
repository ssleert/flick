module;

#include <fcgiapp.h>

#include <format>

#include "logger_macro.hpp"
#include "flick.hpp"

export module controller_create_post;

import logger;
import controller_interface;
import utils;
import http;
import exceptions;

namespace controller_create_post {
  export template<
    class service_create_post
  > class controller : public controller_interface::controller {
    private:
      service_create_post login_user;

    public:
      controller() 
        : login_user(service_create_post())
      {}

      fn operator()(FCGX_Request& req) -> void override {
        http::status status = http::status::ok;
        auto output_data = typename service_create_post::output(); 

        const auto handle_exception = [&](auto err) {
          output_data = typename service_create_post::output{
            .error = err.what()
          };
          status = err.status_code;
        };

        try {
          const auto input_data = service_create_post::input::from_request(req);
          output_data = this->login_user.invoke(std::move(input_data));
        } catch (const exceptions::wrapped_exception& err) {
          log_warn("wrapped_exception: {}", err.what());
          handle_exception(err);
        } catch(const std::exception& err) {
          log_warn("exception: {}", err.what());
          handle_exception(exceptions::wrapped_exception());
        } catch (...) {
          log_warn("unknown exception");
          handle_exception(exceptions::wrapped_exception());
        }

        utils::write_status(req, status);
        utils::write_http_headers(req, utils::custom_http_headers());
        utils::write_http_headers(req, service_create_post::output::headers());

        utils::start_http_body(req);
        utils::write_bytes_to_http_request(req, output_data.to_bytes());
        utils::finish_http_request(req);
      }
  };
}

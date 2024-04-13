
module;

#include <fcgiapp.h>

#include <format>

#include "logger_macro.hpp"
#include "flick.hpp"

export module controller_refresh_tokens;

import logger;
import controller_interface;
import utils;
import http;
import exceptions;

namespace controller_refresh_tokens {
  export template<
    class refresh_tokens_service
  > class controller : public controller_interface::controller {
    private:
      refresh_tokens_service login_user;

    public:
      controller() 
        : login_user(refresh_tokens_service())
      {}

      fn operator()(FCGX_Request& req) -> void override {
        http::status status = http::status::ok;
        auto output_data = typename refresh_tokens_service::output(); 

        const auto handle_exception = [&](auto err) {
          output_data = typename refresh_tokens_service::output{
            .error = err.what()
          };
          status = err.status_code;
        };

        try {
          const auto input_data = refresh_tokens_service::input::from_request(req);
          output_data = this->login_user.invoke(input_data);
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
        utils::write_http_headers(req, refresh_tokens_service::output::headers());

        utils::start_http_body(req);
        utils::write_bytes_to_http_request(req, output_data.to_bytes());
        utils::finish_http_request(req);
      }
  };
}

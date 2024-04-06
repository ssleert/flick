module;

#include <fcgiapp.h>

#include "logger_macro.hpp"
#include "flick.hpp"

#include <vector>
#include <string_view>

#include <nlohmann/json.hpp>

export module service_add_user;

import utils;
import logger;
import http;
import vars;
import validate;
import exceptions;

using json = nlohmann::json;
using namespace std::literals::string_view_literals;

namespace service_add_user {
  export class input {
    public:
      static fn from_request(FCGX_Request& req) -> input {
        const auto params = utils::get_url_params_from_request(req);

        const auto email    = utils::get_value_from_container(params, "email"sv);
        const auto username = utils::get_value_from_container(params, "username"sv);
        const auto password = utils::get_value_from_container(params, "password"sv);

        // TODO: enchance validation

        validate::validate_iterable(
          validate::registration_param,
          email, username, password
        );

        return input(email, username, password);
      }

      const std::string_view email;
      const std::string_view username;
      const std::string_view password;
  };

  export class output {
    public:
      std::string error = "null";

      fn to_bytes() const -> std::vector<uint8_t> {
        const json data = {
          {"error", error}
        };

        const auto str = data.dump();
        std::vector<uint8_t> vec(str.begin(), str.end());
        return vec;
      }

      static fn headers() -> std::vector<http::header> {
        return { 
          {"Content-type", "text/json"}
        };
      }
  };

  export template<
    class input_data,
    class output_data
  > class service {
    public:
      using input = input_data;
      using output = output_data;

      service() {
        log_trace("service_add_user created");
      }

      fn invoke(input data) -> output {
        log_trace("email = {}", data.email);
        log_trace("username = {}", data.username);
        log_trace("password = {}", data.password);
        return output();
      }
  };
}

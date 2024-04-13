module;

#include <fcgiapp.h>

#include "logger_macro.hpp"
#include "flick.hpp"

#include <vector>
#include <string>
#include <string_view>

#include <stdexcept>

#include <nlohmann/json.hpp>

export module service_refresh_tokens;

import utils;
import logger;
import http;
import ttime;
import vars;
import validate;
import crypto;
import exceptions;

using json = nlohmann::json;
using namespace std::literals::string_view_literals;

namespace service_refresh_tokens {
  export class input {
    public:
      static fn from_request(FCGX_Request& req) -> input {
        const auto params = utils::get_url_params_from_request(req);

        const auto access_token  = utils::get_value_from_container(params, "access_token"sv);
        const auto refresh_token = utils::get_value_from_container(params, "refresh_token"sv);

        validate::validate_iterable(
          validate::registration_param_size,
          access_token, refresh_token
        );

        return input(access_token, refresh_token);
      }

      const std::string_view access_token = "";
      const std::string_view refresh_token = "";
  };

  export class output {
    public:
      std::string error = "";
      std::string access_token = "";
      std::string refresh_token = "";

      fn to_bytes() const -> std::vector<uint8_t> {
        const json data = {
          {"error",         this->error},
          {"access_token",  this->access_token},
          {"refresh_token", this->refresh_token}
        };

        const auto bin = json::to_msgpack(data);
        return bin;
      }

      static fn headers() -> std::vector<http::header> {
        return { 
          {"Content-type", "application/msgpack"}
        };
      }
  };

  export template<
    class input_data,
    class output_data,
    class auth_impl
  > class service {
    private:
      auth_impl& auth;

    public:
      using input = input_data;
      using output = output_data;

      service()
        : auth(auth_impl::get_instance())
      {}

      fn invoke(const input& data) -> output { 
        const auto [
          access_token, 
          refresh_token
        ] = this->auth.regen_tokens(data.access_token, data.refresh_token);
        
        return output{
          .access_token = access_token,
          .refresh_token = refresh_token,
        };
      }
  };
}

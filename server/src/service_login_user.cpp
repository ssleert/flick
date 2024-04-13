module;

#include <fcgiapp.h>

#include "logger_macro.hpp"
#include "flick.hpp"

#include <vector>
#include <string>
#include <string_view>

#include <stdexcept>

#include <nlohmann/json.hpp>

export module service_login_user;

import utils;
import logger;
import http;
import ttime;
import db;
import vars;
import validate;
import crypto;
import exceptions;

using json = nlohmann::json;
using namespace std::literals::string_view_literals;

namespace service_login_user {
  export class input {
    public:
      static fn from_request(FCGX_Request& req) -> input {
        const auto params = utils::get_url_params_from_request(req);

        const auto http_user_agent = FCGX_GetParam("HTTP_USER_AGENT", req.envp);
        if (http_user_agent == nullptr) {
          throw exceptions::headers_incorrect();
        }

        const auto email      = utils::get_value_from_container(params, "email"sv);
        const auto password   = utils::get_value_from_container(params, "password"sv);
        const auto user_agent = std::string_view(http_user_agent);
        const auto ip_addr    = std::string_view(FCGX_GetParam("REMOTE_ADDR", req.envp));


        validate::validate_iterable(
          validate::registration_param_size,
          email, password, user_agent, ip_addr
        );

        validate::email(email);

        return input(email, password, user_agent, ip_addr);
      }

      const std::string_view email = "";
      const std::string_view password = "";
      const std::string_view user_agent = "";
      const std::string_view ip_addr = "";
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
    class db_impl,
    class auth_impl
  > class service {
    private:
      db_impl&   storage;
      auth_impl& auth;

    public:
      using input = input_data;
      using output = output_data;

      service()
        : storage(db_impl::get_instance()),
          auth(auth_impl::get_instance())
      {}

      fn invoke(const input& data) -> output { 
        const auto password_hash = crypto::hash_password(data.password);

        const auto user_id = this->storage.get_user_id_by_email_n_password_hash(data.email, password_hash); 
        if (!user_id.has_value()) {
          throw exceptions::user_not_found();
        }

        const auto [
          access_token, 
          refresh_token
        ] = this->auth.new_tokens(user_id.value(), data.ip_addr, data.user_agent);
        
        return output{
          .access_token = access_token,
          .refresh_token = refresh_token,
        };
      }
  };
}

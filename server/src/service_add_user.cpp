module;

#include <fcgiapp.h>

#include "logger_macro.hpp"
#include "flick.hpp"

#include <vector>
#include <string>
#include <string_view>

#include <nlohmann/json.hpp>

export module service_add_user;

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
      int32_t user_id = 0;

      fn to_bytes() const -> std::vector<uint8_t> {
        const json data = {
          {"error", error},
          {"user_id", user_id}
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
    class db_impl
  > class service {
    private:
      db_impl storage;

    public:
      using input = input_data;
      using output = output_data;

      service()
        : storage(db_impl())
      {}

      fn invoke(input data) -> output {
        log_trace("email = {}", data.email);
        log_trace("username = {}", data.username);
        log_trace("password = {}", data.password);

        const auto password_hash = crypto::hash_password(data.password);

        const auto user = db::user{
          .email = data.email,
          .password_hash = password_hash,
          .nickname = data.username,
          .avatar_img = vars::default_avatar_img,
          .creation_date = ttime::now()
        };

        const auto user_id = this->storage.add_user(std::move(user));

        return output{
          .user_id = user_id
        };
      }
  };
}

module;

#include <fcgiapp.h>

#include "logger_macro.hpp"
#include "flick.hpp"

#include <vector>
#include <string>
#include <string_view>

#include <stdexcept>

#include <nlohmann/json.hpp>

export module service_create_post;

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

namespace service_create_post {
  export class input {
    public:
      static fn from_request(FCGX_Request& req) -> input;

      std::string_view access_token;
      std::string_view body;
      std::vector<std::string_view> attachments;
      bool is_comments_disallowed;
  };

  NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(input, body, attachments, is_comments_disallowed);

  fn input::from_request(FCGX_Request& req) -> input {
    const auto access_token = utils::get_http_param(req, vars::auth_header_name);
    const auto body = utils::read_request_body(req);
    input input_data;

    try {
      json data = json::from_msgpack(body);
      input_data = data.template get<input>();
      input_data.access_token = access_token;
    } catch (const std::exception& err) {
      log_warn("msgpack unpack: {}", err.what());
      throw exceptions::validation_error();
    }

    return input_data;
  }

  export class output {
    public:
      std::string error = "";
      int32_t     post_id = 0;

      fn to_bytes() const -> std::vector<uint8_t> {
        const json data = {
          {"error",   this->error},
          {"post_id", this->post_id},
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
        const auto user_id = this->auth.get_user_id_from_access_token(data.access_token);

        const auto post_id = this->storage.create_post(
            db::input::post{
              .user_id = user_id,
              .creation_date = ttime::now(),
              .body = data.body,
              .attachments = data.attachments,
              .likes_amount = 0,
              .dislikes_amount = 0,
              .comments_amount = 0,
              .is_comments_disallowed = data.is_comments_disallowed,
            }
        );

        return output{
          .post_id = post_id,
        };
      }
  };
}

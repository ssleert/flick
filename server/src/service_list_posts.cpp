module;

#include <fcgiapp.h>

#include "logger_macro.hpp"
#include "flick.hpp"

#include <vector>
#include <string>
#include <string_view>

#include <stdexcept>

#include <nlohmann/json.hpp>

export module service_list_posts;

import utils;
import logger;
import http;
import ttime;
import vars;
import validate;
import db;
import crypto;
import exceptions;

using json = nlohmann::json;
using namespace std::literals::string_view_literals;

namespace service_list_posts {
  export class input {
    public:
      static fn from_request(FCGX_Request& req) -> input {
        const auto params = utils::get_url_params_from_request(req);

        const auto get_int32 = [&params](const auto idx) -> int32_t {
          return utils::get_number_from_string_view<int32_t>(
            utils::get_value_from_container(
              params, std::forward<decltype(idx)>(idx)
            )
          );
        };

        const int32_t offset  = get_int32("offset"sv);
        const int32_t limit   = get_int32("limit"sv);

        if ((offset < 0) || (limit < 1 || limit > 100)) {
          throw exceptions::validation_error();
        }

        return input("", offset, limit);
      }

      std::string_view access_token = "";
      int32_t offset = 0;
      int32_t limit = 0;
  };

  export class output {
    private:
      using post = db::output::post_n_user;

    public:
      std::vector<post> posts = {};
      std::string error = "";

      fn to_bytes() const -> std::vector<uint8_t> {
        const json data = {
          {"posts", this->posts},
          {"error", this->error}
        };
 
        const auto bin = json::to_msgpack(data);
        return bin;
      }

      static constexpr fn headers() -> const std::vector<http::header> {
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
      db_impl& storage;
      auth_impl& auth;

    public:
      using input = input_data;
      using output = output_data;

      service()
        : storage(db_impl::get_instance()),
          auth(auth_impl::get_instance())
      {}

      fn invoke(const input& data) -> output { 
        log_trace("list_posts service invkoke");
        auth.is_valid_access_token(data.access_token);

        const auto posts = this->storage.list_posts_n_users(data.offset, data.limit);
        return output{
          .posts = std::move(posts),
        };
      }
  };
}

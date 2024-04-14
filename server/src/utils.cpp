module;

#include <fcgiapp.h>

#include <string>
#include <charconv>
#include <string_view>
#include <format>
#include <vector>
#include <unordered_map>
#include <type_traits>


#include "flick.hpp"
#include "logger_macro.hpp"

export module utils;

import exceptions;
import vars;
import http;
import logger;

namespace utils {
  export constexpr fn custom_http_headers() -> const std::vector<http::header> {
    return {
      {"x-powered-by", "flick/C++23"}
    };
  }

  export fn write_string_to_http_request(
    FCGX_Request& req, 
    const std::string_view data
  ) noexcept -> void {
    // TODO: add exception on error
    FCGX_PutStr(data.data(), data.size(), req.out);
  }

  export fn write_http_header(
    FCGX_Request& req,
    const http::header& header
  ) -> void {
    write_string_to_http_request(req, header.to_string());
  }

  export fn write_http_headers(
    FCGX_Request& req,
    const std::vector<http::header>& http_headers
  ) noexcept -> void {
    for (const auto& header : http_headers) {
      write_http_header(req, header);
    }
  }

  export fn write_bytes_to_http_request(
    FCGX_Request& req, 
    const std::vector<uint8_t>& data
  ) noexcept -> void {
    // TODO: add exception on error
    FCGX_PutStr(reinterpret_cast<const char*>(data.data()), data.size(), req.out);
  }

  export fn start_http_body(
    FCGX_Request& req
  ) noexcept -> void {
    FCGX_PutStr("\n", 1, req.out);
  }

  export fn finish_http_request(FCGX_Request& req) -> void {
    // TODO: add exception on error
    FCGX_Finish_r(&req);
  }

  export fn get_url_params(
    const std::string_view query_string
  ) -> std::unordered_map<std::string_view, std::string_view> {
    if (query_string.size() < 3) {
      throw exceptions::malformed_query_string();
    }

    // TODO: add max query_string max size check

    std::unordered_map<std::string_view, std::string_view> result;

    // TODO: from vars module
    result.reserve(vars::map_prealloc_size);

    size_t pos = 0;
    while (pos < query_string.size()) {
      size_t ampersand_pos = query_string.find('&', pos);
      if (ampersand_pos == std::string_view::npos) {
        ampersand_pos = query_string.size();
      }

      const size_t equals_pos = query_string.find('=', pos);
      if (equals_pos == std::string_view::npos || equals_pos > ampersand_pos) {
        log_warn("incorrect url params format");
        throw exceptions::malformed_query_string();
      }

      const std::string_view key = query_string.substr(pos, equals_pos - pos);
      const std::string_view value = query_string.substr(equals_pos + 1, ampersand_pos - equals_pos - 1);

      result[key] = value;

      pos = ampersand_pos + 1;
    }

    return result;
  }

  export fn get_url_params_from_request(
    FCGX_Request& req
  ) -> decltype(get_url_params(std::string_view(""))) {
    return get_url_params(FCGX_GetParam("QUERY_STRING", req.envp));
  }

  export fn write_not_found(
    FCGX_Request& req
  ) -> void {
    utils::write_http_header(req,
      http::header("Status", "404")
    );
    utils::start_http_body(req);
    utils::finish_http_request(req);
  }

  export fn write_method_not_allowed(
    FCGX_Request& req
  ) -> void {
    utils::write_http_header(req,
      http::header("Status", "405")
    );
    utils::start_http_body(req);
    utils::finish_http_request(req);
  }

  export fn write_status(
    FCGX_Request& req, 
    http::status status
  ) -> void {
    utils::write_http_header(req,
      http::header("Status", std::to_string(
        static_cast<int16_t>(status)
      ).c_str())
    );
  }

  export template<
    class Container,
    class Value
  > fn get_value_from_container(
    Container& obj,
    Value&& val
  ) -> const decltype(obj.at(val)) {
    if (obj.contains(val)) {
      return obj.at(val);
    }
    log_warn("container doesnt have such key");
    throw exceptions::validation_error();
  }

  export fn get_http_param(const FCGX_Request& req, const char* key) -> std::string_view {
    const auto param = FCGX_GetParam(key, req.envp);
    if (param == nullptr) {
      log_warn("http header is empty");
      throw exceptions::validation_error();
    }

    return std::string_view(param);
  }

  export fn get_http_param(const FCGX_Request& req, const std::string& key) -> std::string_view {
    return get_http_param(req, key.c_str());
  }


  export template<class Number> fn get_number_from_string_view(const std::string_view str) -> Number {
    Number result = 0;
    const auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result);
    if (ec != std::errc()) {
      log_warn("incorrect number in string");
      throw exceptions::validation_error();
    }
    
    return result;
  }

  export fn read_request_body(const FCGX_Request& req) -> std::vector<uint8_t> {
    const auto content_length = get_http_param(req, "HTTP_CONTENT_LENGTH");
    const auto input_data_size = get_number_from_string_view<size_t>(content_length);

    auto input_data = std::vector<uint8_t>(input_data_size);
    
    const auto success = FCGX_GetStr(reinterpret_cast<char*>(input_data.data()), input_data_size, req.in);
    if (success < 0) {
      log_warn("request body error");
      throw exceptions::headers_incorrect();
    }

    return input_data;
  }
}

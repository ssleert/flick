module;

#include <fcgiapp.h>

#include <string>
#include <string_view>
#include <format>
#include <vector>
#include <unordered_map>
#include <type_traits>

#include "flick.hpp"

export module utils;

import exceptions;
import vars;
import http;

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
    throw exceptions::validation_error();
  }
}

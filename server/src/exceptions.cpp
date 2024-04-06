module;

#include <stdexcept>
#include <exception>

export module exceptions;

import http;

namespace exceptions {
  export class wrapped_exception : public std::runtime_error {
    public:  
      wrapped_exception(const char* what_arg, http::status status_code)
        : std::runtime_error(what_arg), status_code(status_code)
      {}

      wrapped_exception(const std::exception& error)
        : std::runtime_error(error.what()),
          status_code(http::status::internal_server_error)
      {}

      wrapped_exception()
        : std::runtime_error("internal server error"),
          status_code(http::status::internal_server_error)
      {}

      virtual ~wrapped_exception() = default;
      http::status status_code;
  };

  export class malformed_query_string : public wrapped_exception {
    public:
      malformed_query_string()
        : wrapped_exception(
            "malformed query string", 
            http::status::unprocessable_entity
          )
      {}
  };

  export class validation_error : public wrapped_exception {
    public:
      validation_error()
        : wrapped_exception(
            "validation error", 
            http::status::unprocessable_entity
          )
      {}
  };
}

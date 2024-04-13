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

      wrapped_exception(const char* what_arg)
        : std::runtime_error(what_arg),
          status_code(http::status::internal_server_error)
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

  export class malformed_email : public wrapped_exception {
    public:
      malformed_email()
        : wrapped_exception(
            "malformed email", 
            http::status::unprocessable_entity
          )
      {}
  };

  export class db_error : public wrapped_exception {
    public:
      db_error()
        : wrapped_exception(
            "database error"
          )
      {}
  };

  export class access_token_is_not_expired : public wrapped_exception {
    public:
      access_token_is_not_expired()
        : wrapped_exception(
            "access token is not expired"
          )
      {}
  };

  export class access_token_expired : public wrapped_exception {
    public:
      access_token_expired()
        : wrapped_exception(
            "access token expired"
          )
      {}
  };

  export class access_token_incorrect : public wrapped_exception {
    public:
      access_token_incorrect()
        : wrapped_exception(
            "access token incorrect"
          )
      {}
  };

  export class refresh_token_expired : public wrapped_exception {
    public:
      refresh_token_expired()
        : wrapped_exception(
            "refresh token expired"
          )
      {}
  };

  export class user_not_found : public wrapped_exception {
    public:
      user_not_found()
        : wrapped_exception(
            "user not found",
            http::status::not_found
          )
      {}
  };

  export class headers_incorrect : public wrapped_exception {
    public:
      headers_incorrect()
        : wrapped_exception(
            "headers incorrect",
            http::status::unprocessable_entity
          )
      {}
  };

  export class auth_headers_incorrect : public wrapped_exception {
    public:
      auth_headers_incorrect()
        : wrapped_exception(
            "auth headers incorrect",
            http::status::unprocessable_entity
          )
      {}
  };
}

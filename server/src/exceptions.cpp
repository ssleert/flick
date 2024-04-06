module;

#include <stdexcept>

export module exceptions;

import utils;

namespace exceptions {
  export class wrapped_exception : public std::runtime_error {
    public:  
      wrapped_exception(const char* what_arg, utils::http_status status_code)
        : std::runtime_error(what_arg), status_code(status_code)
      {}
      ~wrapped_exception() {}

      utils::http_status status_code;
  };
}

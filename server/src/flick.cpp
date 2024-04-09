#include <exception>

#include "flick.hpp"
#include "logger_macro.hpp"

import logger;
import server;

fn main() -> int32_t {
  log_info("flick server started");

  try {
    server::start();
  } catch (std::exception& err) {
    log_error("UNHANDLED EXCEPTION");
    log_error("{}", err.what());
    throw;
  }

  return 0;
}

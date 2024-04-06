#include <fcgiapp.h>

#include <exception>
#include <thread>
#include <string>
#include <utility>
#include <vector>
#include <csignal>
#include <functional>

#include "flick.hpp"
#include "logger_macro.hpp"

import logger;
import fcgi_server;
import http_router;
import routes;

fn main() -> int32_t {
  log_info("flick server started");

  const auto workers_amount = static_cast<int32_t>(std::thread::hardware_concurrency());

  auto router = http_router::router(routes::routes);
  auto server_instance = fcgi_server::server(
    workers_amount, 30, router
  );

  try {
    server_instance.start("0.0.0.0:9000");
  } catch (std::exception& err) {
    log_error("UNHANDLED EXCEPTION");
    log_fatal("{}", err.what());
  }

  return 0;
}

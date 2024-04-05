#include <exception>
#include <thread>
#include <string>
#include <utility>
#include <vector>

#include <fcgiapp.h>

#include "flick.hpp"
#include "logger_macro.hpp"

import printer;
import logger;
import fcgi_server;
import http_router;
import controller_interface;

using namespace std::string_literals;

class page_first : public controller_interface::controller {
  public:
    void operator()(FCGX_Request& req) {
      log_trace("http connect");

      FCGX_PutS("Content-type: text/html\r\n", req.out); 
      FCGX_PutS("\r\n", req.out); 
      FCGX_PutS("first page\n", req.out);

      FCGX_Finish_r(&req);
    }
};

class page_second : public controller_interface::controller {
  public:
    void operator()(FCGX_Request& req) {
      log_trace("http connect");

      FCGX_PutS("Content-type: text/html\r\n", req.out); 
      FCGX_PutS("\r\n", req.out); 
      FCGX_PutS("second page\n", req.out);

      FCGX_Finish_r(&req);
    }
};

fn main() -> int32_t {
  log_info("flick server started");

  const auto workers_amount = static_cast<int32_t>(std::thread::hardware_concurrency());

  auto p1 = page_first();
  auto p2 = page_second();
  
  std::vector<std::pair<std::string, controller_interface::controller&>> routes = {
    {"/first"s, p1},
    {"/second"s, p2},
  };

  auto router = http_router::router(routes);
  auto server_instance = fcgi_server::server(
    workers_amount, 20, router
  );

  try {
    server_instance.start("0.0.0.0:9000");
  } catch (std::exception& err) {
    log_error("UNHANDLED EXCEPTION");
    log_fatal("{}", err.what());
  }

  return 0;
}

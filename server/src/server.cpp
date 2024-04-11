module;

#include <string>
#include <vector>
#include <thread>

#include "flick.hpp"

export module server;

import fcgi_server;
import http_router;
import db_pg;

import controller_interface;
import controller_hello_world;

import controller_register_user;
import service_add_user;

import controller_login_user;
import service_login_user;

namespace server {
  export fn start() -> void {
    const auto workers_amount = static_cast<int32_t>(std::thread::hardware_concurrency());

    auto hello_world = controller_hello_world::controller();
    auto register_user = controller_register_user::controller<
      service_add_user::service<
        service_add_user::input,
        service_add_user::output,
        db_pg::database
      >
    >();

    auto login_user = controller_login_user::controller<
      service_login_user::service<
        service_login_user::input,
        service_login_user::output,
        db_pg::database
      >
    >();

    std::vector<http_router::route<controller_interface::controller&>> routes = {
      {"/api/hello_world",   "GET",  hello_world},
      {"/api/register_user", "POST", register_user},
      {"/api/login_user",    "POST", login_user},
    };

    auto router = http_router::router(routes);
    auto server_instance = fcgi_server::server(
      workers_amount, 30, router
    );

    server_instance.start("/run/sockets/fastcgi.sock");
  }

}

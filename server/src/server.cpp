module;

#include <string>
#include <vector>
#include <thread>

#include "flick.hpp"

export module server;

import fcgi_server;
import http_router;
import db_pg;
import auth;
import cache_basic;

import controller_interface;
import controller_hello_world;

import controller_register_user;
import service_add_user;

import controller_login_user;
import service_login_user;

import controller_refresh_tokens;
import service_refresh_tokens;

import controller_create_post;
import service_create_post;

import controller_list_posts;
import service_list_posts;

namespace server {
  export fn start() -> void {
    const auto workers_amount = static_cast<int32_t>(std::thread::hardware_concurrency());

    using db_impl = db_pg::database;
    using cache_impl = cache_basic::cache_basic;
    using auth_impl = auth::provider<db_impl, cache_impl>;

    auto hello_world = controller_hello_world::controller();
    auto register_user = controller_register_user::controller<
      service_add_user::service<
        service_add_user::input,
        service_add_user::output,
        db_impl
      >
    >();

    auto login_user = controller_login_user::controller<
      service_login_user::service<
        service_login_user::input,
        service_login_user::output,
        db_impl,
        auth_impl
      >
    >();

    auto refresh_tokens = controller_refresh_tokens::controller<
      service_refresh_tokens::service<
        service_refresh_tokens::input,
        service_refresh_tokens::output,
        auth_impl
      >
    >();

    auto create_post = controller_create_post::controller<
      service_create_post::service<
        service_create_post::input,
        service_create_post::output,
        db_impl,
        auth_impl
      >
    >();

    auto list_posts = controller_list_posts::controller<
      service_list_posts::service<
        service_list_posts::input,
        service_list_posts::output,
        db_impl,
        auth_impl
      >
    >();

    std::vector<http_router::route<controller_interface::controller&>> routes = {
      {"/api/v1/hello_world",    "GET",  hello_world},
      {"/api/v1/register_user",  "POST", register_user},
      {"/api/v1/login_user",     "POST", login_user},
      {"/api/v1/refresh_tokens", "POST", refresh_tokens},
      {"/api/v1/create_post",    "POST", create_post},
      {"/api/v1/list_posts",     "GET",  list_posts},
    };

    auto router = http_router::router(routes);
    auto server_instance = fcgi_server::server(
      workers_amount, 30, router
    );

    server_instance.start("/run/sockets/fastcgi.sock");
  }

}

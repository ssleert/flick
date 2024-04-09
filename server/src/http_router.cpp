module;

#include <fcgiapp.h>

#include <vector>
#include <string>
#include <string_view>
#include <array>
#include <tuple>

#include "logger_macro.hpp"
#include "flick.hpp"

export module http_router;

import logger;
import utils;
import http;

namespace http_router {
  export template<class Controller> class route {
    public:
      std::string path;
      std::string method;
      Controller controller;
  };

  export template<class RouteHandler> class router {
    public:
      router(const std::vector<route<RouteHandler>>& routes) 
        : routes(routes)
      {}

      fn operator()(FCGX_Request& req) -> void { 
        const auto document_uri = std::string_view(FCGX_GetParam("DOCUMENT_URI", req.envp)); 
        size_t matched_amount = 0;
        const route<RouteHandler> * matched_route = &routes[0];
        for (const auto& route : routes) {
          size_t i = 0;
          size_t current_matched_amount = 0;
          for (const char& ch : route.path) {
            if (ch == document_uri[i]) {
              ++current_matched_amount;
            }
            ++i;
            if (i > document_uri.size()) {
              break;
            }
          }
          if (current_matched_amount > matched_amount) {
            matched_amount = current_matched_amount;
            matched_route = &route;
          }
        }


        if (matched_route->path != document_uri) {
          utils::write_not_found(req);
          return;
        }

        if (matched_route->method != FCGX_GetParam("REQUEST_METHOD", req.envp)) {
          utils::write_method_not_allowed(req);
          return;
        }

        matched_route->controller(req);
      }
    private:
      std::vector<route<RouteHandler>> routes;
  };
}

module;

#include <fcgiapp.h>

#include <vector>
#include <utility>
#include <string>
#include <string_view>
#include <array>

#include "logger_macro.hpp"
#include "flick.hpp"

export module http_router;

import logger;

namespace http_router {
  export template<class RouteHandler> class router {
    public:
      router(const std::vector<std::pair<std::string, RouteHandler>>& routes) 
        : routes(routes)
      {}


      fn operator()(FCGX_Request& req) -> void { 
        const auto document_uri = std::string_view(FCGX_GetParam("REQUEST_URI", req.envp)); 

        log_trace("{}", document_uri);

        size_t matched_amount = 0;
        const std::pair<std::string, RouteHandler> * matched_route = &routes[0];
        for (const auto& route : routes) {
          size_t i = 0;
          size_t current_matched_amount = 0;
          for (const char& ch : route.first) {
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

        matched_route->second(req);
      }
    private:
      std::vector<std::pair<std::string, RouteHandler>> routes;
  };
}

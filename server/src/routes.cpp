module;

#include <string>
#include <vector>

export module routes;

import controller_interface;
import controller_hello_world;
import controller_register_user;

namespace routes {
  auto hello_world = controller_hello_world::controller();

  auto register_user = controller_register_user::controller();

  export std::vector<std::pair<std::string, controller_interface::controller&>> routes = {
    {"/api/hello_world", hello_world},
    {"/api/register_user", register_user},
  };
}

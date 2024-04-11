module;

#include <string>

#include "flick.hpp"

export module db;

import ttime;

namespace db {
  export class user {
    public:
      std::string_view email;
      std::string_view password_hash;
      std::string_view nickname;
      std::string_view avatar_img;
      ttime::point     creation_date;
  };

  export class token {
    public:
      int32_t          user_id;
      std::string_view refresh_token;
      std::string_view ip_addr;
      std::string_view user_agent;
      ttime::point     token_date;
  };

  export class token_id_n_date {
    public:
      int32_t token_id;
      ttime::point token_date;
  };
}

module;

#include <string_view>
#include <vector>

#include "flick.hpp"

export module db;

import ttime;

namespace db {
  export namespace input {
    class user {
      public:
        std::string_view email;
        std::string_view password_hash;
        std::string_view nickname;
        std::string_view avatar_img;
        ttime::point     creation_date;
    };

    class token {
      public:
        int32_t          user_id;
        std::string_view refresh_token;
        std::string_view ip_addr;
        std::string_view user_agent;
        ttime::point     token_date;
    };

    class post {
      public:
        int32_t                       user_id;
        ttime::point                  creation_date;
        std::string_view              body;
        std::vector<std::string_view> attachments;
        int32_t                       likes_amount;
        int32_t                       dislikes_amount;
        int32_t                       comments_amount;
        bool                          is_comments_disallowed;
    };
  }

  export namespace output {
    class token_id_n_date {
      public:
        int32_t token_id;
        ttime::point token_date;
    };
  }
}

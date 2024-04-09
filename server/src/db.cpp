module;

#include <string>

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
}

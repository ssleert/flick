module;

#include <xxhash.h>

#include <string>
#include <string_view>

#include "flick.hpp"

export module crypto;

import vars;

namespace crypto {
  constexpr uint64_t seed = 20070707;
  export fn get_xxhash(const std::string_view buf) -> uint64_t {
    return XXH64(buf.data(), buf.size(), seed);
  }

  export fn get_xxhash_str(const std::string_view buf) -> std::string {
    return std::to_string(get_xxhash(std::move(buf)));
  }

  export fn hash_password(const std::string_view pass) -> std::string {
    const auto password_n_hash = vars::getenv().global_pow + std::string(pass);

    return get_xxhash_str(password_n_hash);
  }

  export fn compare_password_hashes(
    const std::string_view pass, 
    const std::string_view hash
  ) -> bool { 
    const auto password_n_hash = vars::getenv().global_pow + std::string(pass);

    return password_n_hash == hash;
  }
}

module;

#include <xxhash.h>

#include <string>
#include <vector>
#include <random>
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

  // TODO: rewrite normally
  export fn random_chars(size_t len) -> std::vector<char> {
    auto rd = std::random_device();
    auto gen = std::mt19937(rd());
    auto dist = std::uniform_int_distribution<char>(-127, 127);

    auto seq = std::vector<char>(len);
    for (size_t i = 0; i < len; ++i) {
        seq[i] = dist(gen); // Generate a random byte
    }

    return seq;
  }

  export fn random_xxhash_str() -> std::string {
    auto bytes = random_chars(384);
    return get_xxhash_str(std::string_view(bytes.data(), bytes.size()));
  }
}

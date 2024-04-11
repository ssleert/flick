module;

#include <unordered_map>
#include <string_view>
#include <string>
#include <mutex>
#include <any>
#include <optional>

#include "flick.hpp"

export module cache_basic;

import vars;
import cache;

using namespace std::string_literals;

namespace cache_basic {
  export class cache_basic {
    private:
      std::mutex storage_mtx;
      std::unordered_map<std::string, std::any> storage;

      static constexpr auto token_prefix = "access_tokens/"s;

    public:
      static fn get_instance() -> cache_basic& {
        static cache_basic instance;
        return instance;
      }

      cache_basic() {
        this->storage.reserve(vars::map_prealloc_size);
      }
      cache_basic(const cache_basic& obj) = delete;

      fn set_token(const std::string_view key, const cache::token&& val) -> void {
        std::scoped_lock lock(this->storage_mtx);
        this->storage[token_prefix + std::string(key)] = std::make_any<cache::token>(val);
      }

      fn del_token(const std::string_view key) -> void {
        std::scoped_lock lock(this->storage_mtx);
        this->storage.erase(token_prefix + std::string(key));
      }

      fn get_token(const std::string_view key) -> cache::token {
        std::scoped_lock lock(this->storage_mtx);
        return std::any_cast<cache::token>(
          this->storage.at(token_prefix + std::string(key))
        );
      }
  };
}

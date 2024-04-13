module;

#include <string>
#include <string_view>
#include <utility>
#include <chrono>
#include <stdexcept>

#include "flick.hpp"
#include "logger_macro.hpp"

export module auth;

import crypto;
import ttime;
import cache;
import vars;
import db;
import exceptions;
import logger;

namespace auth {
  export template<
    class db_impl,
    class cache_impl
  > class provider {
    private:
      db_impl& storage;
      cache_impl& temp;

      static fn gen_tokens_pair() -> std::pair<std::string, std::string> {
        return {
          crypto::random_xxhash_str(),
          // TODO: rewrite refresh token gen with uuid
          crypto::random_xxhash_str(),
        };
      }

    public:
      static fn get_instance() -> provider& {
        static provider instance;
        return instance;
      }

      provider()
        : storage(db_impl::get_instance()),
          temp(cache_impl::get_instance())
      {}

      fn new_tokens(
        int32_t user_id,
        const std::string_view ip_addr,
        const std::string_view user_agent
      ) -> std::pair<std::string, std::string> {
        const auto tokens = gen_tokens_pair();

        const auto& [access_token, refresh_token] = tokens; 
        const auto time_now = ttime::now();

        this->temp.set_token(access_token, cache::token{user_id, time_now});
        this->storage.add_token(db::token{
          .user_id = user_id,
          .refresh_token = refresh_token,
          .ip_addr = ip_addr,
          .user_agent = user_agent,
          .token_date = time_now
        });

        return tokens;
      }

      fn regen_tokens(
        const std::string_view access_token,
        const std::string_view refresh_token
      ) -> std::pair<std::string, std::string> {
        const auto time_now = ttime::now();

        int32_t user_id = 0;
        try {
          const auto [token_user_id, token_date] = this->temp.get_token(access_token);
          
          user_id = token_user_id;

          const auto time_diff = ttime::diff(time_now, token_date) / 1000;

          if (time_diff <= vars::getenv().access_token_life_seconds) {
            throw exceptions::access_token_is_not_expired();
          }

          this->temp.del_token(access_token);
        } catch (const exceptions::access_token_is_not_expired& _) {
          throw;
        } catch (const std::out_of_range& _) {
          log_warn("access not found");
        } catch (const std::exception& err) {
          log_warn("auth: {}", err.what());
          throw exceptions::wrapped_exception();
        }

        const auto [
          refresh_token_id, 
          refresh_token_date
        ] = this->storage.find_token_id_n_date_by_token(refresh_token);

        const auto time_diff = ttime::diff(time_now, refresh_token_date) / 1000;

        if (time_diff >= vars::getenv().refresh_token_life_seconds) {
          throw exceptions::refresh_token_expired();
        }

        const auto tokens = gen_tokens_pair();
        const auto& [new_access_token, new_refresh_token] = tokens;

        this->storage.update_token_by_id(refresh_token_id, new_refresh_token, time_now);
        this->temp.set_token(new_access_token, cache::token{user_id, time_now});

        return tokens;
      }

      fn is_valid_access_token(const std::string_view access_token) -> bool {
        const auto& [user_id, token_date] = this->temp.get_token(access_token);
        const auto time_now = ttime::now();

        const auto time_diff = ttime::diff(time_now, token_date) / 1000;

        if (time_diff >= vars::getenv().access_token_life_seconds) {
          return false;
        }
         
        return true;
      }
  };
}

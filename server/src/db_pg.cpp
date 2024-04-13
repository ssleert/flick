module;

#include <pqxx/pqxx>

#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>
#include <optional>

#include "flick.hpp"
#include "defer.hpp"
#include "logger_macro.hpp"

export module db_pg;

import db;
import vars;
import exceptions;
import logger;
import ttime;

namespace db_pg {
  template<class ConnectionInit>
  class connection_pool {
    private:
      std::mutex connections_mtx;
      std::condition_variable connection_added;
      std::queue<std::unique_ptr<pqxx::connection>> connections;

    public:
      connection_pool(
        const char* connection_string,
        size_t amount_of_connections,
        ConnectionInit init_func
      ) {
        for (size_t i = 0; i < amount_of_connections; ++i) {
          auto conn = std::make_unique<pqxx::connection>(
            connection_string    
          );

          init_func(*conn);

          this->connections.push(std::move(conn));
        }
      }

      fn get() -> std::unique_ptr<pqxx::connection> {
        auto lock = std::unique_lock(this->connections_mtx);
        this->connection_added.wait(lock, [this]{
          return !this->connections.empty();
        });

        auto conn = std::move(this->connections.front());
        this->connections.pop();

        return conn;
      }

      fn put(std::unique_ptr<pqxx::connection>& conn) -> void {
        {
          std::scoped_lock lock(this->connections_mtx);
          this->connections.push(std::move(conn));
        }

        this->connection_added.notify_one();
      }
  };

  export class database {
    static constexpr size_t connection_pool_size = 4;

    static constexpr const char* add_user_stmt = "add_user";
    static constexpr const char* add_token_stmt = "add_token";
    static constexpr const char* find_token_id_n_date_by_token_stmt = "find_token_id_n_date_by_token";
    static constexpr const char* update_token_by_id_stmt = "update_token_by_id";
    static constexpr const char* get_user_id_by_email_n_password_hash_stmt = "get_user_id_by_email_n_password_hash";

    static fn prepare_add_user(pqxx::connection& c) -> void {
      c.prepare(add_user_stmt, R"(
        INSERT INTO users (
          email, password_hash, 
          nickname, avatar_img, creation_date
        )
        VALUES ($1, $2, $3, $4, $5)
        RETURNING user_id
      )");
    }

    static fn prepare_add_token(pqxx::connection& c) -> void {
      c.prepare(add_token_stmt, R"(
        INSERT INTO users_tokens (
          user_id, refresh_token, ip_addr, 
          user_agent, token_date
        )
        VALUES ($1, $2, $3, $4, $5)
        RETURNING token_id
      )");
    }

    static fn prepare_find_token_id_n_date_by_token(pqxx::connection& c) -> void {
      c.prepare(find_token_id_n_date_by_token_stmt, R"(
        SELECT token_id, token_date
        FROM users_tokens
        WHERE refresh_token = $1
      )");
    }

    static fn prepare_update_token_by_id(pqxx::connection& c) -> void {
      c.prepare(update_token_by_id_stmt, R"(
        UPDATE users_tokens
        SET refresh_token = $2,
            token_date = $3
        WHERE token_id = $1
      )");
    }

    static fn prepare_get_user_id_by_email_n_password_hash(pqxx::connection& c) -> void {
      c.prepare(get_user_id_by_email_n_password_hash_stmt, R"(
        SELECT user_id
        FROM users
        WHERE email = $1 AND password_hash = $2
      )");
    }

    static fn init_connection(pqxx::connection& c) -> void {
      prepare_add_user(c);
      prepare_add_token(c);
      prepare_find_token_id_n_date_by_token(c);
      prepare_update_token_by_id(c);
      prepare_get_user_id_by_email_n_password_hash(c);
    }

    private:
      connection_pool<decltype(init_connection)> pool; 

    public:
      static fn get_instance() -> database& {
        static database instance;
        return instance;
      }

      database() 
        : pool(connection_pool<decltype(init_connection)>(
            vars::getenv().pg_connection_string.c_str(),
            connection_pool_size,
            init_connection
          ))
      {}

      fn add_user(const db::user& user) -> int32_t {
        try {
          auto c = this->pool.get();
          defer (this->pool.put(c));

          auto t = pqxx::work(*c);

          auto user_id = t.exec_prepared1(add_user_stmt,
              user.email,
              user.password_hash,
              user.nickname,
              user.avatar_img,
              ttime::to_epoch(user.creation_date)
          ).at(0).as<int32_t>();

          t.commit();

          return user_id;
        } catch (const std::exception& err) {
          log_warn("db: {}", err.what());
          throw exceptions::db_error();
        }
      }

      fn add_token(const db::token& token) -> int32_t {
        try {
          auto c = this->pool.get();
          defer (this->pool.put(c));

          auto t = pqxx::work(*c);

          const auto token_id = t.exec_prepared1(add_token_stmt,
              token.user_id,
              token.refresh_token,
              token.ip_addr,
              token.user_agent,
              ttime::to_epoch(token.token_date)
          ).at(0).as<int32_t>();

          t.commit();

          return token_id;
        } catch (const std::exception& err) {
          log_warn("db: {}", err.what());
          throw exceptions::db_error();
        }
      }

      fn find_token_id_n_date_by_token(const std::string_view token) -> db::token_id_n_date { 
        try {
          auto c = this->pool.get();
          defer (this->pool.put(c));

          auto t = pqxx::work(*c);

          const auto [id, date_in_ms] = t.exec_prepared1(
            find_token_id_n_date_by_token_stmt, 
            token
          ).as<int32_t, int64_t>();

          t.commit();

          return {id, ttime::from_epoch(date_in_ms)};
        } catch (const std::exception& err) {
          log_warn("db: {}", err.what());
          throw exceptions::db_error();
        }
      }

      fn update_token_by_id(
        int32_t token_id, 
        const std::string_view refresh_token,
        const ttime::point& token_date
      ) -> void {
        try {
          auto c = this->pool.get();
          defer (this->pool.put(c));

          auto t = pqxx::work(*c);

          t.exec_prepared0(update_token_by_id_stmt,
              token_id,
              refresh_token,
              ttime::to_epoch(token_date)
          );

          t.commit();
        } catch (const std::exception& err) {
          log_warn("db: {}", err.what());
          throw exceptions::db_error();
        }
      }

      fn get_user_id_by_email_n_password_hash(
        const std::string_view email,
        const std::string_view password_hash
      ) -> std::optional<int32_t> {
        try {
          auto c = this->pool.get();
          defer (this->pool.put(c));

          auto t = pqxx::work(*c);

          int32_t user_id = 0;
          try {
            user_id = t.exec_prepared1(get_user_id_by_email_n_password_hash_stmt,
                email, password_hash
            ).at(0).as<int32_t>(); 
          } catch (const pqxx::unexpected_rows& err) {
            t.abort();
            return std::nullopt;
          }

          t.commit();

          return user_id;
        } catch (const std::exception& err) {
          log_warn("db: {}", err.what());
          throw exceptions::db_error();
        }
      }
  };
}

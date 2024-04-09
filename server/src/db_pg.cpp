module;

#include <pqxx/pqxx>

#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>

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

    static fn init_connection(pqxx::connection& c) -> void {
      prepare_add_user(c);
    }

    private:
      connection_pool<decltype(init_connection)> pool; 

    public:
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
  };
}

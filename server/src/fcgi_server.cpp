module;

#include <fcgiapp.h>

#include <thread>
#include <vector>
#include <stdexcept>
#include <atomic>
#include <mutex>
#include <chrono>

#include "logger_macro.hpp"
#include "flick.hpp"


export module fcgi_server;

import logger;

namespace fcgi_server {
  static bool libfcgi_inited = false;
  export template<class Function> class server {
    public:
      server(int32_t workers, int32_t backlog_size, Function callback) 
        : workers_amount(workers), backlog_size(backlog_size), callback(callback)
      {
        if (libfcgi_inited == false) {
          const int32_t rc = FCGX_Init();
          if (rc < 0) {
            throw std::runtime_error("libfcgi init error");
          }
        }
      }

      fn start(const std::string_view socket_str) -> void {
        if (this->is_working.load() == true) {
          return;
        }

        this->socketfd = FCGX_OpenSocket(socket_str.data(), this->backlog_size);
        if (this->socketfd < 0) {
          throw std::runtime_error("libfcgi OpenSocket failed");
        }

        this->is_working.store(true);

        this->workers.reserve(this->workers_amount);

        for (int32_t i = 0; i < this->workers_amount; ++i) {
          this->workers.push_back(
            std::thread(&server::worker, this)
          );
        }
        
        for (auto& thrd : this->workers) {
          thrd.join();
        }
      }

      fn stop() -> void {
        this->is_working.store(false); 
      }

    private: 
      fn worker() noexcept -> void {
        auto request = FCGX_Request{};
        FCGX_InitRequest(&request, this->socketfd, 0);

        while (true) {
          if (this->is_working.load() == false) {
            return;
          }

          this->connection_mtx.lock();
          const int32_t rc = FCGX_Accept_r(&request);
          this->connection_mtx.unlock();
          if (rc < 0) {
            this->stop();
            return;
          }

          try {
            const auto start = std::chrono::high_resolution_clock::now();
            this->callback(request);
            const auto stop = std::chrono::high_resolution_clock::now();
            const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            log_trace("execution time in micros: {}", duration.count());
          } catch (std::exception& err) {
            FCGX_Finish_r(&request);
            log_warn("{}", err.what());
          } catch (...) {
            FCGX_Finish_r(&request);
            log_warn("worker got uknown exception");
          }
        }
      }

      const int32_t workers_amount = 0;
      const int32_t backlog_size = 0;
      Function callback;

      std::vector<std::thread> workers;
      std::atomic_bool is_working = false;

      std::mutex connection_mtx;
      int32_t socketfd = 0;
  };
}

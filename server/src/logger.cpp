module;

#include <format>
#include <cstdio>
#include <cstdlib>
#include <array>
#include <ctime>

#include "flick.hpp"

export module logger;

namespace logger {
  export enum class verbosity : int8_t {
    trace,
    info,
    warn,
    error,
    fatal,
    none,
  };

  constexpr const char* verbosity_strings_colored[] = {
    "[\033[0;34mTRACE\033[1;0m]", "[\033[0;32mINFO\033[1;0m] ",
    "[\033[0;33mWARN\033[1;0m] ", "[\033[0;31mERROR\033[1;0m]",
    "[\033[0;31mFATAL\033[1;0m]", "sfome on the swag",
  };

  constexpr const char* verbosity_strings[] = {
    "[TRACE]",
    "[INFO] ",
    "[WARN] ",
    "[ERROR]",
    "[FATAL]",
    "why you're watching inside binary?",
  };

  export template<
    verbosity verb, 
    bool use_date,
    bool use_color,
    class... Args
  > fn log(
      std::FILE* stream,
      int32_t line,
      const char* filename,

      const std::format_string<Args...> format_string,
      Args&&... format_args
  ) noexcept -> void {
    if constexpr (verb != verbosity::none) {
      constexpr auto date_format = use_date == true
        ? "%Y-%m-%d %H:%M:%S"
        : "%H:%M:%S";
      constexpr auto verb_str = use_color == true
        ? verbosity_strings_colored[static_cast<int8_t>(verb)]
        : verbosity_strings[static_cast<int8_t>(verb)];

      auto date_buffer = std::array<char, 64>();
      const auto current_time = std::time(nullptr);
      std::strftime(
        date_buffer.data(), 
        date_buffer.size(),
        date_format,
        std::localtime(&current_time)
      );
   
      const auto log_str = std::format(
        "{} {} {}:{}: {}\n", 
        date_buffer.data(),
        verb_str,
        filename, line,
        std::vformat(format_string.get(), std::make_format_args(format_args...))
      );

      std::fprintf(stream, "%s", log_str.c_str());
      std::fflush(stream);

      if constexpr (verb == verbosity::fatal) {
        std::exit(1);
      }
    }
  }
}


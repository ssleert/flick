#ifndef __LOGGER_MACRO__
#define __LOGGER_MACRO__

#include <cstdio>

#ifndef LOGGER_COLORED
#define LOGGER_COLORED true
#endif

#ifndef LOGGER_USE_DATE
#define LOGGER_USE_DATE true
#endif

#define log_trace(...) \
  logger::log<logger::verbosity::trace, LOGGER_USE_DATE, LOGGER_COLORED>(stdout, __LINE__, __FILE__, __VA_ARGS__)
#define log_info(...) \
  logger::log<logger::verbosity::info, LOGGER_USE_DATE, LOGGER_COLORED>(stdout, __LINE__, __FILE__, __VA_ARGS__)
#define log_warn(...) \
  logger::log<logger::verbosity::warn, LOGGER_USE_DATE, LOGGER_COLORED>(stderr, __LINE__, __FILE__, __VA_ARGS__)
#define log_error(...) \
  logger::log<logger::verbosity::error, LOGGER_USE_DATE, LOGGER_COLORED>(stderr, __LINE__, __FILE__, __VA_ARGS__)
#define log_fatal(...) \
  logger::log<logger::verbosity::fatal, LOGGER_USE_DATE, LOGGER_COLORED>(stderr, __LINE__, __FILE__, __VA_ARGS__)

#endif

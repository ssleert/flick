cmake_minimum_required(VERSION 3.28)
project(flick)

include(FetchContent)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 23)

FetchContent_Declare(
  json 
  URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

add_compile_options(
    -Wall -Wextra -pedantic
    -Wformat=2 -Wno-unused-parameter -Wshadow
    -Wwrite-strings -Wredundant-decls
    -Wmissing-include-dirs -Wno-format-nonliteral
    -fcolor-diagnostics
    -fexperimental-library
    -Wno-deprecated-declarations
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(
    -O0 -g3 -fstack-protector -ftrapv -fwrapv
    -fsanitize=address,undefined
    -DDEBUG -DTRACE
  )
  add_link_options(-fsanitize=address,undefined)
else()
  add_compile_options(
    -O3 -DNDEBUG -DNTRACE
  )
endif()

add_executable(flick)

target_sources(flick PUBLIC
                     src/flick.cpp)

target_sources(flick PUBLIC
  FILE_SET CXX_MODULES FILES
  src/printer.cpp
  src/logger.cpp
  src/fcgi_server.cpp
  src/http_router.cpp
  src/controller_interface.cpp
  src/routes.cpp
  src/utils.cpp
  src/exceptions.cpp
  src/controller_hello_world.cpp
  src/controller_register_user.cpp)

target_link_libraries(flick PRIVATE 
                            -lfcgi 
                            nlohmann_json::nlohmann_json)
                          #-static-libgcc -static-libstdc++)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
  set_target_properties(flick PROPERTIES INTERPROCEDURAL_OPTIMIZATION true)
endif()

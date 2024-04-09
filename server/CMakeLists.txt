cmake_minimum_required(VERSION 3.28)
project(flick)

include(FetchContent)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 23)

add_compile_options(
    -Wall -Wextra -pedantic
    -Wformat=2 -Wshadow
    -Wwrite-strings -Wredundant-decls
    -Wmissing-include-dirs

    -fcolor-diagnostics
    #-Wno-deprecated-declarations
    #"-Wno-#warnings"
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(
    -O2 -g3 -fstack-protector -ftrapv -fwrapv
    -fsanitize=address,undefined
    -DDEBUG -DTRACE
  )
  add_link_options(-fsanitize=address,undefined)
else()
  add_compile_options(
    -march=native
    -Ofast -DNDEBUG -DNTRACE
  )
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(PQ REQUIRED libpq)
pkg_check_modules(FCGI REQUIRED fcgi)

option(BUILD_SHARED_LIBS "Build shared libs" OFF)
set(XXHASH_BUILD_XXHSUM OFF)
#set(DISPATCH ON)

add_subdirectory(deps/libpqxx build-pqxx)
add_subdirectory(deps/xxHash/cmake_unofficial ./xxhash_build EXCLUDE_FROM_ALL)

FetchContent_Declare(
  json 
  URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)


add_executable(flick)

target_sources(flick PUBLIC
                     src/flick.cpp)

target_sources(flick PUBLIC
  FILE_SET CXX_MODULES FILES
  src/printer.cpp
  src/logger.cpp
  src/fcgi_server.cpp
  src/http_router.cpp
  src/http.cpp
  src/server.cpp
  src/utils.cpp
  src/exceptions.cpp
  src/ttime.cpp
  src/vars.cpp
  src/validate.cpp
  src/crypto.cpp
  src/db.cpp
  src/db_pg.cpp
  src/get_env.cpp
  src/controller_interface.cpp
  src/controller_hello_world.cpp
  src/controller_register_user.cpp
  src/service_add_user.cpp)

target_link_libraries(flick PRIVATE
                            ${FCGI_LIBRARIES}
                            ${PQ_LIBRARIES}
                            pqxx
                            xxHash::xxhash
                            nlohmann_json::nlohmann_json)
                          #-static-libgcc -static-libstdc++)

target_include_directories(flick PRIVATE 
                                 ${FCGI_INCLUDE_DIRS}
                                 ${PQ_INCLUDE_DIRS})

target_compile_options(flick PRIVATE 
                             ${FCGI_CFLAGS_OTHER}
                             ${PQ_CFLAGS_OTHER})

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
  set_target_properties(flick PROPERTIES INTERPROCEDURAL_OPTIMIZATION true)
endif()

cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_COVERAGE "Build coverage" OFF)

set(
  HUNTER_CACHE_SERVERS
  "https://github.com/bmstu-iu8-cpp-sem-3/hunter-cache"
  CACHE STRING "Default cache server"
)

include("tools/gate/cmake/HunterGate.cmake")

HunterGate(
  URL "https://github.com/ruslo/hunter/archive/v0.23.214.tar.gz"
  SHA1 "e14bc153a7f16d6a5eeec845fb0283c8fad8c358"
)

project(AsioSyncClient)
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")

hunter_add_package(Boost COMPONENTS filesystem log thread regex)
find_package(Boost CONFIG REQUIRED filesystem log log_setup thread regex)
find_package(Threads)

add_library(
  ${PROJECT_NAME}Core
  STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/Utils.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/core/Server.cpp
)

add_executable(
  ${PROJECT_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/LogSetup.cpp
)

target_include_directories(
  ${PROJECT_NAME}Core
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core
)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
  ${PROJECT_NAME}Core
  ${CMAKE_THREAD_LIBS_INIT}
  Boost::log_setup
  Boost::log
  Boost::thread
  Boost::filesystem
  Boost::regex
)

target_link_libraries(
  ${PROJECT_NAME}
  ${PROJECT_NAME}Core
)

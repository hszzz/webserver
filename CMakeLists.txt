cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
project(webserver C CXX)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(SRCS http_request.cc http_server.cc http_response.cc http_parser.cc socket_wrap.cc)

add_library(httpserver ${SRCS})

add_subdirectory(tests)


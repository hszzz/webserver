cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
project(webserver C CXX)

set(SRCS http_request.cc http_server.cc socket_wrap.cc)

add_library(httpserver ${SRCS})

add_subdirectory(tests)


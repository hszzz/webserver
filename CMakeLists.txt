cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
project(webserver C CXX)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(SRCS http_request.cc http_server.cc http_response.cc http_parser.cc socket_wrap.cc buffer.cc log.cc)

find_package(Threads)

add_library(httpserver ${SRCS})

target_link_libraries(httpserver ${CMAKE_THREAD_LIBS_INIT})

add_subdirectory(tests)


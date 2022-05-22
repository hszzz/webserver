add_executable(http_request_test http_request_test.cc)
target_link_libraries(http_request_test httpserver)

add_executable(http_server_test http_server_test.cc)
target_link_libraries(http_server_test httpserver)

add_executable(buffer_test buffer_test.cc)
target_link_libraries(buffer_test httpserver)

add_executable(log_test log_test.cc)
target_link_libraries(log_test httpserver)

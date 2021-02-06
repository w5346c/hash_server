add_library(hash_server_lib
    hash_calculator_impl.cpp
    response_writer_impl.cpp
    request_responder_impl.cpp)

target_include_directories(hash_server_lib PUBLIC
    ${CMAKE_SOURCE_DIR}/include)
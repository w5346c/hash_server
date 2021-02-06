add_executable(hash_server_tests
    request_responder_tests.cpp
    main.cpp)

find_package(GTest REQUIRED)

target_include_directories(hash_server_tests PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${GTEST_INCLUDE_DIR})

target_link_libraries(hash_server_tests PUBLIC
        hash_server_lib
        GTest::GTest
        GTest::Main
        libgmock.a
        libgmock_main.a)
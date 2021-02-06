add_executable(hash_server
    hash_server.cpp)

target_include_directories(hash_server PUBLIC
    ${CMAKE_SOURCE_DIR}/include)

find_package(Boost 1.71 REQUIRED COMPONENTS
    system
    thread
    program_options)

target_link_libraries(hash_server PUBLIC
        hash_server_lib
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        ${Boost_PROGRAM_OPTIONS_LIBRARY})
cmake_minimum_required(VERSION 3.16)

project(hash_server VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

add_subdirectory(lib)
add_subdirectory(app)
add_subdirectory(tests)

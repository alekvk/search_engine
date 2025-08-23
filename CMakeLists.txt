cmake_minimum_required(VERSION 3.17)
project("Search engine" VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)

set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(src)
add_subdirectory(nlohmann_json)
add_subdirectory(tests)

target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)
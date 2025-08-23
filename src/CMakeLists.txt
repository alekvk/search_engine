cmake_minimum_required(VERSION 3.17)

set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_executable(search_engine main.cpp 
               ${MY_INCLUDE_DIR}/converter_json.h
               ${MY_INCLUDE_DIR}/search_server.h
               ${MY_INCLUDE_DIR}/inverted_index.h
               inverted_index.cpp
               converter_json.cpp
               search_server.cpp
               )

target_include_directories(search_engine PUBLIC ${MY_INCLUDE_DIR})




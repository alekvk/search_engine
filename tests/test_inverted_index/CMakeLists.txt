cmake_minimum_required(VERSION 3.24)

project(test_invert_index)

include(FetchContent)

FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL
        https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(gtest_disable_pthreads on) #needed in MinGW
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)
# Now simply link against gtest or gtest_main as needed. Eg

enable_testing()

add_executable(test_invert_index
        test_invert_index.cpp
        ${CMAKE_SOURCE_DIR}/src/inverted_index.cpp)

target_link_libraries(test_invert_index PRIVATE gtest_main)
cmake_minimum_required(VERSION 3.10)
project(OOP_Labs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- GoogleTest ---
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# === Libraries for labs ===
add_library(lab1_lib labs/lab1.cpp headers/lab1.h)
target_include_directories(lab1_lib PUBLIC headers)

add_library(lab2_lib labs/lab2.cpp headers/lab2.h)
target_include_directories(lab2_lib PUBLIC headers)

# === Executables (labs themselves) ===
add_executable(lab1 labs/lab1.cpp)
target_link_libraries(lab1 PRIVATE lab1_lib)

add_executable(lab2 labs/lab2.cpp)
target_link_libraries(lab2 PRIVATE lab2_lib)

# === Tests ===
add_executable(lab1_test tests/lab1_test.cpp)
target_link_libraries(lab1_test PRIVATE lab1_lib GTest::gtest_main)

add_executable(lab2_test tests/lab2_test.cpp)
target_link_libraries(lab2_test PRIVATE lab2_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(lab1_test)
gtest_discover_tests(lab2_test)

# === Compiler warnings ===
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    foreach(tgt lab1 lab2 lab1_test lab2_test)
        target_compile_options(${tgt} PRIVATE -Wall -Wextra -pedantic -g)
    endforeach()
endif()

cmake_minimum_required(VERSION 3.10)
project(LRUCacheTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

include(FetchContent)

# Fetch GoogleTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/releases/download/v1.15.0/googletest-1.15.0.tar.gz
)

# Make GoogleTest available
FetchContent_MakeAvailable(googletest)

# Add your LRUCache implementation
add_executable(LRUCacheTest ../lruCache.hpp lruCacheTest.cpp)

# Link GoogleTest to your test executable
target_link_libraries(LRUCacheTest gtest_main gtest pthread)

# Enable testing
enable_testing()
add_test(NAME LRUCacheTest COMMAND LRUCacheTest)

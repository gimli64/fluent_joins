cmake_minimum_required(VERSION 2.6)
project(fluent_joins)

add_subdirectory(HashingMethods)
add_subdirectory(tests)

include(CTest)
enable_testing ()
add_test(NAME test COMMAND BucketTests)

add_executable(main main.cpp)
target_link_libraries(main Bucket)

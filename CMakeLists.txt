cmake_minimum_required(VERSION 2.6)
project(fluent_joins)

add_subdirectory(HashingMethods)
add_subdirectory(tests)

include(CTest)
enable_testing ()
add_test(NAME bucketTest COMMAND BucketTests)
add_test(NAME directoryTest COMMAND DirectoryTests)
add_test(NAME hashingMethodTest COMMAND HashingMethodTests)

add_executable(main main.cpp)


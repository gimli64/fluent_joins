cmake_minimum_required(VERSION 2.6)
project(fluent_joins)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/module)

find_package(Pqxx REQUIRED)

add_subdirectory(HashingMethods)
add_subdirectory(tests)

include(CTest)
enable_testing()
add_test(NAME bucketTest COMMAND BucketTests)
add_test(NAME directoryTest COMMAND DirectoryTests)
add_test(NAME hashingMethodTest COMMAND HashingMethodTests)

include_directories(${PROJECT_SOURCE_DIR}/HashingMethods)
add_executable(main main.cpp)

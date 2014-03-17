cmake_minimum_required(VERSION 2.6)
project(fluent_joins)

#set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")

add_subdirectory(HashingMethods)
add_subdirectory(tests)

include(CTest)
enable_testing ()
add_test(NAME bucketTest COMMAND BucketTests)
add_test(NAME directoryTest COMMAND DirectoryTests)

add_executable(main main.cpp)

include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/common)
include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/common/directory)
include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/common/bucket)
target_link_libraries(main Bucket HashingMethod Directory)

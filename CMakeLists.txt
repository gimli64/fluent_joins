cmake_minimum_required(VERSION 2.6)
project(fluent_joins)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/module)

find_package(Pqxx REQUIRED)

#FIND_PACKAGE(OpenMP REQUIRED)
#if(OPENMP_FOUND)
#    message("OPENMP FOUND")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
#endif()

add_subdirectory(HashingMethods)
add_subdirectory(tests)

include(CTest)
enable_testing()
add_test(NAME bucketTest COMMAND BucketTests)
add_test(NAME directoryTest COMMAND DirectoryTests)
add_test(NAME hashingMethodTest COMMAND HashingMethodTests)

include_directories(${PROJECT_SOURCE_DIR}/HashingMethods)

add_executable(main main.cpp)

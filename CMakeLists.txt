cmake_minimum_required(VERSION 2.6)
project(fluent_joins)

include (FindPackageHandleStandardArgs)

find_path (Pqxx_INCLUDE_DIRS
    NAME
        pqxx
    PATHS
        /usr/include
        /usr/local/include
    PATH_SUFFIXES
        pqxx
    DOC
        "Directory for pqxx headers"
)

find_library (Pqxx_LIBRARIES
    NAMES
        pqxx
)

FIND_PACKAGE_HANDLE_STANDARD_ARGS("Pqxx"
    "libpqxx couldn't be found"
    Pqxx_LIBRARIES
    Pqxx_INCLUDE_DIRS
)

mark_as_advanced (Pqxx_INCLUDE_DIR Pqxx_LIBRARY)


add_subdirectory(HashingMethods)
add_subdirectory(tests)

include(CTest)
enable_testing ()
add_test(NAME bucketTest COMMAND BucketTests)
add_test(NAME directoryTest COMMAND DirectoryTests)
add_test(NAME hashingMethodTest COMMAND HashingMethodTests)

include_directories(${PROJECT_SOURCE_DIR}/HashingMethods)
add_executable(main main.cpp)

target_link_libraries (main
                       Bucket
                       ChainedBucket
                       DepthBucket
                       Directory
                       HybridDirectory
                       HashingMethod
                       LinearHashing
                       ExtendibleHashing
                       HybridHashing)

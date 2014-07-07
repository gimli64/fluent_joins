include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/)

add_executable(BucketTests bucketTests.cpp)
add_executable(DirectoryTests directoryTests.cpp)
add_executable(HashingMethodTests hashingMethodTests.cpp)

target_link_libraries (BucketTests
                       Bucket
                       DepthBucket
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       )

target_link_libraries (DirectoryTests
                       Directory
                       BucketFactory
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       )

target_link_libraries (HashingMethodTests
                       BucketFactory
                       ExtendibleHashing
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       )

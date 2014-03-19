add_executable(BucketTests bucketTests.cpp)
add_executable(DirectoryTests directoryTests.cpp)
add_executable(HashingMethodTests hashingMethodTests.cpp)

include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/common)
include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/common/bucket)
include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/common/directory)
include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/extendible)
include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/hybrid)
include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/linear)

target_link_libraries (BucketTests
                       Bucket
                       DepthBucket
                       ChainedBucket
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       )

target_link_libraries (DirectoryTests
                       Directory
                       ChainedDirectory
                       HashingMethod
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       )

target_link_libraries (HashingMethodTests
                       HashingMethod
                       ExtendibleHashing
                       LinearHashing
                       HybridHashing
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       )

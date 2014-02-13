add_executable(BucketTests bucketTests.cpp)
include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/common/bucket)
target_link_libraries (BucketTests
                       Bucket
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       )

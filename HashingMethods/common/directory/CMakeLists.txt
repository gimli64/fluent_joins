add_library(Directory directory.cpp)
add_library(ChainedDirectory chainedDirectory.cpp)

include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/common)
include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/common/bucket)

target_link_libraries(Directory Bucket DepthBucket HashingMethod)

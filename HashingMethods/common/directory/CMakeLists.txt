add_library(Directory directory.cpp)

# Needed to link hashingMethod.h
include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/common)
include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/common/bucket)

target_link_libraries(Directory Bucket DepthBucket HashingMethod)

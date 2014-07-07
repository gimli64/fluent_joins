add_library(Directory directory.cpp)

target_link_libraries(Directory Bucket DepthBucket BucketFactory)

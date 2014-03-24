add_library(Directory directory.cpp)
add_library(ChainedDirectory chainedDirectory.cpp)

target_link_libraries(Directory Bucket DepthBucket BucketFactory)

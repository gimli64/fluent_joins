add_library(Directory directory.cpp)
#add_library(HybridDirectory hybridDirectory.cpp)

target_link_libraries(Directory Bucket DepthBucket BucketFactory)

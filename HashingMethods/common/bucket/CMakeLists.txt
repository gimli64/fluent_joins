add_library(Bucket bucket.cpp)
add_library(DepthBucket depthBucket.cpp)
add_library(ChainedBucket chainedBucket.cpp)

include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/common)
target_link_libraries(Bucket
                      HashingMethod
                      ${Boost_SYSTEM_LIBRARY}
                      ${Boost_SERIALIZATION_LIBRARY} )

add_library(Bucket bucket.cpp)
add_library(DepthBucket depthBucket.cpp)
#add_library(ChainedBucket chainedBucket.cpp)
add_library(BucketFactory bucketFactory.cpp)
add_library(Couple couple.cpp)

target_link_libraries(Couple
                      pqxx
                      ${Boost_SYSTEM_LIBRARY}
                      ${Boost_SERIALIZATION_LIBRARY} )

target_link_libraries(Bucket
                      Couple
                      ${Boost_SYSTEM_LIBRARY}
                      ${Boost_SERIALIZATION_LIBRARY} )



add_library(Bucket bucket.cpp)
add_library(BucketFactory bucketFactory.cpp)
add_library(Couple couple.cpp)
add_library(Directory directory.cpp)

target_link_libraries(Couple
                      pqxx
                      ${Boost_SYSTEM_LIBRARY}
                      ${Boost_SERIALIZATION_LIBRARY} )

target_link_libraries(Bucket
                      Couple
                      ${Boost_SYSTEM_LIBRARY}
                      ${Boost_SERIALIZATION_LIBRARY} )

target_link_libraries(Directory Bucket BucketFactory)



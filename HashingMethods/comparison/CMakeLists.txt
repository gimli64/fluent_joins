add_library(QueryExecuter queryExecuter.cpp)
add_library(TableFactory tableFactory.cpp)
add_executable(comparison comparison.cxx)

target_link_libraries(TableFactory
                      pqxx
                      ExtendibleHashing
#                      LinearHashing
#                      HybridHashing
#                      MultikeyHybridHashing
                      MultikeyExtendibleHashing
                      BucketFactory
                      )

target_link_libraries(comparison TableFactory)

add_library(Comparer comparer.cpp)
add_executable(comparison comparison.cxx)

target_link_libraries(Comparer
                      pqxx
                      ExtendibleHashing
                      LinearHashing
                      HybridHashing
                      BucketFactory
                      )

target_link_libraries(comparison Comparer)

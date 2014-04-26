add_library(Comparer comparer.cpp)
add_executable(comparison comparison.cxx)

target_link_libraries(Comparer
                      pqxx
                      ExtendibleHashing
                      LinearHashing
                      HybridHashing
                      MultikeyHybridHashing
                      MultikeyExtendibleHashing
                      MultikeyLinearHashing
                      BucketFactory
                      )

target_link_libraries(comparison Comparer)

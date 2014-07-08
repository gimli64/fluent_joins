add_library(HashTable hashTable.cpp)
add_library(ExtendibleHashing extendibleHashing.cpp)

target_link_libraries(ExtendibleHashing HashTable Directory)


add_library(MurmurHash3 MurmurHash3.cpp)
add_library(HashTable hashTable.cpp)
add_library(MultikeyHashTable multikeyHashTable.cpp)

target_link_libraries(HashTable CityHash MurmurHash3)
target_link_libraries(MultikeyHashTable HashTable)

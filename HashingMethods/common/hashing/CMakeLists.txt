add_library(HashTable hashTable.cpp)
add_library(MultikeyHashTable multikeyHashTable.cpp)

target_link_libraries(HashTable CityHash)
target_link_libraries(MultikeyHashTable HashTable)

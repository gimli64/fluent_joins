add_library(MurmurHash3 MurmurHash3.cpp)
add_library(HashingMethod hashingMethod.cpp)
target_link_libraries(HashingMethod CityHash MurmurHash3)

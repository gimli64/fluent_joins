add_subdirectory(bucket)
add_subdirectory(directory)
add_subdirectory(structure)

find_package(CityHash REQUIRED)

add_library(HashingMethod hashingMethod.cpp)
target_link_libraries(HashingMethod CityHash)

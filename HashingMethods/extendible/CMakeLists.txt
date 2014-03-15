add_library(ExtendibleHashing extendibleHashing.cpp)

include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/common)
include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/common/directory)
include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/common/bucket)

target_link_libraries(ExtendibleHashing HashingMethod Directory)

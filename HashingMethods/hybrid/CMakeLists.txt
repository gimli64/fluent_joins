add_library(HybridHashing hybridHashing.cpp)

include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/common)
include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/common/directory)
include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/common/bucket)

target_link_libraries(HybridHashing HashingMethod ChainedDirectory)

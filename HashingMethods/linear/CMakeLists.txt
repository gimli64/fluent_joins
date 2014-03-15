add_library(LinearHashing linearHashing.cpp)

include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/common)
include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/common/bucket)

target_link_libraries(LinearHashing ChainedBucket Directory)

add_library(Comparer comparer.cpp)

include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/common)
include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/common/bucket)
include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/common/directory)
include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/extendible)
include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/hybrid)
include_directories(${PROJECT_SOURCE_DIR}/HashingMethods/linear)

target_link_libraries (Comparer
                       ExtendibleHashing
                       LinearHashing
                       HybridHashing
                       )

add_executable(comparison comparison.cxx)

target_link_libraries(comparison Comparer)

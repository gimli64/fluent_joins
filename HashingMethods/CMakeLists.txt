find_package(Boost REQUIRED COMPONENTS thread serialization system filesystem)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/HashingMethods)

find_package(Threads REQUIRED)

add_subdirectory(common)
add_subdirectory(extendible)
add_subdirectory(comparison)

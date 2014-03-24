find_package(Boost REQUIRED COMPONENTS serialization system filesystem)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/HashingMethods)

add_subdirectory(common)
add_subdirectory(extendible)
add_subdirectory(linear)
add_subdirectory(hybrid)
add_subdirectory(comparison)

find_package(Boost REQUIRED COMPONENTS system filesystem)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/HashingMethods)

find_package(Threads REQUIRED)

add_subdirectory(common)
add_subdirectory(page)
add_subdirectory(hashing)
add_subdirectory(experiments)

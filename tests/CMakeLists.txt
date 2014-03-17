find_package(Boost REQUIRED COMPONENTS system filesystem unit_test_framework)
add_definitions(-DBOOST_TEST_DYN_LINK)
include_directories (${Boost_INCLUDE_DIRS})
add_subdirectory(HashingMethods)

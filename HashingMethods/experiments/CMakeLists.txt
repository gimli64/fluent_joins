add_library(Manager manager.cpp)
add_executable(experiments experiments.cxx)

target_link_libraries(experiments
                      pqxx
                      Manager
                      ExtendibleHashing
                      )

add_executable(experiments experiments.cxx)

target_link_libraries(experiments
                      pqxx
                      ExtendibleHashing
                      )

add_library(QueryExecuter queryExecuter.cpp)
#add_library(TableFactory tableFactory.cpp)

add_executable(experiments experiments.cxx)

#target_link_libraries(TableFactory
#                      pqxx
#                      ExtendibleHashing
#                      BucketFactory
#                      )

target_link_libraries(QueryExecuter
                      ${CMAKE_THREAD_LIBS_INIT}
                      ${Boost_LIBRARIES}
                     )

target_link_libraries(experiments
                      pqxx
                      ExtendibleHashing
                      QueryExecuter
                      )

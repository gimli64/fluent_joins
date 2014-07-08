add_library(Couple couple.cpp)

target_link_libraries(Couple
                      pqxx
                      ${Boost_SYSTEM_LIBRARY}
                      ${Boost_SERIALIZATION_LIBRARY} )

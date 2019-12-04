add_executable(no-default-ctor main.cpp no-default-ctor.cpp)
target_link_libraries(no-default-ctor ${Boost_SERIALIZATION_LIBRARY})

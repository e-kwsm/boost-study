add_executable(no-default-ctor-inheritance inheritance.cpp main.cpp serial.cpp)
target_link_libraries(no-default-ctor-inheritance ${Boost_SERIALIZATION_LIBRARY})

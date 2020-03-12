find_package(Threads REQUIRED)
add_executable(process process.cpp)
target_link_libraries(process Threads::Threads)

add_executable(multi_predecessors multi_predecessors.cpp)
if(Graphviz_FOUND)
  target_compile_definitions(multi_predecessors PUBLIC "-DGRAPHVIZ")
endif()

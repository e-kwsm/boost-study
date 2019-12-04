add_executable(weight_map weight_map.cpp)
if(Graphviz_FOUND)
  target_compile_definitions(weight_map PUBLIC "-DGRAPHVIZ")
endif()

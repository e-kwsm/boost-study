add_executable(cut cut.cpp)
if(Graphviz_FOUND)
  target_compile_definitions(cut PUBLIC "-DGRAPHVIZ")
endif()

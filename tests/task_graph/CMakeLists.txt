

add_executable(task_graph task_graph.cpp)
target_link_libraries(task_graph hipSYCL)
install(TARGETS task_graph
        RUNTIME DESTINATION share/hipSYCL/tests/)

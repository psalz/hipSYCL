
add_library(hipSYCL SHARED
  device.cpp
  device_selector.cpp
  exception.cpp
  queue.cpp
  handler.cpp
  buffer.cpp
  task_graph.cpp
  accessor.cpp
  async_worker.cpp)

install(TARGETS hipSYCL
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)

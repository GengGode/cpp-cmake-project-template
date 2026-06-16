add_library(cpp_cmake_project_template.interface INTERFACE)

target_include_directories(cpp_cmake_project_template.interface INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_include_directories(cpp_cmake_project_template.interface INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
target_include_directories(cpp_cmake_project_template.interface INTERFACE $<INSTALL_INTERFACE:include>)

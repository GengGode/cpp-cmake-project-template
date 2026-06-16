auto_add_all_subdirectory() 


add_executable(app)

set_cxx_compile_options(app)
target_include_directories(app INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_include_directories(app INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
target_include_directories(app INTERFACE $<INSTALL_INTERFACE:include>)

target_sources(app PRIVATE app.cpp)

target_link_libraries(app PRIVATE cpp_cmake_project_template)

find_package(fmt REQUIRED)
target_link_libraries(cpp_cmake_project_template PUBLIC fmt::fmt)

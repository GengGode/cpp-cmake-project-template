#pragma once
#if defined(_WIN32)
    #ifdef cpp_cmake_project_template_identifier_EXPORTS
        #define cpp_cmake_project_template_identifier_API __declspec(dllexport)
    #else
        #define cpp_cmake_project_template_identifier_API __declspec(dllimport)
    #endif
#else
    #ifdef cpp_cmake_project_template_identifier_EXPORTS
        #define cpp_cmake_project_template_identifier_API __attribute__((visibility("default")))
    #else
        #define cpp_cmake_project_template_identifier_API
    #endif
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    cpp_cmake_project_template_identifier_API const char* get_version();
    cpp_cmake_project_template_identifier_API int test();

#ifdef __cplusplus
}
#endif

#undef cpp_cmake_project_template_identifier_API

#include <shared.hpp>

#include <fmt/format.h>

#include <iostream>

#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #include <windows.h>
    #undef WIN32_LEAN_AND_MEAN
    #undef NOMINMAX
#endif

int main(int argc, char* argv[])
{
    std::ignore = argc;
    std::ignore = argv;

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    std::setlocale(LC_ALL, "ZH-CN.UTF-8");

    std::cout << "Hello, World!" << std::endl;
    std::cout << "Version: " << get_version() << std::endl;

    int error_code = test();
    std::cout << fmt::format("Error code: {}", error_code) << std::endl;

    return 0;
}
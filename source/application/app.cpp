#include <fmt/format.h>
#include <shared.hpp>
#include <static.hpp>

#include <iostream>

int main(int argc, char* argv[])
{
    std::ignore = argc;
    std::ignore = argv;

    std::cout << "Hello, World!" << std::endl;
    std::cout << "Version: " << get_version() << std::endl;

    for (auto& errdef : ::error_invoker::locations)
    {
        std::cout << fmt::format("Error: {}:{}:{}: {}", errdef.path, errdef.line, errdef.col, errdef.error_msg) << std::endl;
    }

    int error_code = test();
    std::cout << fmt::format("Error code: {}: {}", error_code, error_code_info(error_code)) << std::endl;

    return 0;
}
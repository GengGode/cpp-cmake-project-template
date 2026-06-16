#include "shared.hpp"

#include <global-register-error.hpp>

#include <fmt/format.h>

int test_shared_regerr()
{ return code_err("测试错误: {}", "shared test info"); }

const char* get_version()
{ return "1.0.0"; }

#include <fmt/format.h>
#include <iostream>
int test()
{
    for (auto& errdef : global::error_invoker::locations)
    {
        std::cout << fmt::format("Error: {}:{}:{}: {}", errdef.path, errdef.line, errdef.col, errdef.error_msg) << std::endl;
    }
    return global::error_impl("测试错误: shared test info");
}
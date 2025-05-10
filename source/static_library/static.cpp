#include "static.hpp"

int test_regerr()
{
    return code_err("测试错误: {}", "test info");
}
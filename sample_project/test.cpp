// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "test.h"

#include <iostream>

// ReSharper disable once CppMemberFunctionMayBeStatic
void Test::foo() // NOLINT(*-convert-member-functions-to-static)
{
    std::cout << __func__;
}

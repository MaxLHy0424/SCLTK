#pragma once
namespace cpp_utils
{
#ifndef NDEBUG
    constexpr auto is_debugging_build{ true };
#else
    constexpr auto is_debugging_build{ false };
#endif
}
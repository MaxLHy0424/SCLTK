#pragma once
namespace cpp_utils
{
#ifndef NDEBUG
    constexpr auto is_debug_build{ true };
#else
    constexpr auto is_debug_build{ false };
#endif
}
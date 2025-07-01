#pragma once
#include <version>
namespace cpp_utils
{
#ifndef NDEBUG
    constexpr auto is_debugging_build{ true };
#else
    constexpr auto is_debugging_build{ false };
#endif
#if defined( __GXX_RTTI ) || ( defined( _CPPRTTI ) && _CPPRTTI )
    constexpr auto has_rtti{ true };
#else
    constexpr auto has_rtti{ false };
#endif
#if ( defined( __EXCEPTIONS ) && __EXCEPTIONS ) || ( defined( _CPPUNWIND ) && _CPPUNWIND )
    constexpr auto has_exceptions{ true };
#else
    constexpr auto has_exceptions{ false };
#endif
}
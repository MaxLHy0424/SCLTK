#pragma once
#include <version>
namespace cpp_utils
{
#ifndef NDEBUG
    inline constexpr auto is_debugging_build{ true };
#else
    inline constexpr auto is_debugging_build{ false };
#endif
#if defined( __GXX_RTTI ) || ( defined( _CPPRTTI ) && _CPPRTTI )
    inline constexpr auto has_rtti{ true };
#else
    inline constexpr auto has_rtti{ false };
#endif
#if ( defined( __EXCEPTIONS ) && __EXCEPTIONS ) || ( defined( _CPPUNWIND ) && _CPPUNWIND )
    inline constexpr auto has_exceptions{ true };
#else
    inline constexpr auto has_exceptions{ false };
#endif
}
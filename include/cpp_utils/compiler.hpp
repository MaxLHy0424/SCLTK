#pragma once
#include <version>
namespace cpp_utils
{
#ifndef NDEBUG
    inline constexpr auto is_debugging_build{ true };
# define CPP_UTILS_MACRO_IS_DEBUGGING_BUILD true
#else
    inline constexpr auto is_debugging_build{ false };
# define CPP_UTILS_MACRO_IS_DEBUGGING_BUILD false
#endif
#if defined( __GXX_RTTI ) || ( defined( _CPPRTTI ) && _CPPRTTI )
    inline constexpr auto has_rtti{ true };
# define CPP_UTILS_MACRO_HAS_RTTI true
#else
    inline constexpr auto has_rtti{ false };
# define CPP_UTILS_MACRO_HAS_RTTI false
#endif
#if ( defined( __EXCEPTIONS ) && __EXCEPTIONS ) || ( defined( _CPPUNWIND ) && _CPPUNWIND )
    inline constexpr auto has_exceptions{ true };
# define CPP_UTILS_MACRO_HAS_EXCEPTIONS true
#else
    inline constexpr auto has_exceptions{ false };
# define CPP_UTILS_MACRO_HAS_EXCEPTIONS false
#endif
}
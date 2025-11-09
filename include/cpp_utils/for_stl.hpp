#pragma once
#include <concepts>
#include <cstddef>
#include <type_traits>
#include <utility>
namespace cpp_utils
{
    template < typename... Ts >
    struct overloads final : Ts...
    {
        using Ts::operator()...;
    };
    template < typename... Ts >
    overloads( Ts... ) -> overloads< Ts... >;
}
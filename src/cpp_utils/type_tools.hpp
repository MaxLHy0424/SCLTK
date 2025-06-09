#pragma once
#include <cstddef>
#include <type_traits>
namespace cpp_utils
{
    using size_t    = std::size_t;
    using nullptr_t = std::nullptr_t;
    template < typename T >
    using type_set = T;
    template < typename T >
    using add_const_lvalue_reference_t = std::add_lvalue_reference_t< std::add_const_t< T > >;
}
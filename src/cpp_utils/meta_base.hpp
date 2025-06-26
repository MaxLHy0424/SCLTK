#pragma once
#include <cstddef>
#include <type_traits>
#include <utility>
namespace cpp_utils
{
    using size_t    = std::size_t;
    using nullptr_t = std::nullptr_t;
    template < typename T >
    using type_alloc = T;
    template < typename T >
    using add_const_lvalue_reference_t = std::add_lvalue_reference_t< std::add_const_t< T > >;
    template < bool Expr >
    concept test = Expr;
}
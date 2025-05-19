#pragma once
#include <cstddef>
#include <type_traits>
namespace cpp_utils {
    using size_type    = std::size_t;
    using nullptr_type = std::nullptr_t;
    template < typename _type_ >
    using type_set = _type_;
    template < typename _type_ >
    using add_const_lvalue_reference_type = std::add_lvalue_reference_t< std::add_const_t< _type_ > >;
}
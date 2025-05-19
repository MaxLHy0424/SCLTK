#pragma once
#include <algorithm>
#include <concepts>
#include <format>
#include <string>
#include <type_traits>
#include "type_tools.hpp"
namespace cpp_utils {
    template < typename _type_ >
    concept pointer_type = std::is_pointer_v< _type_ >;
    template < pointer_type _type_ >
    inline auto pointer_to_string( const _type_ _ptr )
    {
        using namespace std::string_literals;
        return _ptr == nullptr ? "nullptr"s : std::format( "0x{:x}", reinterpret_cast< std::uintptr_t >( _ptr ) );
    }
    template < pointer_type _type_ >
        requires( !std::same_as< std::decay_t< _type_ >, void * > && !std::is_const_v< _type_ > )
    class raw_pointer_wrapper final {
      private:
        _type_ ptr_{};
      public:
        auto get() const
        {
            return ptr_;
        }
        operator _type_()
        {
            return ptr_;
        }
        auto &operator*() const
        {
            return *ptr_;
        }
        auto &operator[]( const size_type _n ) const
        {
            return ptr_[ _n ];
        }
        auto operator+( const size_type _n ) const
        {
            return ptr_ + _n;
        }
        auto operator-( const size_type _n ) const
        {
            return ptr_ - _n;
        }
        auto operator++() -> raw_pointer_wrapper< _type_ > &
        {
            ++ptr_;
            return *this;
        }
        auto operator++( int ) -> raw_pointer_wrapper< _type_ >
        {
            return ptr_++;
        }
        constexpr auto operator=( const raw_pointer_wrapper< _type_ > & ) noexcept -> raw_pointer_wrapper< _type_ > & = default;
        constexpr auto operator=( raw_pointer_wrapper< _type_ > && ) noexcept -> raw_pointer_wrapper< _type_ > &      = default;
        constexpr raw_pointer_wrapper() noexcept                                                                      = default;
        constexpr raw_pointer_wrapper( _type_ _ptr ) noexcept
          : ptr_{ _ptr }
        { }
        constexpr raw_pointer_wrapper( const raw_pointer_wrapper< _type_ > & ) noexcept = default;
        constexpr raw_pointer_wrapper( raw_pointer_wrapper< _type_ > && ) noexcept      = default;
        ~raw_pointer_wrapper() noexcept                                                 = default;
    };
}
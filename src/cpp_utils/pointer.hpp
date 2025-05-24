#pragma once
#include <algorithm>
#include <concepts>
#include <format>
#include <string>
#include <type_traits>
#include "type_tools.hpp"
namespace cpp_utils {
    template < typename T >
    concept pointer_type = std::is_pointer_v< T >;
    template < pointer_type T >
    inline auto to_universal_pointer( const T pointer )
    {
        return reinterpret_cast< void * >( pointer );
    }
    template < pointer_type T >
    inline auto pointer_to_string( const T pointer )
    {
        using namespace std::string_literals;
        return pointer == nullptr ? "nullptr"s : std::format( "0x{:x}", reinterpret_cast< std::uintptr_t >( pointer ) );
    }
    template < pointer_type T >
        requires( !std::same_as< std::decay_t< T >, void * > && !std::is_const_v< T > )
    class raw_pointer_wrapper final {
      private:
        T pointer_{};
      public:
        auto get() const noexcept
        {
            return pointer_;
        }
        operator T() const noexcept
        {
            return pointer_;
        }
        auto &operator*() const
        {
            return *pointer_;
        }
        auto &operator[]( const size_type n ) const
        {
            return pointer_[ n ];
        }
        auto operator+( const size_type n ) const noexcept
        {
            return pointer_ + n;
        }
        auto operator-( const size_type n ) const noexcept
        {
            return pointer_ - n;
        }
        auto operator++() noexcept -> raw_pointer_wrapper< T > &
        {
            ++pointer_;
            return *this;
        }
        auto operator++( int ) noexcept -> raw_pointer_wrapper< T >
        {
            return pointer_++;
        }
        constexpr auto operator=( const raw_pointer_wrapper< T > & ) noexcept -> raw_pointer_wrapper< T > & = default;
        constexpr auto operator=( raw_pointer_wrapper< T > && ) noexcept -> raw_pointer_wrapper< T > &      = default;
        constexpr raw_pointer_wrapper() noexcept                                                            = default;
        constexpr raw_pointer_wrapper( T pointer ) noexcept
          : pointer_{ pointer }
        { }
        constexpr raw_pointer_wrapper( const raw_pointer_wrapper< T > & ) noexcept = default;
        constexpr raw_pointer_wrapper( raw_pointer_wrapper< T > && ) noexcept      = default;
        ~raw_pointer_wrapper() noexcept                                            = default;
    };
}
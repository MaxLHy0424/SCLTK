#pragma once
#include <algorithm>
#include <concepts>
#include <format>
#include <string>
#include <type_traits>
#include "meta_base.hpp"
namespace cpp_utils
{
    template < typename T >
    concept pointer = std::is_pointer_v< T >;
    template < pointer T >
    inline auto to_universal_pointer( const T ptr ) noexcept
    {
        return reinterpret_cast< void* >( ptr );
    }
    template < pointer T >
    inline auto to_const_universal_pointer( const T ptr ) noexcept
    {
        return reinterpret_cast< const void* >( ptr );
    }
    template < pointer T >
    inline auto pointer_to_string( const T ptr )
    {
        using namespace std::string_literals;
        return ptr == nullptr ? "nullptr"s : std::format( "0x{:x}", reinterpret_cast< std::uintptr_t >( ptr ) );
    }
    template < pointer T >
    inline auto pointer_to_wstring( const T ptr )
    {
        using namespace std::string_literals;
        return ptr == nullptr ? L"nullptr"s : std::format( L"0x{:x}", reinterpret_cast< std::uintptr_t >( ptr ) );
    }
    template < pointer T >
        requires( !std::is_const_v< T > )
    class raw_pointer_wrapper final
    {
      private:
        T ptr_{};
      public:
        auto reset( const T src ) noexcept
        {
            ptr_ = src;
        }
        auto get() const noexcept
        {
            return ptr_;
        }
        operator T() const noexcept
        {
            return ptr_;
        }
        auto& operator*() const
        {
            return *ptr_;
        }
        auto& operator[]( const size_t n ) const
        {
            return ptr_[ n ];
        }
        auto operator+( const size_t n ) const noexcept
        {
            return raw_pointer_wrapper< T >{ ptr_ + n };
        }
        auto operator+=( const size_t n ) noexcept -> raw_pointer_wrapper< T >&
        {
            return ptr_ += n;
        }
        auto operator++() noexcept -> raw_pointer_wrapper< T >&
        {
            ++ptr_;
            return *this;
        }
        auto operator++( int ) noexcept -> raw_pointer_wrapper< T >
        {
            return ptr_++;
        }
        auto operator-( const size_t n ) const noexcept
        {
            return raw_pointer_wrapper< T >{ ptr_ - n };
        }
        auto operator-=( const size_t n ) noexcept -> raw_pointer_wrapper< T >&
        {
            return ptr_ -= n;
        }
        auto operator--() noexcept -> raw_pointer_wrapper< T >&
        {
            --ptr_;
            return *this;
        }
        auto operator--( int ) noexcept -> raw_pointer_wrapper< T >
        {
            return ptr_--;
        }
        constexpr auto operator=( const raw_pointer_wrapper< T >& ) noexcept -> raw_pointer_wrapper< T >& = default;
        constexpr auto operator=( raw_pointer_wrapper< T >&& ) noexcept -> raw_pointer_wrapper< T >&      = default;
        constexpr raw_pointer_wrapper() noexcept                                                          = default;
        constexpr raw_pointer_wrapper( T ptr ) noexcept
          : ptr_{ ptr }
        { }
        constexpr raw_pointer_wrapper( const raw_pointer_wrapper< T >& ) noexcept = default;
        constexpr raw_pointer_wrapper( raw_pointer_wrapper< T >&& ) noexcept      = default;
        ~raw_pointer_wrapper() noexcept                                           = default;
    };
    template <>
    class raw_pointer_wrapper< void* > final
    {
      private:
        using T = void*;
        T ptr_{};
      public:
        auto reset( void* const src ) noexcept
        {
            ptr_ = src;
        }
        auto get() const noexcept
        {
            return ptr_;
        }
        operator T() const noexcept
        {
            return ptr_;
        }
        constexpr auto operator=( const raw_pointer_wrapper< T >& ) noexcept -> raw_pointer_wrapper< T >& = default;
        constexpr auto operator=( raw_pointer_wrapper< T >&& ) noexcept -> raw_pointer_wrapper< T >&      = default;
        constexpr raw_pointer_wrapper() noexcept                                                          = default;
        constexpr raw_pointer_wrapper( const T ptr ) noexcept
          : ptr_{ ptr }
        { }
        constexpr raw_pointer_wrapper( const raw_pointer_wrapper< T >& ) noexcept = default;
        constexpr raw_pointer_wrapper( raw_pointer_wrapper< T >&& ) noexcept      = default;
        ~raw_pointer_wrapper() noexcept                                           = default;
    };
    template <>
    class raw_pointer_wrapper< const void* > final
    {
      private:
        using T = const void*;
        T ptr_{};
      public:
        auto reset( void* const src ) noexcept
        {
            ptr_ = src;
        }
        auto get() const noexcept
        {
            return ptr_;
        }
        operator T() const noexcept
        {
            return ptr_;
        }
        constexpr auto operator=( const raw_pointer_wrapper< T >& ) noexcept -> raw_pointer_wrapper< T >& = default;
        constexpr auto operator=( raw_pointer_wrapper< T >&& ) noexcept -> raw_pointer_wrapper< T >&      = default;
        constexpr raw_pointer_wrapper() noexcept                                                          = default;
        constexpr raw_pointer_wrapper( const T ptr ) noexcept
          : ptr_{ ptr }
        { }
        constexpr raw_pointer_wrapper( const raw_pointer_wrapper< T >& ) noexcept = default;
        constexpr raw_pointer_wrapper( raw_pointer_wrapper< T >&& ) noexcept      = default;
        ~raw_pointer_wrapper() noexcept                                           = default;
    };
}
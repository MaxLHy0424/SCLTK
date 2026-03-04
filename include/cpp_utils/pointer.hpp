#pragma once
#include <algorithm>
#include <bit>
#include <concepts>
#include <format>
#include <string>
#include <type_traits>
namespace cpp_utils
{
    template < typename T >
    concept pointer = std::is_pointer_v< T >;
    template < pointer T >
    [[nodiscard]] inline auto to_universal_pointer( const T ptr ) noexcept
    {
        return reinterpret_cast< void* >( ptr );
    }
    template < pointer T >
    [[nodiscard]] inline auto to_const_universal_pointer( const T ptr ) noexcept
    {
        return reinterpret_cast< const void* >( ptr );
    }
    template < pointer T >
    [[nodiscard]] inline auto pointer_to_string( const T ptr )
    {
        using namespace std::string_literals;
        return ptr == nullptr ? "nullptr"s : std::format( "0x{:x}", reinterpret_cast< const void* >( ptr ) );
    }
    template < pointer T >
    [[nodiscard]] inline auto pointer_to_wstring( const T ptr )
    {
        using namespace std::string_literals;
        return ptr == nullptr ? L"nullptr"s : std::format( L"0x{:x}", reinterpret_cast< const void* >( ptr ) );
    }
    template < typename T >
        requires( !std::is_const_v< T > && pointer< T > )
    class raw_pointer_wrapper final
    {
      private:
        T ptr_{};
      public:
        constexpr auto reset( const T src ) noexcept
        {
            ptr_ = src;
        }
        [[nodiscard]] constexpr auto get() const noexcept
        {
            return ptr_;
        }
        [[nodiscard]] constexpr operator T() const noexcept
        {
            return ptr_;
        }
        [[nodiscard]] constexpr auto&& operator*() const noexcept
            requires( !std::same_as< std::remove_cv_t< std::remove_pointer_t< T > >, void > )
        {
            return *ptr_;
        }
        [[nodiscard]] constexpr auto&& operator[]( const std::size_t n ) const noexcept
            requires( !std::same_as< std::remove_cv_t< std::remove_pointer_t< T > >, void > )
        {
            return ptr_[ n ];
        }
        [[nodiscard]] constexpr auto operator+( const std::size_t n ) const noexcept
            requires( !std::same_as< std::remove_cv_t< std::remove_pointer_t< T > >, void > )
        {
            return raw_pointer_wrapper< T >{ ptr_ + n };
        }
        constexpr auto operator+=( const std::size_t n ) noexcept -> raw_pointer_wrapper< T >&
            requires( !std::same_as< std::remove_cv_t< std::remove_pointer_t< T > >, void > )
        {
            return ptr_ += n;
        }
        constexpr auto operator++() noexcept -> raw_pointer_wrapper< T >&
            requires( !std::same_as< std::remove_cv_t< std::remove_pointer_t< T > >, void > )
        {
            ++ptr_;
            return *this;
        }
        constexpr auto operator++( int ) noexcept -> raw_pointer_wrapper< T >
            requires( !std::same_as< std::remove_cv_t< std::remove_pointer_t< T > >, void > )
        {
            return ptr_++;
        }
        [[nodiscard]] constexpr auto operator-( const std::size_t n ) const noexcept
            requires( !std::same_as< std::remove_cv_t< std::remove_pointer_t< T > >, void > )
        {
            return raw_pointer_wrapper< T >{ ptr_ - n };
        }
        constexpr auto operator-=( const std::size_t n ) noexcept -> raw_pointer_wrapper< T >&
            requires( !std::same_as< std::remove_cv_t< std::remove_pointer_t< T > >, void > )
        {
            return ptr_ -= n;
        }
        constexpr auto operator--() noexcept -> raw_pointer_wrapper< T >&
            requires( !std::same_as< std::remove_cv_t< std::remove_pointer_t< T > >, void > )
        {
            --ptr_;
            return *this;
        }
        constexpr auto operator--( int ) noexcept -> raw_pointer_wrapper< T >
            requires( !std::same_as< std::remove_cv_t< std::remove_pointer_t< T > >, void > )
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
}
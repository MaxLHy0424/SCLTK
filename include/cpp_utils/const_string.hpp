#pragma once
#include <algorithm>
#include <array>
#include <concepts>
#include <print>
#include "compiler.hpp"
namespace cpp_utils
{
    template < typename T >
    concept character
      = std::same_as< std::decay_t< T >, char > || std::same_as< std::decay_t< T >, wchar_t >
     || std::same_as< std::decay_t< T >, char8_t > || std::same_as< std::decay_t< T >, char16_t >
     || std::same_as< std::decay_t< T >, char32_t >;
    template < character T, std::size_t N >
        requires std::same_as< T, std::decay_t< T > >
    class basic_const_string final
    {
      private:
        std::array< T, N + 1 > storage_{};
      public:
        constexpr const auto& base() const noexcept
        {
            return storage_;
        }
        constexpr auto c_str() const noexcept
        {
            return const_cast< const T* >( storage_.data() );
        }
        constexpr auto empty() const noexcept
        {
            return N == 0;
        }
        constexpr auto size() const noexcept
        {
            return N;
        }
        constexpr auto max_size() const noexcept
        {
            return storage_.max_size() - 1;
        }
        constexpr const auto& front() const noexcept
        {
            return storage_.front();
        }
        constexpr const auto& back() const noexcept
        {
            if constexpr ( empty() ) {
                return storage_.back();
            } else {
                return storage_[ size() - 1 ];
            }
        }
        constexpr auto begin() const noexcept
        {
            return storage_.cbegin();
        }
        constexpr auto rbegin() const noexcept
        {
            return storage_.crbegin();
        }
        constexpr auto end() const noexcept
        {
            return storage_.cend() - 1;
        }
        constexpr auto rend() const noexcept
        {
            return storage_.crend() - 1;
        }
        constexpr const auto& operator[]( const std::size_t index ) const noexcept
        {
            return storage_[ index ];
        }
        constexpr const auto& at( const std::size_t index ) const noexcept
        {
            if constexpr ( is_debugging_build ) {
                return storage_.at( index );
            } else {
                return ( *this )[ index ];
            }
        }
        constexpr operator std::basic_string_view< T >() const noexcept
        {
            return std::basic_string_view< T >{ c_str(), size() };
        }
        consteval auto operator=( const basic_const_string< T, N >& ) -> basic_const_string< T, N >& = default;
        auto operator=( basic_const_string< T, N >&& ) -> basic_const_string< T, N >&                = delete;
        consteval basic_const_string( const T ( &str )[ N + 1 ] ) noexcept
        {
            static_assert( str[ N ] == '\0' );
            std::ranges::copy( str, storage_.data() );
        }
        consteval basic_const_string( const std::array< T, N > str ) noexcept
        {
            std::ranges::copy( str, storage_.data() );
            storage_.back() = '\0';
        }
        consteval basic_const_string( const basic_const_string< T, N >& )     = default;
        consteval basic_const_string( basic_const_string< T, N >&& ) noexcept = delete;
        ~basic_const_string() noexcept                                        = default;
    };
    template < character T, std::size_t N >
    basic_const_string( const T ( & )[ N ] ) -> basic_const_string< T, N - 1 >;
    template < std::size_t N >
    using const_string = basic_const_string< char, N >;
    template < std::size_t N >
    using const_wstring = basic_const_string< wchar_t, N >;
    template < std::size_t N >
    using const_u8string = basic_const_string< char8_t, N >;
    template < std::size_t N >
    using const_u16string = basic_const_string< char16_t, N >;
    template < std::size_t N >
    using const_u32string = basic_const_string< char32_t, N >;
    template < auto C, std::size_t N >
        requires character< decltype( C ) >
    inline consteval auto make_repeated_const_string() noexcept
    {
        using T = decltype( C );
        std::array< T, N > str;
        str.fill( C );
        return basic_const_string{ str };
    }
}
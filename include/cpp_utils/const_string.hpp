#pragma once
#include <algorithm>
#include <array>
#include <concepts>
#include <ranges>
#include "compiler.hpp"
namespace cpp_utils
{
    template < typename T >
    concept character
      = std::same_as< std::decay_t< T >, char > || std::same_as< std::decay_t< T >, wchar_t >
     || std::same_as< std::decay_t< T >, char8_t > || std::same_as< std::decay_t< T >, char16_t >
     || std::same_as< std::decay_t< T >, char32_t >;
    template < character T, std::size_t N >
        requires( std::same_as< T, std::decay_t< T > > && N > 0 )
    class basic_const_string final
    {
      private:
        std::array< T, N > storage_{};
      public:
        constexpr const auto& data() const noexcept
        {
            return storage_;
        }
        constexpr auto c_str() const noexcept
        {
            return const_cast< const T* >( storage_.data() );
        }
        constexpr auto size() const noexcept
        {
            return N - 1;
        }
        constexpr auto capacity() const noexcept
        {
            return N;
        }
        constexpr auto max_size() const noexcept
        {
            return storage_.max_size() - 1;
        }
        constexpr auto max_capacity() const noexcept
        {
            return storage_.max_size();
        }
        constexpr const auto& front() const noexcept
        {
            return storage_.front();
        }
        constexpr const auto& back() const noexcept
        {
            if constexpr ( N == 1 ) {
                return storage_.back();
            } else {
                return *( &storage_.back() - 1 );
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
            return storage_.cend();
        }
        constexpr auto rend() const noexcept
        {
            return storage_.crend();
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
        consteval basic_const_string( const T ( &str )[ N ] ) noexcept
        {
            std::ranges::copy( str, storage_.data() );
        }
        consteval basic_const_string( const std::array< T, N >& str ) noexcept
          : storage_{ str }
        { }
        consteval basic_const_string( const basic_const_string< T, N >& )     = default;
        consteval basic_const_string( basic_const_string< T, N >&& ) noexcept = delete;
        ~basic_const_string() noexcept                                        = default;
    };
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
        std::array< T, N + 1 > str;
        str.fill( C );
        str.back() = '\0';
        return basic_const_string< T, N + 1 >{ str };
    }
}
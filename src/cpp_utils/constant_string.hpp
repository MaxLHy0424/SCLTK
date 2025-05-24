#pragma once
#include <algorithm>
#include <concepts>
#include <ranges>
#include "type_tools.hpp"
namespace cpp_utils {
    template < typename T >
    concept char_type
      = std::same_as< std::decay_t< T >, char > || std::same_as< std::decay_t< T >, wchar_t >
     || std::same_as< std::decay_t< T >, char8_t > || std::same_as< std::decay_t< T >, char16_t >
     || std::same_as< std::decay_t< T >, char32_t >;
    template < char_type T, size_type N >
        requires( std::same_as< T, std::decay_t< T > > && N > 0 )
    class constant_string final {
      private:
        T data_[ N ]{};
      public:
        auto get() const noexcept
        {
            return const_cast< const T * >( data_ );
        }
        constexpr auto compare( const T *const src ) const noexcept
        {
            if ( src == nullptr ) {
                return false;
            }
            size_type src_size{ 0 };
            while ( src[ src_size ] != '\0' ) {
                ++src_size;
            }
            if ( src_size + 1 != N ) {
                return false;
            }
            for ( const auto i : std::ranges::iota_view{ decltype( N ){ 0 }, N } ) {
                if ( data_[ i ] != src[ i ] ) {
                    return false;
                }
            }
            return true;
        }
        template < size_type SrcN >
        constexpr auto compare( const T ( &src )[ SrcN ] ) const noexcept
        {
            if ( SrcN != N ) {
                return false;
            }
            for ( const auto i : std::ranges::iota_view{ decltype( N ){ 0 }, N } ) {
                if ( data_[ i ] != src[ i ] ) {
                    return false;
                }
            }
            return true;
        }
        template < size_type SrcN >
        constexpr auto compare( const constant_string< T, SrcN > &src ) const noexcept
        {
            if ( SrcN != N ) {
                return false;
            }
            for ( const auto i : std::ranges::iota_view{ decltype( N ){ 0 }, N } ) {
                if ( data_[ i ] != src.data_[ i ] ) {
                    return false;
                }
            }
            return true;
        }
        constexpr auto operator==( const T *const src ) const noexcept
        {
            return compare( src );
        }
        template < size_type SrcN >
        constexpr auto operator==( const T ( &src )[ SrcN ] ) const noexcept
        {
            return compare( src );
        }
        template < size_type SrcN >
        constexpr auto operator==( const constant_string< T, SrcN > &src ) const noexcept
        {
            return compare( src );
        }
        constexpr auto operator!=( const T *const src ) const noexcept
        {
            return !compare( src );
        }
        template < size_type SrcN >
        constexpr auto operator!=( const T ( &src )[ SrcN ] ) const noexcept
        {
            return !compare( src );
        }
        template < size_type SrcN >
        constexpr auto operator!=( const constant_string< T, SrcN > &src ) const noexcept
        {
            return !compare( src );
        }
        const auto &operator[]( const size_type index ) const noexcept
        {
            return data_[ index ];
        }
        auto operator=( const constant_string< T, N > & ) -> constant_string< T, N > & = delete;
        auto operator=( constant_string< T, N > && ) -> constant_string< T, N > &      = delete;
        consteval constant_string( const T ( &str )[ N ] ) noexcept
        {
            std::ranges::copy( str, data_ );
        }
        consteval constant_string( const constant_string< T, N > & )     = default;
        consteval constant_string( constant_string< T, N > && ) noexcept = delete;
        ~constant_string() noexcept                                      = default;
    };
    template < size_type N >
    using constant_ansi_string = constant_string< char, N >;
    template < size_type N >
    using constant_wide_string = constant_string< wchar_t, N >;
    template < size_type N >
    using constant_utf8_string = constant_string< char8_t, N >;
    template < size_type N >
    using constant_utf16_string = constant_string< char16_t, N >;
    template < size_type N >
    using constant_utf32_string = constant_string< char32_t, N >;
}
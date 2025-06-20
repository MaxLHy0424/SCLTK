#pragma once
#include <algorithm>
#include <array>
#include <concepts>
#include <ranges>
#include "type_tools.hpp"
namespace cpp_utils
{
    template < typename T >
    concept character
      = std::same_as< std::decay_t< T >, char > || std::same_as< std::decay_t< T >, wchar_t >
     || std::same_as< std::decay_t< T >, char8_t > || std::same_as< std::decay_t< T >, char16_t >
     || std::same_as< std::decay_t< T >, char32_t >;
    template < character T, size_t N >
        requires( std::same_as< T, std::decay_t< T > > && N > 0 )
    class basic_constant_string final
    {
      private:
        std::array< T, N > data_{};
      public:
        constexpr auto data() const noexcept
        {
            return const_cast< const T* >( data_.data() );
        }
        constexpr auto size() const noexcept
        {
            return data_.size();
        }
        constexpr auto max_size() const noexcept
        {
            return data_.max_size();
        }
        constexpr auto& at( const size_t index ) const noexcept
        {
            return data_.at( index );
        }
        const auto& operator[]( const size_t index ) const noexcept
        {
            return data_[ index ];
        }
        constexpr auto compare( const T* const src ) const
        {
            if ( src == nullptr ) {
                return false;
            }
            size_t src_size{ 0 };
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
        template < size_t SrcN >
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
        template < size_t SrcN >
        constexpr auto compare( const basic_constant_string< T, SrcN >& src ) const noexcept
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
        constexpr auto operator==( const T* const src ) const
        {
            return compare( src );
        }
        template < size_t SrcN >
        constexpr auto operator==( const T ( &src )[ SrcN ] ) const noexcept
        {
            return compare( src );
        }
        template < size_t SrcN >
        constexpr auto operator==( const basic_constant_string< T, SrcN >& src ) const noexcept
        {
            return compare( src );
        }
        constexpr auto operator!=( const T* const src ) const noexcept
        {
            return !compare( src );
        }
        template < size_t SrcN >
        constexpr auto operator!=( const T ( &src )[ SrcN ] ) const noexcept
        {
            return !compare( src );
        }
        template < size_t SrcN >
        constexpr auto operator!=( const basic_constant_string< T, SrcN >& src ) const noexcept
        {
            return !compare( src );
        }
        auto operator=( const basic_constant_string< T, N >& ) -> basic_constant_string< T, N >& = delete;
        auto operator=( basic_constant_string< T, N >&& ) -> basic_constant_string< T, N >&      = delete;
        consteval basic_constant_string( const T ( &str )[ N ] ) noexcept
        {
            std::ranges::copy( str, data_.data() );
        }
        consteval basic_constant_string( const basic_constant_string< T, N >& )     = default;
        consteval basic_constant_string( basic_constant_string< T, N >&& ) noexcept = delete;
        ~basic_constant_string() noexcept                                           = default;
    };
    template < size_t N >
    using constant_string = basic_constant_string< char, N >;
    template < size_t N >
    using constant_wstring = basic_constant_string< wchar_t, N >;
    template < size_t N >
    using constant_u8string = basic_constant_string< char8_t, N >;
    template < size_t N >
    using constant_u16string = basic_constant_string< char16_t, N >;
    template < size_t N >
    using constant_u32string = basic_constant_string< char32_t, N >;
}
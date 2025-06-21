#pragma once
#include <algorithm>
#include <array>
#include <concepts>
#include <ranges>
#include "compiler.hpp"
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
    class basic_const_string final
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
        constexpr auto& front() const noexcept
        {
            return const_cast< const T& >( data_.front() );
        }
        constexpr auto& back() const noexcept
        {
            return const_cast< const T& >( data_.back() );
        }
        constexpr auto begin() const noexcept
        {
            return data_.cbegin();
        }
        constexpr auto rbegin() const noexcept
        {
            return data_.crbegin();
        }
        constexpr auto end() const noexcept
        {
            return data_.cend();
        }
        constexpr auto rend() const noexcept
        {
            return data_.crend();
        }
        constexpr const auto& operator[]( const size_t index ) const noexcept
        {
            return const_cast< const T& >( data_[ index ] );
        }
        constexpr const auto& at( const size_t index ) const noexcept
        {
            if constexpr ( is_debug_build ) {
                return const_cast< const T& >( data_.at( index ) );
            } else {
                return ( *this )[ index ];
            }
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
        constexpr auto compare( const basic_const_string< T, SrcN >& src ) const noexcept
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
        constexpr auto operator==( const basic_const_string< T, SrcN >& src ) const noexcept
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
        constexpr auto operator!=( const basic_const_string< T, SrcN >& src ) const noexcept
        {
            return !compare( src );
        }
        consteval auto operator=( const basic_const_string< T, N >& ) -> basic_const_string< T, N >& = default;
        auto operator=( basic_const_string< T, N >&& ) -> basic_const_string< T, N >&                = delete;
        consteval basic_const_string( const T ( &str )[ N ] ) noexcept
        {
            std::ranges::copy( str, data_.data() );
        }
        consteval basic_const_string( const std::array< T, N >& str ) noexcept
          : data_{ str }
        { }
        consteval basic_const_string( const basic_const_string< T, N >& )     = default;
        consteval basic_const_string( basic_const_string< T, N >&& ) noexcept = delete;
        ~basic_const_string() noexcept                                        = default;
    };
    template < size_t N >
    using const_string = basic_const_string< char, N >;
    template < size_t N >
    using const_wstring = basic_const_string< wchar_t, N >;
    template < size_t N >
    using const_u8string = basic_const_string< char8_t, N >;
    template < size_t N >
    using const_u16string = basic_const_string< char16_t, N >;
    template < size_t N >
    using const_u32string = basic_const_string< char32_t, N >;
    template < size_t N, auto C >
        requires character< decltype( C ) >
    consteval auto make_repeated_const_string() noexcept
    {
        using T = decltype( C );
        std::array< T, N + 1 > str;
        str.fill( C );
        str.back() = '\0';
        return basic_const_string< T, N + 1 >{ str };
    }
}
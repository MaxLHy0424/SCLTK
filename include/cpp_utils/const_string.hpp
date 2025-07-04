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
        std::array< T, N > data_{};
      public:
        constexpr auto data() const noexcept
        {
            return const_cast< const T* >( data_.data() );
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
            return data_.max_size() - 1;
        }
        constexpr auto max_capacity() const noexcept
        {
            return data_.max_size();
        }
        constexpr const auto& front() const noexcept
        {
            return data_.front();
        }
        constexpr const auto& back() const noexcept
        {
            return data_.back();
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
        constexpr const auto& operator[]( const std::size_t index ) const noexcept
        {
            return data_[ index ];
        }
        constexpr const auto& at( const std::size_t index ) const noexcept
        {
            if constexpr ( is_debugging_build ) {
                return data_.at( index );
            } else {
                return ( *this )[ index ];
            }
        }
        constexpr auto compare( const std::basic_string_view< T > src ) const
        {
            return src == this->data();
        }
        template < std::size_t SrcN >
        constexpr auto compare( const T ( &src )[ SrcN ] ) const noexcept
        {
            if ( SrcN != N ) {
                return false;
            }
            for ( decltype( N ) i{ 0 }; i < N; ++i ) {
                if ( data_[ i ] != src[ i ] ) {
                    return false;
                }
            }
            return true;
        }
        template < std::size_t SrcN >
        constexpr auto compare( const basic_const_string< T, SrcN >& src ) const noexcept
        {
            if ( SrcN != N ) {
                return false;
            }
            for ( decltype( N ) i{ 0 }; i < N; ++i ) {
                if ( data_[ i ] != src[ i ] ) {
                    return false;
                }
            }
            return true;
        }
        constexpr auto operator==( const std::basic_string_view< T > src ) const
        {
            return compare( src );
        }
        template < std::size_t SrcN >
        constexpr auto operator==( const T ( &src )[ SrcN ] ) const noexcept
        {
            return compare( src );
        }
        template < std::size_t SrcN >
        constexpr auto operator==( const basic_const_string< T, SrcN >& src ) const noexcept
        {
            return compare( src );
        }
        constexpr auto operator!=( const T* const src ) const noexcept
        {
            return !compare( src );
        }
        template < std::size_t SrcN >
        constexpr auto operator!=( const T ( &src )[ SrcN ] ) const noexcept
        {
            return !compare( src );
        }
        template < std::size_t SrcN >
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
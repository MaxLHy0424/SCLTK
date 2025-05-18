#pragma once
#include <algorithm>
#include <concepts>
#include <ranges>
#include "type.hpp"
namespace cpp_utils {
    template < typename _type_ >
    concept char_type
      = std::same_as< std::decay_t< _type_ >, char > || std::same_as< std::decay_t< _type_ >, wchar_t >
     || std::same_as< std::decay_t< _type_ >, char8_t > || std::same_as< std::decay_t< _type_ >, char16_t >
     || std::same_as< std::decay_t< _type_ >, char32_t >;
    template < char_type _type_, size_type _capacity_ >
        requires( std::same_as< _type_, std::decay_t< _type_ > > && _capacity_ > 0 )
    class constant_string final {
      private:
        _type_ data_[ _capacity_ ]{};
      public:
        auto c_str() const noexcept
        {
            return const_cast< const _type_ * >( data_ );
        }
        auto compare( const _type_ *const _src ) const noexcept
        {
            if ( _src == nullptr ) {
                return false;
            }
            size_type src_size{ 0 };
            while ( _src[ src_size ] != '\0' ) {
                ++src_size;
            }
            if ( src_size + 1 != _capacity_ ) {
                return false;
            }
            for ( const auto i : std::ranges::iota_view{ decltype( _capacity_ ){ 0 }, _capacity_ } ) {
                if ( data_[ i ] != _src[ i ] ) {
                    return false;
                }
            }
            return true;
        }
        template < size_type _src_capacity_ >
        auto compare( const _type_ ( &_src )[ _src_capacity_ ] ) const noexcept
        {
            if ( _src_capacity_ != _capacity_ ) {
                return false;
            }
            for ( const auto i : std::ranges::iota_view{ decltype( _capacity_ ){ 0 }, _capacity_ } ) {
                if ( data_[ i ] != _src[ i ] ) {
                    return false;
                }
            }
            return true;
        }
        template < size_type _src_capacity_ >
        auto compare( const constant_string< _type_, _src_capacity_ > &_src ) const noexcept
        {
            if ( _src_capacity_ != _capacity_ ) {
                return false;
            }
            for ( const auto i : std::ranges::iota_view{ decltype( _capacity_ ){ 0 }, _capacity_ } ) {
                if ( data_[ i ] != _src.data_[ i ] ) {
                    return false;
                }
            }
            return true;
        }
        auto operator==( const _type_ *const _src ) const noexcept
        {
            return compare( _src );
        }
        template < size_type _src_capacity_ >
        auto operator==( const _type_ ( &_src )[ _src_capacity_ ] ) const noexcept
        {
            return compare( _src );
        }
        template < size_type _src_capacity_ >
        auto operator==( const constant_string< _type_, _src_capacity_ > &_src ) const noexcept
        {
            return compare( _src );
        }
        const auto &operator[]( const size_type _index ) const noexcept
        {
            return data_[ _index ];
        }
        auto operator=( const constant_string< _type_, _capacity_ > & ) -> constant_string< _type_, _capacity_ > & = delete;
        auto operator=( constant_string< _type_, _capacity_ > && ) -> constant_string< _type_, _capacity_ > &      = delete;
        consteval constant_string( const _type_ ( &_str )[ _capacity_ ] ) noexcept
        {
            std::copy( _str, _str + _capacity_, data_ );
        }
        consteval constant_string( const constant_string< _type_, _capacity_ > & )     = default;
        consteval constant_string( constant_string< _type_, _capacity_ > && ) noexcept = delete;
        ~constant_string() noexcept                                                    = default;
    };
    template < size_type _capacity_ >
    using constant_ansi_string = constant_string< char, _capacity_ >;
    template < size_type _capacity_ >
    using constant_wide_string = constant_string< wchar_t, _capacity_ >;
    template < size_type _capacity_ >
    using constant_utf8_string = constant_string< char8_t, _capacity_ >;
    template < size_type _capacity_ >
    using constant_utf16_string = constant_string< char16_t, _capacity_ >;
    template < size_type _capacity_ >
    using constant_utf32_string = constant_string< char32_t, _capacity_ >;
}
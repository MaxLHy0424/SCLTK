#pragma once
#include <algorithm>
#include <array>
#include <concepts>
#include <string_view>
#include <vector>
#include "compiler.hpp"
#include "meta.hpp"
namespace cpp_utils
{
    template < typename T >
    concept character
      = std::same_as< std::remove_cv_t< T >, char > || std::same_as< std::remove_cv_t< T >, wchar_t >
     || std::same_as< std::remove_cv_t< T >, char8_t > || std::same_as< std::remove_cv_t< T >, char16_t >
     || std::same_as< std::remove_cv_t< T >, char32_t >;
    template < character T, std::size_t N >
        requires std::same_as< T, std::decay_t< T > >
    struct basic_const_string final
    {
        using char_t = T;
        std::array< T, N + 1 > storage_{};
        [[nodiscard]] constexpr const auto& data() const noexcept
        {
            return storage_;
        }
        [[nodiscard]] constexpr auto c_str() const noexcept
        {
            return static_cast< const T* >( storage_.data() );
        }
        [[nodiscard]] constexpr auto empty() const noexcept
        {
            return N == 0;
        }
        [[nodiscard]] constexpr auto size() const noexcept
        {
            return N;
        }
        [[nodiscard]] constexpr auto max_size() const noexcept
        {
            return storage_.max_size() - 1;
        }
        [[nodiscard]] constexpr const auto& front() const noexcept
        {
            return storage_.front();
        }
        [[nodiscard]] constexpr const auto& back() const noexcept
        {
            if constexpr ( empty() ) {
                return storage_.back();
            } else {
                return storage_[ size() - 1 ];
            }
        }
        [[nodiscard]] constexpr auto begin() const noexcept
        {
            return storage_.cbegin();
        }
        [[nodiscard]] constexpr auto rbegin() const noexcept
        {
            return storage_.crbegin();
        }
        [[nodiscard]] constexpr auto end() const noexcept
        {
            return storage_.cend() - 1;
        }
        [[nodiscard]] constexpr auto rend() const noexcept
        {
            return storage_.crend() - 1;
        }
        [[nodiscard]] constexpr const auto& operator[]( const std::size_t index ) const noexcept
        {
            return storage_[ index ];
        }
        [[nodiscard]] constexpr const auto& at( const std::size_t index ) const noexcept
        {
            if constexpr ( is_debugging_build ) {
                return storage_.at( index );
            } else {
                return ( *this )[ index ];
            }
        }
        [[nodiscard]] constexpr auto view() const noexcept
        {
            return std::basic_string_view< T >{ c_str(), size() };
        }
        template < character OtherCharT, std::size_t OtherN >
        [[nodiscard]] constexpr auto operator==( const basic_const_string< OtherCharT, OtherN >& other ) const noexcept
        {
            if constexpr ( !std::is_same_v< T, OtherCharT > || N != OtherN ) {
                return false;
            } else {
                return other.view() == this->view();
            }
        }
        consteval auto operator=( const basic_const_string< T, N >& ) noexcept -> basic_const_string< T, N >& = delete;
        consteval auto operator=( basic_const_string< T, N >&& ) noexcept -> basic_const_string< T, N >&      = delete;
        consteval basic_const_string( const T ( &str )[ N + 1 ] ) noexcept
        {
            std::ranges::copy( str, storage_.data() );
            storage_.back() = '\0';
        }
        consteval basic_const_string( const std::array< T, N > str ) noexcept
        {
            std::ranges::copy( str, storage_.data() );
            storage_.back() = '\0';
        }
        consteval basic_const_string( const basic_const_string< T, N >& ) noexcept = default;
        consteval basic_const_string( basic_const_string< T, N >&& ) noexcept      = default;
        constexpr ~basic_const_string() noexcept                                   = default;
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
    template < character auto C, std::size_t N >
    [[nodiscard]] inline consteval auto make_repeated_const_string() noexcept
    {
        std::array< decltype( C ), N > str;
        str.fill( C );
        return basic_const_string{ str };
    }
    template < character T, std::size_t... Ns >
    [[nodiscard]] inline consteval auto concat_const_string( const basic_const_string< T, Ns >... strings ) noexcept
    {
        std::array< T, ( Ns + ... ) > storage;
        auto begin{ storage.data() };
        ( [ & ]< std::size_t N >( basic_const_string< T, N > string ) consteval
        {
            std::ranges::copy( string.c_str(), string.c_str() + N, begin );
            begin += N;
        }( strings ), ... );
        return basic_const_string{ storage };
    }
    template < character T, std::integral auto N >
    [[nodiscard]] inline consteval auto make_const_string_from_integral() noexcept
    {
        if constexpr ( N == 0 ) {
            return make_repeated_const_string< static_cast< T >( '0' ), 1 >();
        } else {
            constexpr auto f{ [] static
            {
                auto abs_n{ std::abs( N ) };
                std::vector< T > abs_chars;
                while ( abs_n != 0 ) {
                    abs_chars.emplace_back( static_cast< T >( abs_n % 10 + '0' ) );
                    abs_n /= 10;
                }
                std::ranges::reverse( abs_chars );
                std::vector< T > final_chars;
                if constexpr ( N < 0 ) {
                    final_chars.emplace_back( static_cast< T >( '-' ) );
                }
                final_chars.append_range( abs_chars );
                return final_chars;
            } };
            return basic_const_string{ invoke_to_array< f >() };
        }
    }
    namespace const_string_literals
    {
        template < basic_const_string S >
        [[nodiscard]] inline consteval auto operator""_cs() noexcept
        {
            return S;
        }
    }
    namespace details
    {
        template < basic_const_string S >
        struct split_const_string_impl
        {
          private:
            template < std::size_t... Is >
            static consteval auto impl_( const std::index_sequence< Is... > ) noexcept
            {
                return type_list< value_identity< S[ Is ] >... >{};
            }
          public:
            using type = decltype( impl_( std::make_index_sequence< S.size() >{} ) );
        };
    }
    template < basic_const_string S >
    struct split_const_string final
    {
        using type = typename details::split_const_string_impl< S >::type;
    };
    template < basic_const_string S >
    using split_const_string_t = typename split_const_string< S >::type;
}
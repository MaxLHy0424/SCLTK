#pragma once
#include <concepts>
#include <numeric>
#include <type_traits>
namespace cpp_utils
{
    template < std::integral T >
    [[nodiscard]] inline constexpr auto is_prime_number( const T n ) noexcept
    {
        if ( n == 2 ) {
            return true;
        }
        if ( n < 2 ) {
            return false;
        }
        for ( T i{ 2 }; i * i <= n; ++i ) {
            if ( n % i == 0 ) {
                return false;
            }
        }
        return true;
    }
    template < std::integral T >
    [[nodiscard]] inline constexpr auto count_digits( const T n ) noexcept
    {
        using result_type = unsigned short;
        result_type count{ 1 };
        auto value{ +n };
        if constexpr ( std::is_signed_v< T > ) {
            while ( value <= -10 || value >= 10 ) {
                value /= 10;
                ++count;
            }
        } else {
            while ( value >= 10 ) {
                value /= 10;
                ++count;
            }
        }
        return count;
    }
    template < typename T >
    concept number = std::integral< T > || std::floating_point< T >;
    template < typename T >
    concept saturating_integer
      = std::integral< T > && !std::same_as< std::remove_cv_t< T >, bool > && !std::same_as< std::remove_cv_t< T >, char >
     && !std::same_as< std::remove_cv_t< T >, wchar_t > && !std::same_as< std::remove_cv_t< T >, char8_t >
     && !std::same_as< std::remove_cv_t< T >, char16_t > && !std::same_as< std::remove_cv_t< T >, char32_t >;
    template < saturating_integer T >
    class sat_num final
    {
      private:
        T data_;
        template < std::integral U >
        static constexpr auto to_stored_( const U n ) noexcept
        {
            using promoted_type = decltype( +n );
            return std::saturating_cast< T >( static_cast< promoted_type >( n ) );
        }
      public:
        using value_type = T;
        [[nodiscard]] constexpr auto data() const noexcept
        {
            return data_;
        }
        [[nodiscard]] constexpr operator T() const noexcept
        {
            return data_;
        }
        template < number U >
        [[nodiscard]] constexpr auto operator<=>( const sat_num< U >& src ) const noexcept
        {
            if constexpr ( std::integral< U > ) {
                return data_ <=> to_stored_( src.data() );
            } else {
                return data_ <=> src.data();
            }
        }
        template < number U >
        [[nodiscard]] constexpr auto operator<=>( const U n ) const noexcept
        {
            if constexpr ( std::integral< U > ) {
                return data_ <=> to_stored_( n );
            } else {
                return data_ <=> n;
            }
        }
        template < std::integral U >
        [[nodiscard]] constexpr auto operator+( const sat_num< U > src ) const noexcept
        {
            return sat_num< T >{ std::saturating_add( data_, to_stored_( src.data() ) ) };
        }
        template < std::integral U >
        [[nodiscard]] constexpr auto operator-( const sat_num< U > src ) const noexcept
        {
            return sat_num< T >{ std::saturating_sub( data_, to_stored_( src.data() ) ) };
        }
        template < std::integral U >
        [[nodiscard]] constexpr auto operator*( const sat_num< U > src ) const noexcept
        {
            return sat_num< T >{ std::saturating_mul( data_, to_stored_( src.data() ) ) };
        }
        template < std::integral U >
        [[nodiscard]] constexpr auto operator/( const sat_num< U > src ) const noexcept
        {
            return sat_num< T >{ std::saturating_div( data_, to_stored_( src.data() ) ) };
        }
        template < std::integral U >
        constexpr auto& operator+=( const sat_num< U > src ) noexcept
        {
            data_ = std::saturating_add( data_, to_stored_( src.data() ) );
            return *this;
        }
        template < std::integral U >
        constexpr auto& operator-=( const sat_num< U > src ) noexcept
        {
            data_ = std::saturating_sub( data_, to_stored_( src.data() ) );
            return *this;
        }
        template < std::integral U >
        constexpr auto& operator*=( const sat_num< U > src ) noexcept
        {
            data_ = std::saturating_mul( data_, to_stored_( src.data() ) );
            return *this;
        }
        template < std::integral U >
        constexpr auto& operator/=( const sat_num< U > src ) noexcept
        {
            data_ = std::saturating_div( data_, to_stored_( src.data() ) );
            return *this;
        }
        template < std::integral U >
        [[nodiscard]] constexpr auto operator+( const U n ) const noexcept
        {
            return sat_num< T >{ std::saturating_add( data_, to_stored_( n ) ) };
        }
        template < std::integral U >
        [[nodiscard]] constexpr auto operator-( const U n ) const noexcept
        {
            return sat_num< T >{ std::saturating_sub( data_, to_stored_( n ) ) };
        }
        template < std::integral U >
        [[nodiscard]] constexpr auto operator*( const U n ) const noexcept
        {
            return sat_num< T >{ std::saturating_mul( data_, to_stored_( n ) ) };
        }
        template < std::integral U >
        [[nodiscard]] constexpr auto operator/( const U n ) const noexcept
        {
            return sat_num< T >{ std::saturating_div( data_, to_stored_( n ) ) };
        }
        template < std::integral U >
        constexpr auto& operator+=( const U n ) noexcept
        {
            data_ = std::saturating_add( data_, to_stored_( n ) );
            return *this;
        }
        template < std::integral U >
        constexpr auto& operator-=( const U n ) noexcept
        {
            data_ = std::saturating_sub( data_, to_stored_( n ) );
            return *this;
        }
        template < std::integral U >
        constexpr auto& operator*=( const U n ) noexcept
        {
            data_ = std::saturating_mul( data_, to_stored_( n ) );
            return *this;
        }
        template < std::integral U >
        constexpr auto& operator/=( const U n ) noexcept
        {
            data_ = std::saturating_div( data_, to_stored_( n ) );
            return *this;
        }
        constexpr auto& operator=( const sat_num< T >& src ) noexcept
        {
            data_ = src.data_;
            return *this;
        }
        constexpr auto& operator=( sat_num< T >&& src ) noexcept
        {
            data_     = src.data_;
            src.data_ = {};
            return *this;
        }
        constexpr sat_num( const T n ) noexcept
          : data_{ n }
        { }
        constexpr sat_num( const sat_num< T >& src ) noexcept
          : data_{ src.data_ }
        { }
        constexpr sat_num( sat_num< T >&& src ) noexcept
          : data_{ src.data_ }
        {
            src.data_ = {};
        }
        ~sat_num() noexcept = default;
    };
    template < saturating_integer T >
    sat_num( T ) -> sat_num< std::decay_t< T > >;
}
#pragma once
#include <cmath>
#include <concepts>
#include <numeric>
namespace cpp_utils
{
    template < std::integral T >
    inline constexpr auto is_prime_number( const T n ) noexcept
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
    inline constexpr auto count_digits( const T n ) noexcept
    {
        using result_t = unsigned short;
        T abs_n{ n < 0 ? -n : n };
        if ( abs_n == 0 ) {
            return static_cast< result_t >( 1 );
        }
        result_t count{ 0 };
        while ( abs_n > 0 ) {
            abs_n /= 10;
            ++count;
        }
        return count;
    }
    template < typename T >
    concept number = std::integral< T > || std::floating_point< T > || std::same_as< T, std::decay< T > >;
    template < number T >
    class sat_num final
    {
      private:
        T data_;
      public:
        constexpr auto base() const noexcept
        {
            return data_;
        }
        constexpr operator T() const noexcept
        {
            return data_;
        }
        template < number U >
        constexpr auto operator<=>( const sat_num< U >& src ) const noexcept
        {
            if constexpr ( std::is_floating_point_v< U > ) {
                return data_ <=> static_cast< T >( src.base() );
            } else {
                return data_ <=> std::saturate_cast< T >( src.base() );
            }
        }
        template < number U >
        constexpr auto operator<=>( const U n ) const noexcept
        {
            if constexpr ( std::is_floating_point_v< U > ) {
                return data_ <=> static_cast< T >( n );
            } else {
                return data_ <=> std::saturate_cast< T >( n );
            }
        }
        template < number U >
        constexpr auto operator+( const sat_num< U > src ) const noexcept
        {
            return sat_num< T >{ std::add_sat< T >( data_, src.base() ) };
        }
        template < number U >
        constexpr auto operator-( const sat_num< U > src ) const noexcept
        {
            return sat_num< T >{ std::sub_sat< T >( data_, src.base() ) };
        }
        template < number U >
        constexpr auto operator*( const sat_num< U > src ) const noexcept
        {
            return sat_num< T >{ std::mul_sat< T >( data_, src.base() ) };
        }
        template < number U >
        constexpr auto operator/( const sat_num< U > src ) const noexcept
        {
            return sat_num< T >{ std::div_sat< T >( data_, src.base() ) };
        }
        template < number U >
        constexpr auto& operator+=( const sat_num< U > src ) noexcept
        {
            data_ = std::add_sat< T >( data_, src.base() );
            return *this;
        }
        template < number U >
        constexpr auto& operator-=( const sat_num< U > src ) noexcept
        {
            data_ = std::sub_sat< T >( data_, src.base() );
            return *this;
        }
        template < number U >
        constexpr auto& operator*=( const sat_num< U > src ) noexcept
        {
            data_ = std::mul_sat< T >( data_, src.base() );
            return *this;
        }
        template < number U >
        constexpr auto& operator/=( const sat_num< U > src ) noexcept
        {
            data_ = std::div_sat< T >( data_, src.base() );
            return *this;
        }
        template < number U >
        constexpr auto operator+( const U n ) const noexcept
        {
            return sat_num< T >{ std::add_sat< T >( data_, n ) };
        }
        template < number U >
        constexpr auto operator-( const U n ) const noexcept
        {
            return sat_num< T >{ std::sub_sat< T >( data_, n ) };
        }
        template < number U >
        constexpr auto operator*( const U n ) const noexcept
        {
            return sat_num< T >{ std::mul_sat< T >( data_, n ) };
        }
        template < number U >
        constexpr auto operator/( const U n ) const noexcept
        {
            return sat_num< T >{ std::div_sat< T >( data_, n ) };
        }
        template < number U >
        constexpr auto& operator+=( const U n ) noexcept
        {
            data_ = std::add_sat< T >( data_, n );
            return *this;
        }
        template < number U >
        constexpr auto& operator-=( const U n ) noexcept
        {
            data_ = std::sub_sat< T >( data_, n );
            return *this;
        }
        template < number U >
        constexpr auto& operator*=( const U n ) noexcept
        {
            data_ = std::mul_sat< T >( data_, n );
            return *this;
        }
        template < number U >
        constexpr auto& operator/=( const U n ) noexcept
        {
            data_ = std::div_sat< T >( data_, n );
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
        constexpr sat_num( const T n )
          : data_{ n }
        { }
        constexpr sat_num( const sat_num< T >& src )
          : data_{ src.data_ }
        { }
        constexpr sat_num( sat_num< T >&& src )
          : data_{ src.data_ }
        {
            src.data_ = {};
        }
        ~sat_num() = default;
    };
    template < typename T >
        requires std::integral< T > || std::floating_point< T >
    sat_num( T ) -> sat_num< std::decay_t< T > >;
}
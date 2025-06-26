#pragma once
#include <cmath>
#include <concepts>
#include <vector>
namespace cpp_utils
{
    template < std::integral T >
    inline constexpr auto is_prime( const T n ) noexcept
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
    template < std::unsigned_integral T >
    inline constexpr auto fib( const T n )
    {
        using result_t = unsigned long long;
        if ( n == 0 ) {
            return result_t{ 0 };
        }
        if ( n == 1 ) {
            return result_t{ 1 };
        }
        result_t f1{ 1 };
        result_t f2{ 0 };
        result_t fn{ 0 };
        for ( result_t i{ 2 }; i <= n; i++ ) {
            fn = f2 + f1;
            f2 = f1;
            f1 = fn;
        }
        return fn;
    }
    template < std::unsigned_integral T >
    inline constexpr auto catalan( const T n )
    {
        using result_t = unsigned long long;
        std::vector< result_t > f( n + 1, 0 );
        f[ 0 ] = 1;
        for ( result_t i{ 1 }; i <= n; ++i ) {
            for ( result_t j{ 0 }; j < i; ++j ) {
                f[ i ] += f[ j ] * f[ i - j - 1 ];
            }
        }
        return f[ n ];
    }
}
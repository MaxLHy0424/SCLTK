#pragma once
#include <cmath>
#include <concepts>
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
}
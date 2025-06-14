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
    inline constexpr auto count_digits( std::decay_t< T > n ) noexcept
    {
        if ( n < 0 ) {
            n = -n;
        }
        if ( n == 0 ) {
            return 1;
        }
        size_t count{ 0 };
        while ( n > 0 ) {
            n /= 10;
            ++count;
        }
        return count;
    }
}
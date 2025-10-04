#pragma once
#include <concepts>
#include <cstddef>
#include <type_traits>
#include <utility>
namespace cpp_utils
{
    namespace details
    {
        template < typename T >
        concept has_emplace_back = requires( T t, typename T::value_type&& v ) {
            t.emplace_back( std::forward< typename T::value_type >( v ) );
        };
        template < typename T >
        concept has_emplace_after = requires( T t, typename T::value_type&& v ) {
            t.emplace_after( t.before_begin(), std::forward< typename T::value_type >( v ) );
        };
        template < typename T >
        concept is_set_like = requires {
            typename T::key_type;
            typename T::value_type;
        } && std::same_as< typename T::key_type, typename T::value_type >;
        template < typename T >
        concept is_map_like = requires {
            typename T::key_type;
            typename T::mapped_type;
            typename T::value_type;
        } && std::same_as< typename T::value_type, std::pair< const typename T::key_type, typename T::mapped_type > >;
    }
    template < typename T, typename... Args >
        requires( requires { typename T::value_type; } && ( !details::is_set_like< T > ) && ( !details::is_map_like< T > )
                  && ( std::constructible_from< typename T::value_type, Args > && ... ) )
    [[nodiscard]] inline constexpr auto init_container_from_elements( Args&&... args )
    {
        T container;
        if constexpr ( requires { container.reserve( 1 ); } ) {
            container.reserve( sizeof...( Args ) );
        }
        if constexpr ( details::has_emplace_back< T > ) {
            ( container.emplace_back( std::forward< Args >( args ) ), ... );
        } else if constexpr ( details::has_emplace_after< T > ) {
            ( ( void ) container.emplace_after( container.before_begin(), std::forward< Args >( args ) ), ... );
        } else if constexpr ( requires { ( container.emplace( std::forward< Args >( args ) ), ... ); } ) {
            ( container.emplace( std::forward< Args >( args ) ), ... );
        } else {
            static_assert( false, "cannot construct elements for this container type!" );
        }
        return container;
    }
    template < typename T, typename... Args >
        requires( details::is_set_like< T > && ( std::constructible_from< typename T::key_type, Args > && ... ) )
    [[nodiscard]] inline constexpr auto init_container_from_elements( Args&&... args )
    {
        T container;
        if constexpr ( requires { container.reserve( 1 ); } ) {
            container.reserve( sizeof...( Args ) );
        }
        ( container.emplace( std::forward< Args >( args ) ), ... );
        return container;
    }
    template < typename T, typename... Args >
        requires( details::is_map_like< T > && ( std::constructible_from< typename T::value_type, Args > && ... ) )
    [[nodiscard]] inline constexpr auto init_container_from_elements( Args&&... args )
    {
        T container;
        if constexpr ( requires { container.reserve( 1 ); } ) {
            container.reserve( sizeof...( Args ) );
        }
        ( container.emplace( std::forward< Args >( args ) ), ... );
        return container;
    }
    template < typename... Ts >
    struct overloads final : Ts...
    {
        using Ts::operator()...;
    };
    template < typename... Ts >
    overloads( Ts... ) -> overloads< Ts... >;
}
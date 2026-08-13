#pragma once
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <optional>
#include <ranges>
#include <thread>
#include <utility>
#include <vector>
namespace cpp_utils
{
    using nproc_t = decltype( std::thread::hardware_concurrency() );
    template < std::random_access_iterator It, std::sentinel_for< It > W, typename F >
        requires std::invocable< F, decltype( *std::declval< It >() ) > && std::copyable< std::decay_t< F > >
    [[nodiscard]] inline auto create_parallel_task( const nproc_t nproc, It&& begin, W&& end, F&& func )
      -> std::optional< std::vector< std::thread > >
    {
        if ( begin == end || nproc == 0 ) [[unlikely]] {
            return std::nullopt;
        }
        const auto total{ static_cast< std::ptrdiff_t >( std::ranges::distance( begin, end ) ) };
        const auto nproc_for_executing{ std::ranges::min( static_cast< std::ptrdiff_t >( nproc ), total ) };
        const auto chunk_size{ total / nproc_for_executing };
        const auto remainder{ total % nproc_for_executing };
        std::vector< std::thread > threads;
        threads.reserve( nproc_for_executing );
        for ( std::ptrdiff_t i{ 0 }; i < nproc_for_executing; ++i ) {
            auto chunk_start{ begin + i * chunk_size + std::ranges::min( i, remainder ) };
            auto chunk_end{ chunk_start + chunk_size + ( i < remainder ? 1 : 0 ) };
            threads.emplace_back( [ =, func ] mutable
            {
                for ( auto& it{ chunk_start }; it != chunk_end; ++it ) {
                    func( *it );
                }
            } );
        }
        return threads;
    }
    template < std::random_access_iterator It, std::sentinel_for< It > W, typename F >
        requires std::invocable< F, decltype( *std::declval< It >() ) > && std::copyable< std::decay_t< F > >
    [[nodiscard]] inline auto create_parallel_task( It&& begin, W&& end, F&& func )
    {
        return create_parallel_task(
          std::ranges::max( std::thread::hardware_concurrency(), 2u ), std::forward< It >( begin ), std::forward< W >( end ),
          std::forward< F >( func ) );
    }
    template < std::random_access_iterator It, std::sentinel_for< It > W, typename F >
        requires std::invocable< F, decltype( *std::declval< It >() ) > && std::copyable< std::decay_t< F > >
    inline auto parallel_for_each( const nproc_t nproc, It&& begin, W&& end, F&& func )
    {
        auto threads{
          create_parallel_task( nproc, std::forward< It >( begin ), std::forward< W >( end ), std::forward< F >( func ) ) };
        if ( !threads.has_value() ) [[unlikely]] {
            return;
        }
        for ( auto& thread : threads.value() ) {
            if ( thread.joinable() ) {
                thread.join();
            }
        }
    }
    template < std::random_access_iterator It, std::sentinel_for< It > W, typename F >
        requires std::invocable< F, decltype( *std::declval< It >() ) > && std::copyable< std::decay_t< F > >
    inline auto parallel_for_each( It&& begin, W&& end, F&& func )
    {
        parallel_for_each(
          std::ranges::max( std::thread::hardware_concurrency(), 2u ), std::forward< It >( begin ), std::forward< W >( end ),
          std::forward< F >( func ) );
    }
}
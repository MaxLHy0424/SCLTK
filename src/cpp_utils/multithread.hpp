#pragma once
#include <algorithm>
#include <deque>
#include <iterator>
#include <print>
#include <ranges>
#include <thread>
#include <utility>
#include <vector>
#include "compiler.hpp"
namespace cpp_utils
{
    using thread_num_t = decltype( std::thread::hardware_concurrency() );
    template < std::random_access_iterator It, typename F >
        requires std::invocable< F, decltype( *std::declval< It >() ) >
    inline auto parallel_for_each( thread_num_t thread_num, It&& begin, It&& end, F&& func )
    {
        if ( thread_num == 0 ) {
            if constexpr ( is_debugging_build ) {
                std::print( "'thread_num' cannot be zero!\n" );
                std::terminate();
            } else {
                std::unreachable();
            }
        }
        if ( begin == end ) {
            return;
        }
        const auto total{ std::distance( begin, end ) };
        thread_num = std::min< thread_num_t >( thread_num, total );
        const auto chunk_size{ total / thread_num };
        const auto remainder{ total % thread_num };
        std::vector< std::thread > threads;
        threads.reserve( thread_num );
        for ( const auto i : std::ranges::iota_view{ thread_num_t{ 0 }, thread_num } ) {
            const auto chunk_start{ begin + i * chunk_size + std::min< thread_num_t >( i, remainder ) };
            const auto chunk_end{ chunk_start + chunk_size + ( i < remainder ? 1 : 0 ) };
            threads.emplace_back( [ =, &func ]
            {
                for ( auto it : std::ranges::iota_view{ chunk_start, chunk_end } ) {
                    func( *it );
                }
            } );
        }
        for ( auto& thread : threads ) {
            thread.join();
        }
    }
    template < std::random_access_iterator It, typename F >
        requires std::invocable< F, decltype( *std::declval< It >() ) >
    inline auto parallel_for_each( It&& begin, It&& end, F&& func )
    {
        parallel_for_each(
          std::max( std::thread::hardware_concurrency(), 2U ), std::forward< It >( begin ), std::forward< It >( end ),
          std::forward< F >( func ) );
    }
    class thread_manager final
    {
      private:
        std::deque< std::jthread > threads_{};
      public:
        constexpr auto empty() const noexcept
        {
            return threads_.empty();
        }
        constexpr auto size() const noexcept
        {
            return threads_.size();
        }
        constexpr auto max_size() const noexcept
        {
            return threads_.max_size();
        }
        auto& optimize_storage() noexcept
        {
            threads_.shrink_to_fit();
            return *this;
        }
        auto& swap( thread_manager& src ) noexcept
        {
            threads_.swap( src.threads_ );
            return *this;
        }
        template < typename F, typename... Args >
            requires std::invocable< F, Args... > || std::invocable< F, std::stop_token, Args... >
        auto& add( F&& func, Args&&... args )
        {
            threads_.emplace_back( std::forward< F >( func ), std::forward< Args >( args )... );
            return *this;
        }
        auto& join()
        {
            for ( auto& thread : threads_ ) {
                thread.join();
            }
            return *this;
        }
        auto& safe_join() noexcept
        {
            for ( auto& thread : threads_ ) {
                if ( thread.joinable() ) {
                    thread.join();
                }
            }
            return *this;
        }
        auto& detach()
        {
            for ( auto& thread : threads_ ) {
                thread.detach();
            }
            return *this;
        }
        auto& safe_detach() noexcept
        {
            for ( auto& thread : threads_ ) {
                if ( thread.joinable() ) {
                    thread.detach();
                }
            }
            return *this;
        }
        auto& request_stop() noexcept
        {
            for ( auto& thread : threads_ ) {
                thread.request_stop();
            }
            return *this;
        }
        auto operator=( const thread_manager& ) -> thread_manager& = delete;
        auto operator=( thread_manager&& ) -> thread_manager&      = default;
        thread_manager() noexcept                                  = default;
        thread_manager( const thread_manager& )                    = delete;
        thread_manager( thread_manager&& ) noexcept                = default;
        ~thread_manager()                                          = default;
    };
}
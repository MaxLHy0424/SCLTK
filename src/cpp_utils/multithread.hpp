#pragma once
#include <algorithm>
#include <deque>
#include <iterator>
#include <ranges>
#include <thread>
#include <utility>
#include <vector>
namespace cpp_utils {
    using thread_num_type = unsigned int;
    template < std::random_access_iterator Iter, typename Invocable >
        requires std::invocable< Invocable, decltype( *std::declval< Iter >() ) >
    inline auto parallel_for_each_impl( thread_num_type thread_num, Iter &&begin, Iter &&end, Invocable &&func )
    {
        if ( thread_num == 0 ) {
            std::unreachable();
        }
        if ( begin == end ) {
            return;
        }
        const auto total{ end - begin };
        thread_num = std::min< thread_num_type >( thread_num, total );
        const auto chunk_size{ total / thread_num };
        const auto remainder{ total % thread_num };
        std::vector< std::thread > threads;
        threads.reserve( thread_num );
        for ( const auto i : std::ranges::iota_view{ 0U, thread_num } ) {
            const auto chunk_start{ begin + i * chunk_size + std::min< thread_num_type >( i, remainder ) };
            const auto chunk_end{ chunk_start + chunk_size + ( i < remainder ? 1 : 0 ) };
            threads.emplace_back( [ =, func = std::forward< Invocable >( func ) ]
            {
                for ( auto it = chunk_start; it != chunk_end; ++it ) {
                    func( *it );
                }
            } );
        }
        for ( auto &thread : threads ) {
            thread.join();
        }
    }
    template < std::random_access_iterator Iter, typename Invocable >
        requires std::invocable< Invocable, decltype( *std::declval< Iter >() ) >
    inline auto parallel_for_each( Iter &&begin, Iter &&end, Invocable &&func )
    {
        parallel_for_each_impl(
          std::max( std::thread::hardware_concurrency(), 2U ), std::forward< Iter >( begin ), std::forward< Iter >( end ),
          std::forward< Invocable >( func ) );
    }
    class thread_manager final {
      private:
        std::deque< std::jthread > threads_{};
      public:
        auto empty() const noexcept
        {
            return threads_.empty();
        }
        auto size() const noexcept
        {
            return threads_.size();
        }
        auto max_size() const noexcept
        {
            return threads_.max_size();
        }
        auto &optimize_storage() noexcept
        {
            threads_.shrink_to_fit();
            return *this;
        }
        auto &swap( thread_manager &src ) noexcept
        {
            threads_.swap( src.threads_ );
            return *this;
        }
        template < typename Invocable, typename... Args >
            requires std::invocable< Invocable, Args... > || std::invocable< Invocable, std::stop_token, Args... >
        auto &add( Invocable &&func, Args &&...args )
        {
            threads_.emplace_back( std::forward< Invocable >( func ), std::forward< Args >( args )... );
            return *this;
        }
        auto &join()
        {
            for ( auto &thread : threads_ ) {
                thread.join();
            }
            return *this;
        }
        auto &safe_join() noexcept
        {
            for ( auto &thread : threads_ ) {
                if ( thread.joinable() ) {
                    thread.join();
                }
            }
            return *this;
        }
        auto &detach()
        {
            for ( auto &thread : threads_ ) {
                thread.detach();
            }
            return *this;
        }
        auto &safe_detach() noexcept
        {
            for ( auto &thread : threads_ ) {
                if ( thread.joinable() ) {
                    thread.detach();
                }
            }
            return *this;
        }
        auto &request_stop() noexcept
        {
            for ( auto &thread : threads_ ) {
                thread.request_stop();
            }
            return *this;
        }
        auto operator=( const thread_manager & ) -> thread_manager & = delete;
        auto operator=( thread_manager && ) -> thread_manager &      = default;
        thread_manager() noexcept                                    = default;
        thread_manager( const thread_manager & )                     = delete;
        thread_manager( thread_manager && ) noexcept                 = default;
        ~thread_manager()                                            = default;
    };
}
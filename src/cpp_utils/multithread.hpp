#pragma once
#include <algorithm>
#include <deque>
#include <iterator>
#include <ranges>
#include <thread>
#include <utility>
#include <vector>
namespace cpp_utils {
    template < std::random_access_iterator _iterator_, typename _callable_ >
    inline auto parallel_for_each_impl( unsigned int _thread_num, _iterator_ &&_begin, _iterator_ &&_end, _callable_ &&_func )
    {
        [[assume( _thread_num > 0 )]];
        const auto chunk_size{ ( _end - _begin ) / _thread_num };
        std::vector< std::thread > threads;
        threads.reserve( _thread_num );
        for ( const auto i : std::ranges::iota_view{ decltype( _thread_num ){ 0 }, _thread_num } ) {
            const auto chunk_start{ _begin + i * chunk_size };
            const auto chunk_end{ ( i == _thread_num - 1 ) ? _end : chunk_start + chunk_size };
            threads.emplace_back( [ =, &_func ]()
            {
                for ( auto it{ chunk_start }; it != chunk_end; ++it ) {
                    _func( *it );
                }
                std::this_thread::yield();
            } );
        }
        for ( auto &thread : threads ) {
            thread.join();
        }
    }
    template < std::random_access_iterator _iterator_, typename _callable_ >
    inline auto parallel_for_each( _iterator_ &&_begin, _iterator_ &&_end, _callable_ &&_func )
    {
        parallel_for_each_impl(
          std::ranges::max( std::thread::hardware_concurrency(), 1U ), std::forward< _iterator_ >( _begin ),
          std::forward< _iterator_ >( _end ), std::forward< _callable_ >( _func ) );
    }
    class thread_pool final {
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
        auto &swap( thread_pool &_src ) noexcept
        {
            threads_.swap( _src.threads_ );
            return *this;
        }
        template < typename _func_, typename... _args_ >
        auto &add( _func_ &&_func, _args_ &&..._args )
        {
            threads_.emplace_back( std::forward< _func_ >( _func ), std::forward< _args_ >( _args )... );
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
        auto operator=( const thread_pool & ) -> thread_pool & = delete;
        auto operator=( thread_pool && ) -> thread_pool &      = default;
        thread_pool() noexcept                                 = default;
        thread_pool( const thread_pool & )                     = delete;
        thread_pool( thread_pool && ) noexcept                 = default;
        ~thread_pool()                                         = default;
    };
}
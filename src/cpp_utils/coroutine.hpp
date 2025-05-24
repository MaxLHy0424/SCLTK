#pragma once
#include <concepts>
#include <coroutine>
#include <exception>
#include <iterator>
#include <optional>
#include <utility>
namespace cpp_utils {
    template < typename T >
    concept coroutine_func_return_type = requires { typename T::promise_type; };
    template < std::movable T >
    class coroutine final {
      public:
        struct promise_type;
      private:
        using handle_ = std::coroutine_handle< promise_type >;
        handle_ coroutine_handle_{};
      public:
        struct promise_type final {
            std::optional< T > current_value{};
            std::exception_ptr current_exception{};
            auto get_return_object() noexcept
            {
                return coroutine< T >{ handle_::from_promise( *this ) };
            }
            static auto initial_suspend() noexcept
            {
                return std::suspend_always{};
            }
            static auto final_suspend() noexcept
            {
                return std::suspend_always{};
            }
            template < typename... Args >
            auto yield_value( Args &&...args )
            {
                current_value.emplace( std::forward< Args >( args )... );
                return std::suspend_always{};
            }
            template < typename... Args >
            auto return_value( Args &&...args )
            {
                current_value.emplace( std::forward< Args >( args )... );
                return std::suspend_always{};
            }
            auto unhandled_exception() noexcept
            {
                current_exception = std::current_exception();
            }
        };
        class iterator final {
          private:
            const handle_ coroutine_handle_;
          public:
            auto operator++() -> coroutine< T >::iterator &
            {
                coroutine_handle_.resume();
                return *this;
            }
            auto operator++( int ) -> coroutine< T >::iterator
            {
                coroutine_handle_.resume();
                return *this;
            }
            auto &operator*()
            {
                return coroutine_handle_.promise().current_value.value();
            }
            const auto &operator*() const
            {
                return coroutine_handle_.promise().current_value.value();
            }
            auto operator==( std::default_sentinel_t ) const
            {
                return !coroutine_handle_ || coroutine_handle_.done();
            }
            auto operator=( const iterator & ) -> iterator &     = default;
            auto operator=( iterator && ) noexcept -> iterator & = default;
            iterator( const handle_ coroutine_handle ) noexcept
              : coroutine_handle_{ coroutine_handle }
            { }
            iterator( const iterator & ) noexcept = default;
            iterator( iterator && ) noexcept      = default;
            ~iterator() noexcept                  = default;
        };
        auto empty() const noexcept
        {
            return coroutine_handle_ == nullptr;
        }
        auto done() const noexcept
        {
            return coroutine_handle_.done();
        }
        auto address() const noexcept
        {
            return coroutine_handle_.address();
        }
        auto &destroy() const
        {
            coroutine_handle_.destroy();
            return *this;
        }
        auto &safe_destroy() noexcept
        {
            if ( !empty() ) {
                destroy();
                coroutine_handle_ = {};
            }
            return *this;
        }
        auto &reset( coroutine< T > &&src ) noexcept
        {
            if ( this != &src ) [[likely]] {
                if ( !empty() ) {
                    destroy();
                }
                coroutine_handle_     = src.coroutine_handle_;
                src.coroutine_handle_ = {};
            }
            return *this;
        }
        auto has_exception() const noexcept
        {
            return coroutine_handle_.promise().current_exception != nullptr;
        }
        [[noreturn]] auto rethrow_exception() const
        {
            std::rethrow_exception( coroutine_handle_.promise().current_exception );
        }
        auto safe_rethrow_exception() const
        {
            if ( has_exception() ) {
                std::rethrow_exception( coroutine_handle_.promise().current_exception );
            }
        }
        auto &resume() const
        {
            coroutine_handle_.resume();
            safe_rethrow_exception();
            return *this;
        }
        auto &safe_resume() const
        {
            if ( empty() ) {
                return *this;
            }
            if ( !done() ) {
                resume();
                safe_rethrow_exception();
            }
            return *this;
        }
        auto copy_value() const
        {
            return coroutine_handle_.promise().current_value.value();
        }
        auto &reference_value()
        {
            return coroutine_handle_.promise().current_value.value();
        }
        const auto &const_reference_value() const
        {
            return coroutine_handle_.promise().current_value.value();
        }
        auto &&move_value()
        {
            return std::move( coroutine_handle_.promise().current_value.value() );
        }
        auto begin()
        {
            if ( !empty() ) {
                coroutine_handle_.resume();
            }
            return iterator{ coroutine_handle_ };
        }
        auto end() noexcept
        {
            return std::default_sentinel_t{};
        }
        auto operator=( const coroutine< T > & ) -> coroutine< T > & = delete;
        auto operator=( coroutine< T > &&src ) noexcept -> coroutine< T > &
        {
            return reset( std::move( src ) );
        }
        coroutine() noexcept = default;
        coroutine( const handle_ coroutine_handle ) noexcept
          : coroutine_handle_{ coroutine_handle }
        { }
        coroutine( const coroutine< T > & ) = delete;
        coroutine( coroutine< T > &&src ) noexcept
          : coroutine_handle_{ src.coroutine_handle_ }
        {
            src.coroutine_handle_ = {};
        }
        ~coroutine() noexcept
        {
            if ( !empty() ) {
                destroy();
            }
        }
    };
    class coroutine_void final {
      public:
        struct promise_type;
      private:
        using handle_ = std::coroutine_handle< promise_type >;
        handle_ coroutine_handle_{};
      public:
        struct promise_type final {
            std::exception_ptr current_exception{};
            auto get_return_object() noexcept
            {
                return coroutine_void{ handle_::from_promise( *this ) };
            }
            static auto initial_suspend() noexcept
            {
                return std::suspend_always{};
            }
            static auto final_suspend() noexcept
            {
                return std::suspend_always{};
            }
            static auto return_void() noexcept { }
            auto unhandled_exception() noexcept
            {
                current_exception = std::current_exception();
            }
        };
        auto empty() const noexcept
        {
            return coroutine_handle_ == nullptr;
        }
        auto done() const noexcept
        {
            return coroutine_handle_.done();
        }
        auto address() const noexcept
        {
            return coroutine_handle_.address();
        }
        auto &destroy() const
        {
            coroutine_handle_.destroy();
            return *this;
        }
        auto &safe_destroy() noexcept
        {
            if ( !empty() ) {
                destroy();
                coroutine_handle_ = {};
            }
            return *this;
        }
        auto &reset( coroutine_void &&src ) noexcept
        {
            if ( this != &src ) [[likely]] {
                if ( !empty() ) {
                    destroy();
                }
                coroutine_handle_     = src.coroutine_handle_;
                src.coroutine_handle_ = {};
            }
            return *this;
        }
        auto has_exception() const noexcept
        {
            return coroutine_handle_.promise().current_exception != nullptr;
        }
        [[noreturn]] auto rethrow_exception() const
        {
            std::rethrow_exception( coroutine_handle_.promise().current_exception );
        }
        auto safe_rethrow_exception() const
        {
            if ( has_exception() ) {
                std::rethrow_exception( coroutine_handle_.promise().current_exception );
            }
        }
        auto &resume() const
        {
            coroutine_handle_.resume();
            safe_rethrow_exception();
            return *this;
        }
        auto &safe_resume() const
        {
            if ( empty() ) {
                return *this;
            }
            if ( !done() ) {
                resume();
                safe_rethrow_exception();
            }
            return *this;
        }
        auto operator=( const coroutine_void & ) -> coroutine_void & = delete;
        auto operator=( coroutine_void &&src ) noexcept -> coroutine_void &
        {
            return reset( std::move( src ) );
        }
        coroutine_void() noexcept = default;
        coroutine_void( const handle_ coroutine_handle ) noexcept
          : coroutine_handle_{ coroutine_handle }
        { }
        coroutine_void( const coroutine_void & ) = delete;
        coroutine_void( coroutine_void &&src ) noexcept
          : coroutine_handle_{ src.coroutine_handle_ }
        {
            src.coroutine_handle_ = {};
        }
        ~coroutine_void() noexcept
        {
            if ( !empty() ) {
                destroy();
            }
        }
    };
}
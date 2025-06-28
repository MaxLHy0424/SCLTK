#pragma once
#include <concepts>
#include <coroutine>
#include <exception>
#include <iterator>
#include <memory>
#include <utility>
namespace cpp_utils
{
    template < typename T >
    concept coroutine_func_return_t = requires { typename T::promise_type; };
    namespace details
    {
        template < typename R, template < typename > typename Coroutine >
        struct coroutine_promise final
        {
            std::unique_ptr< R > current_value{};
            std::exception_ptr current_exception{};
            auto get_return_object() noexcept
            {
                return Coroutine< R >{ std::coroutine_handle< coroutine_promise< R, Coroutine > >::from_promise( *this ) };
            }
            static auto initial_suspend() noexcept
            {
                return std::suspend_always{};
            }
            static auto final_suspend() noexcept
            {
                return std::suspend_always{};
            }
            auto yield_value( const R& args )
            {
                current_value = std::make_unique< R >( args );
                return std::suspend_always{};
            }
            auto yield_value( R&& args )
            {
                current_value = std::make_unique< R >( std::move( args ) );
                return std::suspend_always{};
            }
            auto return_value( const R& args )
            {
                current_value = std::make_unique< R >( args );
                return std::suspend_always{};
            }
            auto return_value( R&& args )
            {
                current_value = std::make_unique< R >( std::move( args ) );
                return std::suspend_always{};
            }
            auto unhandled_exception() noexcept
            {
                current_exception = std::current_exception();
            }
            auto& get_value()
            {
                return *current_value;
            }
        };
        template < typename R, template < typename > typename Coroutine >
        struct coroutine_promise_noexcept final
        {
            std::unique_ptr< R > current_value{};
            auto get_return_object() noexcept
            {
                return Coroutine< R >{ std::coroutine_handle< coroutine_promise_noexcept< R, Coroutine > >::from_promise( *this ) };
            }
            static auto initial_suspend() noexcept
            {
                return std::suspend_always{};
            }
            static auto final_suspend() noexcept
            {
                return std::suspend_always{};
            }
            auto yield_value( const R& args )
            {
                current_value = std::make_unique< R >( args );
                return std::suspend_always{};
            }
            auto yield_value( R&& args )
            {
                current_value = std::make_unique< R >( std::move( args ) );
                return std::suspend_always{};
            }
            auto return_value( const R& args )
            {
                current_value = std::make_unique< R >( args );
                return std::suspend_always{};
            }
            auto return_value( R&& args )
            {
                current_value = std::make_unique< R >( std::move( args ) );
                return std::suspend_always{};
            }
            static auto unhandled_exception() noexcept
            { }
            auto& get_value()
            {
                return *current_value;
            }
        };
        template < template < typename > typename Coroutine >
        struct coroutine_promise< void, Coroutine > final
        {
            std::exception_ptr current_exception{};
            auto get_return_object() noexcept
            {
                return Coroutine< void >{ std::coroutine_handle< coroutine_promise< void, Coroutine > >::from_promise( *this ) };
            }
            static auto initial_suspend() noexcept
            {
                return std::suspend_always{};
            }
            static auto final_suspend() noexcept
            {
                return std::suspend_always{};
            }
            static auto return_void() noexcept
            { }
            auto unhandled_exception() noexcept
            {
                current_exception = std::current_exception();
            }
        };
        template < template < typename > typename Coroutine >
        struct coroutine_promise_noexcept< void, Coroutine > final
        {
            auto get_return_object() noexcept
            {
                return Coroutine< void >{
                  std::coroutine_handle< coroutine_promise_noexcept< void, Coroutine > >::from_promise( *this ) };
            }
            static auto initial_suspend() noexcept
            {
                return std::suspend_always{};
            }
            static auto final_suspend() noexcept
            {
                return std::suspend_always{};
            }
            static auto unhandled_exception() noexcept
            { }
            static auto return_void() noexcept
            { }
        };
        template < typename Handle >
            requires( requires( Handle v ) { v.promise(); } )
        class coroutine_iterator final
        {
          private:
            const Handle coroutine_handle_;
          public:
            auto operator++() -> coroutine_iterator< Handle >&
            {
                coroutine_handle_.resume();
                return *this;
            }
            auto operator++( int ) -> coroutine_iterator< Handle >
            {
                coroutine_handle_.resume();
                return *this;
            }
            auto& operator*()
            {
                return coroutine_handle_.promise().get_value();
            }
            const auto& operator*() const
            {
                return coroutine_handle_.promise().get_value();
            }
            auto operator==( std::default_sentinel_t ) const noexcept
            {
                return !coroutine_handle_ || coroutine_handle_.done();
            }
            auto operator=( const coroutine_iterator< Handle >& ) -> coroutine_iterator< Handle >&     = default;
            auto operator=( coroutine_iterator< Handle >&& ) noexcept -> coroutine_iterator< Handle >& = default;
            coroutine_iterator( const Handle coroutine_handle ) noexcept
              : coroutine_handle_{ coroutine_handle }
            { }
            coroutine_iterator( const coroutine_iterator< Handle >& ) noexcept = default;
            coroutine_iterator( coroutine_iterator< Handle >&& ) noexcept      = default;
            ~coroutine_iterator() noexcept                                     = default;
        };
    }
    template < typename R >
    class coroutine final
    {
      public:
        using promise_type = details::coroutine_promise< R, coroutine >;
      private:
        using handle_ = std::coroutine_handle< promise_type >;
        handle_ coroutine_handle_{};
      public:
        using iterator = details::coroutine_iterator< handle_ >;
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
        auto& destroy() const
        {
            coroutine_handle_.destroy();
            return *this;
        }
        auto& safe_destroy() noexcept
        {
            if ( !empty() ) {
                destroy();
                coroutine_handle_ = {};
            }
            return *this;
        }
        auto& reset( coroutine< R >&& src ) noexcept
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
        auto rethrow_exception_if_have() const
        {
            if ( has_exception() ) {
                std::rethrow_exception( coroutine_handle_.promise().current_exception );
            }
        }
        auto& resume() const
        {
            coroutine_handle_.resume();
            rethrow_exception_if_have();
            return *this;
        }
        auto& resume_if_valid() const
        {
            if ( empty() ) {
                return *this;
            }
            if ( !done() ) {
                resume();
            }
            return *this;
        }
        auto&& get_value()
        {
            return std::move( coroutine_handle_.promise().get_value() );
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
        auto operator=( const coroutine< R >& ) -> coroutine< R >& = delete;
        auto operator=( coroutine< R >&& src ) noexcept -> coroutine< R >&
        {
            return reset( std::move( src ) );
        }
        coroutine() noexcept = default;
        coroutine( const handle_ coroutine_handle ) noexcept
          : coroutine_handle_{ coroutine_handle }
        { }
        coroutine( const coroutine< R >& ) = delete;
        coroutine( coroutine< R >&& src ) noexcept
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
    template <>
    class coroutine< void > final
    {
      public:
        using promise_type = details::coroutine_promise< void, coroutine >;
      private:
        using handle_ = std::coroutine_handle< promise_type >;
        handle_ coroutine_handle_{};
      public:
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
        auto& destroy() const
        {
            coroutine_handle_.destroy();
            return *this;
        }
        auto& safe_destroy() noexcept
        {
            if ( !empty() ) {
                destroy();
                coroutine_handle_ = {};
            }
            return *this;
        }
        auto& reset( coroutine< void >&& src ) noexcept
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
        auto rethrow_exception_if_have() const
        {
            if ( has_exception() ) {
                std::rethrow_exception( coroutine_handle_.promise().current_exception );
            }
        }
        auto& resume() const
        {
            coroutine_handle_.resume();
            rethrow_exception_if_have();
            return *this;
        }
        auto& resume_if_valid() const
        {
            if ( empty() ) {
                return *this;
            }
            if ( !done() ) {
                resume();
            }
            return *this;
        }
        auto operator=( const coroutine< void >& ) -> coroutine< void >& = delete;
        auto operator=( coroutine< void >&& src ) noexcept -> coroutine< void >&
        {
            return reset( std::move( src ) );
        }
        coroutine() noexcept = default;
        coroutine( const handle_ coroutine_handle ) noexcept
          : coroutine_handle_{ coroutine_handle }
        { }
        coroutine( const coroutine< void >& ) = delete;
        coroutine( coroutine< void >&& src ) noexcept
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
    template < typename R >
    class coroutine_noexcept final
    {
      public:
        using promise_type = details::coroutine_promise_noexcept< R, coroutine_noexcept >;
      private:
        using handle_ = std::coroutine_handle< promise_type >;
        handle_ coroutine_handle_{};
      public:
        using iterator = details::coroutine_iterator< handle_ >;
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
        auto& destroy() const
        {
            coroutine_handle_.destroy();
            return *this;
        }
        auto& safe_destroy() noexcept
        {
            if ( !empty() ) {
                destroy();
                coroutine_handle_ = {};
            }
            return *this;
        }
        auto& reset( coroutine_noexcept< R >&& src ) noexcept
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
        auto& resume() const
        {
            coroutine_handle_.resume();
            return *this;
        }
        auto& resume_if_valid() const
        {
            if ( empty() ) {
                return *this;
            }
            if ( !done() ) {
                resume();
            }
            return *this;
        }
        auto&& get_value()
        {
            return std::move( coroutine_handle_.promise().get_value() );
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
        auto operator=( const coroutine_noexcept< R >& ) -> coroutine_noexcept< R >& = delete;
        auto operator=( coroutine_noexcept< R >&& src ) noexcept -> coroutine_noexcept< R >&
        {
            return reset( std::move( src ) );
        }
        coroutine_noexcept() noexcept = default;
        coroutine_noexcept( const handle_ coroutine_handle ) noexcept
          : coroutine_handle_{ coroutine_handle }
        { }
        coroutine_noexcept( const coroutine_noexcept< R >& ) = delete;
        coroutine_noexcept( coroutine_noexcept< R >&& src ) noexcept
          : coroutine_handle_{ src.coroutine_handle_ }
        {
            src.coroutine_handle_ = {};
        }
        ~coroutine_noexcept() noexcept
        {
            if ( !empty() ) {
                destroy();
            }
        }
    };
    template <>
    class coroutine_noexcept< void > final
    {
      public:
        using promise_type = details::coroutine_promise_noexcept< void, coroutine_noexcept >;
      private:
        using handle_ = std::coroutine_handle< promise_type >;
        handle_ coroutine_handle_{};
      public:
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
        auto& destroy() const
        {
            coroutine_handle_.destroy();
            return *this;
        }
        auto& safe_destroy() noexcept
        {
            if ( !empty() ) {
                destroy();
                coroutine_handle_ = {};
            }
            return *this;
        }
        auto& reset( coroutine_noexcept< void >&& src ) noexcept
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
        auto& resume() const
        {
            coroutine_handle_.resume();
            return *this;
        }
        auto& resume_if_valid() const
        {
            if ( empty() ) {
                return *this;
            }
            if ( !done() ) {
                resume();
            }
            return *this;
        }
        auto operator=( const coroutine_noexcept< void >& ) -> coroutine_noexcept< void >& = delete;
        auto operator=( coroutine_noexcept< void >&& src ) noexcept -> coroutine_noexcept< void >&
        {
            return reset( std::move( src ) );
        }
        coroutine_noexcept() noexcept = default;
        coroutine_noexcept( const handle_ coroutine_handle ) noexcept
          : coroutine_handle_{ coroutine_handle }
        { }
        coroutine_noexcept( const coroutine_noexcept< void >& ) = delete;
        coroutine_noexcept( coroutine_noexcept< void >&& src ) noexcept
          : coroutine_handle_{ src.coroutine_handle_ }
        {
            src.coroutine_handle_ = {};
        }
        ~coroutine_noexcept() noexcept
        {
            if ( !empty() ) {
                destroy();
            }
        }
    };
};
#pragma once
#if defined( _WIN32 ) || defined( _WIN64 )
# include <windows.h>
#endif
#include <chrono>
#include <concepts>
#include <coroutine>
#include <deque>
#include <exception>
#include <functional>
#include <optional>
#include <print>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>
namespace cpp_utils {
    using io_buffer             = std::FILE;
    using size_type             = std::size_t;
    using nullptr_type          = std::nullptr_t;
    using ansi_char             = char;
    using ansi_std_string       = std::string;
    using ansi_std_string_view  = std::string_view;
    using wide_char             = wchar_t;
    using wide_std_string       = std::wstring;
    using wide_std_string_view  = std::wstring_view;
    using utf8_char             = char8_t;
    using utf8_std_string       = std::u8string;
    using utf8_std_string_view  = std::u8string_view;
    using utf16_char            = char16_t;
    using utf16_std_string      = std::u16string;
    using utf16_std_string_view = std::u16string_view;
    using utf32_char            = char32_t;
    using utf32_std_string      = std::u32string;
    using utf32_std_string_view = std::u32string_view;
    template < typename _type_ >
    using type_alloc = _type_;
    template < typename _char_type >
    using std_string = std::basic_string< _char_type >;
    template < typename _char_type >
    using std_string_view = std::basic_string_view< _char_type >;
    template < typename _type_ >
    concept char_type
      = std::same_as< std::decay_t< _type_ >, ansi_char > || std::same_as< std::decay_t< _type_ >, wide_char >
     || std::same_as< std::decay_t< _type_ >, utf8_char > || std::same_as< std::decay_t< _type_ >, utf16_char >
     || std::same_as< std::decay_t< _type_ >, utf32_char >;
    template < typename _type_ >
    concept pointer_type = std::is_pointer_v< _type_ >;
    template < typename _type_ >
    concept callable_type
      = !std::same_as< std::decay_t< _type_ >, std::thread > && !std::same_as< std::decay_t< _type_ >, std::jthread >;
    template < typename _type_ >
    concept std_chrono_type = requires {
        requires(
          std::same_as< std::decay_t< _type_ >, std::chrono::nanoseconds >
          || std::same_as< std::decay_t< _type_ >, std::chrono::microseconds >
          || std::same_as< std::decay_t< _type_ >, std::chrono::milliseconds >
          || std::same_as< std::decay_t< _type_ >, std::chrono::seconds >
          || std::same_as< std::decay_t< _type_ >, std::chrono::minutes >
          || std::same_as< std::decay_t< _type_ >, std::chrono::hours > || std::same_as< std::decay_t< _type_ >, std::chrono::days >
          || std::same_as< std::decay_t< _type_ >, std::chrono::weeks > || std::same_as< std::decay_t< _type_ >, std::chrono::months >
          || std::same_as< std::decay_t< _type_ >, std::chrono::years > );
    };
    template < pointer_type _type_ >
    inline auto ptr_to_string( const _type_ _ptr )
    {
        using namespace std::string_literals;
        return _ptr == nullptr ? "nullptr"s : std::format( "0x{:x}", reinterpret_cast< std::uintptr_t >( _ptr ) );
    }
    template < char_type _type_, size_type _capacity_ >
        requires( std::same_as< _type_, std::decay_t< _type_ > > && _capacity_ > 0 )
    struct constant_string final {
      private:
        _type_ data_[ _capacity_ ]{};
      public:
        auto c_str() const noexcept
        {
            return const_cast< const _type_ * >( data_ );
        }
        auto compare( const _type_ *const _src ) const noexcept
        {
            if ( _src == nullptr ) {
                return false;
            }
            size_type src_size{ 0 };
            while ( _src[ src_size ] != '\0' ) {
                ++src_size;
            }
            if ( src_size + 1 != _capacity_ ) {
                return false;
            }
            for ( size_type i{ 0 }; i < _capacity_; ++i ) {
                if ( data_[ i ] != _src[ i ] ) {
                    return false;
                }
            }
            return true;
        }
        template < size_type _src_capacity_ >
        auto compare( const _type_ ( &_src )[ _src_capacity_ ] ) const noexcept
        {
            if ( _src_capacity_ != _capacity_ ) {
                return false;
            }
            for ( size_type i{ 0 }; i < _capacity_; ++i ) {
                if ( data_[ i ] != _src[ i ] ) {
                    return false;
                }
            }
            return true;
        }
        template < size_type _src_capacity_ >
        auto compare( const constant_string< _type_, _src_capacity_ > &_src ) const noexcept
        {
            if ( _src_capacity_ != _capacity_ ) {
                return false;
            }
            for ( size_type i{ 0 }; i < _capacity_; ++i ) {
                if ( data_[ i ] != _src.data_[ i ] ) {
                    return false;
                }
            }
            return true;
        }
        auto operator==( const _type_ *const _src ) const noexcept
        {
            return compare( _src );
        }
        template < size_type _src_capacity_ >
        auto operator==( const _type_ ( &_src )[ _src_capacity_ ] ) const noexcept
        {
            return compare( _src );
        }
        template < size_type _src_capacity_ >
        auto operator==( const constant_string< _type_, _src_capacity_ > &_src ) const noexcept
        {
            return compare( _src );
        }
        const auto &operator[]( const size_type _index ) const noexcept
        {
            return data_[ _index ];
        }
        auto operator=( const constant_string< _type_, _capacity_ > & ) -> constant_string< _type_, _capacity_ > & = delete;
        auto operator=( constant_string< _type_, _capacity_ > && ) -> constant_string< _type_, _capacity_ > &      = delete;
        consteval constant_string( const _type_ ( &_str )[ _capacity_ ] ) noexcept
        {
            std::copy( _str, _str + _capacity_, data_ );
        }
        consteval constant_string( const constant_string< _type_, _capacity_ > & )     = default;
        consteval constant_string( constant_string< _type_, _capacity_ > && ) noexcept = delete;
        consteval ~constant_string() noexcept                                          = default;
    };
    template < size_type _capacity_ >
    using constant_ansi_string = constant_string< ansi_char, _capacity_ >;
    template < size_type _capacity_ >
    using constant_wide_string = constant_string< wide_char, _capacity_ >;
    template < size_type _capacity_ >
    using constant_utf8_string = constant_string< utf8_char, _capacity_ >;
    template < size_type _capacity_ >
    using constant_utf16_string = constant_string< utf16_char, _capacity_ >;
    template < size_type _capacity_ >
    using constant_utf32_string = constant_string< utf32_char, _capacity_ >;
    template < std::movable _type_ >
    class coroutine final {
      public:
        struct promise_type;
      private:
        using handle_      = std::coroutine_handle< promise_type >;
        using return_type_ = _type_;
        handle_ coroutine_handle_{};
      public:
        struct promise_type final {
            std::optional< return_type_ > current_value{ std::nullopt };
            std::exception_ptr current_exception{ nullptr };
            auto get_return_object() noexcept
            {
                return coroutine< return_type_ >{ handle_::from_promise( *this ) };
            }
            static auto initial_suspend() noexcept
            {
                return std::suspend_always{};
            }
            static auto final_suspend() noexcept
            {
                return std::suspend_always{};
            }
            auto yield_value( const return_type_ &_value ) noexcept
            {
                current_value = _value;
                return std::suspend_always{};
            }
            auto yield_value( return_type_ &&_value ) noexcept
            {
                current_value = std::move( _value );
                return std::suspend_always{};
            }
            auto return_value( const return_type_ &_value ) noexcept
            {
                current_value = _value;
                return std::suspend_always{};
            }
            auto return_value( return_type_ &&_value ) noexcept
            {
                current_value = std::move( _value );
                return std::suspend_always{};
            }
            auto unhandled_exception() noexcept
            {
                current_exception = std::current_exception();
            }
            auto await_transform() -> void                               = delete;
            auto operator=( const promise_type & ) -> promise_type &     = default;
            auto operator=( promise_type && ) noexcept -> promise_type & = default;
            promise_type() noexcept                                      = default;
            promise_type( const promise_type & ) noexcept                = default;
            promise_type( promise_type && ) noexcept                     = default;
            ~promise_type() noexcept                                     = default;
        };
        class iterator final {
          private:
            const handle_ coroutine_handle_;
          public:
            auto operator++() -> coroutine< return_type_ >::iterator &
            {
                coroutine_handle_.resume();
                return *this;
            }
            auto operator++( int ) -> coroutine< return_type_ >::iterator
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
            iterator( const handle_ _coroutine_handle ) noexcept
              : coroutine_handle_{ _coroutine_handle }
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
        auto destroy() const
        {
            coroutine_handle_.destroy();
        }
        auto &safe_destroy() noexcept
        {
            if ( !empty() ) {
                destroy();
                coroutine_handle_ = {};
            }
            return *this;
        }
        auto &reset( coroutine< return_type_ > &&_src ) noexcept
        {
            if ( this != &_src ) {
                if ( !empty() ) {
                    destroy();
                }
                coroutine_handle_      = _src.coroutine_handle_;
                _src.coroutine_handle_ = {};
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
        auto operator=( const coroutine< return_type_ > & ) -> coroutine< return_type_ > & = delete;
        auto operator=( coroutine< return_type_ > &&_src ) noexcept -> coroutine< return_type_ > &
        {
            reset( std::move( _src ) );
            return *this;
        }
        coroutine() noexcept = default;
        coroutine( const handle_ _coroutine_handle ) noexcept
          : coroutine_handle_{ _coroutine_handle }
        { }
        coroutine( const coroutine< return_type_ > & ) = delete;
        coroutine( coroutine< return_type_ > &&_src ) noexcept
          : coroutine_handle_{ _src.coroutine_handle_ }
        {
            _src.coroutine_handle_ = {};
        }
        ~coroutine() noexcept
        {
            if ( !empty() ) {
                destroy();
            }
        }
    };
    template < std::movable _type_ >
    class coroutine< std::optional< _type_ > > final {
      public:
        struct promise_type;
      private:
        using handle_      = std::coroutine_handle< promise_type >;
        using return_type_ = std::optional< _type_ >;
        handle_ coroutine_handle_{};
      public:
        struct promise_type final {
            return_type_ current_value{ std::nullopt };
            std::exception_ptr current_exception{ nullptr };
            auto get_return_object() noexcept
            {
                return coroutine< return_type_ >{ handle_::from_promise( *this ) };
            }
            static auto initial_suspend() noexcept
            {
                return std::suspend_always{};
            }
            static auto final_suspend() noexcept
            {
                return std::suspend_always{};
            }
            auto yield_value( const return_type_ &_value ) noexcept
            {
                current_value = _value;
                return std::suspend_always{};
            }
            auto yield_value( return_type_ &&_value ) noexcept
            {
                current_value = std::move( _value );
                return std::suspend_always{};
            }
            auto return_value( const return_type_ &_value ) noexcept
            {
                current_value = _value;
                return std::suspend_always{};
            }
            auto return_value( return_type_ &&_value ) noexcept
            {
                current_value = std::move( _value );
                return std::suspend_always{};
            }
            auto unhandled_exception() noexcept
            {
                current_exception = std::current_exception();
            }
            auto await_transform() -> void                               = delete;
            auto operator=( const promise_type & ) -> promise_type &     = default;
            auto operator=( promise_type && ) noexcept -> promise_type & = default;
            promise_type() noexcept                                      = default;
            promise_type( const promise_type & ) noexcept                = default;
            promise_type( promise_type && ) noexcept                     = default;
            ~promise_type() noexcept                                     = default;
        };
        class iterator final {
          private:
            const handle_ coroutine_handle_;
          public:
            auto operator++() -> coroutine< return_type_ >::iterator &
            {
                coroutine_handle_.resume();
                return *this;
            }
            auto operator++( int ) -> coroutine< return_type_ >::iterator
            {
                coroutine_handle_.resume();
                return *this;
            }
            auto &operator*()
            {
                return coroutine_handle_.promise().current_value;
            }
            const auto &operator*() const
            {
                return coroutine_handle_.promise().current_value;
            }
            auto operator==( std::default_sentinel_t ) const
            {
                return !coroutine_handle_ || coroutine_handle_.done();
            }
            auto operator=( const iterator & ) -> iterator &     = default;
            auto operator=( iterator && ) noexcept -> iterator & = default;
            iterator( const handle_ _coroutine_handle ) noexcept
              : coroutine_handle_{ _coroutine_handle }
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
        auto destroy() const
        {
            coroutine_handle_.destroy();
        }
        auto &safe_destroy() noexcept
        {
            if ( !empty() ) {
                destroy();
                coroutine_handle_ = {};
            }
            return *this;
        }
        auto &reset( coroutine< return_type_ > &&_src ) noexcept
        {
            if ( this != &_src ) {
                if ( !empty() ) {
                    destroy();
                }
                coroutine_handle_      = _src.coroutine_handle_;
                _src.coroutine_handle_ = {};
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
            return coroutine_handle_.promise().current_value;
        }
        auto &reference_value()
        {
            return coroutine_handle_.promise().current_value;
        }
        const auto &const_reference_value() const
        {
            return coroutine_handle_.promise().current_value;
        }
        auto &&move_value()
        {
            return std::move( coroutine_handle_.promise().current_value );
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
        auto operator=( const coroutine< return_type_ > & ) -> coroutine< return_type_ > & = delete;
        auto operator=( coroutine< return_type_ > &&_src ) noexcept -> coroutine< return_type_ > &
        {
            reset( std::move( _src ) );
            return *this;
        }
        coroutine() noexcept = default;
        coroutine( const handle_ _coroutine_handle ) noexcept
          : coroutine_handle_{ _coroutine_handle }
        { }
        coroutine( const coroutine< return_type_ > & ) = delete;
        coroutine( coroutine< return_type_ > &&_src ) noexcept
          : coroutine_handle_{ _src.coroutine_handle_ }
        {
            _src.coroutine_handle_ = {};
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
            std::exception_ptr current_exception{ nullptr };
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
            auto await_transform() -> void                               = delete;
            auto operator=( const promise_type & ) -> promise_type &     = default;
            auto operator=( promise_type && ) noexcept -> promise_type & = default;
            promise_type() noexcept                                      = default;
            promise_type( const promise_type & ) noexcept                = default;
            promise_type( promise_type && ) noexcept                     = default;
            ~promise_type() noexcept                                     = default;
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
        auto destroy() const
        {
            coroutine_handle_.destroy();
        }
        auto &safe_destroy() noexcept
        {
            if ( !empty() ) {
                destroy();
                coroutine_handle_ = {};
            }
            return *this;
        }
        auto reset( coroutine_void &&_src ) noexcept
        {
            if ( this != &_src ) {
                if ( !empty() ) {
                    destroy();
                }
                coroutine_handle_      = _src.coroutine_handle_;
                _src.coroutine_handle_ = {};
            }
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
        auto operator=( coroutine_void &&_src ) noexcept -> coroutine_void &
        {
            reset( std::move( _src ) );
            return *this;
        }
        coroutine_void() noexcept = default;
        coroutine_void( const handle_ _coroutine_handle ) noexcept
          : coroutine_handle_{ _coroutine_handle }
        { }
        coroutine_void( const coroutine_void & ) = delete;
        coroutine_void( coroutine_void &&_src ) noexcept
          : coroutine_handle_{ _src.coroutine_handle_ }
        {
            _src.coroutine_handle_ = {};
        }
        ~coroutine_void() noexcept
        {
            if ( !empty() ) {
                destroy();
            }
        }
    };
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
        auto &resize( const size_type _size )
        {
            threads_.resize( _size );
            return *this;
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
        auto &swap( const size_type _index, std::jthread &_src )
        {
            threads_.at( _index ).swap( _src );
            return *this;
        }
        auto joinable( const size_type _index ) const
        {
            return threads_.at( _index ).joinable();
        }
        auto get_id( const size_type _index ) const
        {
            return threads_.at( _index ).get_id();
        }
        auto native_handle( const size_type _index )
        {
            return threads_.at( _index ).native_handle();
        }
        template < callable_type _callee_, typename... _args_ >
        auto &add( _callee_ &&_func, _args_ &&..._args )
        {
            threads_.emplace_back( std::forward< _callee_ >( _func ), std::forward< _args_ >( _args )... );
            return *this;
        }
        auto &join( const size_type _index )
        {
            threads_.at( _index ).join();
            return *this;
        }
        auto &safe_join( const size_type _index )
        {
            auto &thread{ threads_.at( _index ) };
            if ( thread.joinable() ) {
                thread.join();
            }
            return *this;
        }
        auto &join_all()
        {
            for ( auto &thread : threads_ ) {
                thread.join();
            }
            return *this;
        }
        auto &safe_join_all()
        {
            for ( auto &thread : threads_ ) {
                if ( thread.joinable() ) {
                    thread.join();
                }
            }
            return *this;
        }
        auto &detach( const size_type _index )
        {
            threads_.at( _index ).detach();
            return *this;
        }
        auto &safe_detach( const size_type _index )
        {
            auto &thread{ threads_.at( _index ) };
            if ( thread.joinable() ) {
                thread.detach();
            }
            return *this;
        }
        auto &detach_all()
        {
            for ( auto &thread : threads_ ) {
                thread.detach();
            }
            return *this;
        }
        auto &safe_detach_all()
        {
            for ( auto &thread : threads_ ) {
                if ( thread.joinable() ) {
                    thread.detach();
                }
            }
            return *this;
        }
        auto get_stop_source( const size_type _index )
        {
            return threads_.at( _index ).get_stop_source();
        }
        auto get_stop_token( const size_type _index ) const
        {
            return threads_.at( _index ).get_stop_token();
        }
        auto request_stop( const size_type _index )
        {
            return threads_.at( _index ).request_stop();
        }
        auto &request_stop_to_all()
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
#if defined( _WIN32 ) || defined( _WIN64 )
    namespace mouse {
        inline constexpr DWORD button_left{ FROM_LEFT_1ST_BUTTON_PRESSED };
        inline constexpr DWORD button_middle{ FROM_LEFT_2ND_BUTTON_PRESSED };
        inline constexpr DWORD button_right{ RIGHTMOST_BUTTON_PRESSED };
        inline constexpr DWORD click{ 0x0000 };
        inline constexpr DWORD click_double{ DOUBLE_CLICK };
        inline constexpr DWORD move{ MOUSE_MOVED };
        inline constexpr DWORD wheel_height{ MOUSE_HWHEELED };
        inline constexpr DWORD wheel{ MOUSE_WHEELED };
    };
    namespace keyboard {
        inline constexpr DWORD right_alt_press{ RIGHT_ALT_PRESSED };
        inline constexpr DWORD left_alt_press{ LEFT_ALT_PRESSED };
        inline constexpr DWORD right_ctrl_press{ RIGHT_CTRL_PRESSED };
        inline constexpr DWORD left_ctrl_press{ LEFT_CTRL_PRESSED };
        inline constexpr DWORD shift_press{ SHIFT_PRESSED };
        inline constexpr DWORD num_lock_on{ NUMLOCK_ON };
        inline constexpr DWORD scroll_lock_on{ SCROLLLOCK_ON };
        inline constexpr DWORD caps_lock_on{ CAPSLOCK_ON };
        inline constexpr DWORD enhanced_key{ ENHANCED_KEY };
    }
    namespace console_handle_flag {
        inline constexpr DWORD std_input{ STD_INPUT_HANDLE };
        inline constexpr DWORD std_output{ STD_OUTPUT_HANDLE };
        inline constexpr DWORD std_error{ STD_ERROR_HANDLE };
    };
    namespace console_text {
        inline constexpr WORD default_set{ FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE };
        inline constexpr WORD foreground_red{ FOREGROUND_RED };
        inline constexpr WORD foreground_green{ FOREGROUND_GREEN };
        inline constexpr WORD foreground_blue{ FOREGROUND_BLUE };
        inline constexpr WORD foreground_intensity{ FOREGROUND_INTENSITY };
        inline constexpr WORD background_red{ BACKGROUND_RED };
        inline constexpr WORD background_green{ BACKGROUND_GREEN };
        inline constexpr WORD background_blue{ BACKGROUND_BLUE };
        inline constexpr WORD background_intensity{ BACKGROUND_INTENSITY };
        inline constexpr WORD lvb_leading_byte{ COMMON_LVB_LEADING_BYTE };
        inline constexpr WORD lvb_trailing_byte{ COMMON_LVB_TRAILING_BYTE };
        inline constexpr WORD lvb_grid_horizontal{ COMMON_LVB_GRID_HORIZONTAL };
        inline constexpr WORD lvb_grid_lvertical{ COMMON_LVB_GRID_LVERTICAL };
        inline constexpr WORD lvb_grid_rvertical{ COMMON_LVB_GRID_RVERTICAL };
        inline constexpr WORD lvb_reverse_video{ COMMON_LVB_REVERSE_VIDEO };
        inline constexpr WORD lvb_underscore{ COMMON_LVB_UNDERSCORE };
        inline constexpr WORD lvb_sbcsdbcs{ COMMON_LVB_SBCSDBCS };
    };
    namespace window_state {
        inline constexpr UINT hide{ SW_HIDE };
        inline constexpr UINT show{ SW_SHOW };
        inline constexpr UINT show_without_activating{ SW_SHOWNA };
        inline constexpr UINT show_default{ SW_SHOWDEFAULT };
        inline constexpr UINT show_normal{ SW_SHOWNORMAL };
        inline constexpr UINT show_normal_without_activating{ SW_SHOWNOACTIVATE };
        inline constexpr UINT minimize{ SW_SHOWMINIMIZED };
        inline constexpr UINT minimize_and_activate_next_window_with_z_order{ SW_MINIMIZE };
        inline constexpr UINT minimize_without_activating{ SW_SHOWMINNOACTIVE };
        inline constexpr UINT minimize_force{ SW_FORCEMINIMIZE };
        inline constexpr UINT maximize{ SW_SHOWMAXIMIZED };
        inline constexpr UINT restore{ SW_RESTORE };
    };
    inline auto is_run_as_admin() noexcept
    {
        BOOL is_admin;
        PSID admins_group;
        SID_IDENTIFIER_AUTHORITY nt_authority{ SECURITY_NT_AUTHORITY };
        if ( AllocateAndInitializeSid(
               &nt_authority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &admins_group )
             == TRUE )
        {
            CheckTokenMembership( nullptr, admins_group, &is_admin );
            FreeSid( admins_group );
        }
        return is_admin ? true : false;
    }
    inline auto relaunch() noexcept
    {
        wide_char file_path[ MAX_PATH ]{};
        GetModuleFileNameW( nullptr, file_path, MAX_PATH );
        ShellExecuteW( nullptr, L"open", file_path, nullptr, nullptr, SW_SHOWNORMAL );
        std::exit( 0 );
    }
    inline auto relaunch_as_admin() noexcept
    {
        wide_char file_path[ MAX_PATH ]{};
        GetModuleFileNameW( nullptr, file_path, MAX_PATH );
        ShellExecuteW( nullptr, L"runas", file_path, nullptr, nullptr, SW_SHOWNORMAL );
        std::exit( 0 );
    }
    inline auto get_current_console_std_handle( const DWORD _std_handle_flag ) noexcept
    {
        return GetStdHandle( _std_handle_flag );
    }
    inline auto get_current_window_handle() noexcept
    {
        auto window_handle{ GetConsoleWindow() };
        if ( window_handle == nullptr ) {
            window_handle = GetForegroundWindow();
        }
        if ( window_handle == nullptr ) {
            window_handle = GetActiveWindow();
        }
        return window_handle;
    }
    inline auto get_window_state( const HWND _window_handle ) noexcept
    {
        WINDOWPLACEMENT wp;
        wp.length = sizeof( WINDOWPLACEMENT );
        GetWindowPlacement( _window_handle, &wp );
        return wp.showCmd;
    }
    inline auto set_window_state( const HWND _window_handle, const UINT _state ) noexcept
    {
        ShowWindow( _window_handle, _state );
    }
    inline auto keep_window_top( const HWND _window_handle, const DWORD _thread_id, const DWORD _window_thread_process_id ) noexcept
    {
        AttachThreadInput( _thread_id, _window_thread_process_id, TRUE );
        set_window_state( _window_handle, get_window_state( _window_handle ) );
        SetForegroundWindow( _window_handle );
        SetWindowPos( _window_handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
        AttachThreadInput( _thread_id, _window_thread_process_id, FALSE );
    }
    inline auto keep_current_window_top() noexcept
    {
        auto window_handle{ get_current_window_handle() };
        keep_window_top( window_handle, GetCurrentThreadId(), GetWindowThreadProcessId( window_handle, nullptr ) );
    }
    template < std_chrono_type _chrono_type_, callable_type _callee_, typename... _args_ >
    inline auto loop_keep_window_top(
      const HWND _window_handle, const DWORD _thread_id, const DWORD _window_thread_process_id, const _chrono_type_ _sleep_time,
      _callee_ &&_condition_checker, _args_ &&..._condition_checker_args )
    {
        while ( _condition_checker( std::forward< _args_ >( _condition_checker_args )... ) ) {
            AttachThreadInput( _thread_id, _window_thread_process_id, TRUE );
            set_window_state( _window_handle, get_window_state( _window_handle ) );
            SetForegroundWindow( _window_handle );
            SetWindowPos( _window_handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
            AttachThreadInput( _thread_id, _window_thread_process_id, FALSE );
            std::this_thread::sleep_for( _sleep_time );
        }
    }
    template < std_chrono_type _chrono_type_, callable_type _callee_, typename... _args_ >
    inline auto loop_keep_current_window_top(
      const _chrono_type_ _sleep_time, _callee_ &&_condition_checker, _args_ &&..._condition_checker_args )
    {
        auto window_handle{ get_current_window_handle() };
        loop_keep_window_top(
          window_handle, GetCurrentThreadId(), GetWindowThreadProcessId( window_handle, nullptr ), _sleep_time,
          std::forward< _callee_ >( _condition_checker ), std::forward< _args_ >( _condition_checker_args )... );
    }
    inline auto cancel_top_window( const HWND _window_handle ) noexcept
    {
        SetWindowPos( _window_handle, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
    }
    inline auto ignore_console_exit_signal( const bool _is_ignore ) noexcept
    {
        return SetConsoleCtrlHandler( nullptr, static_cast< WINBOOL >( _is_ignore ) );
    }
    inline auto clear_console_screen( const HANDLE _std_output_handle )
    {
        DWORD mode;
        GetConsoleMode( _std_output_handle, &mode );
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode( _std_output_handle, mode );
        std::print( "\033[H\033[2J" );
    }
    inline auto reset_console_screen( const HANDLE _std_output_handle )
    {
        DWORD mode;
        GetConsoleMode( _std_output_handle, &mode );
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode( _std_output_handle, mode );
        std::print( "\033c" );
    }
    inline auto set_console_title( const ansi_char *const _title ) noexcept
    {
        SetConsoleTitleA( _title );
    }
    inline auto set_console_title( const ansi_std_string &_title ) noexcept
    {
        SetConsoleTitleA( _title.data() );
    }
    inline auto set_console_title( const wide_char *const _title ) noexcept
    {
        SetConsoleTitleW( _title );
    }
    inline auto set_console_title( const wide_std_string &_title ) noexcept
    {
        SetConsoleTitleW( _title.data() );
    }
    inline auto set_console_charset( const UINT _charset_id ) noexcept
    {
        SetConsoleOutputCP( _charset_id );
        SetConsoleCP( _charset_id );
    }
    inline auto
      set_console_size( const HWND _window_handle, const HANDLE _std_output_handle, const SHORT _width, const SHORT _height ) noexcept
    {
        SMALL_RECT wrt{ 0, 0, static_cast< SHORT >( _width - 1 ), static_cast< SHORT >( _height - 1 ) };
        set_window_state( _window_handle, SW_SHOWNORMAL );
        SetConsoleScreenBufferSize( _std_output_handle, { _width, _height } );
        SetConsoleWindowInfo( _std_output_handle, TRUE, &wrt );
        SetConsoleScreenBufferSize( _std_output_handle, { _width, _height } );
        clear_console_screen( _std_output_handle );
    }
    inline auto set_window_translucency( const HWND _window_handle, const BYTE _value ) noexcept
    {
        SetLayeredWindowAttributes( _window_handle, RGB( 0, 0, 0 ), _value, LWA_ALPHA );
    }
    inline auto fix_window_size( const HWND _window_handle, const bool _is_enable ) noexcept
    {
        SetWindowLongPtrW(
          _window_handle, GWL_STYLE,
          _is_enable
            ? GetWindowLongPtrW( _window_handle, GWL_STYLE ) & ~WS_SIZEBOX
            : GetWindowLongPtrW( _window_handle, GWL_STYLE ) | WS_SIZEBOX );
    }
    inline auto enable_window_menu( const HWND _window_handle, const bool _is_enable ) noexcept
    {
        SetWindowLongPtrW(
          _window_handle, GWL_STYLE,
          _is_enable
            ? GetWindowLongPtrW( _window_handle, GWL_STYLE ) | WS_SYSMENU
            : GetWindowLongPtrW( _window_handle, GWL_STYLE ) & ~WS_SYSMENU );
    }
    inline auto enable_window_minimize_ctrl( const HWND _window_handle, const bool _is_enable ) noexcept
    {
        SetWindowLongPtrW(
          _window_handle, GWL_STYLE,
          _is_enable
            ? GetWindowLongPtrW( _window_handle, GWL_STYLE ) | WS_MINIMIZEBOX
            : GetWindowLongPtrW( _window_handle, GWL_STYLE ) & ~WS_MINIMIZEBOX );
    }
    inline auto enable_window_maximize_ctrl( const HWND _window_handle, const bool _is_enable ) noexcept
    {
        SetWindowLongPtrW(
          _window_handle, GWL_STYLE,
          _is_enable
            ? GetWindowLongPtrW( _window_handle, GWL_STYLE ) | WS_MAXIMIZEBOX
            : GetWindowLongPtrW( _window_handle, GWL_STYLE ) & ~WS_MAXIMIZEBOX );
    }
    inline auto enable_window_close_ctrl( const HWND _window_handle, const bool _is_enable ) noexcept
    {
        EnableMenuItem(
          GetSystemMenu( _window_handle, FALSE ), SC_CLOSE,
          _is_enable ? MF_BYCOMMAND | MF_ENABLED : MF_BYCOMMAND | MF_DISABLED | MF_GRAYED );
    }
    class console_ui final {
      public:
        using func_return_type = bool;
        inline static constexpr func_return_type back{ false };
        inline static constexpr func_return_type exit{ true };
        struct func_args final {
            console_ui &parent_ui;
            const size_type node_index;
            const DWORD button_state;
            const DWORD ctrl_state;
            const DWORD event_flag;
            auto operator=( const func_args & ) noexcept -> func_args & = default;
            auto operator=( func_args && ) noexcept -> func_args &      = default;
            func_args(
              console_ui &_parent_ui, const size_type _node_index,
              const MOUSE_EVENT_RECORD _event = MOUSE_EVENT_RECORD{ {}, mouse::button_left, {}, {} } ) noexcept
              : parent_ui{ _parent_ui }
              , node_index{ _node_index }
              , button_state{ _event.dwButtonState }
              , ctrl_state{ _event.dwControlKeyState }
              , event_flag{ _event.dwEventFlags }
            { }
            func_args( const func_args & ) noexcept = default;
            func_args( func_args && ) noexcept      = default;
            ~func_args() noexcept                   = default;
        };
        using callback_type = std::function< func_return_type( func_args ) >;
      private:
        inline static HANDLE std_input_handle_;
        inline static HANDLE std_output_handle_;
        enum class console_attrs_ : ansi_char { normal, lock_text, lock_all };
        struct line_node_ final {
            ansi_std_string text{};
            callback_type func{};
            WORD default_attrs{};
            WORD intensity_attrs{};
            WORD last_attrs{};
            COORD position{};
            auto set_attrs( const WORD _attrs ) noexcept
            {
                SetConsoleTextAttribute( std_output_handle_, _attrs );
                last_attrs = _attrs;
            }
            auto operator==( const COORD _position ) const noexcept
            {
                return position.Y == _position.Y && position.X <= _position.X
                    && _position.X < ( position.X + static_cast< SHORT >( text.size() ) );
            }
            auto operator!=( const COORD _position ) const noexcept
            {
                return !operator==( _position );
            }
            auto operator=( const line_node_ & ) noexcept -> line_node_ & = default;
            auto operator=( line_node_ && ) noexcept -> line_node_ &      = default;
            line_node_() noexcept
              : default_attrs{ console_text::default_set }
              , intensity_attrs{ console_text::foreground_green | console_text::foreground_blue }
              , last_attrs{ console_text::default_set }
            { }
            line_node_(
              const ansi_std_string_view _text, callback_type &_func, const WORD _default_attrs, const WORD _intensity_attrs ) noexcept
              : text{ _text }
              , func{ std::move( _func ) }
              , default_attrs{ _default_attrs }
              , intensity_attrs{ _intensity_attrs }
              , last_attrs{ _default_attrs }
            { }
            line_node_( const line_node_ & )     = default;
            line_node_( line_node_ && ) noexcept = default;
            ~line_node_() noexcept               = default;
        };
        std::deque< line_node_ > lines_{};
        static auto show_cursor_( const WINBOOL _is_show ) noexcept
        {
            CONSOLE_CURSOR_INFO cursor_data;
            GetConsoleCursorInfo( std_output_handle_, &cursor_data );
            cursor_data.bVisible = _is_show;
            SetConsoleCursorInfo( std_output_handle_, &cursor_data );
        }
        static auto edit_console_attrs_( const console_attrs_ _attrs ) noexcept
        {
            DWORD attrs;
            GetConsoleMode( std_input_handle_, &attrs );
            switch ( _attrs ) {
                case console_attrs_::normal :
                    attrs |= ENABLE_QUICK_EDIT_MODE;
                    attrs |= ENABLE_INSERT_MODE;
                    attrs |= ENABLE_MOUSE_INPUT;
                    break;
                case console_attrs_::lock_text :
                    attrs &= ~ENABLE_QUICK_EDIT_MODE;
                    attrs &= ~ENABLE_INSERT_MODE;
                    attrs |= ENABLE_MOUSE_INPUT;
                    break;
                case console_attrs_::lock_all :
                    attrs &= ~ENABLE_QUICK_EDIT_MODE;
                    attrs &= ~ENABLE_INSERT_MODE;
                    attrs &= ~ENABLE_MOUSE_INPUT;
                    break;
            }
            SetConsoleMode( std_input_handle_, attrs );
        }
        static auto get_cursor_() noexcept
        {
            CONSOLE_SCREEN_BUFFER_INFO console_data;
            GetConsoleScreenBufferInfo( std_output_handle_, &console_data );
            return console_data.dwCursorPosition;
        }
        static auto set_cursor_( const COORD _cursor_position ) noexcept
        {
            SetConsoleCursorPosition( std_output_handle_, _cursor_position );
        }
        static auto wait_event_( const bool _is_move = true ) noexcept
        {
            using namespace std::chrono_literals;
            INPUT_RECORD record;
            DWORD reg;
            while ( true ) {
                std::this_thread::sleep_for( 10ms );
                ReadConsoleInputW( std_input_handle_, &record, 1, &reg );
                if ( record.EventType == MOUSE_EVENT && ( _is_move || record.Event.MouseEvent.dwEventFlags != mouse::move ) ) {
                    return record.Event.MouseEvent;
                }
            }
        }
        static auto get_console_size_() noexcept
        {
            CONSOLE_SCREEN_BUFFER_INFO console_data;
            GetConsoleScreenBufferInfo( std_output_handle_, &console_data );
            return console_data.dwSize;
        }
        static auto cls_()
        {
            auto [ width, height ]{ get_console_size_() };
            set_cursor_( COORD{ 0, 0 } );
            std::print( "{}", ansi_std_string( static_cast< unsigned int >( width ) * static_cast< unsigned int >( height ), ' ' ) );
            set_cursor_( COORD{ 0, 0 } );
        }
        static auto write_( const ansi_std_string_view _text, const bool _is_endl = false )
        {
            std::print( "{}", _text );
            if ( _is_endl ) {
                std::print( "\n" );
            }
        }
        static auto rewrite_( const COORD _cursor_position, const ansi_std_string_view _text )
        {
            set_cursor_( COORD{ 0, _cursor_position.Y } );
            write_( ansi_std_string( _cursor_position.X, ' ' ) );
            set_cursor_( COORD{ 0, _cursor_position.Y } );
            write_( _text );
            set_cursor_( COORD{ 0, _cursor_position.Y } );
        }
        auto init_pos_()
        {
            cls_();
            for ( auto &line : lines_ ) {
                line.position = get_cursor_();
                line.set_attrs( line.default_attrs );
                write_( line.text, true );
            }
        }
        auto refresh_( const COORD _hang_position )
        {
            for ( auto &line : lines_ ) {
                if ( line == _hang_position && line.last_attrs != line.intensity_attrs ) {
                    line.set_attrs( line.intensity_attrs );
                    rewrite_( line.position, line.text );
                }
                if ( line != _hang_position && line.last_attrs != line.default_attrs ) {
                    line.set_attrs( line.default_attrs );
                    rewrite_( line.position, line.text );
                }
            }
        }
        auto call_func_( const MOUSE_EVENT_RECORD &_event )
        {
            auto is_exit{ back };
            auto size{ lines_.size() };
            for ( size_type idx{ 0 }; idx < size; ++idx ) {
                auto &line{ lines_[ idx ] };
                if ( line != _event.dwMousePosition ) {
                    continue;
                }
                if ( line.func == nullptr ) {
                    break;
                }
                cls_();
                line.set_attrs( line.default_attrs );
                show_cursor_( FALSE );
                edit_console_attrs_( console_attrs_::lock_all );
                is_exit = line.func( func_args{ *this, idx, _event } );
                show_cursor_( FALSE );
                edit_console_attrs_( console_attrs_::lock_text );
                init_pos_();
                break;
            }
            return is_exit;
        }
      public:
        auto empty() const noexcept
        {
            return lines_.empty();
        }
        auto size() const noexcept
        {
            return lines_.size();
        }
        auto max_size() const noexcept
        {
            return lines_.max_size();
        }
        auto &resize( const size_type _size )
        {
            lines_.resize( _size );
            return *this;
        }
        auto &optimize_storage() noexcept
        {
            for ( auto &line : lines_ ) {
                line.text.shrink_to_fit();
            }
            lines_.shrink_to_fit();
            return *this;
        }
        auto &swap( console_ui &_src ) noexcept
        {
            lines_.swap( _src.lines_ );
            return *this;
        }
        auto &add_front(
          const ansi_std_string_view _text, callback_type _func = nullptr,
          const WORD _intensity_attrs = console_text::foreground_green | console_text::foreground_blue,
          const WORD _default_attrs   = console_text::default_set )
        {
            lines_.emplace_front( _text, _func, _default_attrs, _func != nullptr ? _intensity_attrs : _default_attrs );
            return *this;
        }
        auto &add_back(
          const ansi_std_string_view _text, callback_type _func = nullptr,
          const WORD _intensity_attrs = console_text::foreground_blue | console_text::foreground_green,
          const WORD _default_attrs   = console_text::default_set )
        {
            lines_.emplace_back( _text, _func, _default_attrs, _func != nullptr ? _intensity_attrs : _default_attrs );
            return *this;
        }
        auto &insert(
          const size_type _index, const ansi_std_string_view _text, callback_type _func = nullptr,
          const WORD _intensity_attrs = console_text::foreground_green | console_text::foreground_blue,
          const WORD _default_attrs   = console_text::default_set )
        {
            lines_.emplace(
              lines_.cbegin() + _index, _text, _func, _default_attrs, _func != nullptr ? _intensity_attrs : _default_attrs );
            return *this;
        }
        auto &edit_text( const size_type _index, const ansi_std_string_view _text )
        {
            lines_.at( _index ).text = _text;
            return *this;
        }
        auto &edit_func( const size_type _index, callback_type _func )
        {
            lines_.at( _index ).func = std::move( _func );
            return *this;
        }
        auto &edit_intensity_attrs( const size_type _index, const WORD _intensity_attrs )
        {
            lines_.at( _index ).intensity_attrs = _intensity_attrs;
            return *this;
        }
        auto &edit_default_attrs( const size_type _index, const WORD _default_attrs )
        {
            lines_.at( _index ).default_attrs = _default_attrs;
            return *this;
        }
        auto &edit(
          const size_type _index, const ansi_std_string_view _text, callback_type _func = nullptr,
          const WORD _intensity_attrs = console_text::foreground_green | console_text::foreground_blue,
          const WORD _default_attrs   = console_text::default_set )
        {
            lines_.at( _index ) = line_node_{ _text, _func, _default_attrs, _func != nullptr ? _intensity_attrs : _default_attrs };
            return *this;
        }
        auto &remove_front() noexcept
        {
            lines_.pop_front();
            return *this;
        }
        auto &remove_back() noexcept
        {
            lines_.pop_back();
            return *this;
        }
        auto &remove( const size_type _begin, const size_type _end )
        {
            lines_.erase( lines_.cbegin() + _begin, lines_.cbegin() + _end );
            return *this;
        }
        auto &clear() noexcept
        {
            lines_.clear();
            return *this;
        }
        auto &show()
        {
            using namespace std::chrono_literals;
            show_cursor_( FALSE );
            edit_console_attrs_( console_attrs_::lock_text );
            init_pos_();
            MOUSE_EVENT_RECORD event;
            auto func_return_value{ back };
            while ( func_return_value == back ) {
                event = wait_event_();
                switch ( event.dwEventFlags ) {
                    case mouse::move : refresh_( event.dwMousePosition ); break;
                    case mouse::click : {
                        if ( event.dwButtonState != false ) {
                            func_return_value = call_func_( event );
                        }
                        break;
                    }
                }
                std::this_thread::sleep_for( 10ms );
            }
            cls_();
            return *this;
        }
        auto &lock( const bool _is_hide_cursor, const bool _is_lock_text ) noexcept
        {
            show_cursor_( static_cast< WINBOOL >( !_is_hide_cursor ) );
            edit_console_attrs_( _is_lock_text ? console_attrs_::lock_all : console_attrs_::normal );
            return *this;
        }
        auto operator=( const console_ui & ) noexcept -> console_ui & = default;
        auto operator=( console_ui && ) noexcept -> console_ui &      = default;
        console_ui() noexcept
        {
            if ( std_input_handle_ == nullptr ) {
                std_input_handle_ = GetStdHandle( console_handle_flag::std_input );
            }
            if ( std_output_handle_ == nullptr ) {
                std_output_handle_ = GetStdHandle( console_handle_flag::std_output );
            }
        }
        console_ui( const HANDLE _std_input_handle, const HANDLE _std_output_handle ) noexcept
        {
            std_input_handle_  = _std_input_handle;
            std_output_handle_ = _std_output_handle;
        }
        console_ui( const console_ui & ) noexcept = default;
        console_ui( console_ui && ) noexcept      = default;
        ~console_ui() noexcept                    = default;
    };
#endif
}
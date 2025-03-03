#pragma once
#if defined( _WIN32 ) || defined( _WIN64 )
# include <windows.h>
#endif
#include <chrono>
#include <concepts>
#include <coroutine>
#include <exception>
#include <expected>
#include <functional>
#include <generator>
#include <optional>
#include <print>
#include <queue>
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
    template < typename _type_a_, typename _type_b_ >
    concept convertible_char_type = char_type< _type_a_ > && char_type< _type_b_ > && sizeof( _type_a_ ) == sizeof( _type_b_ );
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
    template < char_type _target_, char_type _source_ >
        requires convertible_char_type< _target_, _source_ >
    inline auto string_convert( const std_string< _source_ > &_str )
    {
        const auto str_it{ reinterpret_cast< const _target_ * >( _str.c_str() ) };
        return std_string< _target_ >{ str_it, str_it + _str.size() };
    }
    template < char_type _target_, char_type _source_ >
        requires convertible_char_type< _target_, _source_ >
    inline auto string_convert( const std_string_view< _source_ > _str )
    {
        const auto str_it{ reinterpret_cast< const _target_ * >( _str.data() ) };
        return std_string< _target_ >{ str_it, str_it + _str.size() };
    }
    template < char_type _target_, char_type _source_ >
        requires convertible_char_type< _target_, _source_ >
    inline auto string_convert( const _source_ *const _str )
    {
        return std_string< _target_ >{ reinterpret_cast< const _target_ * >( _str ) };
    }
    template < char_type _target_, char_type _source_ >
        requires convertible_char_type< _target_, _source_ >
    inline auto string_view_convert( const std_string< _source_ > &_str )
    {
        const auto str_it{ reinterpret_cast< const _target_ * >( _str.c_str() ) };
        return std_string_view< _target_ >{ str_it, str_it + _str.size() };
    }
    template < char_type _target_, char_type _source_ >
        requires convertible_char_type< _target_, _source_ >
    inline auto string_view_convert( const std_string_view< _source_ > _str )
    {
        const auto str_it{ reinterpret_cast< const _target_ * >( _str.data() ) };
        return std_string_view< _target_ >{ str_it, str_it + _str.size() };
    }
    template < char_type _target_, char_type _source_ >
        requires convertible_char_type< _target_, _source_ >
    inline auto string_view_convert( const _source_ *const _str )
    {
        return std_string_view< _target_ >{ reinterpret_cast< const _target_ * >( _str ) };
    }
    template < pointer_type _type_ >
    inline auto ptr_to_string( const _type_ _ptr )
    {
        using namespace std::string_literals;
        return _ptr == nullptr ? "nullptr"s : std::format( "0x{:x}", reinterpret_cast< std::uintptr_t >( _ptr ) );
    }
    template < typename... _args_ >
    inline auto utf8_format( const utf8_std_string_view _fmt, _args_ &&..._args )
    {
        auto convert_arg{ []< typename _type_ >( _type_ &&_arg ) static -> decltype( auto )
        {
            if constexpr ( std::same_as< std::decay_t< _type_ >, utf8_std_string > ) {
                if constexpr ( std::is_const_v< std::remove_reference_t< _type_ > > ) {
                    return reinterpret_cast< const ansi_std_string * >( &_arg );
                } else {
                    return reinterpret_cast< ansi_std_string * >( &_arg );
                }
            } else if constexpr ( std::same_as< std::decay_t< _type_ >, utf8_std_string_view > ) {
                if constexpr ( std::is_const_v< std::remove_reference_t< _type_ > > ) {
                    return reinterpret_cast< const ansi_std_string_view * >( &_arg );
                } else {
                    return reinterpret_cast< ansi_std_string_view * >( &_arg );
                }
            } else if constexpr ( std::same_as< std::decay_t< _type_ >, utf8_char * > ) {
                return std::make_unique< ansi_char * >( reinterpret_cast< ansi_char * >( &_arg ) );
            } else if constexpr ( std::same_as< std::decay_t< _type_ >, const utf8_char * > ) {
                return std::make_unique< const ansi_char * >( reinterpret_cast< const ansi_char * >( &_arg ) );
            } else if constexpr (
              std::is_pointer_v< std::decay_t< _type_ > >
              && !( std::same_as< std::decay_t< _type_ >, ansi_char * > || std::same_as< std::decay_t< _type_ >, const ansi_char * > ) )
            {
                return std::make_unique< const ansi_std_string >( ptr_to_ansi_string( std::forward< _type_ >( _arg ) ) );
            } else {
                return &_arg;
            }
        } };
        return string_convert< utf8_char >( std::vformat(
          string_view_convert< ansi_char >( _fmt ), std::make_format_args( *convert_arg( std::forward< _args_ >( _args ) )... ) ) );
    }
    template < typename... _args_ >
    inline auto &utf8_format_to( utf8_std_string &_str, const utf8_std_string_view _fmt, _args_ &&..._args )
    {
        _str = utf8_format( _fmt, std::forward< _args_ >( _args )... );
        return _str;
    }
    template < typename... _args_ >
    inline auto utf8_print( const utf8_std_string_view _fmt, _args_ &&..._args )
    {
        std::print( "{}", string_view_convert< ansi_char >( utf8_format( _fmt, std::forward< _args_ >( _args )... ) ) );
    }
    template < typename... _args_ >
    inline auto utf8_print( io_buffer *const _stream, const utf8_std_string_view _fmt, _args_ &&..._args )
    {
        std::print( _stream, "{}", string_view_convert< ansi_char >( utf8_format( _fmt, std::forward< _args_ >( _args )... ) ) );
    }
    template < typename... _args_ >
    inline auto utf8_println( const utf8_std_string_view _fmt, _args_ &&..._args )
    {
        std::println( "{}", string_view_convert< ansi_char >( utf8_format( _fmt, std::forward< _args_ >( _args )... ) ) );
    }
    template < typename... _args_ >
    inline auto utf8_println( io_buffer *const _stream, const utf8_std_string_view _fmt, _args_ &&..._args )
    {
        std::println( _stream, "{}", string_view_convert< ansi_char >( utf8_format( _fmt, std::forward< _args_ >( _args )... ) ) );
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
        constexpr constant_string( const _type_ ( &_str )[ _capacity_ ] ) noexcept
        {
            std::copy( _str, _str + _capacity_, data_ );
        }
        constexpr constant_string( const constant_string< _type_, _capacity_ > & ) = default;
        constexpr constant_string( constant_string< _type_, _capacity_ > && )      = delete;
        constexpr ~constant_string()                                               = default;
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
    class coroutine_void final {
      public:
        struct promise_type final {
            auto get_return_object()
            {
                return coroutine_void{ handle::from_promise( *this ) };
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
            [[noreturn]]
            static auto unhandled_exception()
            {
                throw;
            }
            auto await_transform() -> void                               = delete;
            auto operator=( const promise_type & ) -> promise_type &     = default;
            auto operator=( promise_type && ) noexcept -> promise_type & = default;
            promise_type()                                               = default;
            promise_type( const promise_type & )                         = default;
            promise_type( promise_type && ) noexcept                     = default;
            ~promise_type()                                              = default;
        };
        using handle = std::coroutine_handle< promise_type >;
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
        auto safe_destroy() noexcept
        {
            if ( !empty() ) {
                destroy();
                coroutine_handle_ = {};
            }
        }
        auto reset( coroutine_void &&_src )
        {
            if ( this != &_src ) {
                if ( !empty() ) {
                    destroy();
                }
                coroutine_handle_      = _src.coroutine_handle_;
                _src.coroutine_handle_ = {};
            }
        }
        auto resume() const
        {
            coroutine_handle_.resume();
        }
        auto safe_resume() const noexcept
        {
            if ( !done() ) {
                resume();
            }
        }
        auto operator=( const coroutine_void & ) -> coroutine_void & = delete;
        auto operator=( coroutine_void &&_src ) -> coroutine_void &
        {
            reset( std::move( _src ) );
            return *this;
        }
        coroutine_void() = default;
        coroutine_void( const handle _coroutine_handle )
          : coroutine_handle_{ _coroutine_handle }
        { }
        coroutine_void( const coroutine_void & ) = delete;
        coroutine_void( coroutine_void &&_src ) noexcept
          : coroutine_handle_{ _src.coroutine_handle_ }
        {
            _src.coroutine_handle_ = {};
        }
        ~coroutine_void()
        {
            if ( !empty() ) {
                coroutine_handle_.destroy();
            }
        }
      private:
        handle coroutine_handle_{};
    };
    template < std::movable _type_ >
    class coroutine final {
      public:
        struct promise_type final {
            std::optional< _type_ > current_value{ std::nullopt };
            auto get_return_object()
            {
                return coroutine< _type_ >{ handle::from_promise( *this ) };
            }
            static auto initial_suspend() noexcept
            {
                return std::suspend_always{};
            }
            static auto final_suspend() noexcept
            {
                return std::suspend_always{};
            }
            auto yield_value( _type_ _value ) noexcept
            {
                current_value = std::move( _value );
                return std::suspend_always{};
            }
            auto yield_value( std::nullopt_t ) noexcept
            {
                current_value = std::nullopt;
                return std::suspend_always{};
            }
            auto return_value( _type_ _value ) noexcept
            {
                current_value = std::move( _value );
            }
            auto return_value( std::nullopt_t ) noexcept
            {
                current_value = std::nullopt;
            }
            [[noreturn]]
            static auto unhandled_exception()
            {
                throw;
            }
            auto await_transform() -> void                               = delete;
            auto operator=( const promise_type & ) -> promise_type &     = default;
            auto operator=( promise_type && ) noexcept -> promise_type & = default;
            promise_type()                                               = default;
            promise_type( const promise_type & )                         = default;
            promise_type( promise_type && ) noexcept                     = default;
            ~promise_type()                                              = default;
        };
        using handle = std::coroutine_handle< promise_type >;
        class iterator final {
          private:
            const handle coroutine_handle_;
          public:
            auto operator++() -> coroutine< _type_ >::iterator &
            {
                coroutine_handle_.resume();
                return *this;
            }
            auto operator++( int ) -> coroutine< _type_ >::iterator
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
            auto operator&() const
            {
                return &coroutine_handle_.promise().current_value;
            }
            auto operator==( std::default_sentinel_t ) const
            {
                return !coroutine_handle_ || coroutine_handle_.done();
            }
            auto operator=( const iterator & ) -> iterator &     = default;
            auto operator=( iterator && ) noexcept -> iterator & = default;
            iterator( const handle _coroutine_handle )
              : coroutine_handle_{ _coroutine_handle }
            { }
            iterator( const iterator & )     = default;
            iterator( iterator && ) noexcept = default;
            ~iterator()                      = default;
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
        auto safe_destroy() noexcept
        {
            if ( !empty() ) {
                destroy();
                coroutine_handle_ = {};
            }
        }
        auto reset( coroutine< _type_ > &&_src )
        {
            if ( this != &_src ) {
                if ( !empty() ) {
                    destroy();
                }
                coroutine_handle_      = _src.coroutine_handle_;
                _src.coroutine_handle_ = {};
            }
        }
        auto resume() const
        {
            coroutine_handle_.resume();
        }
        auto safe_resume() const noexcept
        {
            if ( !done() ) {
                resume();
            }
        }
        auto copy_optional() const
        {
            return coroutine_handle_.promise().current_value;
        }
        auto resume_and_copy_optional() const
        {
            resume();
            return coroutine_handle_.promise().current_value;
        }
        auto safe_resume_and_copy_optional() const noexcept
        {
            safe_resume();
            return coroutine_handle_.promise().current_value;
        }
        auto &reference_optional() noexcept
        {
            return coroutine_handle_.promise().current_value;
        }
        auto &resume_and_reference_optional()
        {
            resume();
            return coroutine_handle_.promise().current_value;
        }
        auto &safe_resume_and_reference_optional() noexcept
        {
            safe_resume();
            return coroutine_handle_.promise().current_value;
        }
        auto &&move_optional() noexcept
        {
            return std::move( coroutine_handle_.promise().current_value );
        }
        auto &&resume_and_move_optional() noexcept
        {
            resume();
            return std::move( coroutine_handle_.promise().current_value );
        }
        auto &&safe_resume_and_move_optional() noexcept
        {
            safe_resume();
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
        auto operator=( const coroutine< _type_ > & ) -> coroutine< _type_ > & = delete;
        auto operator=( coroutine< _type_ > &&_src ) noexcept -> coroutine< _type_ > &
        {
            reset( std::move( _src ) );
            return *this;
        }
        coroutine() = default;
        coroutine( const handle _coroutine_handle )
          : coroutine_handle_{ _coroutine_handle }
        { }
        coroutine( const coroutine< _type_ > & ) = delete;
        coroutine( coroutine< _type_ > &&_src ) noexcept
          : coroutine_handle_{ _src.coroutine_handle_ }
        {
            _src.coroutine_handle_ = {};
        }
        ~coroutine()
        {
            safe_destroy();
        }
      private:
        handle coroutine_handle_{};
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
        thread_pool()                                          = default;
        thread_pool( const thread_pool & )                     = delete;
        thread_pool( thread_pool && )                          = default;
        ~thread_pool()                                         = default;
    };
#if defined( _WIN32 ) || defined( _WIN64 )
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
    inline auto get_current_window_handle()
    {
        auto window_handle{ GetActiveWindow() };
        if ( window_handle == nullptr ) {
            window_handle = GetForegroundWindow();
        }
        if ( window_handle == nullptr ) {
            window_handle = GetConsoleWindow();
        }
        return window_handle;
    }
    inline auto get_window_state( const HWND _window_handle )
    {
        WINDOWPLACEMENT wp;
        wp.length = sizeof( WINDOWPLACEMENT );
        GetWindowPlacement( _window_handle, &wp );
        return wp.showCmd;
    }
    inline auto set_window_state( const HWND _window_handle, const UINT _state )
    {
        ShowWindow( _window_handle, _state );
    }
    inline auto keep_window_top( const HWND _window_handle, const DWORD _thread_id, const DWORD _window_thread_process_id )
    {
        AttachThreadInput( _thread_id, _window_thread_process_id, TRUE );
        set_window_state( _window_handle, get_window_state( _window_handle ) );
        SetForegroundWindow( _window_handle );
        AttachThreadInput( _thread_id, _window_thread_process_id, FALSE );
        SetWindowPos( _window_handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
    }
    inline auto keep_current_window_top()
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
            AttachThreadInput( _thread_id, _window_thread_process_id, FALSE );
            SetWindowPos( _window_handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
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
    inline auto cancel_top_window( const HWND _window_handle )
    {
        SetWindowPos( _window_handle, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
    }
    inline auto ignore_console_exit_signal( const bool _is_ignore ) noexcept
    {
        return SetConsoleCtrlHandler( nullptr, static_cast< WINBOOL >( _is_ignore ) );
    }
    inline auto clear_console_screen()
    {
        const auto output_handle{ GetStdHandle( STD_OUTPUT_HANDLE ) };
        DWORD mode;
        GetConsoleMode( output_handle, &mode );
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode( output_handle, mode );
        std::print( "\033[H\033[2J\033c" );
    }
    inline auto set_console_title( const ansi_char *const _title )
    {
        SetConsoleTitleA( _title );
    }
    inline auto set_console_title( const ansi_std_string &_title )
    {
        SetConsoleTitleA( _title.data() );
    }
    inline auto set_console_title( const wide_char *const _title )
    {
        SetConsoleTitleW( _title );
    }
    inline auto set_console_title( const wide_std_string &_title )
    {
        SetConsoleTitleW( _title.data() );
    }
    inline auto set_console_charset( const UINT _charset_id )
    {
        SetConsoleOutputCP( _charset_id );
        SetConsoleCP( _charset_id );
    }
    inline auto set_console_size( const SHORT _width, const SHORT _height )
    {
        HANDLE output_handle{ GetStdHandle( STD_OUTPUT_HANDLE ) };
        SMALL_RECT wrt{ 0, 0, static_cast< SHORT >( _width - 1 ), static_cast< SHORT >( _height - 1 ) };
        ShowWindow( GetConsoleWindow(), SW_SHOWNORMAL );
        SetConsoleScreenBufferSize( output_handle, { _width, _height } );
        SetConsoleWindowInfo( output_handle, TRUE, &wrt );
        SetConsoleScreenBufferSize( output_handle, { _width, _height } );
        clear_console_screen();
    }
    inline auto set_window_translucency( const HWND _window_handle, const BYTE _value )
    {
        SetLayeredWindowAttributes( _window_handle, RGB( 0, 0, 0 ), _value, LWA_ALPHA );
    }
    inline auto fix_window_size( const HWND _window_handle, const bool _is_enable )
    {
        SetWindowLongPtrW(
          _window_handle, GWL_STYLE,
          _is_enable
            ? GetWindowLongPtrW( _window_handle, GWL_STYLE ) & ~WS_SIZEBOX
            : GetWindowLongPtrW( _window_handle, GWL_STYLE ) | WS_SIZEBOX );
    }
    inline auto enable_window_menu( const HWND _window_handle, const bool _is_enable )
    {
        SetWindowLongPtrW(
          _window_handle, GWL_STYLE,
          _is_enable
            ? GetWindowLongPtrW( _window_handle, GWL_STYLE ) | WS_SYSMENU
            : GetWindowLongPtrW( _window_handle, GWL_STYLE ) & ~WS_SYSMENU );
    }
    inline auto enable_window_minimize_ctrl( const HWND _window_handle, const bool _is_enable )
    {
        SetWindowLongPtrW(
          _window_handle, GWL_STYLE,
          _is_enable
            ? GetWindowLongPtrW( _window_handle, GWL_STYLE ) | WS_MINIMIZEBOX
            : GetWindowLongPtrW( _window_handle, GWL_STYLE ) & ~WS_MINIMIZEBOX );
    }
    inline auto enable_window_maximize_ctrl( const HWND _window_handle, const bool _is_enable )
    {
        SetWindowLongPtrW(
          _window_handle, GWL_STYLE,
          _is_enable
            ? GetWindowLongPtrW( _window_handle, GWL_STYLE ) | WS_MAXIMIZEBOX
            : GetWindowLongPtrW( _window_handle, GWL_STYLE ) & ~WS_MAXIMIZEBOX );
    }
    inline auto enable_window_close_ctrl( const HWND _window_handle, const bool _is_enable )
    {
        EnableMenuItem(
          GetSystemMenu( _window_handle, FALSE ), SC_CLOSE,
          _is_enable ? MF_BYCOMMAND | MF_ENABLED : MF_BYCOMMAND | MF_DISABLED | MF_GRAYED );
    }
    namespace console_value {
        inline constexpr DWORD mouse_button_left{ FROM_LEFT_1ST_BUTTON_PRESSED };
        inline constexpr DWORD mouse_button_middle{ FROM_LEFT_2ND_BUTTON_PRESSED };
        inline constexpr DWORD mouse_button_right{ RIGHTMOST_BUTTON_PRESSED };
        inline constexpr DWORD mouse_click{ 0x0000 };
        inline constexpr DWORD mouse_click_double{ DOUBLE_CLICK };
        inline constexpr DWORD mouse_move{ MOUSE_MOVED };
        inline constexpr DWORD mouse_wheel_height{ MOUSE_HWHEELED };
        inline constexpr DWORD mouse_wheel{ MOUSE_WHEELED };
        inline constexpr DWORD key_right_alt_press{ RIGHT_ALT_PRESSED };
        inline constexpr DWORD key_left_alt_press{ LEFT_ALT_PRESSED };
        inline constexpr DWORD key_right_ctrl_press{ RIGHT_CTRL_PRESSED };
        inline constexpr DWORD key_left_ctrl_press{ LEFT_CTRL_PRESSED };
        inline constexpr DWORD key_shift_press{ SHIFT_PRESSED };
        inline constexpr DWORD key_num_lock_on{ NUMLOCK_ON };
        inline constexpr DWORD key_scroll_lock_on{ SCROLLLOCK_ON };
        inline constexpr DWORD key_caps_lock_on{ CAPSLOCK_ON };
        inline constexpr DWORD key_enhanced_key{ ENHANCED_KEY };
        inline constexpr WORD text_default{ FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE };
        inline constexpr WORD text_foreground_red{ FOREGROUND_RED };
        inline constexpr WORD text_foreground_green{ FOREGROUND_GREEN };
        inline constexpr WORD text_foreground_blue{ FOREGROUND_BLUE };
        inline constexpr WORD text_foreground_intensity{ FOREGROUND_INTENSITY };
        inline constexpr WORD text_background_red{ BACKGROUND_RED };
        inline constexpr WORD text_background_green{ BACKGROUND_GREEN };
        inline constexpr WORD text_background_blue{ BACKGROUND_BLUE };
        inline constexpr WORD text_background_intensity{ BACKGROUND_INTENSITY };
        inline constexpr WORD text_lvb_leading_byte{ COMMON_LVB_LEADING_BYTE };
        inline constexpr WORD text_lvb_trailing_byte{ COMMON_LVB_TRAILING_BYTE };
        inline constexpr WORD text_lvb_grid_horizontal{ COMMON_LVB_GRID_HORIZONTAL };
        inline constexpr WORD text_lvb_grid_lvertical{ COMMON_LVB_GRID_LVERTICAL };
        inline constexpr WORD text_lvb_grid_rvertical{ COMMON_LVB_GRID_RVERTICAL };
        inline constexpr WORD text_lvb_reverse_video{ COMMON_LVB_REVERSE_VIDEO };
        inline constexpr WORD text_lvb_underscore{ COMMON_LVB_UNDERSCORE };
        inline constexpr WORD text_lvb_sbcsdbcs{ COMMON_LVB_SBCSDBCS };
    };
    class console_ui final {
      public:
        using func_return_type = bool;
        inline static constexpr func_return_type back{ false };
        inline static constexpr func_return_type exit{ true };
        struct func_args final {
            console_ui &parent_ui;
            const size_type node_index;
            const DWORD mouse_button_state;
            const DWORD ctrl_key_state;
            const DWORD event_flag;
            auto operator=( const func_args & ) noexcept -> func_args & = default;
            auto operator=( func_args && ) noexcept -> func_args &      = default;
            func_args(
              console_ui &_parent_ui, const size_type _node_index,
              const MOUSE_EVENT_RECORD _mouse_event = MOUSE_EVENT_RECORD{ {}, console_value::mouse_button_left, {}, {} } ) noexcept
              : parent_ui{ _parent_ui }
              , node_index{ _node_index }
              , mouse_button_state{ _mouse_event.dwButtonState }
              , ctrl_key_state{ _mouse_event.dwControlKeyState }
              , event_flag{ _mouse_event.dwEventFlags }
            { }
            func_args( const func_args & ) noexcept = default;
            func_args( func_args && ) noexcept      = default;
            ~func_args() noexcept                   = default;
        };
        using callback_type = std::function< func_return_type( func_args ) >;
      private:
        enum class console_attrs_ { normal, lock_text, lock_all };
        struct line_node_ final {
            ansi_std_string text{};
            callback_type func{};
            WORD default_attrs{};
            WORD intensity_attrs{};
            WORD last_attrs{};
            COORD position{};
            auto set_attrs( const WORD _attrs ) noexcept
            {
                SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), _attrs );
                last_attrs = _attrs;
            }
            auto operator==( const COORD _mouse_position ) const noexcept
            {
                return position.Y == _mouse_position.Y && position.X <= _mouse_position.X
                    && _mouse_position.X < ( position.X + static_cast< SHORT >( text.size() ) );
            }
            auto operator!=( const COORD _mouse_position ) const noexcept
            {
                return !operator==( _mouse_position );
            }
            auto operator=( const line_node_ & ) noexcept -> line_node_ & = default;
            auto operator=( line_node_ && ) noexcept -> line_node_ &      = default;
            line_node_() noexcept
              : default_attrs{ console_value::text_default }
              , intensity_attrs{ console_value::text_foreground_green | console_value::text_foreground_blue }
              , last_attrs{ console_value::text_default }
            { }
            line_node_(
              const ansi_std_string_view _text, callback_type &_func, const WORD _default_attrs, const WORD _intensity_attrs ) noexcept
              : text{ _text }
              , func{ std::move( _func ) }
              , default_attrs{ _default_attrs }
              , intensity_attrs{ _intensity_attrs }
              , last_attrs{ _default_attrs }
            { }
            line_node_( const line_node_ & ) noexcept = default;
            line_node_( line_node_ && ) noexcept      = default;
            ~line_node_() noexcept                    = default;
        };
        std::deque< line_node_ > lines_{};
        static auto show_cursor_( const WINBOOL _is_show ) noexcept
        {
            CONSOLE_CURSOR_INFO cursor_data;
            GetConsoleCursorInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &cursor_data );
            cursor_data.bVisible = _is_show;
            SetConsoleCursorInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &cursor_data );
        }
        static auto edit_console_attrs_( const console_attrs_ _attrs ) noexcept
        {
            DWORD attrs;
            GetConsoleMode( GetStdHandle( STD_INPUT_HANDLE ), &attrs );
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
            SetConsoleMode( GetStdHandle( STD_INPUT_HANDLE ), attrs );
        }
        static auto get_cursor_() noexcept
        {
            CONSOLE_SCREEN_BUFFER_INFO console_data;
            GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &console_data );
            return console_data.dwCursorPosition;
        }
        static auto set_cursor_( const COORD _cursor_position ) noexcept
        {
            SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), _cursor_position );
        }
        static auto wait_mouse_event_( const bool _is_mouse_move = true ) noexcept
        {
            using namespace std::chrono_literals;
            INPUT_RECORD record;
            DWORD reg;
            while ( true ) {
                std::this_thread::sleep_for( 10ms );
                ReadConsoleInputW( GetStdHandle( STD_INPUT_HANDLE ), &record, 1, &reg );
                if ( record.EventType == MOUSE_EVENT
                     && ( _is_mouse_move || record.Event.MouseEvent.dwEventFlags != console_value::mouse_move ) )
                {
                    return record.Event.MouseEvent;
                }
            }
        }
        static auto get_console_size_() noexcept
        {
            CONSOLE_SCREEN_BUFFER_INFO console_data;
            GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &console_data );
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
        auto call_func_( const MOUSE_EVENT_RECORD &_mouse_event )
        {
            auto is_exit{ back };
            auto size{ lines_.size() };
            for ( size_type idx{ 0 }; idx < size; ++idx ) {
                auto &line{ lines_[ idx ] };
                if ( line != _mouse_event.dwMousePosition ) {
                    continue;
                }
                if ( line.func == nullptr ) {
                    break;
                }
                cls_();
                line.set_attrs( line.default_attrs );
                show_cursor_( FALSE );
                edit_console_attrs_( console_attrs_::lock_all );
                is_exit = line.func( func_args{ *this, idx, _mouse_event } );
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
          const WORD _intensity_attrs = console_value::text_foreground_green | console_value::text_foreground_blue,
          const WORD _default_attrs   = console_value::text_default )
        {
            lines_.emplace_front( _text, _func, _default_attrs, _func != nullptr ? _intensity_attrs : _default_attrs );
            return *this;
        }
        auto &add_back(
          const ansi_std_string_view _text, callback_type _func = nullptr,
          const WORD _intensity_attrs = console_value::text_foreground_blue | console_value::text_foreground_green,
          const WORD _default_attrs   = console_value::text_default )
        {
            lines_.emplace_back( _text, _func, _default_attrs, _func != nullptr ? _intensity_attrs : _default_attrs );
            return *this;
        }
        auto &insert(
          const size_type _index, const ansi_std_string_view _text, callback_type _func = nullptr,
          const WORD _intensity_attrs = console_value::text_foreground_green | console_value::text_foreground_blue,
          const WORD _default_attrs   = console_value::text_default )
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
          const WORD _intensity_attrs = console_value::text_foreground_green | console_value::text_foreground_blue,
          const WORD _default_attrs   = console_value::text_default )
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
            MOUSE_EVENT_RECORD mouse_event;
            auto func_return_value{ back };
            while ( func_return_value == back ) {
                mouse_event = wait_mouse_event_();
                switch ( mouse_event.dwEventFlags ) {
                    case console_value::mouse_move : refresh_( mouse_event.dwMousePosition ); break;
                    case console_value::mouse_click : {
                        if ( mouse_event.dwButtonState != false ) {
                            func_return_value = call_func_( mouse_event );
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
        console_ui() noexcept                                         = default;
        console_ui( const console_ui & ) noexcept                     = default;
        console_ui( console_ui && ) noexcept                          = default;
        ~console_ui() noexcept                                        = default;
    };
#endif
}
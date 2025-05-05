#pragma once
#if defined( _WIN32 ) || defined( _WIN64 )
# include <windows.h>
#endif
#include <any>
#include <chrono>
#include <concepts>
#include <coroutine>
#include <deque>
#include <exception>
#include <format>
#include <functional>
#include <iterator>
#include <memory>
#include <optional>
#include <print>
#include <ranges>
#include <source_location>
#include <stacktrace>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>
#include <typeindex>
#include <utility>
#include <vector>
namespace cpp_utils {
    using size_type    = std::size_t;
    using nullptr_type = std::nullptr_t;
    template < typename _type_ >
    using type_alloc = _type_;
    template < typename _char_type >
    using std_string = std::basic_string< _char_type >;
    template < typename _char_type >
    using std_string_view = std::basic_string_view< _char_type >;
    template < typename _type_ >
    using add_const_lvalue_reference_type = std::add_lvalue_reference_t< std::add_const_t< _type_ > >;
    template < typename _type_ >
    concept char_type
      = std::same_as< std::decay_t< _type_ >, char > || std::same_as< std::decay_t< _type_ >, wchar_t >
     || std::same_as< std::decay_t< _type_ >, char8_t > || std::same_as< std::decay_t< _type_ >, char16_t >
     || std::same_as< std::decay_t< _type_ >, char32_t >;
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
    auto make_log(
      const std::string_view _message,
      const std::source_location _source_location = std::source_location::current(),
      const std::stacktrace _stacktrace           = std::stacktrace::current() )
    {
        return std::format(
          "{}({}:{}) `{}`: {}\n{}\n", _source_location.file_name(), _source_location.line(), _source_location.column(),
          _source_location.function_name(), _message, _stacktrace );
    }
    auto dynamic_assert(
      const bool _expression,
      const std::string_view _failed_message      = "assertion failid!",
      const std::source_location _source_location = std::source_location::current(),
      std::stacktrace _stacktrace                 = std::stacktrace::current() ) noexcept
    {
        if ( _expression == false ) {
            std::print( "{}", make_log( _failed_message, _source_location, std::move( _stacktrace ) ) );
            std::terminate();
        }
    }
    template < bool _condition_ >
    auto dynamic_assert_if(
      const bool _expression,
      const std::string_view _failed_message      = "assertion failid!",
      const std::source_location _source_location = std::source_location::current(),
      std::stacktrace _stacktrace                 = std::stacktrace::current() ) noexcept
    {
        if constexpr ( _condition_ == true ) {
            dynamic_assert( _expression, _failed_message, _source_location, std::move( _stacktrace ) );
        }
    }
    template < std::random_access_iterator _iterator_ >
    auto parallel_for_each( _iterator_ &&_begin, _iterator_ &&_end, auto &&_func )
    {
        const auto max_thread_num{ std::thread::hardware_concurrency() };
        [[assume( max_thread_num > 0 )]];
        const auto chunk_size{ ( _end - _begin ) / max_thread_num };
        std::vector< std::thread > threads;
        threads.reserve( max_thread_num );
        for ( const auto i : std::ranges::iota_view{ decltype( max_thread_num ){ 0 }, max_thread_num } ) {
            const auto chunk_start{ _begin + i * chunk_size };
            const auto chunk_end{ ( i == max_thread_num - 1 ) ? _end : chunk_start + chunk_size };
            threads.emplace_back( [ =, &_func ]()
            {
                for ( auto it{ chunk_start }; it != chunk_end; ++it ) {
                    _func( *it );
                }
            } );
        }
        for ( auto &thread : threads ) {
            thread.join();
        }
    }
    template < std::random_access_iterator _iterator_ >
    auto parallel_for_each( unsigned int _thread_num, _iterator_ &&_begin, _iterator_ &&_end, auto &&_func )
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
            } );
        }
        for ( auto &thread : threads ) {
            thread.join();
        }
    }
    template < pointer_type _type_ >
    inline auto pointer_to_string( const _type_ _ptr )
    {
        using namespace std::string_literals;
        return _ptr == nullptr ? "nullptr"s : std::format( "0x{:x}", reinterpret_cast< std::uintptr_t >( _ptr ) );
    }
    template < pointer_type _type_ >
        requires( !std::same_as< std::decay_t< _type_ >, void * > && !std::is_const_v< _type_ > )
    class raw_pointer_wrapper final {
      private:
        _type_ ptr_{};
      public:
        auto get() const
        {
            return ptr_;
        }
        auto &operator*() const
        {
            return *ptr_;
        }
        auto &operator[]( const size_type _n ) const
        {
            return ptr_[ _n ];
        }
        auto operator+( const size_type _n ) const
        {
            return ptr_ + _n;
        }
        auto operator-( const size_type _n ) const
        {
            return ptr_ - _n;
        }
        auto operator++() -> raw_pointer_wrapper< _type_ > &
        {
            ++ptr_;
            return *this;
        }
        auto operator++( int ) -> raw_pointer_wrapper< _type_ >
        {
            return ptr_++;
        }
        auto operator=( const raw_pointer_wrapper< _type_ > & ) -> raw_pointer_wrapper< _type_ > & = default;
        auto operator=( raw_pointer_wrapper< _type_ > && ) -> raw_pointer_wrapper< _type_ > &      = default;
        constexpr raw_pointer_wrapper()                                                            = default;
        constexpr raw_pointer_wrapper( _type_ _ptr )
          : ptr_{ _ptr }
        { }
        constexpr raw_pointer_wrapper( const raw_pointer_wrapper< _type_ > & ) = default;
        constexpr raw_pointer_wrapper( raw_pointer_wrapper< _type_ > && )      = default;
        ~raw_pointer_wrapper()                                                 = default;
    };
    template < char_type _type_, size_type _capacity_ >
        requires( std::same_as< _type_, std::decay_t< _type_ > > && _capacity_ > 0 )
    class constant_string final {
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
            for ( const auto i : std::ranges::iota_view{ decltype( _capacity_ ){ 0 }, _capacity_ } ) {
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
            for ( const auto i : std::ranges::iota_view{ decltype( _capacity_ ){ 0 }, _capacity_ } ) {
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
            for ( const auto i : std::ranges::iota_view{ decltype( _capacity_ ){ 0 }, _capacity_ } ) {
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
        ~constant_string() noexcept                                                    = default;
    };
    template < size_type _capacity_ >
    using constant_ansi_string = constant_string< char, _capacity_ >;
    template < size_type _capacity_ >
    using constant_wide_string = constant_string< wchar_t, _capacity_ >;
    template < size_type _capacity_ >
    using constant_utf8_string = constant_string< char8_t, _capacity_ >;
    template < size_type _capacity_ >
    using constant_utf16_string = constant_string< char16_t, _capacity_ >;
    template < size_type _capacity_ >
    using constant_utf32_string = constant_string< char32_t, _capacity_ >;
#if ( defined( __GNUC__ ) && defined( __GXX_RTTI ) ) || ( defined( _MSC_VER ) && defined( _CPPRTTI ) ) \
  || ( defined( __clang__ ) && __has_feature( cxx_rtti ) )
    class func_wrapper_impl {
      public:
        virtual ~func_wrapper_impl()                                                 = default;
        virtual auto empty() const -> bool                                           = 0;
        virtual auto args_type() const -> const std::vector< std::type_index > &     = 0;
        virtual auto invoke( const std::vector< std::any > &args ) const -> std::any = 0;
    };
    template < typename _return_type_, typename... _args_ >
    class func_wrapper : public func_wrapper_impl {
      private:
        std::function< _return_type_( _args_... ) > func_;
        std::vector< std::type_index > args_type_{ std::type_index{ typeid( _args_ ) }... };
        template < size_type... _args_index_ >
        auto invoke_impl_( const std::vector< std::any > &_args, std::index_sequence< _args_index_... > ) const -> std::any
        {
            if constexpr ( std::is_void_v< _return_type_ > ) {
                std::invoke( func_, std::any_cast< _args_ >( _args[ _args_index_ ] )... );
                return {};
            } else {
                return std::invoke( func_, std::any_cast< _args_ >( _args[ _args_index_ ] )... );
            }
        }
      public:
        virtual auto empty() const -> bool
        {
            return func_ == nullptr;
        }
        virtual auto args_type() const -> const std::vector< std::type_index > & override final
        {
            return args_type_;
        }
        virtual auto invoke( const std::vector< std::any > &_args ) const -> std::any override final
        {
            if ( sizeof...( _args_ ) < _args.size() ) [[unlikely]] {
                throw std::invalid_argument{ "arguments error" };
            }
            return invoke_impl_( _args, std::index_sequence_for< _args_... >{} );
        }
        func_wrapper( std::function< _return_type_( _args_... ) > _f )
          : func_{ std::move( _f ) }
        { }
    };
    class func_container final {
      private:
        std::deque< std::unique_ptr< func_wrapper_impl > > func_nodes_{};
      public:
        auto empty() const noexcept
        {
            return func_nodes_.empty();
        }
        auto size() const noexcept
        {
            return func_nodes_.size();
        }
        auto max_size() const noexcept
        {
            return func_nodes_.max_size();
        }
        auto &resize( const size_type _size )
        {
            func_nodes_.resize( _size );
            return *this;
        }
        auto &optimize_storage() noexcept
        {
            func_nodes_.shrink_to_fit();
            return *this;
        }
        auto &swap( func_container &_src ) noexcept
        {
            func_nodes_.swap( _src.func_nodes_ );
            return *this;
        }
        template < typename... _args_ >
        static auto make_args( _args_ &&..._args )
        {
            std::vector< std::any > args;
            args.reserve( sizeof...( _args ) );
            ( args.emplace_back( std::forward< _args_ >( _args ) ), ... );
            return args;
        }
        template < typename _return_type_, typename... _args_ >
        auto &add_front( std::function< _return_type_( _args_... ) > _func )
        {
            func_nodes_.emplace_front( std::make_unique< func_wrapper< _return_type_, _args_... > >( std::move( _func ) ) );
            return *this;
        }
        template < typename _return_type_, typename... _args_ >
        auto &add_back( std::function< _return_type_( _args_... ) > _func )
        {
            func_nodes_.emplace_back( std::make_unique< func_wrapper< _return_type_, _args_... > >( std::move( _func ) ) );
            return *this;
        }
        template < typename _return_type_, typename... _args_ >
        auto &insert( const size_type _index, std::function< _return_type_( _args_... ) > _func )
        {
            func_nodes_.emplace(
              func_nodes_.cbegin() + _index, std::make_unique< func_wrapper< _return_type_, _args_... > >( std::move( _func ) ) );
            return *this;
        }
        template < typename _return_type_, typename... _args_ >
        auto &edit( const size_type _index, std::function< _return_type_( _args_... ) > _func )
        {
            func_nodes_.at( _index ) = std::make_unique< func_wrapper< _return_type_, _args_... > >( std::move( _func ) );
            return *this;
        }
        auto &remove_front() noexcept
        {
            func_nodes_.pop_front();
            return *this;
        }
        auto &remove_back() noexcept
        {
            func_nodes_.pop_back();
            return *this;
        }
        auto &remove( const size_type _begin, const size_type _length )
        {
            func_nodes_.erase( func_nodes_.cbegin() + _begin, func_nodes_.cbegin() + _begin + _length );
            return *this;
        }
        auto &clear() noexcept
        {
            func_nodes_.clear();
            return *this;
        }
        template < typename _return_type_, typename... _args_ >
        auto invoke( const size_type _index, _args_ &&..._args ) const
        {
            if constexpr ( std::same_as< std::decay_t< _return_type_ >, void > ) {
                func_nodes_.at( _index )->invoke( make_args( std::forward< _args_ >( _args )... ) );
            } else {
                return std::any_cast< _return_type_ >(
                  func_nodes_.at( _index )->invoke( make_args( std::forward< _args_ >( _args )... ) ) );
            }
        }
        template < typename _return_type_ >
        auto dynamic_invoke( const size_type _index, const std::vector< std::any > &_args ) const
        {
            if constexpr ( std::same_as< std::decay_t< _return_type_ >, void > ) {
                func_nodes_.at( _index )->invoke( _args );
            } else {
                return std::any_cast< _return_type_ >( func_nodes_.at( _index )->invoke( _args ) );
            }
        }
    };
#endif
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
            std::optional< return_type_ > current_value{};
            std::exception_ptr current_exception{};
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
            if ( this != &_src ) [[likely]] {
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
            return_type_ current_value{};
            std::exception_ptr current_exception{};
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
            if ( this != &_src ) [[likely]] {
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
            if ( this != &_src ) [[likely]] {
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
        template < callable_type _func_, typename... _args_ >
        auto &add( _func_ &&_func, _args_ &&..._args )
        {
            threads_.emplace_back( std::forward< _func_ >( _func ), std::forward< _args_ >( _args )... );
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
        auto &safe_join_all() noexcept
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
        auto &safe_detach_all() noexcept
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
        auto &request_stop_to_all() noexcept
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
        inline constexpr WORD default_attrs{ FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE };
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
    inline auto relaunch( const int _exit_code ) noexcept
    {
        wchar_t file_path[ MAX_PATH ]{};
        GetModuleFileNameW( nullptr, file_path, MAX_PATH );
        ShellExecuteW( nullptr, L"open", file_path, nullptr, nullptr, SW_SHOWNORMAL );
        std::exit( _exit_code );
    }
    inline auto relaunch_as_admin( const int _exit_code ) noexcept
    {
        wchar_t file_path[ MAX_PATH ]{};
        GetModuleFileNameW( nullptr, file_path, MAX_PATH );
        ShellExecuteW( nullptr, L"runas", file_path, nullptr, nullptr, SW_SHOWNORMAL );
        std::exit( _exit_code );
    }
    inline auto get_current_console_std_handle( const DWORD _std_handle_flag ) noexcept
    {
        return GetStdHandle( _std_handle_flag );
    }
    inline auto get_current_window_handle() noexcept
    {
        auto window_handle{ GetConsoleWindow() };
        if ( window_handle == nullptr ) [[unlikely]] {
            window_handle = GetForegroundWindow();
        }
        if ( window_handle == nullptr ) [[unlikely]] {
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
    template < std_chrono_type _chrono_type_, callable_type _func_, typename... _args_ >
    inline auto loop_keep_window_top(
      const HWND _window_handle, const DWORD _thread_id, const DWORD _window_thread_process_id, const _chrono_type_ _sleep_time,
      _func_ &&_condition_checker, _args_ &&..._condition_checker_args )
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
    template < std_chrono_type _chrono_type_, callable_type _func_, typename... _args_ >
    inline auto loop_keep_current_window_top(
      const _chrono_type_ _sleep_time, _func_ &&_condition_checker, _args_ &&..._condition_checker_args )
    {
        auto window_handle{ get_current_window_handle() };
        loop_keep_window_top(
          window_handle, GetCurrentThreadId(), GetWindowThreadProcessId( window_handle, nullptr ), _sleep_time,
          std::forward< _func_ >( _condition_checker ), std::forward< _args_ >( _condition_checker_args )... );
    }
    inline auto cancel_top_window( const HWND _window_handle ) noexcept
    {
        SetWindowPos( _window_handle, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
    }
    inline auto ignore_console_exit_signal( const bool _is_ignore ) noexcept
    {
        return SetConsoleCtrlHandler( nullptr, static_cast< WINBOOL >( _is_ignore ) );
    }
    inline auto enable_virtual_terminal_processing( const HANDLE _std_output_handle ) noexcept
    {
        DWORD mode;
        GetConsoleMode( _std_output_handle, &mode );
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode( _std_output_handle, mode );
    }
    inline auto disable_virtual_terminal_processing( const HANDLE _std_output_handle ) noexcept
    {
        DWORD mode;
        GetConsoleMode( _std_output_handle, &mode );
        mode &= ~ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode( _std_output_handle, mode );
    }
    inline auto clear_console( const HANDLE _std_output_handle ) noexcept
    {
        enable_virtual_terminal_processing( _std_output_handle );
        std::print( "\033[H\033[2J" );
    }
    inline auto reset_console( const HANDLE _std_output_handle ) noexcept
    {
        enable_virtual_terminal_processing( _std_output_handle );
        std::print( "\033c" );
    }
    inline auto set_console_title( const char *const _title ) noexcept
    {
        SetConsoleTitleA( _title );
    }
    inline auto set_console_title( const std::string &_title ) noexcept
    {
        SetConsoleTitleA( _title.data() );
    }
    inline auto set_console_title( const wchar_t *const _title ) noexcept
    {
        SetConsoleTitleW( _title );
    }
    inline auto set_console_title( const std::wstring &_title ) noexcept
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
        clear_console( _std_output_handle );
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
        enum class console_attrs_ : char { normal, lock_text, lock_all };
        struct line_node_ final {
            std::string text{};
            callback_type func{};
            WORD default_attrs{ console_text::default_attrs };
            WORD intensity_attrs{ console_text::foreground_green | console_text::foreground_blue };
            WORD last_attrs{ console_text::default_attrs };
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
            line_node_() noexcept                                         = default;
            line_node_( const std::string_view _text, callback_type &_func, const WORD _default_attrs, const WORD _intensity_attrs ) noexcept
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
            const auto [ width, height ]{ get_console_size_() };
            set_cursor_( COORD{ 0, 0 } );
            std::print( "{}", std::string( static_cast< unsigned int >( width ) * static_cast< unsigned int >( height ), ' ' ) );
            set_cursor_( COORD{ 0, 0 } );
        }
        static auto write_( const std::string_view _text, const bool _is_endl = false )
        {
            std::print( "{}", _text );
            if ( _is_endl ) {
                std::print( "\n" );
            }
        }
        static auto rewrite_( const COORD _cursor_position, const std::string_view _text )
        {
            set_cursor_( COORD{ 0, _cursor_position.Y } );
            write_( std::string( _cursor_position.X, ' ' ) );
            set_cursor_( COORD{ 0, _cursor_position.Y } );
            write_( _text );
            set_cursor_( COORD{ 0, _cursor_position.Y } );
        }
        auto init_pos_()
        {
            cls_();
            const auto tail{ &lines_.back() };
            for ( auto &line : lines_ ) {
                line.position = get_cursor_();
                line.set_attrs( line.default_attrs );
                write_( line.text, &line != tail );
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
        auto invoke_func_( const MOUSE_EVENT_RECORD &_event )
        {
            auto is_exit{ back };
            auto size{ lines_.size() };
            for ( const auto idx : std::ranges::iota_view{ decltype( size ){ 0 }, size } ) {
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
          const std::string_view _text, callback_type _func = nullptr,
          const WORD _intensity_attrs = console_text::foreground_green | console_text::foreground_blue,
          const WORD _default_attrs   = console_text::default_attrs )
        {
            lines_.emplace_front( _text, _func, _default_attrs, _func != nullptr ? _intensity_attrs : _default_attrs );
            return *this;
        }
        auto &add_back(
          const std::string_view _text, callback_type _func = nullptr,
          const WORD _intensity_attrs = console_text::foreground_blue | console_text::foreground_green,
          const WORD _default_attrs   = console_text::default_attrs )
        {
            lines_.emplace_back( _text, _func, _default_attrs, _func != nullptr ? _intensity_attrs : _default_attrs );
            return *this;
        }
        auto &insert(
          const size_type _index, const std::string_view _text, callback_type _func = nullptr,
          const WORD _intensity_attrs = console_text::foreground_green | console_text::foreground_blue,
          const WORD _default_attrs   = console_text::default_attrs )
        {
            lines_.emplace(
              lines_.cbegin() + _index, _text, _func, _default_attrs, _func != nullptr ? _intensity_attrs : _default_attrs );
            return *this;
        }
        auto &edit_text( const size_type _index, const std::string_view _text )
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
          const size_type _index, const std::string_view _text, callback_type _func = nullptr,
          const WORD _intensity_attrs = console_text::foreground_green | console_text::foreground_blue,
          const WORD _default_attrs   = console_text::default_attrs )
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
            if ( empty() ) {
                return *this;
            }
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
                            func_return_value = invoke_func_( event );
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
            if ( std_input_handle_ == nullptr ) [[unlikely]] {
                std_input_handle_ = GetStdHandle( console_handle_flag::std_input );
            }
            if ( std_output_handle_ == nullptr ) [[unlikely]] {
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
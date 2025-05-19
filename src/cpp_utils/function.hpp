#pragma once
#include <any>
#include <concepts>
#include <coroutine>
#include <deque>
#include <functional>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <typeindex>
#include <utility>
#include <vector>
#include "coroutine.hpp"
#include "type_tools.hpp"
namespace cpp_utils {
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
        func_wrapper( std::function< _return_type_( _args_... ) > _fn )
          : func_{ std::move( _fn ) }
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
            requires( !coroutine_func_return_type< _return_type_ > )
        auto &add_front( _return_type_ ( *_func )( _args_... ) )
        {
            func_nodes_.emplace_front( std::make_unique< func_wrapper< _return_type_, _args_... > >( _func ) );
            return *this;
        }
        template < typename _return_type_, typename... _args_ >
            requires( !coroutine_func_return_type< _return_type_ > )
        auto &add_front( std::function< _return_type_( _args_... ) > _func )
        {
            func_nodes_.emplace_front( std::make_unique< func_wrapper< _return_type_, _args_... > >( std::move( _func ) ) );
            return *this;
        }
        template < typename _return_type_, typename... _args_ >
            requires( !coroutine_func_return_type< _return_type_ > )
        auto &add_back( _return_type_ ( *_func )( _args_... ) )
        {
            func_nodes_.emplace_back( std::make_unique< func_wrapper< _return_type_, _args_... > >( _func ) );
            return *this;
        }
        template < typename _return_type_, typename... _args_ >
            requires( !coroutine_func_return_type< _return_type_ > )
        auto &add_back( std::function< _return_type_( _args_... ) > _func )
        {
            func_nodes_.emplace_back( std::make_unique< func_wrapper< _return_type_, _args_... > >( std::move( _func ) ) );
            return *this;
        }
        template < typename _return_type_, typename... _args_ >
            requires( !coroutine_func_return_type< _return_type_ > )
        auto &insert( const size_type _index, _return_type_ ( *_func )( _args_... ) )
        {
            func_nodes_.emplace(
              func_nodes_.cbegin() + _index, std::make_unique< func_wrapper< _return_type_, _args_... > >( _func ) );
            return *this;
        }
        template < typename _return_type_, typename... _args_ >
            requires( !coroutine_func_return_type< _return_type_ > )
        auto &insert( const size_type _index, std::function< _return_type_( _args_... ) > _func )
        {
            func_nodes_.emplace(
              func_nodes_.cbegin() + _index, std::make_unique< func_wrapper< _return_type_, _args_... > >( std::move( _func ) ) );
            return *this;
        }
        template < typename _return_type_, typename... _args_ >
            requires( !coroutine_func_return_type< _return_type_ > )
        auto &edit( const size_type _index, _return_type_ ( *_func )( _args_... ) )
        {
            func_nodes_.at( _index ) = std::make_unique< func_wrapper< _return_type_, _args_... > >( _func );
            return *this;
        }
        template < typename _return_type_, typename... _args_ >
            requires( !coroutine_func_return_type< _return_type_ > )
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
            requires( !coroutine_func_return_type< _return_type_ > )
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
            requires( !coroutine_func_return_type< _return_type_ > )
        auto dynamic_invoke( const size_type _index, const std::vector< std::any > &_args ) const
        {
            if constexpr ( std::same_as< std::decay_t< _return_type_ >, void > ) {
                func_nodes_.at( _index )->invoke( _args );
            } else {
                return std::any_cast< _return_type_ >( func_nodes_.at( _index )->invoke( _args ) );
            }
        }
    };
#else
# error "RTTI must be enabled"
#endif
}
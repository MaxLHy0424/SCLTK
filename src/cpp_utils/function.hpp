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
#include "compiler.hpp"
#include "type_tools.hpp"
namespace cpp_utils
{
#if ( defined( __GNUC__ ) && defined( __GXX_RTTI ) ) || ( defined( _MSC_VER ) && defined( _CPPRTTI ) ) \
  || ( defined( __clang__ ) && __has_feature( cxx_rtti ) )
    class func_wrapper_impl
    {
      public:
        virtual ~func_wrapper_impl()                                                 = default;
        virtual auto empty() const -> bool                                           = 0;
        virtual auto args_type() const -> const std::vector< std::type_index >&      = 0;
        virtual auto invoke( const std::vector< std::any >& args ) const -> std::any = 0;
    };
    template < typename R, typename... Args >
    class func_wrapper final : public func_wrapper_impl
    {
      private:
        std::function< R( Args... ) > func_;
        std::vector< std::type_index > args_type_{ std::type_index{ typeid( Args ) }... };
        template < size_t... Is >
        auto invoke_impl_( const std::vector< std::any >& args, std::index_sequence< Is... > ) const -> std::any
        {
            if constexpr ( std::is_void_v< R > ) {
                std::invoke( func_, std::any_cast< Args >( args[ Is ] )... );
                return {};
            } else {
                return std::make_shared< R >( std::invoke( func_, std::any_cast< Args >( args[ Is ] )... ) );
            }
        }
      public:
        virtual auto empty() const noexcept -> bool
        {
            return func_ == nullptr;
        }
        virtual auto args_type() const -> const std::vector< std::type_index >& override final
        {
            return args_type_;
        }
        virtual auto invoke( const std::vector< std::any >& args ) const -> std::any override final
        {
            if ( sizeof...( Args ) != args.size() ) [[unlikely]] {
                throw std::invalid_argument{ "arguments error" };
            }
            return invoke_impl_( args, std::index_sequence_for< Args... >{} );
        }
        func_wrapper( std::function< R( Args... ) > func )
          : func_{ std::move( func ) }
        { }
    };
    class func_container final
    {
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
        auto& resize( const size_t size )
        {
            func_nodes_.resize( size );
            return *this;
        }
        auto& optimize_storage() noexcept
        {
            func_nodes_.shrink_to_fit();
            return *this;
        }
        auto& swap( func_container& src ) noexcept
        {
            func_nodes_.swap( src.func_nodes_ );
            return *this;
        }
        template < typename... Args >
        static auto make_args( Args&&... args )
        {
            std::vector< std::any > target;
            target.reserve( sizeof...( args ) );
            ( target.emplace_back( std::forward< Args >( args ) ), ... );
            return target;
        }
        template < typename R, typename... Args >
        auto& add_front( R ( *func )( Args... ) )
        {
            func_nodes_.emplace_front( std::make_unique< func_wrapper< R, Args... > >( func ) );
            return *this;
        }
        template < typename R, typename... Args >
        auto& add_front( std::function< R( Args... ) > func )
        {
            func_nodes_.emplace_front( std::make_unique< func_wrapper< R, Args... > >( std::move( func ) ) );
            return *this;
        }
        template < typename R, typename... Args >
        auto& add_back( R ( *func )( Args... ) )
        {
            func_nodes_.emplace_back( std::make_unique< func_wrapper< R, Args... > >( func ) );
            return *this;
        }
        template < typename R, typename... Args >
        auto& add_back( std::function< R( Args... ) > func )
        {
            func_nodes_.emplace_back( std::make_unique< func_wrapper< R, Args... > >( std::move( func ) ) );
            return *this;
        }
        template < typename R, typename... Args >
        auto& insert( const size_t index, R ( *func )( Args... ) )
        {
            func_nodes_.emplace( func_nodes_.cbegin() + index, std::make_unique< func_wrapper< R, Args... > >( func ) );
            return *this;
        }
        template < typename R, typename... Args >
        auto& insert( const size_t index, std::function< R( Args... ) > func )
        {
            func_nodes_.emplace( func_nodes_.cbegin() + index, std::make_unique< func_wrapper< R, Args... > >( std::move( func ) ) );
            return *this;
        }
        template < typename R, typename... Args >
        auto& edit( const size_t index, R ( *func )( Args... ) )
        {
            if constexpr ( is_debug_build ) {
                func_nodes_.at( index ) = std::make_unique< func_wrapper< R, Args... > >( func );
            } else {
                func_nodes_[ index ] = std::make_unique< func_wrapper< R, Args... > >( func );
            }
            return *this;
        }
        template < typename R, typename... Args >
        auto& edit( const size_t index, std::function< R( Args... ) > func )
        {
            if constexpr ( is_debug_build ) {
                func_nodes_.at( index ) = std::make_unique< func_wrapper< R, Args... > >( std::move( func ) );
            } else {
                func_nodes_[ index ] = std::make_unique< func_wrapper< R, Args... > >( std::move( func ) );
            }
            return *this;
        }
        auto& remove_front() noexcept
        {
            func_nodes_.pop_front();
            return *this;
        }
        auto& remove_back() noexcept
        {
            func_nodes_.pop_back();
            return *this;
        }
        auto& remove( const size_t begin, const size_t length )
        {
            func_nodes_.erase( func_nodes_.cbegin() + begin, func_nodes_.cbegin() + begin + length );
            return *this;
        }
        auto& clear() noexcept
        {
            func_nodes_.clear();
            return *this;
        }
        template < typename R, typename... Args >
        decltype( auto ) invoke( const size_t index, Args&&... args ) const
        {
            if constexpr ( std::is_same_v< std::decay_t< R >, void > ) {
                if constexpr ( is_debug_build ) {
                    func_nodes_.at( index )->invoke( make_args( std::forward< Args >( args )... ) );
                } else {
                    func_nodes_[ index ]->invoke( make_args( std::forward< Args >( args )... ) );
                }
            } else {
                if constexpr ( is_debug_build ) {
                    return std::move( *std::any_cast< std::shared_ptr< R > >(
                      func_nodes_.at( index )->invoke( make_args( std::forward< Args >( args )... ) ) ) );
                } else {
                    return std::move( *std::any_cast< std::shared_ptr< R > >(
                      func_nodes_[ index ]->invoke( make_args( std::forward< Args >( args )... ) ) ) );
                }
            }
        }
        template < typename R >
        decltype( auto ) dynamic_invoke( const size_t index, const std::vector< std::any >& args ) const
        {
            if constexpr ( std::is_same_v< std::decay_t< R >, void > ) {
                if constexpr ( is_debug_build ) {
                    func_nodes_.at( index )->invoke( args );
                } else {
                    func_nodes_[ index ]->invoke( args );
                }
            } else {
                if constexpr ( is_debug_build ) {
                    return std::move( *std::any_cast< std::shared_ptr< R > >( func_nodes_.at( index )->invoke( args ) ) );
                } else {
                    return std::move( *std::any_cast< std::shared_ptr< R > >( func_nodes_[ index ]->invoke( args ) ) );
                }
            }
        }
    };
#else
# error "RTTI must be enabled"
#endif
}
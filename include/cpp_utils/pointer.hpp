#pragma once
#include <concepts>
#include <memory>
#include <type_traits>
namespace cpp_utils
{
    template < typename T >
    concept raw_pointer = std::is_pointer_v< T >;
    template < typename NullChecker, typename PointerType >
    concept null_pointer_checker_for = requires( const PointerType ptr ) {
        { NullChecker{}( ptr ) } noexcept -> std::convertible_to< bool >;
    };
    struct default_null_pointer_checker final
    {
        template < raw_pointer T >
        static constexpr auto operator()( const T ptr ) noexcept
        {
            return ptr == nullptr;
        }
    };
    template < raw_pointer T, null_pointer_checker_for< T > NullChecker = default_null_pointer_checker >
        requires( !std::is_const_v< T > )
    class raw_pointer_wrapper final
    {
      private:
        T ptr_{};
      public:
        using value_type   = T;
        using checker_type = NullChecker;
        static constexpr auto computable() noexcept
        {
            return !std::is_same_v< std::remove_cv_t< std::remove_pointer_t< T > >, void >;
        }
        constexpr auto reset( const T src ) noexcept
        {
            ptr_ = src;
        }
        [[nodiscard]] constexpr auto get() const noexcept
        {
            return ptr_;
        }
        constexpr explicit operator bool() const noexcept
        {
            return !NullChecker{}( ptr_ );
        }
        [[nodiscard]] constexpr auto&& operator*() const noexcept
            requires( computable() )
        {
            return *ptr_;
        }
        [[nodiscard]] constexpr auto operator->() const noexcept
            requires( computable() )
        {
            return ptr_;
        }
        [[nodiscard]] constexpr auto&& operator[]( const std::size_t n ) const noexcept
            requires( computable() )
        {
            return ptr_[ n ];
        }
        [[nodiscard]] constexpr auto operator+( const std::size_t n ) const noexcept
            requires( computable() )
        {
            return raw_pointer_wrapper< T, NullChecker >{ ptr_ + n };
        }
        constexpr auto operator+=( const std::size_t n ) noexcept -> raw_pointer_wrapper< T, NullChecker >&
            requires( computable() )
        {
            ptr_ += n;
            return *this;
        }
        constexpr auto operator++() noexcept -> raw_pointer_wrapper< T, NullChecker >&
            requires( computable() )
        {
            ++ptr_;
            return *this;
        }
        constexpr auto operator++( int ) noexcept -> raw_pointer_wrapper< T, NullChecker >
            requires( computable() )
        {
            return ptr_++;
        }
        [[nodiscard]] constexpr auto operator-( const std::size_t n ) const noexcept
            requires( computable() )
        {
            return raw_pointer_wrapper< T, NullChecker >{ ptr_ - n };
        }
        constexpr auto operator-=( const std::size_t n ) noexcept -> raw_pointer_wrapper< T, NullChecker >&
            requires( computable() )
        {
            ptr_ -= n;
            return *this;
        }
        constexpr auto operator--() noexcept -> raw_pointer_wrapper< T, NullChecker >&
            requires( computable() )
        {
            --ptr_;
            return *this;
        }
        constexpr auto operator--( int ) noexcept -> raw_pointer_wrapper< T, NullChecker >
            requires( computable() )
        {
            return ptr_--;
        }
        constexpr auto operator=( const std::nullptr_t ) noexcept -> raw_pointer_wrapper< T, NullChecker >&
        {
            ptr_ = nullptr;
            return *this;
        }
        constexpr auto operator=( const raw_pointer_wrapper< T, NullChecker >& ) noexcept
          -> raw_pointer_wrapper< T, NullChecker >& = default;
        constexpr auto operator=( raw_pointer_wrapper< T, NullChecker >&& ) noexcept
          -> raw_pointer_wrapper< T, NullChecker >& = default;
        constexpr raw_pointer_wrapper() noexcept    = default;
        constexpr raw_pointer_wrapper( const std::nullptr_t ) noexcept
          : ptr_{ nullptr }
        { }
        constexpr raw_pointer_wrapper( const T ptr ) noexcept
          : ptr_{ ptr }
        { }
        constexpr raw_pointer_wrapper( const raw_pointer_wrapper< T, NullChecker >& ) noexcept = default;
        constexpr raw_pointer_wrapper( raw_pointer_wrapper< T, NullChecker >&& ) noexcept      = default;
        ~raw_pointer_wrapper() noexcept                                                        = default;
    };
    template < raw_pointer T, null_pointer_checker_for< T > NullChecker >
    [[nodiscard]] inline constexpr auto
      operator==( const raw_pointer_wrapper< T, NullChecker >& lhs, const raw_pointer_wrapper< T, NullChecker >& rhs ) noexcept
    {
        return lhs.get() == rhs.get();
    }
    template < raw_pointer T, null_pointer_checker_for< T > NullChecker >
    [[nodiscard]] inline constexpr auto
      operator!=( const raw_pointer_wrapper< T, NullChecker >& lhs, const raw_pointer_wrapper< T, NullChecker >& rhs ) noexcept
    {
        return lhs.get() != rhs.get();
    }
    template < raw_pointer T, null_pointer_checker_for< T > NullChecker >
    [[nodiscard]] inline constexpr auto operator==( const raw_pointer_wrapper< T, NullChecker >& lhs, const std::nullptr_t ) noexcept
    {
        return NullChecker{}( lhs.get() );
    }
    template < raw_pointer T, null_pointer_checker_for< T > NullChecker >
    [[nodiscard]] inline constexpr auto operator!=( const raw_pointer_wrapper< T, NullChecker >& lhs, const std::nullptr_t ) noexcept
    {
        return !NullChecker{}( lhs.get() );
    }
    template < raw_pointer T, null_pointer_checker_for< T > NullChecker >
    [[nodiscard]] inline constexpr auto operator==( const std::nullptr_t, const raw_pointer_wrapper< T, NullChecker >& rhs ) noexcept
    {
        return NullChecker{}( rhs.get() );
    }
    template < raw_pointer T, null_pointer_checker_for< T > NullChecker >
    [[nodiscard]] inline constexpr auto operator!=( const std::nullptr_t, const raw_pointer_wrapper< T, NullChecker >& rhs ) noexcept
    {
        return !NullChecker{}( rhs.get() );
    }
    namespace details_
    {
        template < typename T, null_pointer_checker_for< T* > NullChecker, typename Deleter >
        struct unique_ptr_ex_deleter
        {
            using pointer = raw_pointer_wrapper< T*, NullChecker >;
            [[no_unique_address]] Deleter deleter;
            constexpr auto operator()( pointer p )
            {
                deleter( p.get() );
            }
            constexpr unique_ptr_ex_deleter() = default;
            template < typename... Args >
            constexpr explicit unique_ptr_ex_deleter( Args&&... args )
              : deleter{ std::forward< Args >( args )... }
            { }
            ~unique_ptr_ex_deleter() = default;
        };
    }
    template < typename T, null_pointer_checker_for< T* > NullChecker = default_null_pointer_checker,
               typename Deleter = std::default_delete< T > >
        requires requires( Deleter d, T* p ) { d( p ); }
    using unique_ptr_ex = std::unique_ptr< T, details_::unique_ptr_ex_deleter< T, NullChecker, Deleter > >;
}
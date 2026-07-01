#pragma once
#include <concepts>
#include <type_traits>
namespace cpp_utils
{
    template < typename T >
    concept raw_pointer = std::is_pointer_v< T >;
    template < typename NullChecker, typename PointerType >
    concept null_pointer_checker_for = requires( const PointerType ptr ) {
        { NullChecker::empty( ptr ) } noexcept -> std::convertible_to< bool >;
    };
    struct default_null_pointer_checker final
    {
        template < raw_pointer T >
        static constexpr auto empty( const T ptr ) noexcept
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
        constexpr auto computable() const noexcept
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
            return !NullChecker::empty( ptr_ );
        }
        [[nodiscard]] constexpr auto&& operator*() const noexcept
            requires( computable() )
        {
            return *ptr_;
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
        return NullChecker::empty( lhs.get() );
    }
    template < raw_pointer T, null_pointer_checker_for< T > NullChecker >
    [[nodiscard]] inline constexpr auto operator!=( const raw_pointer_wrapper< T, NullChecker >& lhs, const std::nullptr_t ) noexcept
    {
        return !NullChecker::empty( lhs.get() );
    }
    template < raw_pointer T, null_pointer_checker_for< T > NullChecker >
    [[nodiscard]] inline constexpr auto operator==( const std::nullptr_t, const raw_pointer_wrapper< T, NullChecker >& rhs ) noexcept
    {
        return NullChecker::empty( rhs.get() );
    }
    template < raw_pointer T, null_pointer_checker_for< T > NullChecker >
    [[nodiscard]] inline constexpr auto operator!=( const std::nullptr_t, const raw_pointer_wrapper< T, NullChecker >& rhs ) noexcept
    {
        return !NullChecker::empty( rhs.get() );
    }
}
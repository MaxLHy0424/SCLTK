#pragma once
#include <type_traits>
namespace cpp_utils
{
    template < typename T >
    concept pointer = std::is_pointer_v< T >;
    template < typename T >
        requires( !std::is_const_v< T > && pointer< T > )
    class raw_pointer_wrapper final
    {
      private:
        T ptr_{};
      public:
        using value_type = T;
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
            return ptr != nullptr;
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
            return raw_pointer_wrapper< T >{ ptr_ + n };
        }
        constexpr auto operator+=( const std::size_t n ) noexcept -> raw_pointer_wrapper< T >&
            requires( computable() )
        {
            ptr_ += n;
            return *this;
        }
        constexpr auto operator++() noexcept -> raw_pointer_wrapper< T >&
            requires( computable() )
        {
            ++ptr_;
            return *this;
        }
        constexpr auto operator++( int ) noexcept -> raw_pointer_wrapper< T >
            requires( computable() )
        {
            return ptr_++;
        }
        [[nodiscard]] constexpr auto operator-( const std::size_t n ) const noexcept
            requires( computable() )
        {
            return raw_pointer_wrapper< T >{ ptr_ - n };
        }
        constexpr auto operator-=( const std::size_t n ) noexcept -> raw_pointer_wrapper< T >&
            requires( computable() )
        {
            ptr_ -= n;
            return *this;
        }
        constexpr auto operator--() noexcept -> raw_pointer_wrapper< T >&
            requires( computable() )
        {
            --ptr_;
            return *this;
        }
        constexpr auto operator--( int ) noexcept -> raw_pointer_wrapper< T >
            requires( computable() )
        {
            return ptr_--;
        }
        constexpr auto operator=( const std::nullptr_t ) noexcept -> raw_pointer_wrapper< T >&
        {
            ptr_ = nullptr;
            return *this;
        }
        constexpr auto operator=( const raw_pointer_wrapper< T >& ) noexcept -> raw_pointer_wrapper< T >& = default;
        constexpr auto operator=( raw_pointer_wrapper< T >&& ) noexcept -> raw_pointer_wrapper< T >&      = default;
        constexpr raw_pointer_wrapper() noexcept                                                          = default;
        constexpr raw_pointer_wrapper( const std::nullptr_t ) noexcept
          : ptr_{ nullptr }
        { }
        constexpr raw_pointer_wrapper( const T ptr ) noexcept
          : ptr_{ ptr }
        { }
        constexpr raw_pointer_wrapper( const raw_pointer_wrapper< T >& ) noexcept = default;
        constexpr raw_pointer_wrapper( raw_pointer_wrapper< T >&& ) noexcept      = default;
        ~raw_pointer_wrapper() noexcept                                           = default;
    };
    template < pointer T >
    [[nodiscard]] inline constexpr auto operator==( const raw_pointer_wrapper< T >& lhs, const raw_pointer_wrapper< T >& rhs ) noexcept
    {
        return lhs.get() == rhs.get();
    }
    template < pointer T >
    [[nodiscard]] inline constexpr auto operator!=( const raw_pointer_wrapper< T >& lhs, const raw_pointer_wrapper< T >& rhs ) noexcept
    {
        return lhs.get() != rhs.get();
    }
    template < pointer T >
    [[nodiscard]] inline constexpr auto operator==( const raw_pointer_wrapper< T >& lhs, const std::nullptr_t ) noexcept
    {
        return lhs.get() == nullptr;
    }
    template < pointer T >
    [[nodiscard]] inline constexpr auto operator!=( const raw_pointer_wrapper< T >& lhs, const std::nullptr_t ) noexcept
    {
        return lhs.get() != nullptr;
    }
    template < pointer T >
    [[nodiscard]] inline constexpr auto operator==( const std::nullptr_t, const raw_pointer_wrapper< T >& rhs ) noexcept
    {
        return rhs.get() == nullptr;
    }
    template < pointer T >
    [[nodiscard]] inline constexpr auto operator!=( const std::nullptr_t, const raw_pointer_wrapper< T >& rhs ) noexcept
    {
        return rhs.get() != nullptr;
    }
}
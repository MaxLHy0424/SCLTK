#pragma once
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>
namespace cpp_utils
{
    template < bool Expr >
    concept test = Expr;
    struct error_type final
    {
        auto operator=( const error_type& ) -> error_type&     = delete;
        auto operator=( error_type&& ) noexcept -> error_type& = delete;
        error_type()                                           = delete;
        error_type( const error_type& )                        = delete;
        error_type( error_type&& ) noexcept                    = delete;
        ~error_type() noexcept                                 = delete;
    };
    struct undefined_type final
    {
        auto operator=( const undefined_type& ) -> undefined_type&     = delete;
        auto operator=( undefined_type&& ) noexcept -> undefined_type& = delete;
        undefined_type()                                               = delete;
        undefined_type( const undefined_type& )                        = delete;
        undefined_type( undefined_type&& ) noexcept                    = delete;
        ~undefined_type() noexcept                                     = delete;
    };
    template < typename T >
    concept common_type = !std::same_as< std::decay_t< T >, error_type > && !std::same_as< std::decay_t< T >, undefined_type >;
    template < typename T >
    class type_identity final
    {
      public:
        using type = T;
        template < typename U >
        consteval auto operator==( const type_identity< U > ) const noexcept
        {
            return std::is_same_v< T, U >;
        }
        template < typename U >
        consteval auto operator!=( const type_identity< U > ) const noexcept
        {
            return !std::is_same_v< T, U >;
        }
        template < template < typename > typename Pred >
            requires requires {
                { Pred< T >::value } -> std::convertible_to< bool >;
            }
        static inline constexpr auto test{ Pred< T >::value };
        template < template < typename > typename F >
            requires requires { typename F< T >::type; }
        using transform = type_identity< typename F< T >::type >;
        template < template < typename > typename Pred, template < typename > typename F >
            requires requires {
                { Pred< T >::value } -> std::convertible_to< bool >;
                typename F< T >::type;
            }
        using transform_if = type_identity< std::conditional_t< Pred< T >::value, typename F< T >::type, T > >;
    };
    template < common_type... Ts >
    class type_list final
    {
      private:
        static inline constexpr auto size_{ sizeof...( Ts ) };
        static inline constexpr auto empty_{ size_ == 0uz };
        template < typename... Us >
        static consteval auto as_type_list_( std::tuple< Us... > ) -> type_list< Us... >;
        template < std::size_t Offset, std::size_t... Is >
        static consteval auto offset_sequence_( std::index_sequence< Is... > ) -> std::index_sequence< ( Offset + Is )... >;
        template < std::size_t... Is >
        static consteval auto select_( std::index_sequence< Is... > )
          -> type_list< std::tuple_element_t< Is, std::tuple< Ts... > >... >;
        template < std::size_t... Is >
        static consteval auto reverse_index_sequence_( std::index_sequence< Is... > )
          -> std::index_sequence< ( sizeof...( Is ) - 1 - Is )... >;
        template < typename Result, typename Remaining >
        struct basic_unique_impl_;
        template < typename... ResultTs >
        struct basic_unique_impl_< type_list< ResultTs... >, type_list<> > final
        {
            using type = type_list< ResultTs... >;
        };
        template < typename... ResultTs, typename T, typename... Rest >
        struct basic_unique_impl_< type_list< ResultTs... >, type_list< T, Rest... > > final
        {
            static inline constexpr auto found{ ( std::is_same_v< T, ResultTs > || ... ) };
            using next = std::conditional_t<
              found, basic_unique_impl_< type_list< ResultTs... >, type_list< Rest... > >,
              basic_unique_impl_< type_list< ResultTs..., T >, type_list< Rest... > > >;
            using type = typename next::type;
        };
        template < typename U >
        struct is_same_type_ final
        {
            template < typename T >
            using predicate = std::is_same< T, U >;
        };
        template < template < typename > typename Pred >
        struct negate_pred_ final
        {
            template < typename T >
            using predicate = std::bool_constant< !Pred< T >::value >;
        };
        template < template < typename > typename Pred, std::size_t I >
        static consteval auto find_first_if_impl_()
        {
            if constexpr ( I >= size_ ) {
                return size_;
            } else {
                using T = std::tuple_element_t< I, std::tuple< Ts... > >;
                if constexpr ( Pred< T >::value ) {
                    return I;
                } else {
                    return find_first_if_impl_< Pred, I + 1 >();
                }
            }
        }
        template < template < typename > typename Pred, std::size_t I >
        static consteval auto find_last_if_impl_()
        {
            using T = std::tuple_element_t< I, std::tuple< Ts... > >;
            if constexpr ( Pred< T >::value ) {
                return I;
            } else {
                if constexpr ( I == 0 ) {
                    return size_;
                } else {
                    return find_last_if_impl_< Pred, I - 1 >();
                }
            }
        }
        template < std::size_t, bool = empty_ >
        struct at_impl_ final
        {
            static_assert( false, "index out of bounds" );
            using type = error_type;
        };
        template < std::size_t I >
            requires test< ( I < size_ ) >
        struct at_impl_< I, false > final
        {
            using type = std::tuple_element_t< I, std::tuple< Ts... > >;
        };
        template < std::size_t I >
        struct at_impl_< I, true > final
        {
            using type = error_type;
        };
        template < std::size_t = 0uz, bool = empty_ >
        struct remove_front_impl_;
        template < std::size_t _ >
        struct remove_front_impl_< _, false > final
        {
            using type = decltype( select_( offset_sequence_< 1 >( std::make_index_sequence< size_ - 1 >{} ) ) );
        };
        template < std::size_t _ >
        struct remove_front_impl_< _, true > final
        {
            using type = type_list<>;
        };
        template < std::size_t = 0uz, bool = empty_ >
        struct remove_back_impl_;
        template < std::size_t _ >
        struct remove_back_impl_< _, false > final
        {
            using type = decltype( select_( std::make_index_sequence< size_ - 1 >{} ) );
        };
        template < std::size_t _ >
        struct remove_back_impl_< _, true > final
        {
            using type = type_list<>;
        };
        template < std::size_t, std::size_t, bool = empty_ >
        struct sub_list_impl_;
        template < std::size_t Offset, std::size_t Count >
        struct sub_list_impl_< Offset, Count, false > final
        {
            static inline constexpr auto is_valid{ Offset + Count <= size_ };
            static_assert( is_valid, "sub_list range out of bounds" );
            using type = std::conditional_t<
              is_valid, decltype( select_( offset_sequence_< Offset >( std::make_index_sequence< Count >{} ) ) ), error_type >;
        };
        template < std::size_t Offset, std::size_t Count >
        struct sub_list_impl_< Offset, Count, true > final
        {
            using type = type_list<>;
        };
        template < std::size_t = 0uz, bool = empty_ >
        struct reverse_impl_;
        template < std::size_t _ >
        struct reverse_impl_< _, false > final
        {
            using type = decltype( select_( reverse_index_sequence_( std::make_index_sequence< size_ >{} ) ) );
        };
        template < std::size_t _ >
        struct reverse_impl_< _, true > final
        {
            using type = type_list<>;
        };
        template < std::size_t _ = 0uz, bool = empty_ >
        struct unique_impl_;
        template < std::size_t _ >
        struct unique_impl_< _, false > final
        {
            using type = typename basic_unique_impl_< type_list<>, type_list< Ts... > >::type;
        };
        template < std::size_t _ >
        struct unique_impl_< _, true > final
        {
            using type = type_list<>;
        };
        template < template < typename > typename, bool = empty_ >
        struct transform_impl_;
        template < template < typename > typename F >
        struct transform_impl_< F, false > final
        {
            using type = type_list< typename F< Ts >::type... >;
        };
        template < template < typename > typename F >
        struct transform_impl_< F, true > final
        {
            using type = type_list<>;
        };
        template < template < typename > typename, bool = empty_ >
        struct filter_impl_;
        template < template < typename > typename Pred >
        struct filter_impl_< Pred, false > final
        {
            using type = decltype( as_type_list_(
              std::tuple_cat( std::conditional_t< Pred< Ts >::value, std::tuple< Ts >, std::tuple<> >{}... ) ) );
        };
        template < template < typename > typename Pred >
        struct filter_impl_< Pred, true > final
        {
            using type = type_list<>;
        };
      public:
        static inline constexpr auto size{ size_ };
        static inline constexpr auto empty{ empty_ };
        template < typename U >
        static inline constexpr auto contains{ ( std::is_same_v< U, Ts > || ... ) };
        template < typename U >
        static inline constexpr auto count{ [] consteval
        {
            if constexpr ( empty ) {
                return 0uz;
            } else {
                return ( ( std::is_same_v< Ts, U > ? 1uz : 0uz ) + ... );
            }
        }() };
        template < template < typename > typename Pred >
        static inline constexpr auto count_if{ [] consteval
        {
            if constexpr ( empty ) {
                return 0uz;
            } else {
                return ( ( Pred< Ts >::value ? 1uz : 0uz ) + ... );
            }
        }() };
        template < template < typename > typename Pred >
        static inline constexpr auto all_of{ [] consteval
        {
            if constexpr ( empty ) {
                return false;
            } else {
                return ( Pred< Ts >::value && ... );
            }
        }() };
        template < template < typename > typename Pred >
        static inline constexpr auto any_of{ ( Pred< Ts >::value || ... ) };
        template < template < typename > typename Pred >
        static inline constexpr auto none_of{ ( !Pred< Ts >::value && ... ) };
        template < template < typename > typename Pred >
        static inline constexpr auto find_first_if{ [] consteval
        {
            if constexpr ( empty ) {
                return size;
            } else {
                return find_first_if_impl_< Pred, 0 >();
            }
        }() };
        template < template < typename > typename Pred >
        static inline constexpr auto find_last_if{ [] consteval
        {
            if constexpr ( empty ) {
                return size;
            } else {
                return find_last_if_impl_< Pred, size - 1 >();
            }
        }() };
        template < template < typename > typename Pred >
        static inline constexpr auto find_first_if_not{ find_first_if< negate_pred_< Pred >::template predicate > };
        template < template < typename > typename Pred >
        static inline constexpr auto find_last_if_not{ find_last_if< negate_pred_< Pred >::template predicate > };
        template < typename U >
        static inline constexpr auto find_first{ find_first_if< is_same_type_< U >::template predicate > };
        template < typename U >
        static inline constexpr auto find_last{ find_last_if< is_same_type_< U >::template predicate > };
        template < std::size_t I >
        using at    = at_impl_< I >::type;
        using front = at< 0 >;
        using back  = at< size - 1 >;
        template < common_type... Us >
        using add_front = type_list< Us..., Ts... >;
        template < common_type... Us >
        using add_back     = type_list< Ts..., Us... >;
        using remove_front = remove_front_impl_<>::type;
        using remove_back  = remove_back_impl_<>::type;
        template < std::size_t Offset, std::size_t Count >
        using sub_list = sub_list_impl_< Offset, Count >::type;
        using reverse  = reverse_impl_<>::type;
        using unique   = unique_impl_<>::type;
        template < template < typename... > typename Template >
        using apply = Template< Ts... >;
        template < template < typename > typename F >
        using transform = transform_impl_< F >::type;
        template < template < typename > typename Pred >
        using filter = filter_impl_< Pred >::type;
    };
    namespace details
    {
        template < typename, typename >
        struct type_list_concat_impl final
        {
            static_assert( false, "can only concatenate type_list types" );
            using type = error_type;
        };
        template < typename... Ts, typename... Us >
        struct type_list_concat_impl< type_list< Ts... >, type_list< Us... > > final
        {
            using type = type_list< Ts..., Us... >;
        };
        template < typename List >
        struct is_in_type_list final
        {
            template < typename T >
            using predicate = std::bool_constant< List::template contains< T > >;
        };
        template < typename List >
        struct is_not_in_type_list final
        {
            template < typename T >
            using predicate = std::bool_constant< !List::template contains< T > >;
        };
        template < typename List1, typename List2 >
        struct type_list_intersection_impl final
        {
            using type = typename List1::template filter< is_in_type_list< List2 >::template predicate >::unique;
        };
        template < typename List1, typename List2 >
        struct type_list_difference_impl final
        {
            using type = typename List1::template filter< is_not_in_type_list< List2 >::template predicate >::unique;
        };
    }
    template < typename List1, typename List2 >
    using type_list_concat = typename details::type_list_concat_impl< List1, List2 >::type;
    template < typename List1, typename List2 >
    using type_list_intersection = typename details::type_list_intersection_impl< List1, List2 >::type;
    template < typename List1, typename List2 >
    using type_list_difference = typename details::type_list_difference_impl< List1, List2 >::type;
    template < typename List1, typename List2 >
    using type_list_symmetric_difference
      = type_list_concat< type_list_difference< List1, List2 >, type_list_difference< List2, List1 > >::unique;
    template < template < typename > typename F, common_type... Ts >
    inline constexpr auto type_list_for_each( type_list< Ts... > )
    {
        ( F< Ts >{}, ... );
    }
}
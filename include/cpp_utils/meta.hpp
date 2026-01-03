#pragma once
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>
namespace cpp_utils
{
    template < typename... Fs >
    struct overloads final : public Fs...
    {
        using Fs::operator()...;
    };
    template < typename... Fs >
    overloads( Fs... ) -> overloads< Fs... >;
    template < bool Expr >
    concept as_concept = Expr;
    struct error_type final
    {
        auto operator=( const error_type& ) -> error_type&     = delete;
        auto operator=( error_type&& ) noexcept -> error_type& = delete;
        error_type()                                           = delete;
        error_type( const error_type& )                        = delete;
        error_type( error_type&& ) noexcept                    = delete;
        ~error_type() noexcept                                 = delete;
    };
    template < template < typename... > typename F, typename... Ts >
    using apply_type = typename F< Ts... >::type;
    template < template < auto... > typename F, auto... Vs >
    inline constexpr auto apply_value{ F< Vs... >::value };
    template < template < typename... > typename F, typename... Ts >
    struct bind_front_type final
    {
        template < typename... Us >
        using type = apply_type< F, Ts..., Us... >;
    };
    template < template < auto... > typename F, auto... Vs >
    struct bind_front_value final
    {
        template < auto... Ws >
        static constexpr auto value{ apply_value< F, Vs..., Ws... > };
    };
    template < template < typename... > typename F, typename... Ts >
    struct bind_back_type final
    {
        template < typename... Us >
        using type = apply_type< F, Us..., Ts... >;
    };
    template < template < auto... > typename F, auto... Vs >
    struct bind_back_value final
    {
        template < auto... Ws >
        static constexpr auto value{ apply_value< F, Ws..., Vs... > };
    };
    template < typename T >
    concept common_type = !std::same_as< std::decay_t< T >, error_type >;
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
    template < auto V >
    struct value_identity final
    {
        using value_type = decltype( V );
        static inline constexpr auto value{ V };
        constexpr auto operator<=>( const value_identity< V >& ) const        = default;
        constexpr auto operator==( const value_identity< V >& ) const -> bool = default;
    };
    template < template < typename > typename Pred >
    struct negate_pred
    {
        template < typename T >
        using predicate = std::bool_constant< !Pred< T >::value >;
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
        static consteval auto offset_sequence_( const std::index_sequence< Is... > ) -> std::index_sequence< ( Offset + Is )... >;
        template < std::size_t... Is >
        static consteval auto select_( const std::index_sequence< Is... > )
          -> type_list< std::tuple_element_t< Is, std::tuple< Ts... > >... >;
        template < std::size_t... Is >
        static consteval auto reverse_index_sequence_( const std::index_sequence< Is... > )
          -> std::index_sequence< ( sizeof...( Is ) - 1 - Is )... >;
        template < typename U >
        struct is_same_type_ final
        {
            template < typename T >
            using predicate = std::is_same< T, U >;
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
            requires as_concept< ( I < size_ ) >
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
        struct front_impl_;
        template < std::size_t _ >
        struct front_impl_< _, false > final
        {
            using type = typename at_impl_< 0 >::type;
        };
        template < std::size_t _ >
        struct front_impl_< _, true > final
        {
            using type = error_type;
        };
        template < std::size_t = 0uz, bool = empty_ >
        struct back_impl_;
        template < std::size_t _ >
        struct back_impl_< _, false > final
        {
            using type = typename at_impl_< size_ - 1 >::type;
        };
        template < std::size_t _ >
        struct back_impl_< _, true > final
        {
            using type = error_type;
        };
        template < typename... Us >
        struct add_front_impl_ final
        {
            using type = type_list< Us..., Ts... >;
        };
        template < typename... Us >
        struct add_back_impl_ final
        {
            using type = type_list< Ts..., Us... >;
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
            using type = decltype( select_( offset_sequence_< Offset >(
              std::make_index_sequence< ( Offset + Count <= size_ ? Offset + Count : size_ ) - Offset >{} ) ) );
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
            static inline constexpr auto found{ std::disjunction_v< std::is_same< T, ResultTs >... > };
            using next = std::conditional_t<
              found, basic_unique_impl_< type_list< ResultTs... >, type_list< Rest... > >,
              basic_unique_impl_< type_list< ResultTs..., T >, type_list< Rest... > > >;
            using type = typename next::type;
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
        template < std::size_t, std::size_t, bool = empty_ >
        struct swap_impl_;
        template < std::size_t I1, std::size_t I2 >
        struct swap_impl_< I1, I2, true > final
        {
            static_assert( false, "swap index out of bounds" );
            using type = error_type;
        };
        template < std::size_t I1, std::size_t I2 >
        struct swap_impl_< I1, I2, false > final
        {
            static_assert( I1 < size_, "swap first index out of bounds" );
            static_assert( I2 < size_, "swap second index out of bounds" );
            using type_at_i1 = std::tuple_element_t< I1, std::tuple< Ts... > >;
            using type_at_i2 = std::tuple_element_t< I2, std::tuple< Ts... > >;
            template < std::size_t Index >
            using get_swapped_type = std::conditional_t<
              Index == I1, type_at_i2,
              std::conditional_t< Index == I2, type_at_i1, std::tuple_element_t< Index, std::tuple< Ts... > > > >;
            template < std::size_t... Is >
            static consteval auto construct_swapped( const std::index_sequence< Is... > ) -> type_list< get_swapped_type< Is >... >;
            using type = decltype( construct_swapped( std::make_index_sequence< size_ >{} ) );
        };
        template < typename, typename >
        struct concat_impl_;
        template < typename... Ts1, typename... Ts2 >
        struct concat_impl_< type_list< Ts1... >, type_list< Ts2... > > final
        {
            using type = type_list< Ts1..., Ts2... >;
        };
        template < typename T, typename SortedList, template < typename, typename > typename Pred >
        struct basic_insert_sorted_impl_;
        template < typename T, template < typename, typename > typename Pred >
        struct basic_insert_sorted_impl_< T, type_list<>, Pred > final
        {
            using type = type_list< T >;
        };
        template < typename T, typename U, typename... Us, template < typename, typename > typename Pred >
        struct basic_insert_sorted_impl_< T, type_list< U, Us... >, Pred > final
        {
            using type = std::conditional_t<
              Pred< T, U >::value, type_list< T, U, Us... >,
              typename concat_impl_< type_list< U >, typename basic_insert_sorted_impl_< T, type_list< Us... >, Pred >::type >::type >;
        };
        template < typename List, template < typename, typename > typename Pred >
        struct basic_sort_impl_;
        template < template < typename, typename > typename Pred >
        struct basic_sort_impl_< type_list<>, Pred > final
        {
            using type = type_list<>;
        };
        template < typename T, template < typename, typename > typename Pred >
        struct basic_sort_impl_< type_list< T >, Pred > final
        {
            using type = type_list< T >;
        };
        template < typename Head, typename... Rest, template < typename, typename > typename Pred >
        struct basic_sort_impl_< type_list< Head, Rest... >, Pred > final
        {
            using sorted_rest = typename basic_sort_impl_< type_list< Rest... >, Pred >::type;
            using type        = typename basic_insert_sorted_impl_< Head, sorted_rest, Pred >::type;
        };
        template < template < typename... > typename Template >
        struct apply_impl_ final
        {
            using type = Template< Ts... >;
        };
        template < std::size_t Start >
        struct enumerate_impl_ final
        {
            template < std::size_t... Is >
            static consteval auto helper( const std::index_sequence< Is... > )
              -> type_list< type_list< value_identity< Start + Is >, Ts >... >;
            using type = decltype( helper( std::index_sequence_for< Ts... >{} ) );
        };
        template < template < typename > typename F >
        struct transform_impl_ final
        {
            using type = type_list< typename F< Ts >::type... >;
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
        template < template < typename > typename, bool = empty_ >
        struct remove_if_impl_;
        template < template < typename > typename Pred >
        struct remove_if_impl_< Pred, false > final
        {
            using type = typename filter_impl_< negate_pred< Pred >::template predicate >::type;
        };
        template < template < typename > typename Pred >
        struct remove_if_impl_< Pred, true > final
        {
            using type = type_list<>;
        };
      public:
        static inline constexpr auto size{ size_ };
        static inline constexpr auto empty{ empty_ };
        template < typename U >
        static inline constexpr auto contains{ std::disjunction_v< std::is_same< U, Ts >... > };
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
        static inline constexpr auto all_of{ ( Pred< Ts >::value && ... ) };
        template < template < typename > typename Pred >
        static inline constexpr auto any_of{ [] consteval
        {
            if ( empty_ ) {
                return true;
            } else {
                return ( Pred< Ts >::value || ... );
            }
        }() };
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
        static inline constexpr auto find_first_if_not{ find_first_if< negate_pred< Pred >::template predicate > };
        template < template < typename > typename Pred >
        static inline constexpr auto find_last_if_not{ find_last_if< negate_pred< Pred >::template predicate > };
        template < typename U >
        static inline constexpr auto find_first{ find_first_if< is_same_type_< U >::template predicate > };
        template < typename U >
        static inline constexpr auto find_last{ find_last_if< is_same_type_< U >::template predicate > };
        template < std::size_t I >
        using at    = typename at_impl_< I >::type;
        using front = typename front_impl_<>::type;
        using back  = typename back_impl_<>::type;
        template < common_type... Us >
        using add_front = typename add_front_impl_< Us... >::type;
        template < common_type... Us >
        using add_back     = typename add_back_impl_< Us... >::type;
        using remove_front = typename remove_front_impl_<>::type;
        using remove_back  = typename remove_back_impl_<>::type;
        template < std::size_t Offset, std::size_t Count >
        using sub_list = typename sub_list_impl_< Offset, Count >::type;
        template < std::size_t Count >
        using take = sub_list< 0, Count >;
        template < std::size_t Count >
        using drop = sub_list< Count, size - Count >;
        template < std::size_t I1, std::size_t I2 >
        using swap    = typename swap_impl_< I1, I2 >::type;
        using reverse = typename reverse_impl_<>::type;
        using unique  = typename unique_impl_<>::type;
        template < template < common_type, common_type > typename Pred >
        using sort = typename basic_sort_impl_< type_list< Ts... >, Pred >::type;
        template < template < typename... > typename Template >
        using apply = typename apply_impl_< Template >::type;
        template < std::size_t Start >
        using enumerate = typename enumerate_impl_< Start >::type;
        template < template < typename > typename F >
        using transform = typename transform_impl_< F >::type;
        template < template < typename > typename Pred >
        using filter = typename filter_impl_< Pred >::type;
        template < template < typename > typename Pred >
        using filter_not = typename remove_if_impl_< Pred >::type;
        template < template < typename > typename Pred >
        using partition = type_list< filter< Pred >, filter_not< Pred > >;
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
        template < typename T, std::size_t N >
        struct make_repeated_type_list_impl final
        {
            static consteval auto to_identity( const std::size_t ) -> type_identity< T >;
            using type = decltype( []< std::size_t... Is >( const std::index_sequence< Is... > ) consteval static noexcept
            { return type_list< typename decltype( to_identity( Is ) )::type... >{}; }( std::make_index_sequence< N >{} ) );
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
    template < typename T, std::size_t N >
    using make_repeated_type_list = typename details::make_repeated_type_list_impl< T, N >::type;
    template < template < typename > typename F, common_type... Ts >
    inline constexpr auto type_list_for_each( const type_list< Ts... > )
    {
        ( F< Ts >{}, ... );
    }
}
#pragma once
#include <functional>
#include <tuple>
namespace cpp_utils
{
    template < typename T >
    using type_alloc = T;
    template < typename T >
    struct add_const_lvalue_reference final
    {
        using type = std::add_lvalue_reference_t< std::add_const_t< T > >;
    };
    template < typename T >
    using add_const_lvalue_reference_t = add_const_lvalue_reference< T >::type;
    template < bool Expr >
    concept test = Expr;
    struct error final
    { };
    struct undefined final
    { };
    template < typename T >
    concept common_type = !std::same_as< std::decay_t< T >, error > && !std::same_as< std::decay_t< T >, undefined >;
    template < common_type... Ts >
    class type_list final
    {
      private:
        static constexpr auto size_{ sizeof...( Ts ) };
        static constexpr auto empty_{ size_ == 0uz };
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
            static constexpr bool found{ ( std::is_same_v< T, ResultTs > || ... ) };
            using next = std::conditional_t<
              found, basic_unique_impl_< type_list< ResultTs... >, type_list< Rest... > >,
              basic_unique_impl_< type_list< ResultTs..., T >, type_list< Rest... > > >;
            using type = typename next::type;
        };
        template < typename >
        struct concat_impl_ final
        {
            static_assert( false, "cannot concatenate type_list with non-type_list type" );
        };
        template < typename... Us >
        struct concat_impl_< type_list< Us... > > final
        {
            using type = type_list< Ts..., Us... >;
        };
        template < typename U >
        struct type_is_ final
        {
            template < typename T >
            using predicate = std::is_same< T, U >;
        };
        template < template < typename > typename Pred >
        struct negate_ final
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
            using type = error;
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
            using type = error;
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
            requires test< Offset + Count <= size_ >
        struct sub_list_impl_< Offset, Count, false > final
        {
            using type = decltype( select_( offset_sequence_< Offset >( std::make_index_sequence< Count >{} ) ) );
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
        static constexpr auto size{ size_ };
        static constexpr auto empty{ empty_ };
        template < typename U >
        static constexpr auto contains{ ( std::is_same_v< U, Ts > || ... ) };
        template < typename U >
        static constexpr auto count{ [] consteval
        {
            if constexpr ( empty ) {
                return 0uz;
            } else {
                return ( ( std::is_same_v< Ts, U > ? 1uz : 0uz ) + ... );
            }
        }() };
        template < template < typename > typename Pred >
        static constexpr auto count_if{ [] consteval
        {
            if constexpr ( empty ) {
                return 0uz;
            } else {
                return ( ( Pred< Ts >::value ? 1uz : 0uz ) + ... );
            }
        }() };
        template < template < typename > typename Pred >
        static constexpr auto all_of{ [] consteval
        {
            if constexpr ( empty ) {
                return false;
            } else {
                return ( Pred< Ts >::value && ... );
            }
        }() };
        template < template < typename > typename Pred >
        static constexpr auto any_of{ ( Pred< Ts >::value || ... ) };
        template < template < typename > typename Pred >
        static constexpr auto none_of{ ( !Pred< Ts >::value && ... ) };
        template < template < typename > typename Pred >
        static constexpr auto find_first_if{ [] consteval
        {
            if constexpr ( empty ) {
                return size;
            } else {
                return find_first_if_impl_< Pred, 0 >();
            }
        }() };
        template < template < typename > typename Pred >
        static constexpr auto find_last_if{ [] consteval
        {
            if constexpr ( empty ) {
                return size;
            } else {
                return find_last_if_impl_< Pred, size - 1 >();
            }
        }() };
        template < template < typename > typename Pred >
        static constexpr auto find_first_if_not{ find_first_if< negate_< Pred >::template predicate > };
        template < template < typename > typename Pred >
        static constexpr auto find_last_if_not{ find_last_if< negate_< Pred >::template predicate > };
        template < typename U >
        static constexpr auto find_first{ find_first_if< type_is_< U >::template predicate > };
        template < typename U >
        static constexpr auto find_last{ find_last_if< type_is_< U >::template predicate > };
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
        template < typename Other >
        using concat  = typename concat_impl_< Other >::type;
        using reverse = reverse_impl_<>::type;
        using unique  = unique_impl_<>::type;
        template < template < typename... > typename U >
        using apply = U< Ts... >;
        template < template < typename > typename F >
        using transform = transform_impl_< F >::type;
        template < template < typename > typename Pred >
        using filter = filter_impl_< Pred >::type;
    };
    template < auto V >
    struct value_wrapper final
    {
        static constexpr auto value{ V };
    };
    template < auto V >
    class is_equal_to_value_wrapper final
    {
      private:
        static consteval auto is_equal_( auto ) -> std::false_type;
        template < auto W >
            requires( V == W )
        static consteval auto is_equal_( value_wrapper< W > ) -> std::true_type;
      public:
        template < typename W >
        static constexpr auto value{ decltype( is_equal_( W{} ) )::value };
    };
    template < auto... Vs >
    using make_fake_value_list = type_list< value_wrapper< Vs >... >;
    namespace details
    {
        template < typename T >
        struct remove_identity;
        template < typename T >
        struct remove_identity< std::type_identity< T > > final
        {
            using type = T;
        };
        template < typename T, std::size_t N >
        inline consteval auto get_array_element_t( std::array< T, N > ) -> T;
        template < typename T, std::size_t N >
        inline consteval auto get_array_size( std::array< T, N > ) -> std::integral_constant< std::size_t, N >;
        template < typename T, std::size_t N, std::array< T, N > Arr >
        struct make_fake_value_list_from_array_impl final
        {
            using type = decltype( []< std::size_t... Is >( std::index_sequence< Is... > ) consteval
            { return type_list< value_wrapper< Arr[ Is ] >... >{}; }( std::make_index_sequence< N >{} ) );
        };
        template < typename T, std::array< T, 0 > Arr >
        struct make_fake_value_list_from_array_impl< T, 0, Arr > final
        {
            using type = type_list<>;
        };
    }
    template < std::array Arr >
    using make_fake_value_list_from_array = details::make_fake_value_list_from_array_impl<
      decltype( details::get_array_element_t( Arr ) ), decltype( details::get_array_size( Arr ) )::value, Arr >::type;
    template < typename, typename... >
    struct function_traits final
    {
        using return_type = undefined;
        using class_type  = undefined;
        using args_type   = undefined;
    };
    template < typename R, typename... Args >
    struct function_traits< R( Args... ) > final
    {
        using return_type = R;
        using class_type  = undefined;
        using args_type   = type_list< Args... >;
    };
    template < typename R, typename... Args >
    struct function_traits< R ( * )( Args... ) > final
    {
        using return_type = R;
        using class_type  = undefined;
        using args_type   = type_list< Args... >;
    };
    template < typename R, typename T, typename... Args >
    struct function_traits< R ( T::* )( Args... ) > final
    {
        using return_type = R;
        using class_type  = T;
        using args_type   = type_list< Args... >;
    };
    template < typename R, typename... Args >
    struct function_traits< std::function< R( Args... ) > > final
    {
        using return_type = R;
        using class_type  = undefined;
        using args_type   = type_list< Args... >;
    };
    template < template < typename... > typename Template, typename T >
    struct is_specialization_of final : std::false_type
    { };
    template < template < typename... > typename Template, typename... Args >
    struct is_specialization_of< Template, Template< Args... > > final : std::true_type
    { };
    template < typename T >
    struct matcher final
    {
        static constexpr auto matches{ false };
        using result = void;
    };
    template < typename Specific, typename Result >
    struct type_matcher final : matcher< Specific >
    {
        template < typename U >
        static constexpr auto matches{ std::is_same_v< U, Specific > };
        using result = Result;
    };
    template < template < typename... > typename Pattern, typename Result >
    struct template_matcher final
    {
        template < typename T >
        static constexpr auto matches{ is_specialization_of< Pattern, T >::value };
        using result = Result;
    };
    template < typename Result >
    struct default_matcher final
    {
        template < typename T >
        static constexpr auto matches{ true };
        using result = Result;
    };
    namespace details
    {
        template < typename, typename... >
        struct match_impl;
        template < typename T >
        struct match_impl< T > final
        {
            static_assert( sizeof( T ) == 0, "no matching pattern found" );
        };
        template < typename T, typename Matcher, typename... RestMatchers >
        struct match_impl< T, Matcher, RestMatchers... > final
        {
            using type = std::conditional_t<
              Matcher::template matches< T >, typename Matcher::result, typename match_impl< T, RestMatchers... >::type >;
        };
    }
    template < typename T, typename... Matchers >
    struct match final
    {
        using type = typename details::match_impl< T, Matchers... >::type;
    };
    template < typename T, typename... Matchers >
    using match_t = typename match< T, Matchers... >::type;
}
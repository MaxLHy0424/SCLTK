#pragma once
#include <tuple>
#include "meta_base.hpp"
namespace cpp_utils
{
    template < typename... Ts >
    class type_list final
    {
      private:
        template < typename... Us >
        static consteval auto as_type_list_( std::tuple< Us... > ) -> type_list< Us... >;
        template < size_t Offset, size_t... Is >
        static consteval auto offset_sequence_( std::index_sequence< Is... > ) -> std::index_sequence< ( Offset + Is )... >;
        template < size_t... Is >
        static consteval auto select_( std::index_sequence< Is... > )
          -> type_list< std::tuple_element_t< Is, std::tuple< Ts... > >... >;
        template < size_t... Is >
        static consteval auto reverse_index_sequence_( std::index_sequence< Is... > )
          -> std::index_sequence< ( sizeof...( Is ) - 1 - Is )... >;
        template < typename Result, typename Remaining >
        struct unique_impl_;
        template < typename... ResultTs >
        struct unique_impl_< type_list< ResultTs... >, type_list<> > final
        {
            using type = type_list< ResultTs... >;
        };
        template < typename... ResultTs, typename T, typename... Rest >
        struct unique_impl_< type_list< ResultTs... >, type_list< T, Rest... > > final
        {
            static constexpr bool found{ ( std::is_same_v< T, ResultTs > || ... ) };
            using next = std::conditional_t<
              found, unique_impl_< type_list< ResultTs... >, type_list< Rest... > >,
              unique_impl_< type_list< ResultTs..., T >, type_list< Rest... > > >;
            using type = typename next::type;
        };
        template < typename >
        struct concat_impl_;
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
        template < template < typename > typename Pred, size_t I >
        static consteval size_t find_first_if_impl_()
        {
            if constexpr ( I >= size ) {
                return size;
            } else {
                using T = std::tuple_element_t< I, std::tuple< Ts... > >;
                if constexpr ( Pred< T >::value ) {
                    return I;
                } else {
                    return find_first_if_impl_< Pred, I + 1 >();
                }
            }
        }
        template < template < typename > typename Pred, size_t I >
        static consteval size_t find_last_if_impl_()
        {
            using T = std::tuple_element_t< I, std::tuple< Ts... > >;
            if constexpr ( Pred< T >::value ) {
                return I;
            } else {
                if constexpr ( I == 0 ) {
                    return size;
                } else {
                    return find_last_if_impl_< Pred, I - 1 >();
                }
            }
        }
      public:
        static constexpr auto size{ sizeof...( Ts ) };
        template < typename U >
        static constexpr auto contains{ ( std::is_same_v< U, Ts > || ... ) };
        template < typename U >
        static constexpr size_t count{ ( ( std::is_same_v< Ts, U > ? 1 : 0 ) + ... ) };
        template < template < typename > typename Pred >
        static constexpr size_t count_if{ ( ( Pred< Ts >::value ? 1 : 0 ) + ... ) };
        template < template < typename > typename Pred >
        static constexpr auto all_of{ ( Pred< Ts >::value && ... ) };
        template < template < typename > typename Pred >
        static constexpr auto any_of{ ( Pred< Ts >::value || ... ) };
        template < template < typename > typename Pred >
        static constexpr auto none_of{ ( !Pred< Ts >::value && ... ) };
        template < template < typename > typename Pred >
        static constexpr auto find_first_if{ find_first_if_impl_< Pred, 0 >() };
        template < template < typename > typename Pred >
        static constexpr auto find_last_if{ find_last_if_impl_< Pred, size - 1 >() };
        template < template < typename > typename Pred >
        static constexpr auto find_first_if_not{ find_first_if< negate_< Pred >::template predicate > };
        template < template < typename > typename Pred >
        static constexpr auto find_last_if_not{ find_last_if< negate_< Pred >::template predicate > };
        template < typename U >
        static constexpr auto find_first{ find_first_if< type_is_< U >::template predicate > };
        template < typename U >
        static constexpr auto find_last{ find_last_if< type_is_< U >::template predicate > };
        template < size_t I >
            requires test< ( I < size ) >
        using at    = std::tuple_element_t< I, std::tuple< Ts... > >;
        using front = at< 0 >;
        using back  = at< size - 1 >;
        template < typename... Us >
        using prepend = type_list< Us..., Ts... >;
        template < typename... Us >
        using append       = type_list< Ts..., Us... >;
        using remove_front = decltype( select_( offset_sequence_< 1 >( std::make_index_sequence< size - 1 >{} ) ) );
        using remove_back  = decltype( select_( std::make_index_sequence< size - 1 >{} ) );
        template < size_t Offset, size_t Count >
            requires test< Offset + Count <= size >
        using slice = decltype( select_( offset_sequence_< Offset >( std::make_index_sequence< Count >{} ) ) );
        template < typename Other >
        using concat  = typename concat_impl_< Other >::type;
        using reverse = decltype( select_( reverse_index_sequence_( std::make_index_sequence< size >{} ) ) );
        using unique  = typename unique_impl_< type_list<>, type_list< Ts... > >::type;
        template < template < typename... > typename U >
        using apply = U< Ts... >;
        template < template < typename > typename F >
        using transform = type_list< typename F< Ts >::type... >;
        template < template < typename > typename Pred >
        using filter = decltype( as_type_list_(
          std::tuple_cat( std::conditional_t< Pred< Ts >::value, std::tuple< Ts >, std::tuple<> >{}... ) ) );
        type_list()  = delete;
        ~type_list() = delete;
    };
    template <>
    class type_list<> final
    {
      private:
        template < typename >
        struct concat_impl_;
        template < typename... Us >
        struct concat_impl_< type_list< Us... > > final
        {
            using type = type_list< Us... >;
        };
      public:
        static constexpr size_t size{ 0 };
        template < typename >
        static constexpr bool contains{ false };
        template < template < typename > typename >
        static constexpr size_t find_first_if{ 0 };
        template < template < typename > typename >
        static constexpr size_t find_last_if{ 0 };
        template < template < typename > typename >
        static constexpr size_t find_first_if_not{ 0 };
        template < template < typename > typename >
        static constexpr size_t find_last_if_not{ 0 };
        template < typename >
        static constexpr size_t find_first{ 0 };
        template < typename >
        static constexpr size_t find_last{ 0 };
        template < typename... Us >
        using prepend = type_list< Us... >;
        template < typename... Us >
        using append       = type_list< Us... >;
        using remove_front = type_list<>;
        using remove_back  = type_list<>;
        template < typename Other >
        using concat  = typename concat_impl_< Other >::type;
        using reverse = type_list<>;
        using unique  = type_list<>;
        template < template < typename... > typename U >
        using apply = U<>;
        template < template < typename > typename >
        using transform = type_list<>;
        template < template < typename > typename >
        using filter = type_list<>;
        type_list()  = delete;
        ~type_list() = delete;
    };
    template < auto... Vs >
    class value_list final
    {
      private:
        template < auto V >
        struct value_holder_ final
        {
            static constexpr auto value{ V };
        };
        using underlying_type_list_ = type_list< value_holder_< Vs >... >;
        template < typename >
        struct to_value_list_impl_;
        template < typename... Holders >
        struct to_value_list_impl_< type_list< Holders... > > final
        {
            using type = value_list< Holders::value... >;
        };
        template < typename TL >
        using to_value_list_ = typename to_value_list_impl_< TL >::type;
        template < template < auto > typename Pred >
        struct predicate_adapter_ final
        {
            template < typename T >
            using predicate = std::bool_constant< Pred< T::value >::value >;
        };
        template < typename Holder >
        struct extract_value_ final
        {
            static constexpr auto value{ Holder::value };
        };
        template < auto V >
        struct is_equal_ final
        {
            template < auto W >
            using type = std::bool_constant< ( V == W ) >;
        };
        template < template < auto > typename F >
        struct transform_impl_ final
        {
            template < typename T >
            struct apply final
            {
                using type = value_holder_< F< T::value >::value >;
            };
        };
      public:
        static constexpr auto size{ underlying_type_list_::size };
        template < auto W >
        static constexpr auto contains{ underlying_type_list_::template contains< value_holder_< W > > };
        template < auto W >
        static constexpr auto count{ underlying_type_list_::template count< value_holder_< W > > };
        template < template < auto > typename Pred >
        static constexpr auto count_if{ underlying_type_list_::template count_if< predicate_adapter_< Pred >::template predicate > };
        template < template < auto > typename Pred >
        static constexpr auto all_of{ underlying_type_list_::template all_of< predicate_adapter_< Pred >::template predicate > };
        template < template < auto > typename Pred >
        static constexpr auto any_of{ underlying_type_list_::template any_of< predicate_adapter_< Pred >::template predicate > };
        template < template < auto > typename Pred >
        static constexpr auto none_of{ underlying_type_list_::template none_of< predicate_adapter_< Pred >::template predicate > };
        template < template < auto > typename Pred >
        static constexpr auto find_first_if{
          underlying_type_list_::template find_first_if< predicate_adapter_< Pred >::template predicate > };
        template < template < auto > typename Pred >
        static constexpr auto find_last_if{
          underlying_type_list_::template find_last_if< predicate_adapter_< Pred >::template predicate > };
        template < template < auto > typename Pred >
        static constexpr auto find_first_if_not{
          underlying_type_list_::template find_first_if_not< predicate_adapter_< Pred >::template predicate > };
        template < template < auto > typename Pred >
        static constexpr auto find_last_if_not{
          underlying_type_list_::template find_last_if_not< predicate_adapter_< Pred >::template predicate > };
        template < auto W >
        static constexpr auto find_first{ find_first_if< is_equal_< W >::template type > };
        template < auto W >
        static constexpr auto find_last{ find_last_if< is_equal_< W >::template type > };
        template < size_t I >
        static constexpr auto at{ extract_value_< typename underlying_type_list_::template at< I > >::value };
        static constexpr auto front{ at< 0 > };
        static constexpr auto back{ at< size - 1 > };
        template < auto... Ws >
        using prepend = to_value_list_< typename underlying_type_list_::template prepend< value_holder_< Ws >... > >;
        template < auto... Ws >
        using append       = to_value_list_< typename underlying_type_list_::template append< value_holder_< Ws >... > >;
        using remove_front = to_value_list_< typename underlying_type_list_::remove_front >;
        using remove_back  = to_value_list_< typename underlying_type_list_::remove_back >;
        template < size_t Offset, size_t Count >
        using slice = to_value_list_< typename underlying_type_list_::template slice< Offset, Count > >;
        template < typename Other >
        using concat = to_value_list_< typename underlying_type_list_::template concat< typename Other::underlying_type_list_ > >;
        using reverse = to_value_list_< typename underlying_type_list_::reverse >;
        using unique  = to_value_list_< typename underlying_type_list_::unique >;
        template < template < auto... > typename Template >
        using apply = Template< Vs... >;
        template < template < auto > typename F >
        using transform
          = to_value_list_< typename underlying_type_list_::template transform< transform_impl_< F >::template apply > >;
        template < template < auto > typename Pred >
        using filter
          = to_value_list_< typename underlying_type_list_::template filter< predicate_adapter_< Pred >::template predicate > >;
        value_list()  = delete;
        ~value_list() = delete;
    };
    template <>
    class value_list<> final
    {
      private:
        using underlying_type_list_ = type_list<>;
        template < typename TL >
        struct to_value_list_impl_ final
        {
            using type = value_list<>;
        };
        template < typename TL >
        using to_value_list_ = typename to_value_list_impl_< TL >::type;
      public:
        static constexpr size_t size{ 0 };
        template < auto >
        static constexpr auto contains{ false };
        template < auto >
        static constexpr size_t count{ 0 };
        template < template < auto > typename >
        static constexpr size_t count_if{ 0 };
        template < template < auto > typename >
        static constexpr auto all_of{ true };
        template < template < auto > typename >
        static constexpr auto any_of{ false };
        template < template < auto > typename >
        static constexpr auto none_of{ true };
        template < template < auto > typename >
        static constexpr size_t find_first_if{ 0 };
        template < template < auto > typename >
        static constexpr size_t find_last_if{ 0 };
        template < template < auto > typename >
        static constexpr size_t find_first_if_not{ 0 };
        template < template < auto > typename >
        static constexpr size_t find_last_if_not{ 0 };
        template < auto >
        static constexpr size_t find_first{ 0 };
        template < auto >
        static constexpr size_t find_last{ 0 };
        template < auto... Us >
        using prepend = value_list< Us... >;
        template < auto... Us >
        using append       = value_list< Us... >;
        using remove_front = value_list<>;
        using remove_back  = value_list<>;
        template < typename Other >
        using concat  = Other;
        using reverse = value_list<>;
        using unique  = value_list<>;
        template < template < auto... > typename Template >
        using apply = Template<>;
        template < template < auto > typename F >
        using transform = value_list<>;
        template < template < auto > typename Pred >
        using filter  = value_list<>;
        value_list()  = delete;
        ~value_list() = delete;
    };
    namespace details
    {
        template < typename T >
        struct remove_identity;
        template < typename T >
        struct remove_identity< std::type_identity< T > > final
        {
            using type = T;
        };
        template < typename T >
        using remove_identity_t = remove_identity< T >::type;
        template < typename T, size_t... Is >
        inline consteval auto make_repeated_type_list_impl( std::index_sequence< Is... > )
        {
            auto helper{ []( auto v, auto ) consteval { return v; } };
            return std::type_identity< type_list< remove_identity_t< decltype( helper( std::type_identity< T >{}, Is ) ) >... > >{};
        }
        template < auto V, size_t... Is >
        inline consteval auto make_repeated_value_list_impl( std::index_sequence< Is... > )
        {
            auto helper{ []( auto v, auto ) consteval { return v; } };
            return std::type_identity< value_list< helper( V, Is )... > >{};
        }
    }
    template < typename T, size_t N >
    using make_repeated_type_list
      = details::remove_identity_t< decltype( details::make_repeated_type_list_impl< T >( std::make_index_sequence< N >{} ) ) >;
    template < auto V, size_t N >
    using make_repeated_value_list
      = details::remove_identity_t< decltype( details::make_repeated_value_list_impl< V >( std::make_index_sequence< N >{} ) ) >;
    template < typename >
    struct function_traits;
    template < typename R, typename... Args >
    struct function_traits< R( Args... ) > final
    {
        using return_type = R;
        using args_type   = type_list< Args... >;
    };
    template < typename R, typename... Args >
    struct function_traits< R ( * )( Args... ) > final
    {
        using return_type = R;
        using args_type   = type_list< Args... >;
    };
    template < typename R, typename T, typename... Args >
    struct function_traits< R ( T::* )( Args... ) > final
    {
        using return_type = R;
        using class_type  = T;
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
        template < typename T, typename... Matchers >
        struct match_impl;
        template < typename T >
        struct match_impl< T > final
        {
            static_assert( sizeof( T ) == 0, "No matching pattern found" );
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
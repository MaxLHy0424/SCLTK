#pragma once
#include <exception>
#include <format>
#include <memory_resource>
#include <print>
#include <source_location>
#include <stacktrace>
#include <string_view>
#include <utility>
#include "compiler.hpp"
namespace cpp_utils
{
    [[nodiscard]] inline auto make_log(
      const std::string_view message, const std::source_location src_location = std::source_location::current(),
      const std::pmr::stacktrace trace = std::pmr::stacktrace::current() )
    {
        return std::format(
          "{}({}:{}) {}: {}\n{}\n", src_location.file_name(), src_location.line(), src_location.column(),
          src_location.function_name(), message, trace );
    }
    template < typename F >
        requires requires( F&& func ) {
            { std::forward< F >( func )()() } -> std::convertible_to< bool >;
        }
    inline auto dynamic_assert(
      F&& func, const std::source_location src_location = std::source_location::current(),
      std::pmr::stacktrace trace = std::pmr::stacktrace::current() ) noexcept( has_exceptions )
    {
        using namespace std::string_view_literals;
#ifdef CPP_UTILS_MACRO_HAS_EXCEPTIONS
        try {
#endif
            if ( std::forward< F >( func )() == false ) [[unlikely]] {
                std::print( stderr, "{}", make_log( "assertion failid!"sv, src_location, std::move( trace ) ) );
                std::terminate();
            }
#ifdef CPP_UTILS_MACRO_HAS_EXCEPTIONS
        } catch ( const std::exception& e ) {
            std::print( stderr, "{}", make_log( e.what(), src_location, std::move( trace ) ) );
            std::terminate();
        } catch ( ... ) {
            std::print( stderr, "{}", make_log( "unknown exception caught!"sv, src_location, std::move( trace ) ) );
            std::terminate();
        }
#endif
    }
}
#pragma once
#include <exception>
#include <format>
#include <print>
#include <source_location>
#include <stacktrace>
#include <string_view>
#include <utility>
namespace cpp_utils
{
    inline auto make_log(
      const std::string_view message, const std::source_location src_location = std::source_location::current(),
      const std::stacktrace trace = std::stacktrace::current() )
    {
        return std::format(
          "{}({}:{}) {}: {}\n{}\n", src_location.file_name(), src_location.line(), src_location.column(),
          src_location.function_name(), message, trace );
    }
    inline auto dynamic_assert(
      const bool expression, const std::string_view failed_message = "assertion failid!",
      const std::source_location src_location = std::source_location::current(),
      std::stacktrace trace                   = std::stacktrace::current() ) noexcept
    {
        if ( expression == false ) {
            std::print( "{}", make_log( failed_message, src_location, std::move( trace ) ) );
            std::terminate();
        }
    }
    template < bool Cond >
    inline auto dynamic_assert_if(
      const bool expression, const std::string_view failed_message = "assertion failid!",
      const std::source_location src_location = std::source_location::current(),
      std::stacktrace trace                   = std::stacktrace::current() ) noexcept
    {
        if constexpr ( Cond == true ) {
            dynamic_assert( expression, failed_message, src_location, std::move( trace ) );
        }
    }
}
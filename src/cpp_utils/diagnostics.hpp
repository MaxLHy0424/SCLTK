#pragma once
#include <exception>
#include <format>
#include <print>
#include <source_location>
#include <stacktrace>
#include <string_view>
#include <utility>
namespace cpp_utils {
    inline auto make_log(
      const std::string_view _message,
      const std::source_location _source_location = std::source_location::current(),
      const std::stacktrace _stacktrace           = std::stacktrace::current() )
    {
        return std::format(
          "{}({}:{}) `{}`: {}\n{}\n", _source_location.file_name(), _source_location.line(), _source_location.column(),
          _source_location.function_name(), _message, _stacktrace );
    }
    inline auto dynamic_assert(
      const bool _expression,
      const std::string_view _failed_message      = "assertion failid!",
      const std::source_location _source_location = std::source_location::current(),
      std::stacktrace _stacktrace                 = std::stacktrace::current() ) noexcept
    {
        if ( _expression == false ) {
            std::print( "{}", make_log( _failed_message, _source_location, std::move( _stacktrace ) ) );
            std::terminate();
        }
    }
    template < bool _condition_ >
    inline auto dynamic_assert_if(
      const bool _expression,
      const std::string_view _failed_message      = "assertion failid!",
      const std::source_location _source_location = std::source_location::current(),
      std::stacktrace _stacktrace                 = std::stacktrace::current() ) noexcept
    {
        if constexpr ( _condition_ == true ) {
            dynamic_assert( _expression, _failed_message, _source_location, std::move( _stacktrace ) );
        }
    }
}
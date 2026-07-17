#pragma once
#if defined( _WIN32 ) || defined( _WIN64 )
# include <fcntl.h>
# include <io.h>
#endif
#include <cstdio>
#include <format>
#include <ostream>
#include <string_view>
#include <utility>
namespace cpp_utils
{
#if defined( _WIN32 ) || defined( _WIN64 )
    namespace details_
    {
        auto set_stdio_stream_mode_to_binary( std::FILE* const stream ) noexcept
        {
            if ( stream == stdout || stream == stderr ) {
                _setmode( _fileno( stream ), _O_BINARY );
            }
        }

    }
#endif
    struct no_formatting_t final
    { };
    inline constexpr no_formatting_t no_formatting{};
    template < typename... Args >
    auto print( std::FILE* const stream, const std::format_string< Args... > fmt, Args&&... args )
    {
        const auto out{ std::format( fmt, std::forward< Args >( args )... ) };
        fwrite( out.data(), sizeof( typename decltype( out )::value_type ), out.size(), stream );
    }
    template < typename... Args >
    auto print( std::FILE* const stream, const std::wformat_string< Args... > fmt, Args&&... args )
    {
#if defined( _WIN32 ) || defined( _WIN64 )
        details_::set_stdio_stream_mode_to_binary( stream );
#endif
        const auto out{ std::format( fmt, std::forward< Args >( args )... ) };
        fwrite( out.data(), sizeof( typename decltype( out )::value_type ), out.size(), stream );
    }
    template < typename... Args >
    auto print( const std::format_string< Args... > fmt, Args&&... args )
    {
        print( stdout, fmt, std::forward< Args >( args )... );
    }
    template < typename... Args >
    auto print( const std::wformat_string< Args... > fmt, Args&&... args )
    {
        print( stdout, fmt, std::forward< Args >( args )... );
    }
    template < typename... Args >
    auto print( std::ostream& stream, const std::format_string< Args... > fmt, Args&&... args )
    {
        stream << std::format( fmt, std::forward< Args >( args )... );
    }
    template < typename... Args >
    auto print( std::wostream& stream, const std::wformat_string< Args... > fmt, Args&&... args )
    {
        stream << std::format( fmt, std::forward< Args >( args )... );
    }
    auto print( const no_formatting_t, std::FILE* const stream, const std::string_view out ) noexcept
    {
        fwrite( out.data(), sizeof( typename decltype( out )::value_type ), out.size(), stream );
    }
    auto print( const no_formatting_t, std::FILE* const stream, const std::wstring_view out ) noexcept
    {
#if defined( _WIN32 ) || defined( _WIN64 )
        details_::set_stdio_stream_mode_to_binary( stream );
#endif
        fwrite( out.data(), sizeof( typename decltype( out )::value_type ), out.size(), stream );
    }
    auto print( const no_formatting_t, const std::string_view out ) noexcept
    {
        print( no_formatting, stdout, out );
    }
    auto print( const no_formatting_t, const std::wstring_view out ) noexcept
    {
        print( no_formatting, stdout, out );
    }
    auto print( const no_formatting_t, std::ostream& stream, const std::string_view out )
    {
        stream << out;
    }
    auto print( const no_formatting_t, std::wostream& stream, const std::wstring_view out )
    {
        stream << out;
    }
}
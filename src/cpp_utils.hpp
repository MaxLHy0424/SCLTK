#pragma once
#if defined( _WIN32 ) || defined( _WIN64 )
# include <shldisp.h>
# include <shlguid.h>
# include <shlobj.h>
# include <shlwapi.h>
# include <tchar.h>
# include <windows.h>
#endif
#include <ciso646>
#include <iconv.h>
#include <stdatomic.h>
#include <algorithm>
#include <any>
#include <array>
#include <atomic>
#include <barrier>
#include <bit>
#include <bitset>
#include <cassert>
#include <ccomplex>
#include <cctype>
#include <cerrno>
#include <cfenv>
#include <cfloat>
#include <charconv>
#include <chrono>
#include <cinttypes>
#include <climits>
#include <clocale>
#include <cmath>
#include <codecvt>
#include <compare>
#include <complex>
#include <concepts>
#include <condition_variable>
#include <coroutine>
#include <csetjmp>
#include <csignal>
#include <cstdalign>
#include <cstdarg>
#include <cstdbool>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctgmath>
#include <ctime>
#include <cuchar>
#include <cwchar>
#include <cwctype>
#include <deque>
#include <exception>
#include <expected>
#include <filesystem>
#include <format>
#include <forward_list>
#include <fstream>
#include <functional>
#include <future>
#include <generator>
#include <initializer_list>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <latch>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <memory_resource>
#include <mutex>
#include <new>
#include <numbers>
#include <numeric>
#include <optional>
#include <ostream>
#include <print>
#include <queue>
#include <random>
#include <ranges>
#include <ratio>
#include <regex>
#include <scoped_allocator>
#include <semaphore>
#include <set>
#include <shared_mutex>
#include <source_location>
#include <span>
#include <spanstream>
#include <sstream>
#include <stack>
#include <stacktrace>
#include <stdexcept>
#include <stdfloat>
#include <stop_token>
#include <streambuf>
#include <string>
#include <string_view>
#include <syncstream>
#include <system_error>
#include <text_encoding>
#include <thread>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <valarray>
#include <variant>
#include <vector>
#include <version>
namespace cpp_utils {
    using namespace std::chrono_literals;
    using namespace std::string_literals;
    using io_stream             = std::FILE;
    using size_type             = std::size_t;
    using nullptr_type          = std::nullptr_t;
    using ansi_char             = char;
    using ansi_std_string       = std::string;
    using ansi_std_string_view  = std::string_view;
    using wide_char             = wchar_t;
    using wide_std_string       = std::wstring;
    using wide_std_string_view  = std::wstring_view;
    using utf8_char             = char8_t;
    using utf8_std_string       = std::u8string;
    using utf8_std_string_view  = std::u8string_view;
    using utf16_char            = char16_t;
    using utf16_std_string      = std::u16string;
    using utf16_std_string_view = std::u16string_view;
    using utf32_char            = char32_t;
    using utf32_std_string      = std::u32string;
    using utf32_std_string_view = std::u32string_view;
    template < typename _type_ >
    using type_alloc = _type_;
    template < typename _char_type >
    using std_string = std::basic_string< _char_type >;
    template < typename _char_type >
    using std_string_view = std::basic_string_view< _char_type >;
    template < typename _char_type_ >
    inline consteval auto is_char_type()
    {
        return std::is_same_v< _char_type_, ansi_char > || std::is_same_v< _char_type_, wide_char >
            || std::is_same_v< _char_type_, utf8_char > || std::is_same_v< _char_type_, utf16_char >
            || std::is_same_v< _char_type_, utf32_char >;
    }
    template < typename _target_char_type_, typename _source_char_type_ >
        requires( is_char_type< std::decay_t< _target_char_type_ > >() && is_char_type< std::decay_t< _source_char_type_ > >() )
    inline consteval auto is_convertible_char_type()
    {
        return sizeof( std::decay_t< _target_char_type_ > ) == sizeof( std::decay_t< _source_char_type_ > );
    }
    template < typename _target_char_type_, typename _source_char_type_ >
        requires( is_convertible_char_type< _target_char_type_, _source_char_type_ >() )
    inline auto string_convert( const std_string< _source_char_type_ > &_str )
    {
        const auto str_it{ reinterpret_cast< const _target_char_type_ * >( _str.c_str() ) };
        return std_string< _target_char_type_ >{ str_it, str_it + _str.size() };
    }
    template < typename _target_char_type_, typename _source_char_type_ >
        requires( is_convertible_char_type< _target_char_type_, _source_char_type_ >() )
    inline auto string_convert( const std_string_view< _source_char_type_ > _str )
    {
        const auto str_it{ reinterpret_cast< const _target_char_type_ * >( _str.data() ) };
        return std_string< _target_char_type_ >{ str_it, str_it + _str.size() };
    }
    template < typename _target_char_type_, typename _source_char_type_ >
        requires( is_convertible_char_type< _target_char_type_, _source_char_type_ >() )
    inline auto string_convert( const _source_char_type_ *const _str )
    {
        return std_string< _target_char_type_ >{ reinterpret_cast< const _target_char_type_ * >( _str ) };
    }
    template < typename _target_char_type_, typename _source_char_type_ >
        requires( is_convertible_char_type< _target_char_type_, _source_char_type_ >() )
    inline auto string_view_convert( const std_string< _source_char_type_ > &_str )
    {
        const auto str_it{ reinterpret_cast< const _target_char_type_ * >( _str.c_str() ) };
        return std_string_view< _target_char_type_ >{ str_it, str_it + _str.size() };
    }
    template < typename _target_char_type_, typename _source_char_type_ >
        requires( is_convertible_char_type< _target_char_type_, _source_char_type_ >() )
    inline auto string_view_convert( const std_string_view< _source_char_type_ > _str )
    {
        const auto str_it{ reinterpret_cast< const _target_char_type_ * >( _str.data() ) };
        return std_string_view< _target_char_type_ >{ str_it, str_it + _str.size() };
    }
    template < typename _target_char_type_, typename _source_char_type_ >
        requires( is_convertible_char_type< _target_char_type_, _source_char_type_ >() )
    inline auto string_view_convert( const _source_char_type_ *const _str )
    {
        return std_string_view< _target_char_type_ >{ reinterpret_cast< const _target_char_type_ * >( _str ) };
    }
    template < typename _ptr_type_ >
        requires( std::is_pointer_v< _ptr_type_ > )
    inline auto ptr_to_ansi_string( const _ptr_type_ _ptr )
    {
        return _ptr == nullptr ? "nullptr"s : std::format( "0x{:x}", reinterpret_cast< std::uintptr_t >( _ptr ) );
    }
    template < typename _ptr_type_ >
        requires( std::is_pointer_v< _ptr_type_ > )
    inline auto ptr_to_utf8_string( const _ptr_type_ _ptr )
    {
        return string_convert< utf8_char >(
          _ptr == nullptr ? "nullptr"s : std::format( "0x{:x}", reinterpret_cast< std::uintptr_t >( _ptr ) ) );
    }
    template < typename... _args_ >
    inline auto utf8_format( const utf8_std_string_view _fmt, _args_ &&..._args )
    {
        const auto convert_arg{ []( auto &&_arg ) -> decltype( auto )
        {
            if constexpr (
              std::is_same_v< std::decay_t< decltype( _arg ) >, utf8_std_string >
              || std::is_same_v< std::decay_t< decltype( _arg ) >, utf8_std_string_view >
              || std::is_same_v< std::decay_t< decltype( _arg ) >, utf8_char * >
              || std::is_same_v< std::decay_t< decltype( _arg ) >, const utf8_char * > )
            {
                return static_cast< const ansi_std_string_view >( string_view_convert< ansi_char >( utf8_std_string_view{ _arg } ) );
            } else if constexpr (
              std::is_pointer_v< std::decay_t< decltype( _arg ) > >
              && !( std::is_same_v< std::decay_t< decltype( _arg ) >, ansi_char * >
                    || std::is_same_v< std::decay_t< decltype( _arg ) >, const ansi_char * > ) )
            {
                return static_cast< const ansi_std_string >( ptr_to_ansi_string( std::forward< decltype( _arg ) >( _arg ) ) );
            } else {
                return std::as_const( _arg );
            }
        } };
        return string_convert< utf8_char >( std::vformat(
          string_view_convert< ansi_char >( _fmt ), std::make_format_args( convert_arg( std::forward< _args_ >( _args ) )... ) ) );
    }
    template < typename... _args_ >
    inline auto &utf8_format_to( utf8_std_string &_str, const utf8_std_string_view _fmt, _args_ &&..._args )
    {
        auto tmp{ utf8_format( _fmt, std::forward< _args_ >( _args )... ) };
        _str.swap( tmp );
        return _str;
    }
    template < typename... _args_ >
    inline auto utf8_print( const utf8_std_string_view _fmt, _args_ &&..._args )
    {
        std::print( "{}", string_view_convert< ansi_char >( utf8_format( _fmt, std::forward< _args_ >( _args )... ) ) );
    }
    template < typename... _args_ >
    inline auto utf8_print( io_stream *const _stream, const utf8_std_string_view _fmt, _args_ &&..._args )
    {
        std::print( _stream, "{}", string_view_convert< ansi_char >( utf8_format( _fmt, std::forward< _args_ >( _args )... ) ) );
    }
    template < typename... _args_ >
    inline auto utf8_println( const utf8_std_string_view _fmt, _args_ &&..._args )
    {
        std::println( "{}", string_view_convert< ansi_char >( utf8_format( _fmt, std::forward< _args_ >( _args )... ) ) );
    }
    template < typename... _args_ >
    inline auto utf8_println( io_stream *const _stream, const utf8_std_string_view _fmt, _args_ &&..._args )
    {
        std::println( _stream, "{}", string_view_convert< ansi_char >( utf8_format( _fmt, std::forward< _args_ >( _args )... ) ) );
    }
    template < typename _char_type_, std::size_t _length_ >
    struct constexpr_string final {
        _char_type_ data[ _length_ ]{};
        auto operator=( const constexpr_string & ) -> constexpr_string & = delete;
        auto operator=( constexpr_string && ) -> constexpr_string &      = delete;
        constexpr constexpr_string( const _char_type_ ( &_str )[ _length_ ] )
        {
            std::copy( _str, _str + _length_, data );
        }
        constexpr constexpr_string( const constexpr_string & ) = default;
        constexpr constexpr_string( constexpr_string && )      = delete;
        constexpr ~constexpr_string()                          = default;
    };
    template < typename _chrono_type_ >
    inline auto perf_sleep( const _chrono_type_ _time )
    {
        std::this_thread::yield();
        std::this_thread::sleep_for( _time );
    }
    template < typename _char_type_ >
        requires( std::is_same_v< _char_type_, ansi_char > || std::is_same_v< _char_type_, utf8_char > )
    class multithread_task final {
      private:
        struct node_ final {
            std::jthread task_thread{};
            auto operator=( const node_ & ) -> node_ & = delete;
            auto operator=( node_ && ) -> node_ &      = default;
            node_()                                    = default;
            node_( std::jthread &&_task_thread )
              : task_thread{ std::move( _task_thread ) }
            { }
            node_( const node_ & ) = delete;
            node_( node_ && )      = default;
            ~node_()
            {
                if ( task_thread.joinable() ) {
                    if constexpr ( std::is_same_v< _char_type_, ansi_char > ) {
                        std::print( " -> 终止线程 {}.\n", task_thread.get_id() );
                    } else if constexpr ( std::is_same_v< _char_type_, utf8_char > ) {
                        utf8_print( u8" -> 终止线程 {}.\n", task_thread.get_id() );
                    }
                }
            }
        };
        std::vector< node_ > tasks_{};
      public:
        template < typename _callable_, typename... _args_ >
            requires( !( std::is_same_v< std::remove_cvref_t< _callable_ >, std::thread >
                         || std::is_same_v< std::remove_cvref_t< _callable_ >, std::jthread > ) )
        auto &add_task( const std_string_view< _char_type_ > _comment, _callable_ &&_func, _args_ &&..._args )
        {
            if constexpr ( std::is_same_v< _char_type_, ansi_char > ) {
                std::print( " -> 创建线程: {}.\n", _comment );
            } else if constexpr ( std::is_same_v< _char_type_, utf8_char > ) {
                utf8_print( u8" -> 创建线程: {}.\n", _comment );
            }
            tasks_.emplace_back( node_{
              std::jthread{ std::forward< _callable_ >( _func ), std::forward< _args_ >( _args )... }
            } );
            return *this;
        }
        auto &join_task( const size_type _index )
        {
            auto &task{ tasks_.at( _index ).task_thread };
            if ( task.joinable() ) {
                task.join();
            }
            return *this;
        }
        auto &detach_task( const size_type _index )
        {
            auto &task{ tasks_.at( _index ).task_thread };
            if ( task.joinable() ) {
                task.detach();
            }
            return *this;
        }
        auto operator=( const multithread_task & ) -> multithread_task & = delete;
        auto operator=( multithread_task && ) -> multithread_task &      = default;
        multithread_task()                                               = default;
        multithread_task( const multithread_task & )                     = delete;
        multithread_task( multithread_task && )                          = default;
        ~multithread_task()                                              = default;
    };
    using multithread_task_ansi = multithread_task< ansi_char >;
    using multithread_task_utf8 = multithread_task< utf8_char >;
    class multithread_task_nolog final {
      private:
        struct node_ final {
            std::jthread task_thread{};
            auto operator=( const node_ & ) -> node_ & = delete;
            auto operator=( node_ && ) -> node_ &      = default;
            node_()                                    = default;
            node_( std::jthread &&_task_thread )
              : task_thread{ std::move( _task_thread ) }
            { }
            node_( const node_ & ) = delete;
            node_( node_ && )      = default;
            ~node_()               = default;
        };
        std::vector< node_ > tasks_{};
      public:
        template < typename _callable_, typename... _args_ >
            requires( !( std::is_same_v< std::remove_cvref_t< _callable_ >, std::thread >
                         || std::is_same_v< std::remove_cvref_t< _callable_ >, std::jthread > ) )
        auto &add_task( _callable_ &&_func, _args_ &&..._args )
        {
            tasks_.emplace_back( node_{
              std::jthread{ std::forward< _callable_ >( _func ), std::forward< _args_ >( _args )... }
            } );
            return *this;
        }
        auto &join_task( const size_type _index )
        {
            auto &task{ tasks_.at( _index ).task_thread };
            if ( task.joinable() ) {
                task.join();
            }
            return *this;
        }
        auto &detach_task( const size_type _index )
        {
            auto &task{ tasks_.at( _index ).task_thread };
            if ( task.joinable() ) {
                task.detach();
            }
            return *this;
        }
        auto operator=( const multithread_task_nolog & ) -> multithread_task_nolog & = delete;
        auto operator=( multithread_task_nolog && ) -> multithread_task_nolog &      = default;
        multithread_task_nolog()                                                     = default;
        multithread_task_nolog( const multithread_task_nolog & )                     = delete;
        multithread_task_nolog( multithread_task_nolog && )                          = default;
        ~multithread_task_nolog()                                                    = default;
    };
#if defined( _WIN32 ) || defined( _WIN64 )
    inline auto is_run_as_admin()
    {
        BOOL is_admin{};
        PSID admins_group{};
        SID_IDENTIFIER_AUTHORITY nt_authority{ SECURITY_NT_AUTHORITY };
        if ( AllocateAndInitializeSid(
               &nt_authority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &admins_group )
             == TRUE )
        {
            CheckTokenMembership( nullptr, admins_group, &is_admin );
            FreeSid( admins_group );
        }
        return is_admin ? true : false;
    }
    inline auto relaunch_as_admin()
    {
        wide_std_string file_path( MAX_PATH, L'\0' );
        GetModuleFileNameW( nullptr, file_path.data(), MAX_PATH );
        ShellExecuteW( nullptr, L"runas", file_path.c_str(), nullptr, nullptr, SW_SHOWNORMAL );
        std::exit( 0 );
    }
    namespace console_value {
        inline constexpr DWORD mouse_button_left{ FROM_LEFT_1ST_BUTTON_PRESSED };
        inline constexpr DWORD mouse_button_middle{ FROM_LEFT_2ND_BUTTON_PRESSED };
        inline constexpr DWORD mouse_button_right{ RIGHTMOST_BUTTON_PRESSED };
        inline constexpr DWORD mouse_click{ 0x0000 };
        inline constexpr DWORD mouse_click_double{ DOUBLE_CLICK };
        inline constexpr DWORD mouse_move{ MOUSE_MOVED };
        inline constexpr DWORD mouse_wheel_height{ MOUSE_HWHEELED };
        inline constexpr DWORD mouse_wheel{ MOUSE_WHEELED };
        inline constexpr DWORD key_right_alt_press{ RIGHT_ALT_PRESSED };
        inline constexpr DWORD key_left_alt_press{ LEFT_ALT_PRESSED };
        inline constexpr DWORD key_right_ctrl_press{ RIGHT_CTRL_PRESSED };
        inline constexpr DWORD key_left_ctrl_press{ LEFT_CTRL_PRESSED };
        inline constexpr DWORD key_shift_press{ SHIFT_PRESSED };
        inline constexpr DWORD key_num_lock_on{ NUMLOCK_ON };
        inline constexpr DWORD key_scroll_lock_on{ SCROLLLOCK_ON };
        inline constexpr DWORD key_caps_lock_on{ CAPSLOCK_ON };
        inline constexpr DWORD key_enhanced_key{ ENHANCED_KEY };
        inline constexpr WORD text_default{ FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE };
        inline constexpr WORD text_foreground_red{ FOREGROUND_RED };
        inline constexpr WORD text_foreground_green{ FOREGROUND_GREEN };
        inline constexpr WORD text_foreground_blue{ FOREGROUND_BLUE };
        inline constexpr WORD text_foreground_intensity{ FOREGROUND_INTENSITY };
        inline constexpr WORD text_background_red{ BACKGROUND_RED };
        inline constexpr WORD text_background_green{ BACKGROUND_GREEN };
        inline constexpr WORD text_background_blue{ BACKGROUND_BLUE };
        inline constexpr WORD text_background_intensity{ BACKGROUND_INTENSITY };
        inline constexpr WORD text_lvb_leading_byte{ COMMON_LVB_LEADING_BYTE };
        inline constexpr WORD text_lvb_trailing_byte{ COMMON_LVB_TRAILING_BYTE };
        inline constexpr WORD text_lvb_grid_horizontal{ COMMON_LVB_GRID_HORIZONTAL };
        inline constexpr WORD text_lvb_grid_lvertical{ COMMON_LVB_GRID_LVERTICAL };
        inline constexpr WORD text_lvb_grid_rvertical{ COMMON_LVB_GRID_RVERTICAL };
        inline constexpr WORD text_lvb_reverse_video{ COMMON_LVB_REVERSE_VIDEO };
        inline constexpr WORD text_lvb_underscore{ COMMON_LVB_UNDERSCORE };
        inline constexpr WORD text_lvb_sbcsdbcs{ COMMON_LVB_SBCSDBCS };
        inline constexpr auto ui_back{ false };
        inline constexpr auto ui_quit{ true };
    };
    template < typename _char_type_ >
        requires( std::is_same_v< _char_type_, ansi_char > || std::is_same_v< _char_type_, utf8_char > )
    class console_ui final {
      public:
        struct func_args final {
            console_ui< _char_type_ > &parent_ui;
            const DWORD button_state;
            const DWORD ctrl_key_state;
            const DWORD event_flag;
            auto operator=( const func_args & ) -> func_args & = default;
            auto operator=( func_args && ) -> func_args &      = default;
            func_args(
              console_ui< _char_type_ > &_parent_ui,
              const MOUSE_EVENT_RECORD _mouse_event = MOUSE_EVENT_RECORD{ {}, console_value::mouse_button_left, {}, {} } )
              : parent_ui{ _parent_ui }
              , button_state{ _mouse_event.dwButtonState }
              , ctrl_key_state{ _mouse_event.dwControlKeyState }
              , event_flag{ _mouse_event.dwEventFlags }
            { }
            func_args( const func_args & ) = default;
            func_args( func_args && )      = default;
            ~func_args()                   = default;
        };
        using callback_type = std::function< bool( func_args ) >;
      private:
        enum class console_attrs_ { normal, lock_text, lock_all };
        struct line_node_ final {
            std_string< _char_type_ > text{};
            callback_type func{};
            WORD default_attrs{};
            WORD intensity_attrs{};
            WORD last_attrs{};
            COORD position{};
            auto set_attrs( const WORD _attrs )
            {
                SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), _attrs );
                last_attrs = _attrs;
            }
            auto operator==( const COORD _mouse_position ) const
            {
                return position.Y == _mouse_position.Y && position.X <= _mouse_position.X
                    && _mouse_position.X < ( position.X + static_cast< SHORT >( text.size() ) );
            }
            auto operator!=( const COORD _mouse_position ) const
            {
                return !operator==( _mouse_position );
            }
            auto operator=( const line_node_ & ) -> line_node_ & = default;
            auto operator=( line_node_ && ) -> line_node_ &      = default;
            line_node_()
              : default_attrs{ console_value::text_default }
              , intensity_attrs{ console_value::text_foreground_green | console_value::text_foreground_blue }
              , last_attrs{ console_value::text_default }
            { }
            line_node_(
              const std_string_view< _char_type_ > _text, callback_type &_func, const WORD _default_attrs,
              const WORD _intensity_attrs )
              : text{ _text }
              , func{ std::move( _func ) }
              , default_attrs{ _default_attrs }
              , intensity_attrs{ _intensity_attrs }
              , last_attrs{ _default_attrs }
            { }
            line_node_( const line_node_ & ) = default;
            line_node_( line_node_ && )      = default;
            ~line_node_()                    = default;
        };
        std::deque< line_node_ > lines_{};
        SHORT console_width_{};
        SHORT console_height_{};
        static auto show_cursor_( const bool _is_shown )
        {
            CONSOLE_CURSOR_INFO cursor_data;
            GetConsoleCursorInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &cursor_data );
            cursor_data.bVisible = _is_shown;
            SetConsoleCursorInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &cursor_data );
        }
        static auto edit_console_attrs_( const console_attrs_ _mod )
        {
            DWORD attrs;
            GetConsoleMode( GetStdHandle( STD_INPUT_HANDLE ), &attrs );
            switch ( _mod ) {
                case console_attrs_::normal :
                    attrs |= ENABLE_QUICK_EDIT_MODE;
                    attrs |= ENABLE_INSERT_MODE;
                    attrs |= ENABLE_MOUSE_INPUT;
                    break;
                case console_attrs_::lock_text :
                    attrs &= ~ENABLE_QUICK_EDIT_MODE;
                    attrs &= ~ENABLE_INSERT_MODE;
                    attrs |= ENABLE_MOUSE_INPUT;
                    break;
                case console_attrs_::lock_all :
                    attrs &= ~ENABLE_QUICK_EDIT_MODE;
                    attrs &= ~ENABLE_INSERT_MODE;
                    attrs &= ~ENABLE_MOUSE_INPUT;
                    break;
            }
            SetConsoleMode( GetStdHandle( STD_INPUT_HANDLE ), attrs );
        }
        static auto get_cursor_()
        {
            CONSOLE_SCREEN_BUFFER_INFO console_data;
            GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &console_data );
            return console_data.dwCursorPosition;
        }
        static auto set_cursor_( const COORD _cursor_position )
        {
            SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), _cursor_position );
        }
        static auto wait_mouse_event_( const bool _is_mouse_moved = true )
        {
            INPUT_RECORD record;
            DWORD reg;
            while ( true ) {
                perf_sleep( 10ms );
                ReadConsoleInputW( GetStdHandle( STD_INPUT_HANDLE ), &record, 1, &reg );
                if ( record.EventType == MOUSE_EVENT
                     && _is_mouse_moved | ( record.Event.MouseEvent.dwEventFlags != console_value::mouse_move ) )
                {
                    return record.Event.MouseEvent;
                }
            }
        }
        auto get_console_size_()
        {
            CONSOLE_SCREEN_BUFFER_INFO console_data;
            GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &console_data );
            console_height_ = console_data.dwSize.Y;
            console_width_  = console_data.dwSize.X;
        }
        auto cls_()
        {
            get_console_size_();
            set_cursor_( { 0, 0 } );
            if constexpr ( std::is_same_v< _char_type_, ansi_char > ) {
                std::print(
                  "{}",
                  ansi_std_string( static_cast< size_type >( console_width_ ) * static_cast< size_type >( console_height_ ), ' ' ) );
            } else if constexpr ( std::is_same_v< _char_type_, utf8_char > ) {
                utf8_print(
                  u8"{}",
                  utf8_std_string( static_cast< size_type >( console_width_ ) * static_cast< size_type >( console_height_ ), ' ' ) );
            }
            set_cursor_( { 0, 0 } );
        }
        static auto write_( const std_string_view< _char_type_ > _text, const bool _is_endl = false )
        {
            if constexpr ( std::is_same_v< _char_type_, ansi_char > ) {
                std::print( "{}{}", _text, _is_endl ? '\n' : '\0' );
            } else if constexpr ( std::is_same_v< _char_type_, utf8_char > ) {
                utf8_print( u8"{}{}", _text, _is_endl ? '\n' : '\0' );
            }
        }
        static auto rewrite_( const COORD _cursor_position, const std_string_view< _char_type_ > _text )
        {
            set_cursor_( { 0, _cursor_position.Y } );
            if constexpr ( std::is_same_v< _char_type_, ansi_char > ) {
                write_( ansi_std_string( _cursor_position.X, ' ' ) );
            } else if constexpr ( std::is_same_v< _char_type_, utf8_char > ) {
                write_( utf8_std_string( _cursor_position.X, ' ' ) );
            }
            set_cursor_( { 0, _cursor_position.Y } );
            write_( _text );
            set_cursor_( { 0, _cursor_position.Y } );
        }
        auto init_pos_()
        {
            cls_();
            for ( auto &line : lines_ ) {
                line.position = get_cursor_();
                line.set_attrs( line.default_attrs );
                write_( line.text, true );
            }
        }
        auto refresh_( const COORD _hang_position )
        {
            for ( auto &line : lines_ ) {
                if ( line == _hang_position && line.last_attrs != line.intensity_attrs ) {
                    line.set_attrs( line.intensity_attrs );
                    rewrite_( line.position, line.text );
                }
                if ( line != _hang_position && line.last_attrs != line.default_attrs ) {
                    line.set_attrs( line.default_attrs );
                    rewrite_( line.position, line.text );
                }
            }
        }
        auto call_func_( const MOUSE_EVENT_RECORD &_mouse_event )
        {
            auto is_exited{ console_value::ui_back };
            for ( auto &line : lines_ ) {
                if ( line != _mouse_event.dwMousePosition ) {
                    continue;
                }
                if ( line.func == nullptr ) {
                    break;
                }
                cls_();
                line.set_attrs( line.default_attrs );
                show_cursor_( false );
                edit_console_attrs_( console_attrs_::lock_all );
                is_exited = line.func( func_args{ *this, _mouse_event } );
                show_cursor_( false );
                edit_console_attrs_( console_attrs_::lock_text );
                init_pos_();
                break;
            }
            return is_exited;
        }
      public:
        auto empty() const
        {
            return lines_.empty();
        }
        auto size() const
        {
            return lines_.size();
        }
        auto max_size() const
        {
            return lines_.max_size();
        }
        auto &resize( const size_type _size )
        {
            lines_.resize( _size );
            return *this;
        }
        auto &optimize_storage()
        {
            lines_.shrink_to_fit();
            return *this;
        }
        auto &optimize_text()
        {
            for ( auto &line : lines_ ) {
                line.text.shrink_to_fit();
            }
            return *this;
        }
        auto &swap( console_ui &_src )
        {
            lines_.swap( _src.lines_ );
            return *this;
        }
        auto &add_front(
          const std_string_view< _char_type_ > _text, callback_type _func = nullptr,
          const WORD _intensity_attrs = console_value::text_foreground_green | console_value::text_foreground_blue,
          const WORD _default_attrs   = console_value::text_default )
        {
            lines_.emplace_front( line_node_{
              _text,
              _func,
              _default_attrs,
              _func != nullptr ? _intensity_attrs : _default_attrs,
            } );
            return *this;
        }
        auto &add_back(
          const std_string_view< _char_type_ > _text, callback_type _func = nullptr,
          const WORD _intensity_attrs = console_value::text_foreground_blue | console_value::text_foreground_green,
          const WORD _default_attrs   = console_value::text_default )
        {
            lines_.emplace_back( line_node_{
              _text,
              _func,
              _default_attrs,
              _func != nullptr ? _intensity_attrs : _default_attrs,
            } );
            return *this;
        }
        auto &insert(
          const size_type _index, const std_string_view< _char_type_ > _text, callback_type _func = nullptr,
          const WORD _intensity_attrs = console_value::text_foreground_green | console_value::text_foreground_blue,
          const WORD _default_attrs   = console_value::text_default )
        {
            lines_.emplace(
              lines_.cbegin() + _index,
              line_node_{ _text, _func, _default_attrs, _func != nullptr ? _intensity_attrs : _default_attrs } );
            return *this;
        }
        auto &edit(
          const size_type _index, const std_string_view< _char_type_ > _text, callback_type _func = nullptr,
          const WORD _intensity_attrs = console_value::text_foreground_green | console_value::text_foreground_blue,
          const WORD _default_attrs   = console_value::text_default )
        {
            lines_.at( _index ) = line_node_{ _text, _func, _default_attrs, _func != nullptr ? _intensity_attrs : _default_attrs };
            return *this;
        }
        auto &remove_front()
        {
            lines_.pop_front();
            return *this;
        }
        auto &remove_back()
        {
            lines_.pop_back();
            return *this;
        }
        auto &remove( const size_type _begin, const size_type _end )
        {
            lines_.erase( lines_.cbegin() + _begin, lines_.cbegin() + _end );
            return *this;
        }
        auto &clear()
        {
            lines_.clear();
            return *this;
        }
        auto &show()
        {
            show_cursor_( false );
            edit_console_attrs_( console_attrs_::lock_text );
            init_pos_();
            auto mouse_event{ MOUSE_EVENT_RECORD{} };
            auto func_return_value{ console_value::ui_back };
            while ( func_return_value == console_value::ui_back ) {
                mouse_event = wait_mouse_event_();
                switch ( mouse_event.dwEventFlags ) {
                    case console_value::mouse_move : refresh_( mouse_event.dwMousePosition ); break;
                    case console_value::mouse_click : {
                        if ( mouse_event.dwButtonState != false ) {
                            func_return_value = call_func_( mouse_event );
                        }
                        break;
                    }
                }
                perf_sleep( 10ms );
            }
            cls_();
            return *this;
        }
        auto &set_console(
          const std_string_view< _char_type_ > _title, const UINT _code_page, const SHORT _width, const SHORT _height,
          const bool _is_fixed_size, const bool _is_enabled_minimize_ctrl, const bool is_enabled_close_window_ctrl,
          const BYTE _translucency_value )
        {
            SetConsoleOutputCP( _code_page );
            SetConsoleCP( _code_page );
            if constexpr ( std::is_same_v< _char_type_, ansi_char > ) {
                SetConsoleTitleA( _title.data() );
            } else if constexpr ( std::is_same_v< _char_type_, utf8_char > ) {
                SetConsoleTitleA( string_view_convert< ansi_char >( _title ).c_str() );
            }
            std::system( std::format( "mode.com con cols={} lines={}", _width, _height ).c_str() );
            SetWindowLongPtrW(
              GetConsoleWindow(), GWL_STYLE,
              _is_fixed_size
                ? GetWindowLongPtrW( GetConsoleWindow(), GWL_STYLE ) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX
                : GetWindowLongPtrW( GetConsoleWindow(), GWL_STYLE ) | WS_SIZEBOX | WS_MAXIMIZEBOX );
            SetWindowLongPtrW(
              GetConsoleWindow(), GWL_STYLE,
              _is_enabled_minimize_ctrl
                ? GetWindowLongPtrW( GetConsoleWindow(), GWL_STYLE ) | WS_MINIMIZEBOX
                : GetWindowLongPtrW( GetConsoleWindow(), GWL_STYLE ) & ~WS_MINIMIZEBOX );
            EnableMenuItem(
              GetSystemMenu( GetConsoleWindow(), FALSE ), SC_CLOSE,
              is_enabled_close_window_ctrl ? MF_BYCOMMAND | MF_ENABLED : MF_BYCOMMAND | MF_DISABLED | MF_GRAYED );
            SetLayeredWindowAttributes( GetConsoleWindow(), RGB( 0, 0, 0 ), _translucency_value, LWA_ALPHA );
            return *this;
        }
        auto &lock( const bool _is_hidden_cursor, const bool _is_locked_text )
        {
            show_cursor_( !_is_hidden_cursor );
            edit_console_attrs_( _is_locked_text ? console_attrs_::lock_all : console_attrs_::normal );
            return *this;
        }
        auto operator=( const console_ui & ) -> console_ui & = default;
        auto operator=( console_ui && ) -> console_ui &      = default;
        console_ui()                                         = default;
        console_ui( const console_ui & )                     = default;
        console_ui( console_ui && )                          = default;
        ~console_ui()                                        = default;
    };
    using console_ui_ansi = console_ui< ansi_char >;
    using console_ui_utf8 = console_ui< utf8_char >;
#endif
}
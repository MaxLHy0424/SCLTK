#pragma once
#if defined( _WIN32 ) || defined( _WIN64 )
# include "windows_definations.hpp"
#endif
#include <chrono>
#include <concepts>
#include <deque>
#include <functional>
#include <print>
#include <ranges>
#include <thread>
#include <type_traits>
#include <utility>
#include "type.hpp"
namespace cpp_utils {
#if defined( _WIN32 ) || defined( _WIN64 )
    inline auto is_run_as_admin() noexcept
    {
        BOOL is_admin;
        PSID admins_group;
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
    inline auto relaunch( const int _exit_code, const wchar_t *const _args ) noexcept
    {
        wchar_t file_path[ MAX_PATH ]{};
        GetModuleFileNameW( nullptr, file_path, MAX_PATH );
        ShellExecuteW( nullptr, L"open", file_path, _args, nullptr, SW_SHOWNORMAL );
        std::exit( _exit_code );
    }
    inline auto relaunch_as_admin( const int _exit_code, const wchar_t *const _args ) noexcept
    {
        wchar_t file_path[ MAX_PATH ]{};
        GetModuleFileNameW( nullptr, file_path, MAX_PATH );
        ShellExecuteW( nullptr, L"runas", file_path, _args, nullptr, SW_SHOWNORMAL );
        std::exit( _exit_code );
    }
    inline auto get_current_console_std_handle( const DWORD _std_handle_flag ) noexcept
    {
        return GetStdHandle( _std_handle_flag );
    }
    inline auto get_current_window_handle() noexcept
    {
        auto window_handle{ GetConsoleWindow() };
        if ( window_handle == nullptr ) [[unlikely]] {
            window_handle = GetForegroundWindow();
        }
        if ( window_handle == nullptr ) [[unlikely]] {
            window_handle = GetActiveWindow();
        }
        return window_handle;
    }
    inline auto get_window_state( const HWND _window_handle ) noexcept
    {
        WINDOWPLACEMENT wp;
        wp.length = sizeof( WINDOWPLACEMENT );
        GetWindowPlacement( _window_handle, &wp );
        return wp.showCmd;
    }
    inline auto set_window_state( const HWND _window_handle, const UINT _state ) noexcept
    {
        ShowWindow( _window_handle, _state );
    }
    inline auto keep_window_top( const HWND _window_handle, const DWORD _thread_id, const DWORD _window_thread_process_id ) noexcept
    {
        AttachThreadInput( _thread_id, _window_thread_process_id, TRUE );
        SetForegroundWindow( _window_handle );
        SetWindowPos( _window_handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
        AttachThreadInput( _thread_id, _window_thread_process_id, FALSE );
    }
    inline auto keep_current_window_top() noexcept
    {
        auto window_handle{ get_current_window_handle() };
        keep_window_top( window_handle, GetCurrentThreadId(), GetWindowThreadProcessId( window_handle, nullptr ) );
    }
    template < typename _chrono_type_perfix_, typename _chrono_type_suffix_, typename _func_, typename... _args_ >
    inline auto loop_keep_window_top(
      const HWND _window_handle, const DWORD _thread_id, const DWORD _window_thread_process_id,
      const std::chrono::duration< _chrono_type_perfix_, _chrono_type_suffix_ > _sleep_time, _func_ &&_condition_checker,
      _args_ &&..._condition_checker_args )
    {
        while ( _condition_checker( std::forward< _args_ >( _condition_checker_args )... ) ) {
            AttachThreadInput( _thread_id, _window_thread_process_id, TRUE );
            SetForegroundWindow( _window_handle );
            SetWindowPos( _window_handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
            AttachThreadInput( _thread_id, _window_thread_process_id, FALSE );
            std::this_thread::sleep_for( _sleep_time );
        }
    }
    template < typename _chrono_type_perfix_, typename _chrono_type_suffix_, typename _func_, typename... _args_ >
    inline auto loop_keep_current_window_top(
      const std::chrono::duration< _chrono_type_perfix_, _chrono_type_suffix_ > _sleep_time, _func_ &&_condition_checker,
      _args_ &&..._condition_checker_args )
    {
        auto window_handle{ get_current_window_handle() };
        loop_keep_window_top(
          window_handle, GetCurrentThreadId(), GetWindowThreadProcessId( window_handle, nullptr ), _sleep_time,
          std::forward< _func_ >( _condition_checker ), std::forward< _args_ >( _condition_checker_args )... );
    }
    inline auto cancel_top_window( const HWND _window_handle ) noexcept
    {
        SetWindowPos( _window_handle, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
    }
    inline auto ignore_console_exit_signal( const bool _is_ignore ) noexcept
    {
        return SetConsoleCtrlHandler( nullptr, static_cast< WINBOOL >( _is_ignore ) );
    }
    inline auto enable_virtual_terminal_processing( const HANDLE _std_output_handle ) noexcept
    {
        DWORD mode;
        GetConsoleMode( _std_output_handle, &mode );
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode( _std_output_handle, mode );
    }
    inline auto disable_virtual_terminal_processing( const HANDLE _std_output_handle ) noexcept
    {
        DWORD mode;
        GetConsoleMode( _std_output_handle, &mode );
        mode &= ~ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode( _std_output_handle, mode );
    }
    inline auto clear_console( const HANDLE _std_output_handle ) noexcept
    {
        enable_virtual_terminal_processing( _std_output_handle );
        std::print( "\033[H\033[2J" );
    }
    inline auto reset_console( const HANDLE _std_output_handle ) noexcept
    {
        enable_virtual_terminal_processing( _std_output_handle );
        std::print( "\033c" );
    }
    inline auto set_console_title( const char *const _title ) noexcept
    {
        SetConsoleTitleA( _title );
    }
    inline auto set_console_title( const std::string &_title ) noexcept
    {
        SetConsoleTitleA( _title.data() );
    }
    inline auto set_console_title( const wchar_t *const _title ) noexcept
    {
        SetConsoleTitleW( _title );
    }
    inline auto set_console_title( const std::wstring &_title ) noexcept
    {
        SetConsoleTitleW( _title.data() );
    }
    inline auto set_console_charset( const UINT _charset_id ) noexcept
    {
        SetConsoleOutputCP( _charset_id );
        SetConsoleCP( _charset_id );
    }
    inline auto
      set_console_size( const HWND _window_handle, const HANDLE _std_output_handle, const SHORT _width, const SHORT _height ) noexcept
    {
        SMALL_RECT wrt{ 0, 0, static_cast< SHORT >( _width - 1 ), static_cast< SHORT >( _height - 1 ) };
        set_window_state( _window_handle, SW_SHOWNORMAL );
        SetConsoleScreenBufferSize( _std_output_handle, { _width, _height } );
        SetConsoleWindowInfo( _std_output_handle, TRUE, &wrt );
        SetConsoleScreenBufferSize( _std_output_handle, { _width, _height } );
        clear_console( _std_output_handle );
    }
    inline auto set_window_translucency( const HWND _window_handle, const BYTE _value ) noexcept
    {
        SetLayeredWindowAttributes( _window_handle, RGB( 0, 0, 0 ), _value, LWA_ALPHA );
    }
    inline auto fix_window_size( const HWND _window_handle, const bool _is_enable ) noexcept
    {
        SetWindowLongPtrW(
          _window_handle, GWL_STYLE,
          _is_enable
            ? GetWindowLongPtrW( _window_handle, GWL_STYLE ) & ~WS_SIZEBOX
            : GetWindowLongPtrW( _window_handle, GWL_STYLE ) | WS_SIZEBOX );
    }
    inline auto enable_window_menu( const HWND _window_handle, const bool _is_enable ) noexcept
    {
        SetWindowLongPtrW(
          _window_handle, GWL_STYLE,
          _is_enable
            ? GetWindowLongPtrW( _window_handle, GWL_STYLE ) | WS_SYSMENU
            : GetWindowLongPtrW( _window_handle, GWL_STYLE ) & ~WS_SYSMENU );
    }
    inline auto enable_window_minimize_ctrl( const HWND _window_handle, const bool _is_enable ) noexcept
    {
        SetWindowLongPtrW(
          _window_handle, GWL_STYLE,
          _is_enable
            ? GetWindowLongPtrW( _window_handle, GWL_STYLE ) | WS_MINIMIZEBOX
            : GetWindowLongPtrW( _window_handle, GWL_STYLE ) & ~WS_MINIMIZEBOX );
    }
    inline auto enable_window_maximize_ctrl( const HWND _window_handle, const bool _is_enable ) noexcept
    {
        SetWindowLongPtrW(
          _window_handle, GWL_STYLE,
          _is_enable
            ? GetWindowLongPtrW( _window_handle, GWL_STYLE ) | WS_MAXIMIZEBOX
            : GetWindowLongPtrW( _window_handle, GWL_STYLE ) & ~WS_MAXIMIZEBOX );
    }
    inline auto enable_window_close_ctrl( const HWND _window_handle, const bool _is_enable ) noexcept
    {
        EnableMenuItem(
          GetSystemMenu( _window_handle, FALSE ), SC_CLOSE,
          _is_enable ? MF_BYCOMMAND | MF_ENABLED : MF_BYCOMMAND | MF_DISABLED | MF_GRAYED );
    }
#else
# error "must be compiled on the Windows OS"
#endif
}
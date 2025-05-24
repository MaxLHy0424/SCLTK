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
#include "type_tools.hpp"
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
    inline auto relaunch( const int exit_code, const wchar_t *const args ) noexcept
    {
        wchar_t file_path[ MAX_PATH ]{};
        GetModuleFileNameW( nullptr, file_path, MAX_PATH );
        ShellExecuteW( nullptr, L"open", file_path, args, nullptr, SW_SHOWNORMAL );
        std::exit( exit_code );
    }
    inline auto relaunch_as_admin( const int exit_code, const wchar_t *const args ) noexcept
    {
        wchar_t file_path[ MAX_PATH ]{};
        GetModuleFileNameW( nullptr, file_path, MAX_PATH );
        ShellExecuteW( nullptr, L"runas", file_path, args, nullptr, SW_SHOWNORMAL );
        std::exit( exit_code );
    }
    inline auto get_current_console_std_handle( const DWORD std_handle_flag ) noexcept
    {
        return GetStdHandle( std_handle_flag );
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
    inline auto get_window_state( const HWND window_handle ) noexcept
    {
        WINDOWPLACEMENT wp;
        wp.length = sizeof( WINDOWPLACEMENT );
        GetWindowPlacement( window_handle, &wp );
        return wp.showCmd;
    }
    inline auto get_current_window_state() noexcept
    {
        return get_window_state( get_current_window_handle() );
    }
    inline auto set_window_state( const HWND window_handle, const UINT state ) noexcept
    {
        ShowWindow( window_handle, state );
    }
    inline auto set_current_window_state( const UINT state ) noexcept
    {
        set_window_state( get_current_window_handle(), state );
    }
    inline auto keep_window_top( const HWND window_handle, const DWORD thread_id, const DWORD window_thread_process_id ) noexcept
    {
        AttachThreadInput( thread_id, window_thread_process_id, TRUE );
        SetForegroundWindow( window_handle );
        SetWindowPos( window_handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
        AttachThreadInput( thread_id, window_thread_process_id, FALSE );
    }
    inline auto keep_current_window_top() noexcept
    {
        auto window_handle{ get_current_window_handle() };
        keep_window_top( window_handle, GetCurrentThreadId(), GetWindowThreadProcessId( window_handle, nullptr ) );
    }
    template < typename ChronoRep, typename ChronoPeriod, typename Invocable, typename... Args >
    inline auto loop_keep_window_top(
      const HWND window_handle, const DWORD thread_id, const DWORD window_thread_process_id,
      const std::chrono::duration< ChronoRep, ChronoPeriod > sleep_time, Invocable &&condition_checker,
      Args &&...condition_checker_args )
    {
        while ( condition_checker( std::forward< Args >( condition_checker_args )... ) ) {
            AttachThreadInput( thread_id, window_thread_process_id, TRUE );
            SetForegroundWindow( window_handle );
            SetWindowPos( window_handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
            AttachThreadInput( thread_id, window_thread_process_id, FALSE );
            std::this_thread::sleep_for( sleep_time );
        }
    }
    template < typename ChronoRep, typename ChronoPeriod, typename Invocable, typename... Args >
    inline auto loop_keep_current_window_top(
      const std::chrono::duration< ChronoRep, ChronoPeriod > sleep_time, Invocable &&condition_checker,
      Args &&...condition_checker_args )
    {
        auto window_handle{ get_current_window_handle() };
        loop_keep_window_top(
          window_handle, GetCurrentThreadId(), GetWindowThreadProcessId( window_handle, nullptr ), sleep_time,
          std::forward< Invocable >( condition_checker ), std::forward< Args >( condition_checker_args )... );
    }
    inline auto cancel_top_window( const HWND window_handle ) noexcept
    {
        SetWindowPos( window_handle, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
    }
    inline auto cancel_top_current_window() noexcept
    {
        cancel_top_window( get_current_window_handle() );
    }
    inline auto ignore_current_console_exit_signal( const bool is_ignore ) noexcept
    {
        return SetConsoleCtrlHandler( nullptr, static_cast< WINBOOL >( is_ignore ) );
    }
    inline auto enable_virtual_terminal_processing( const HANDLE std_output_handle, const bool is_enable ) noexcept
    {
        DWORD mode;
        GetConsoleMode( std_output_handle, &mode );
        is_enable ? mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING : mode &= ~ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode( std_output_handle, mode );
    }
    inline auto enable_current_virtual_terminal_processing( const bool is_enable ) noexcept
    {
        enable_virtual_terminal_processing( GetStdHandle( STD_OUTPUT_HANDLE ), is_enable );
    }
    inline auto clear_console( const HANDLE std_output_handle ) noexcept
    {
        enable_virtual_terminal_processing( std_output_handle, true );
        std::print( "\033[H\033[2J" );
    }
    inline auto clear_current_console() noexcept
    {
        clear_console( GetStdHandle( STD_OUTPUT_HANDLE ) );
    }
    inline auto reset_console( const HANDLE std_output_handle ) noexcept
    {
        enable_virtual_terminal_processing( std_output_handle, true );
        std::print( "\033c" );
    }
    inline auto reset_current_console() noexcept
    {
        reset_console( GetStdHandle( STD_OUTPUT_HANDLE ) );
    }
    inline auto set_current_console_title( const char *const title ) noexcept
    {
        SetConsoleTitleA( title );
    }
    inline auto set_current_console_title( const std::string &title ) noexcept
    {
        SetConsoleTitleA( title.data() );
    }
    inline auto set_current_console_title( const wchar_t *const title ) noexcept
    {
        SetConsoleTitleW( title );
    }
    inline auto set_current_console_title( const std::wstring &title ) noexcept
    {
        SetConsoleTitleW( title.data() );
    }
    inline auto set_current_console_charset( const UINT charset_id ) noexcept
    {
        SetConsoleOutputCP( charset_id );
        SetConsoleCP( charset_id );
    }
    inline auto
      set_console_size( const HWND window_handle, const HANDLE std_output_handle, const SHORT width, const SHORT height ) noexcept
    {
        SMALL_RECT wrt{ 0, 0, static_cast< SHORT >( width - 1 ), static_cast< SHORT >( height - 1 ) };
        set_window_state( window_handle, SW_SHOWNORMAL );
        SetConsoleScreenBufferSize( std_output_handle, { width, height } );
        SetConsoleWindowInfo( std_output_handle, TRUE, &wrt );
        SetConsoleScreenBufferSize( std_output_handle, { width, height } );
        clear_console( std_output_handle );
    }
    inline auto set_current_console_size( const SHORT width, const SHORT height ) noexcept
    {
        set_console_size( GetConsoleWindow(), GetStdHandle( STD_OUTPUT_HANDLE ), width, height );
    }
    inline auto set_window_translucency( const HWND window_handle, const BYTE value ) noexcept
    {
        SetLayeredWindowAttributes( window_handle, RGB( 0, 0, 0 ), value, LWA_ALPHA );
    }
    inline auto set_current_window_translucency( const BYTE value ) noexcept
    {
        set_window_translucency( get_current_window_handle(), value );
    }
    inline auto fix_window_size( const HWND window_handle, const bool is_enable ) noexcept
    {
        SetWindowLongPtrW(
          window_handle, GWL_STYLE,
          is_enable
            ? GetWindowLongPtrW( window_handle, GWL_STYLE ) & ~WS_SIZEBOX
            : GetWindowLongPtrW( window_handle, GWL_STYLE ) | WS_SIZEBOX );
    }
    inline auto fix_current_window_size( const bool is_enable ) noexcept
    {
        fix_window_size( get_current_window_handle(), is_enable );
    }
    inline auto enable_window_menu( const HWND window_handle, const bool is_enable ) noexcept
    {
        SetWindowLongPtrW(
          window_handle, GWL_STYLE,
          is_enable
            ? GetWindowLongPtrW( window_handle, GWL_STYLE ) | WS_SYSMENU
            : GetWindowLongPtrW( window_handle, GWL_STYLE ) & ~WS_SYSMENU );
    }
    inline auto enable_current_window_menu( const bool is_enable ) noexcept
    {
        enable_window_menu( get_current_window_handle(), is_enable );
    }
    inline auto enable_window_minimize_ctrl( const HWND window_handle, const bool is_enable ) noexcept
    {
        SetWindowLongPtrW(
          window_handle, GWL_STYLE,
          is_enable
            ? GetWindowLongPtrW( window_handle, GWL_STYLE ) | WS_MINIMIZEBOX
            : GetWindowLongPtrW( window_handle, GWL_STYLE ) & ~WS_MINIMIZEBOX );
    }
    inline auto enable_current_window_minimize_ctrl( const bool is_enable ) noexcept
    {
        enable_window_minimize_ctrl( get_current_window_handle(), is_enable );
    }
    inline auto enable_window_maximize_ctrl( const HWND window_handle, const bool is_enable ) noexcept
    {
        SetWindowLongPtrW(
          window_handle, GWL_STYLE,
          is_enable
            ? GetWindowLongPtrW( window_handle, GWL_STYLE ) | WS_MAXIMIZEBOX
            : GetWindowLongPtrW( window_handle, GWL_STYLE ) & ~WS_MAXIMIZEBOX );
    }
    inline auto enable_current_window_maximize_ctrl( const bool is_enable ) noexcept
    {
        enable_window_maximize_ctrl( get_current_window_handle(), is_enable );
    }
    inline auto enable_window_close_ctrl( const HWND window_handle, const bool is_enable ) noexcept
    {
        EnableMenuItem(
          GetSystemMenu( window_handle, FALSE ), SC_CLOSE,
          is_enable ? MF_BYCOMMAND | MF_ENABLED : MF_BYCOMMAND | MF_DISABLED | MF_GRAYED );
    }
    inline auto enable_current_window_close_ctrl( const bool is_enable ) noexcept
    {
        enable_window_close_ctrl( get_current_window_handle(), is_enable );
    }
#else
# error "must be compiled on the windows os"
#endif
}
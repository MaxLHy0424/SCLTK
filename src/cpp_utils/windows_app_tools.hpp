#pragma once
#if defined( _WIN32 ) || defined( _WIN64 )
# include "windows_definations.hpp"
# if true
#  include <tlhelp32.h>
# endif
#endif
#include <chrono>
#include <concepts>
#include <functional>
#include <memory>
#include <print>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>
#include "meta_base.hpp"
namespace cpp_utils
{
#if defined( _WIN32 ) || defined( _WIN64 )
    namespace details
    {
        template < UINT Charset >
        inline auto to_wstring( const char* const str ) noexcept
        {
            using namespace std::string_literals;
            if ( str == nullptr ) {
                return L""s;
            }
            if ( str[ 0 ] == '\0' ) {
                return L""s;
            }
            const auto size_needed{ MultiByteToWideChar( Charset, 0, str, -1, nullptr, 0 ) };
            if ( size_needed <= 0 ) {
                return L""s;
            }
            std::wstring result( size_needed, '\0' );
            MultiByteToWideChar( Charset, 0, str, -1, result.data(), size_needed );
            return result;
        }
        inline auto stop_service_and_dependencies( const SC_HANDLE scm, const SC_HANDLE service ) noexcept -> DWORD
        {
            using namespace std::chrono_literals;
            DWORD result{ ERROR_SUCCESS };
            SERVICE_STATUS status;
            DWORD bytes_needed{ 0 };
            if ( !QueryServiceConfigW( service, nullptr, 0, &bytes_needed ) && GetLastError() == ERROR_INSUFFICIENT_BUFFER ) {
                const auto buffer{ std::make_unique< BYTE[] >( bytes_needed ) };
                const auto config{ reinterpret_cast< LPQUERY_SERVICE_CONFIGW >( buffer.get() ) };
                if ( QueryServiceConfigW( service, config, bytes_needed, &bytes_needed ) ) {
                    if ( config->lpDependencies != nullptr && *config->lpDependencies != '\0' ) {
                        auto dependency{ config->lpDependencies };
                        while ( *dependency != L'\0' ) {
                            const auto dependency_service{ OpenServiceW( scm, dependency, SERVICE_STOP | SERVICE_QUERY_STATUS ) };
                            if ( dependency_service != nullptr ) {
                                const auto dependency_result{ stop_service_and_dependencies( scm, dependency_service ) };
                                if ( dependency_result != ERROR_SUCCESS ) {
                                    result = dependency_result;
                                }
                                CloseServiceHandle( dependency_service );
                            }
                            dependency += wcslen( dependency ) + 1;
                        }
                    }
                }
            }
            if ( ControlService( service, SERVICE_CONTROL_STOP, &status ) ) {
                while ( QueryServiceStatus( service, &status ) ) {
                    if ( status.dwCurrentState != SERVICE_STOP_PENDING ) {
                        break;
                    }
                    std::this_thread::sleep_for( 10ms );
                }
                if ( status.dwCurrentState != SERVICE_STOPPED ) {
                    result = ERROR_SERVICE_REQUEST_TIMEOUT;
                }
            } else if ( GetLastError() != ERROR_SERVICE_NOT_ACTIVE ) {
                result = GetLastError();
            }
            return result;
        }
        inline auto start_service_and_dependencies( const SC_HANDLE scm, const SC_HANDLE service ) noexcept -> DWORD
        {
            DWORD result{ ERROR_SUCCESS };
            DWORD bytes_needed;
            if ( QueryServiceConfigW( service, nullptr, 0, &bytes_needed ) || GetLastError() == ERROR_INSUFFICIENT_BUFFER ) {
                const auto buffer{ std::make_unique< BYTE[] >( bytes_needed ) };
                const auto config{ reinterpret_cast< LPQUERY_SERVICE_CONFIGW >( buffer.get() ) };
                if ( QueryServiceConfigW( service, config, bytes_needed, &bytes_needed ) ) {
                    if ( config->lpDependencies && *config->lpDependencies ) {
                        wchar_t* context{ nullptr };
                        auto dependency{ wcstok_s( config->lpDependencies, L"\0", &context ) };
                        while ( dependency != nullptr ) {
                            if ( *dependency != L'@' ) {
                                const auto dependency_service{
                                  OpenServiceW( scm, dependency, SERVICE_START | SERVICE_QUERY_STATUS ) };
                                if ( dependency_service ) {
                                    SERVICE_STATUS status;
                                    if ( !QueryServiceStatus( dependency_service, &status )
                                         || ( status.dwCurrentState != SERVICE_RUNNING && status.dwCurrentState != SERVICE_START_PENDING ) )
                                    {
                                        result = start_service_and_dependencies( scm, dependency_service );
                                    }
                                    CloseServiceHandle( dependency_service );
                                }
                            }
                            dependency = wcstok_s( nullptr, L"\0", &context );
                        }
                    }
                }
            }
            if ( result == ERROR_SUCCESS && !StartServiceW( service, 0, nullptr ) ) {
                const auto err{ GetLastError() };
                if ( err != ERROR_SERVICE_ALREADY_RUNNING ) {
                    result = err;
                }
            }
            return result;
        }
    }
    template < UINT Charset >
    inline auto kill_process_by_name( const char* const process_name ) noexcept
    {
        const auto w_name{ details::to_wstring< Charset >( process_name ) };
        if ( w_name.empty() ) {
            return static_cast< DWORD >( ERROR_INVALID_PARAMETER );
        }
        const auto process_snapshot{ CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 ) };
        if ( process_snapshot == INVALID_HANDLE_VALUE ) {
            return GetLastError();
        }
        PROCESSENTRY32W process_entry{};
        process_entry.dwSize = sizeof( process_entry );
        DWORD result{ ERROR_SUCCESS };
        bool is_found{ false };
        if ( Process32FirstW( process_snapshot, &process_entry ) ) {
            do {
                if ( _wcsicmp( process_entry.szExeFile, w_name.c_str() ) == 0 ) {
                    is_found = true;
                    const auto process_handle{ OpenProcess( PROCESS_TERMINATE, FALSE, process_entry.th32ProcessID ) };
                    if ( process_handle ) {
                        if ( !TerminateProcess( process_handle, 1 ) ) {
                            result = GetLastError();
                        }
                        CloseHandle( process_handle );
                    } else {
                        result = GetLastError();
                    }
                }
            } while ( Process32NextW( process_snapshot, &process_entry ) );
        } else {
            result = GetLastError();
        }
        CloseHandle( process_snapshot );
        return is_found ? result : ERROR_NOT_FOUND;
    }
    template < UINT Charset >
    inline auto create_registry_key(
      const HKEY main_key, const char* const sub_key, const char* const value_name, const DWORD type, const BYTE* const data,
      const DWORD data_size ) noexcept
    {
        using namespace std::string_literals;
        const auto w_sub_key{ details::to_wstring< Charset >( sub_key ) };
        const auto w_value_name{ value_name ? details::to_wstring< Charset >( value_name ) : L""s };
        HKEY key_handle;
        auto result{ RegCreateKeyExW(
          main_key, w_sub_key.c_str(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &key_handle, nullptr ) };
        if ( result != ERROR_SUCCESS ) {
            return result;
        }
        result = RegSetValueExW( key_handle, w_value_name.empty() ? nullptr : w_value_name.c_str(), 0, type, data, data_size );
        RegCloseKey( key_handle );
        return result;
    }
    template < UINT Charset >
    inline auto delete_registry_key( const HKEY main_key, const char* const sub_key, const char* const value_name ) noexcept
    {
        using namespace std::string_literals;
        const auto w_sub_key{ details::to_wstring< Charset >( sub_key ) };
        const auto w_value_name{ value_name ? details::to_wstring< Charset >( value_name ) : L""s };
        HKEY key_handle;
        auto result{ RegOpenKeyExW( main_key, w_sub_key.c_str(), 0, KEY_WRITE, &key_handle ) };
        if ( result != ERROR_SUCCESS ) {
            return result;
        }
        result = RegDeleteValueW( key_handle, w_value_name.empty() ? nullptr : w_value_name.c_str() );
        RegCloseKey( key_handle );
        return result;
    }
    template < UINT Charset >
    inline auto delete_registry_tree( const HKEY main_key, const char* const sub_key ) noexcept
    {
        return RegDeleteTreeW( main_key, details::to_wstring< Charset >( sub_key ).c_str() );
    }
    template < UINT Charset >
    inline auto set_service_status( const char* const service_name, const DWORD status_type ) noexcept
    {
        const auto w_name{ details::to_wstring< Charset >( service_name ) };
        if ( w_name.empty() ) {
            return static_cast< DWORD >( ERROR_INVALID_PARAMETER );
        }
        const auto scm{ OpenSCManagerW( nullptr, nullptr, SC_MANAGER_CONNECT ) };
        if ( scm == nullptr ) {
            return GetLastError();
        }
        const auto service{ OpenServiceW( scm, w_name.c_str(), SERVICE_CHANGE_CONFIG ) };
        DWORD result{ ERROR_SUCCESS };
        if ( service != nullptr ) {
            if ( !ChangeServiceConfigW(
                   service, SERVICE_NO_CHANGE, status_type, SERVICE_NO_CHANGE, nullptr, nullptr, nullptr, nullptr, nullptr,
                   nullptr, nullptr ) )
            {
                result = GetLastError();
            }
            CloseServiceHandle( service );
        } else {
            result = GetLastError();
        }
        CloseServiceHandle( scm );
        return result;
    }
    template < UINT Charset >
    inline auto stop_service_with_dependencies( const char* const service_name ) noexcept
    {
        const auto w_name{ details::to_wstring< Charset >( service_name ) };
        if ( w_name.empty() ) {
            return static_cast< DWORD >( ERROR_INVALID_PARAMETER );
        }
        const auto scm{ OpenSCManagerW( nullptr, nullptr, SC_MANAGER_CONNECT | SC_MANAGER_ENUMERATE_SERVICE ) };
        if ( scm == nullptr ) {
            return GetLastError();
        }
        const auto service{
          OpenServiceW( scm, w_name.c_str(), SERVICE_STOP | SERVICE_QUERY_STATUS | SERVICE_ENUMERATE_DEPENDENTS ) };
        DWORD result{ ERROR_SUCCESS };
        if ( service ) {
            result = details::stop_service_and_dependencies( scm, service );
            CloseServiceHandle( service );
        } else {
            result = GetLastError();
        }
        CloseServiceHandle( scm );
        return result;
    }
    template < UINT Charset >
    inline auto start_service_with_dependencies( const char* const service_name ) noexcept
    {
        const auto w_name{ details::to_wstring< Charset >( service_name ) };
        if ( w_name.empty() ) {
            return static_cast< DWORD >( ERROR_INVALID_PARAMETER );
        }
        const auto scm{ OpenSCManagerW( nullptr, nullptr, SC_MANAGER_CONNECT ) };
        if ( scm == nullptr ) {
            return GetLastError();
        }
        const auto service{ OpenServiceW( scm, w_name.c_str(), SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_QUERY_CONFIG ) };
        DWORD result{ ERROR_SUCCESS };
        if ( service != nullptr ) {
            result = details::start_service_and_dependencies( scm, service );
            CloseServiceHandle( service );
        } else {
            result = GetLastError();
        }
        CloseServiceHandle( scm );
        return result;
    }
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
        return static_cast< bool >( is_admin );
    }
    inline auto relaunch( const int exit_code ) noexcept
    {
        std::array< wchar_t, MAX_PATH > file_path;
        GetModuleFileNameW( nullptr, file_path.data(), MAX_PATH );
        ShellExecuteW( nullptr, L"open", file_path.data(), nullptr, nullptr, SW_SHOWNORMAL );
        std::exit( exit_code );
    }
    inline auto relaunch_as_admin( const int exit_code ) noexcept
    {
        std::array< wchar_t, MAX_PATH > file_path;
        GetModuleFileNameW( nullptr, file_path.data(), MAX_PATH );
        ShellExecuteW( nullptr, L"runas", file_path.data(), nullptr, nullptr, SW_SHOWNORMAL );
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
    template < typename ChronoRep, typename ChronoPeriod >
    inline auto loop_keep_window_top(
      const HWND window_handle, const DWORD thread_id, const DWORD window_thread_process_id,
      const std::chrono::duration< ChronoRep, ChronoPeriod > sleep_time )
    {
        while ( true ) {
            AttachThreadInput( thread_id, window_thread_process_id, TRUE );
            SetForegroundWindow( window_handle );
            SetWindowPos( window_handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
            AttachThreadInput( thread_id, window_thread_process_id, FALSE );
            std::this_thread::sleep_for( sleep_time );
        }
    }
    template < typename ChronoRep, typename ChronoPeriod, typename F, typename... Args >
        requires std::invocable< F, Args... >
    inline auto loop_keep_window_top(
      const HWND window_handle, const DWORD thread_id, const DWORD window_thread_process_id,
      const std::chrono::duration< ChronoRep, ChronoPeriod > sleep_time, F&& condition_checker, Args&&... condition_checker_args )
    {
        while ( condition_checker( std::forward< Args >( condition_checker_args )... ) ) {
            AttachThreadInput( thread_id, window_thread_process_id, TRUE );
            SetForegroundWindow( window_handle );
            SetWindowPos( window_handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
            AttachThreadInput( thread_id, window_thread_process_id, FALSE );
            std::this_thread::sleep_for( sleep_time );
        }
    }
    template < typename ChronoRep, typename ChronoPeriod >
    inline auto loop_keep_current_window_top( const std::chrono::duration< ChronoRep, ChronoPeriod > sleep_time )
    {
        const auto window_handle{ get_current_window_handle() };
        loop_keep_window_top( window_handle, GetCurrentThreadId(), GetWindowThreadProcessId( window_handle, nullptr ), sleep_time );
    }
    template < typename ChronoRep, typename ChronoPeriod, typename F, typename... Args >
        requires std::invocable< F, Args... >
    inline auto loop_keep_current_window_top(
      const std::chrono::duration< ChronoRep, ChronoPeriod > sleep_time, F&& condition_checker, Args&&... condition_checker_args )
    {
        const auto window_handle{ get_current_window_handle() };
        loop_keep_window_top(
          window_handle, GetCurrentThreadId(), GetWindowThreadProcessId( window_handle, nullptr ), sleep_time,
          std::forward< F >( condition_checker ), std::forward< Args >( condition_checker_args )... );
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
        SetConsoleCtrlHandler( nullptr, static_cast< WINBOOL >( is_ignore ) );
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
    inline auto set_current_console_title( const char* const title ) noexcept
    {
        SetConsoleTitleA( title );
    }
    inline auto set_current_console_title( const wchar_t* const title ) noexcept
    {
        SetConsoleTitleW( title );
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
        SetConsoleWindowInfo( std_output_handle, TRUE, &wrt );
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
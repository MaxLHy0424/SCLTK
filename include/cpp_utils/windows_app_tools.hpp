#pragma once
#if defined( _WIN32 ) || defined( _WIN64 )
# include "windows_definitions.hpp"
# if true
#  include <tlhelp32.h>
# endif
#endif
#include <pdh.h>
#include <pdhmsg.h>
#include <psapi.h>
#include <bit>
#include <chrono>
#include <concepts>
#include <expected>
#include <flat_map>
#include <functional>
#include <memory>
#include <memory_resource>
#include <numeric>
#include <print>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>
namespace cpp_utils
{
#if defined( _WIN32 ) || defined( _WIN64 )
    namespace details
    {
        template < UINT Charset >
        inline auto to_wstring( const char* const str, std::pmr::memory_resource* const resource ) noexcept
        {
            using namespace std::string_literals;
            if ( str == nullptr || str[ 0 ] == '\0' ) {
                return std::pmr::wstring{ resource };
            }
            const int size_needed{ MultiByteToWideChar( Charset, 0, str, -1, nullptr, 0 ) };
            if ( size_needed <= 0 ) {
                return std::pmr::wstring{ resource };
            }
            std::pmr::wstring result{ static_cast< std::size_t >( size_needed - 1 ), L'\0', resource };
            if ( !MultiByteToWideChar( Charset, 0, str, -1, result.data(), size_needed ) ) {
                return std::pmr::wstring{ resource };
            }
            return result;
        }
        inline auto stop_service_and_dependencies(
          const SC_HANDLE scm, const SC_HANDLE service, std::pmr::memory_resource* const resource ) noexcept -> DWORD
        {
            using namespace std::chrono_literals;
            DWORD result{ ERROR_SUCCESS };
            SERVICE_STATUS status{};
            DWORD bytes_needed{ 0 };
            if ( !QueryServiceConfigW( service, nullptr, 0, &bytes_needed ) && GetLastError() == ERROR_INSUFFICIENT_BUFFER ) {
                std::pmr::vector< BYTE > buffer( bytes_needed, resource );
                auto config{ std::bit_cast< LPQUERY_SERVICE_CONFIGW >( buffer.data() ) };
                if ( QueryServiceConfigW( service, config, bytes_needed, &bytes_needed ) ) {
                    if ( config->lpDependencies != nullptr && *config->lpDependencies != L'\0' ) {
                        auto dependency = config->lpDependencies;
                        while ( *dependency != L'\0' ) {
                            const auto dependency_service = OpenServiceW( scm, dependency, SERVICE_STOP | SERVICE_QUERY_STATUS );
                            if ( dependency_service != nullptr ) {
                                const auto dep_result = stop_service_and_dependencies( scm, dependency_service, resource );
                                if ( dep_result != ERROR_SUCCESS && result == ERROR_SUCCESS ) {
                                    result = dep_result;
                                }
                                CloseServiceHandle( dependency_service );
                            }
                            dependency += std::wcslen( dependency ) + 1;
                        }
                    }
                }
            }
            if ( ControlService( service, SERVICE_CONTROL_STOP, &status ) ) {
                bool query_success{ true };
                while ( query_success && status.dwCurrentState == SERVICE_STOP_PENDING ) {
                    query_success = QueryServiceStatus( service, &status );
                    std::this_thread::sleep_for( 5ms );
                }
                if ( !query_success || status.dwCurrentState != SERVICE_STOPPED ) {
                    result = ERROR_SERVICE_REQUEST_TIMEOUT;
                }
            } else if ( GetLastError() != ERROR_SERVICE_NOT_ACTIVE ) {
                result = GetLastError();
            }
            return result;
        }
        inline auto start_service_and_dependencies(
          const SC_HANDLE scm, const SC_HANDLE service, std::pmr::memory_resource* const resource ) noexcept -> DWORD
        {
            DWORD result{ ERROR_SUCCESS };
            DWORD bytes_needed{ 0 };
            if ( !QueryServiceConfigW( service, nullptr, 0, &bytes_needed ) && GetLastError() == ERROR_INSUFFICIENT_BUFFER ) {
                std::pmr::vector< BYTE > buffer( bytes_needed, resource );
                auto config{ std::bit_cast< LPQUERY_SERVICE_CONFIGW >( buffer.data() ) };
                if ( QueryServiceConfigW( service, config, bytes_needed, &bytes_needed ) ) {
                    if ( config->lpDependencies != nullptr && *config->lpDependencies != L'\0' ) {
                        auto dependency{ config->lpDependencies };
                        while ( *dependency != L'\0' ) {
                            if ( *dependency != L'@' ) {
                                const auto dependency_service{
                                  OpenServiceW( scm, dependency, SERVICE_START | SERVICE_QUERY_STATUS ) };
                                if ( dependency_service != nullptr ) {
                                    SERVICE_STATUS status{};
                                    if ( !QueryServiceStatus( dependency_service, &status )
                                         || ( status.dwCurrentState != SERVICE_RUNNING && status.dwCurrentState != SERVICE_START_PENDING ) )
                                    {
                                        const auto dep_result{ start_service_and_dependencies( scm, dependency_service, resource ) };
                                        if ( dep_result != ERROR_SUCCESS && result == ERROR_SUCCESS ) {
                                            result = dep_result;
                                        }
                                    }
                                    CloseServiceHandle( dependency_service );
                                }
                            }
                            dependency += std::wcslen( dependency ) + 1;
                        }
                    }
                }
            }
            if ( result == ERROR_SUCCESS && !StartServiceW( service, 0, nullptr ) ) {
                if ( const auto err{ GetLastError() }; err != ERROR_SERVICE_ALREADY_RUNNING ) {
                    result = err;
                }
            }
            return result;
        }
    }
    template < UINT Charset >
    inline auto kill_process_by_name(
      const char* const process_name, std::pmr::memory_resource* const resource = std::pmr::get_default_resource() ) noexcept
    {
        const auto w_name{ details::to_wstring< Charset >( process_name, resource ) };
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
      const DWORD data_size, std::pmr::memory_resource* const resource = std::pmr::get_default_resource() ) noexcept
    {
        using namespace std::string_literals;
        const auto w_sub_key{ details::to_wstring< Charset >( sub_key, resource ) };
        const auto w_value_name{
          value_name ? details::to_wstring< Charset >( value_name, resource ) : std::pmr::wstring{ resource } };
        HKEY key_handle;
        auto result{ RegCreateKeyExW(
          main_key, w_sub_key.c_str(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &key_handle, nullptr ) };
        if ( result != ERROR_SUCCESS ) {
            return static_cast< DWORD >( result );
        }
        result = RegSetValueExW( key_handle, w_value_name.empty() ? nullptr : w_value_name.c_str(), 0, type, data, data_size );
        RegCloseKey( key_handle );
        return static_cast< DWORD >( result );
    }
    template < UINT Charset >
    inline auto delete_registry_key(
      const HKEY main_key, const char* const sub_key, const char* const value_name,
      std::pmr::memory_resource* const resource = std::pmr::get_default_resource() ) noexcept
    {
        using namespace std::string_literals;
        const auto w_sub_key{ details::to_wstring< Charset >( sub_key, resource ) };
        const auto w_value_name{
          value_name ? details::to_wstring< Charset >( value_name, resource ) : std::pmr::wstring{ resource } };
        HKEY key_handle;
        auto result{ RegOpenKeyExW( main_key, w_sub_key.c_str(), 0, KEY_WRITE, &key_handle ) };
        if ( result != ERROR_SUCCESS ) {
            return result;
        }
        result = RegDeleteValueW( key_handle, w_value_name.empty() ? nullptr : w_value_name.c_str() );
        RegCloseKey( key_handle );
        return static_cast< DWORD >( result );
    }
    template < UINT Charset >
    inline auto delete_registry_tree(
      const HKEY main_key, const char* const sub_key,
      std::pmr::memory_resource* const resource = std::pmr::get_default_resource() ) noexcept
    {
        return static_cast< DWORD >( RegDeleteTreeW( main_key, details::to_wstring< Charset >( sub_key, resource ).c_str() ) );
    }
    template < UINT Charset >
    inline auto set_service_status(
      const char* const service_name, const DWORD status_type,
      std::pmr::memory_resource* const resource = std::pmr::get_default_resource() ) noexcept
    {
        const auto w_name{ details::to_wstring< Charset >( service_name, resource ) };
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
    inline auto stop_service_with_dependencies(
      const char* const service_name, std::pmr::memory_resource* const resource = std::pmr::get_default_resource() ) noexcept
    {
        const auto w_name{ details::to_wstring< Charset >( service_name, resource ) };
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
            result = details::stop_service_and_dependencies( scm, service, resource );
            CloseServiceHandle( service );
        } else {
            result = GetLastError();
        }
        CloseServiceHandle( scm );
        return result;
    }
    template < UINT Charset >
    inline auto start_service_with_dependencies(
      const char* const service_name, std::pmr::memory_resource* const resource = std::pmr::get_default_resource() ) noexcept
    {
        const auto w_name{ details::to_wstring< Charset >( service_name, resource ) };
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
            result = details::start_service_and_dependencies( scm, service, resource );
            CloseServiceHandle( service );
        } else {
            result = GetLastError();
        }
        CloseServiceHandle( scm );
        return result;
    }
    template < UINT Charset >
    inline auto reset_service_failure_action(
      const char* const service_name, std::pmr::memory_resource* const resource = std::pmr::get_default_resource() ) noexcept
    {
        if ( service_name == nullptr || service_name[ 0 ] == '\0' ) {
            return static_cast< DWORD >( ERROR_INVALID_PARAMETER );
        }
        auto w_service_name{ details::to_wstring< Charset >( service_name, resource ) };
        if ( w_service_name.empty() ) {
            return static_cast< DWORD >( ERROR_INVALID_PARAMETER );
        }
        using scm_handle = std::unique_ptr< std::remove_pointer_t< SC_HANDLE >, decltype( []( SC_HANDLE h ) static noexcept
        {
            if ( h != nullptr ) {
                CloseServiceHandle( h );
            }
        } ) >;
        scm_handle scm{ OpenSCManagerW( nullptr, nullptr, SC_MANAGER_CONNECT ) };
        if ( !scm ) {
            return GetLastError();
        }
        scm_handle service{ OpenServiceW( scm.get(), w_service_name.c_str(), SERVICE_CHANGE_CONFIG ) };
        if ( !service ) {
            return GetLastError();
        }
        constexpr DWORD actions_count{ 3 };
        constexpr SC_ACTION non_action{ .Type{ SC_ACTION_NONE }, .Delay{ 0 } };
        SC_ACTION actions[ actions_count ]{ non_action, non_action, non_action };
        SERVICE_FAILURE_ACTIONSW service_fail_actions{
          .dwResetPeriod{ 0 }, .lpRebootMsg{ nullptr }, .lpCommand{ nullptr }, .cActions{ actions_count }, .lpsaActions{ actions } };
        if ( !ChangeServiceConfig2W( service.get(), SERVICE_CONFIG_FAILURE_ACTIONS, &service_fail_actions ) ) {
            return GetLastError();
        }
        return static_cast< DWORD >( ERROR_SUCCESS );
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
    [[noreturn]] inline auto relaunch( const int exit_code ) noexcept
    {
        std::array< wchar_t, MAX_PATH > file_path;
        GetModuleFileNameW( nullptr, file_path.data(), MAX_PATH );
        ShellExecuteW( nullptr, L"open", file_path.data(), nullptr, nullptr, SW_SHOWNORMAL );
        std::exit( exit_code );
    }
    [[noreturn]] inline auto relaunch_as_admin( const int exit_code ) noexcept
    {
        std::array< wchar_t, MAX_PATH > file_path;
        GetModuleFileNameW( nullptr, file_path.data(), MAX_PATH );
        ShellExecuteW( nullptr, L"runas", file_path.data(), nullptr, nullptr, SW_SHOWNORMAL );
        std::exit( exit_code );
    }
    namespace details
    {
        template < typename D >
        class basic_window
        {
          public:
            HWND window_handle;
            auto get_state() const noexcept
            {
                WINDOWPLACEMENT wp;
                wp.length = sizeof( WINDOWPLACEMENT );
                GetWindowPlacement( window_handle, &wp );
                return wp.showCmd;
            }
            auto& set_state( const UINT state ) const noexcept
            {
                ShowWindow( window_handle, state );
                return static_cast< const D& >( *this );
            }
            auto& force_show() const noexcept
            {
                const auto thread_id{ GetCurrentThreadId() };
                const auto window_thread_process_id{ GetWindowThreadProcessId( window_handle, nullptr ) };
                AttachThreadInput( thread_id, window_thread_process_id, TRUE );
                SetForegroundWindow( window_handle );
                SetWindowPos( window_handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
                AttachThreadInput( thread_id, window_thread_process_id, FALSE );
                return static_cast< const D& >( *this );
            }
            template < typename ChronoRep, typename ChronoPeriod >
            [[noreturn]] auto force_show_forever( const std::chrono::duration< ChronoRep, ChronoPeriod > sleep_time ) const noexcept
            {
                const auto thread_id{ GetCurrentThreadId() };
                const auto window_thread_process_id{ GetWindowThreadProcessId( window_handle, nullptr ) };
                while ( true ) {
                    AttachThreadInput( thread_id, window_thread_process_id, TRUE );
                    SetForegroundWindow( window_handle );
                    SetWindowPos( window_handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
                    AttachThreadInput( thread_id, window_thread_process_id, FALSE );
                    std::this_thread::sleep_for( sleep_time );
                }
            }
            template < typename ChronoRep, typename ChronoPeriod, std::invocable F >
            auto& force_show_until( const std::chrono::duration< ChronoRep, ChronoPeriod > sleep_time, F&& condition_checker ) const noexcept
            {
                const auto thread_id{ GetCurrentThreadId() };
                const auto window_thread_process_id{ GetWindowThreadProcessId( window_handle, nullptr ) };
                while ( !std::forward< F >( condition_checker )() ) {
                    AttachThreadInput( thread_id, window_thread_process_id, TRUE );
                    SetForegroundWindow( window_handle );
                    SetWindowPos( window_handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
                    AttachThreadInput( thread_id, window_thread_process_id, FALSE );
                    std::this_thread::sleep_for( sleep_time );
                }
                return static_cast< const D& >( *this );
            }
            auto& cancel_force_show() const noexcept
            {
                SetWindowPos( window_handle, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
                return static_cast< const D& >( *this );
            }
            auto& fix_size( const bool is_enable ) const noexcept
            {
                SetWindowLongPtrW(
                  window_handle, GWL_STYLE,
                  is_enable
                    ? GetWindowLongPtrW( window_handle, GWL_STYLE ) & ~WS_SIZEBOX
                    : GetWindowLongPtrW( window_handle, GWL_STYLE ) | WS_SIZEBOX );
                return static_cast< const D& >( *this );
            }
            auto& enable_context_menu( const bool is_enable ) const noexcept
            {
                SetWindowLongPtrW(
                  window_handle, GWL_STYLE,
                  is_enable
                    ? GetWindowLongPtrW( window_handle, GWL_STYLE ) | WS_SYSMENU
                    : GetWindowLongPtrW( window_handle, GWL_STYLE ) & ~WS_SYSMENU );
                return static_cast< const D& >( *this );
            }
            auto& enable_window_minimize_ctrl( const bool is_enable ) const noexcept
            {
                SetWindowLongPtrW(
                  window_handle, GWL_STYLE,
                  is_enable
                    ? GetWindowLongPtrW( window_handle, GWL_STYLE ) | WS_MINIMIZEBOX
                    : GetWindowLongPtrW( window_handle, GWL_STYLE ) & ~WS_MINIMIZEBOX );
                return static_cast< const D& >( *this );
            }
            auto& enable_window_maximize_ctrl( const bool is_enable ) const noexcept
            {
                SetWindowLongPtrW(
                  window_handle, GWL_STYLE,
                  is_enable
                    ? GetWindowLongPtrW( window_handle, GWL_STYLE ) | WS_MAXIMIZEBOX
                    : GetWindowLongPtrW( window_handle, GWL_STYLE ) & ~WS_MAXIMIZEBOX );
                return static_cast< const D& >( *this );
            }
            auto& enable_window_close_ctrl( const bool is_enable ) const noexcept
            {
                EnableMenuItem(
                  GetSystemMenu( window_handle, FALSE ), SC_CLOSE,
                  is_enable ? MF_BYCOMMAND | MF_ENABLED : MF_BYCOMMAND | MF_DISABLED | MF_GRAYED );
                return static_cast< const D& >( *this );
            }
        };
    }
    class window final : public details::basic_window< window >
    {
        auto operator=( const window& ) noexcept -> window& = default;
        window() noexcept
          : details::basic_window< window >{ .window_handle{ GetForegroundWindow() } }
        { }
        window( const window& ) noexcept = default;
        ~window() noexcept               = default;
    };
    class console final : public details::basic_window< console >
    {
      public:
        HANDLE std_input_handle;
        HANDLE std_output_handle;
        auto& press_any_key_to_continue() const noexcept
        {
            DWORD mode;
            GetConsoleMode( std_input_handle, &mode );
            SetConsoleMode( std_input_handle, ENABLE_EXTENDED_FLAGS | ( mode & ~ENABLE_QUICK_EDIT_MODE ) );
            FlushConsoleInputBuffer( std_input_handle );
            INPUT_RECORD record;
            DWORD events;
            do {
                ReadConsoleInputW( std_input_handle, &record, 1, &events );
            } while ( record.EventType != KEY_EVENT || !record.Event.KeyEvent.bKeyDown );
            SetConsoleMode( std_input_handle, mode );
            return *this;
        }
        auto& ignore_exit_signal( const bool is_ignore ) const noexcept
        {
            SetConsoleCtrlHandler( nullptr, static_cast< WINBOOL >( is_ignore ) );
            return *this;
        }
        auto& enable_virtual_terminal_processing( const bool is_enable ) const noexcept
        {
            DWORD mode;
            GetConsoleMode( std_output_handle, &mode );
            is_enable ? mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING : mode &= ~ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode( std_output_handle, mode );
            return *this;
        }
        auto& clear( std::pmr::memory_resource* const resource = std::pmr::get_default_resource() ) const
        {
            CONSOLE_SCREEN_BUFFER_INFO info;
            GetConsoleScreenBufferInfo( std_output_handle, &info );
            constexpr COORD top_left{ 0, 0 };
            const auto area{ info.dwSize.X * info.dwSize.Y };
            DWORD written;
            SetConsoleCursorPosition( std_output_handle, top_left );
            std::print( "{}", std::pmr::string{ static_cast< std::size_t >( area ), ' ', resource } );
            FillConsoleOutputAttribute( std_output_handle, info.wAttributes, area, top_left, &written );
            SetConsoleCursorPosition( std_output_handle, top_left );
            return *this;
        }
        auto get_size() const noexcept
        {
            CONSOLE_SCREEN_BUFFER_INFO info;
            GetConsoleScreenBufferInfo( std_output_handle, &info );
            return info.dwSize;
        }
        auto& set_size(
          const SHORT width, const SHORT height, std::pmr::memory_resource* const resource = std::pmr::get_default_resource() ) const
        {
            SMALL_RECT wrt{ 0, 0, static_cast< SHORT >( width - 1 ), static_cast< SHORT >( height - 1 ) };
            ShowWindow( window_handle, SW_SHOWNORMAL );
            SetConsoleScreenBufferSize( std_output_handle, { width, height } );
            SetConsoleWindowInfo( std_output_handle, TRUE, &wrt );
            SetConsoleScreenBufferSize( std_output_handle, { width, height } );
            SetConsoleWindowInfo( std_output_handle, TRUE, &wrt );
            clear( resource );
            return *this;
        }
        auto& set_title( const char* const title ) const noexcept
        {
            SetConsoleTitleA( title );
            return *this;
        }
        auto& set_title( const wchar_t* const title ) const noexcept
        {
            SetConsoleTitleW( title );
            return *this;
        }
        auto& set_charset( const UINT charset_id ) const noexcept
        {
            SetConsoleOutputCP( charset_id );
            SetConsoleCP( charset_id );
            return *this;
        }
        auto& set_translucency( const BYTE value ) const noexcept
        {
            SetLayeredWindowAttributes( window_handle, RGB( 0, 0, 0 ), value, LWA_ALPHA );
            return *this;
        }
        auto operator=( const console& ) noexcept -> console& = default;
        console() noexcept
          : details::basic_window< console >{ .window_handle{ GetConsoleWindow() } }
          , std_input_handle{ GetStdHandle( STD_INPUT_HANDLE ) }
          , std_output_handle{ GetStdHandle( STD_OUTPUT_HANDLE ) }
        { }
        console( const console& ) noexcept = default;
        ~console() noexcept                = default;
    };
#else
# error "must be compiled on the windows os"
#endif
}
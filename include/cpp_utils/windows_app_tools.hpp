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
#include <ranges>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>
namespace cpp_utils
{
#if defined( _WIN32 ) || defined( _WIN64 )
    [[nodiscard]] inline auto to_string(
      const std::wstring_view str, const UINT charset,
      std::pmr::memory_resource* const resource = std::pmr::get_default_resource() ) noexcept
    {
        if ( str.empty() ) [[unlikely]] {
            return std::pmr::string{ resource };
        }
        const auto str_len{ [ & ] noexcept
        {
            if ( str.size() > static_cast< size_t >( INT_MAX ) ) [[unlikely]] {
                return 0;
            }
            return static_cast< int >( str.size() );
        }() };
        if ( str_len == 0 ) [[unlikely]] {
            return std::pmr::string{ resource };
        }
        DWORD flags{ 0 };
        if ( charset == CP_UTF8 ) {
            flags = WC_ERR_INVALID_CHARS;
        }
        const auto size_needed{ WideCharToMultiByte( charset, flags, str.data(), str_len, nullptr, 0, nullptr, nullptr ) };
        if ( size_needed == 0 ) [[unlikely]] {
            return std::pmr::string{ resource };
        }
        std::pmr::string result{ static_cast< std::size_t >( size_needed ), '\0', resource };
        const auto converted{
          WideCharToMultiByte( charset, flags, str.data(), str_len, result.data(), size_needed, nullptr, nullptr ) };
        if ( converted == 0 || converted != size_needed ) [[unlikely]] {
            return std::pmr::string{ resource };
        }
        return result;
    }
    [[nodiscard]] inline auto to_wstring(
      const std::string_view str, const UINT charset,
      std::pmr::memory_resource* const resource = std::pmr::get_default_resource() ) noexcept
    {
        if ( str.empty() ) [[unlikely]] {
            return std::pmr::wstring{ resource };
        }
        const auto str_len{ [ & ] noexcept
        {
            if ( str.size() > static_cast< size_t >( INT_MAX ) ) [[unlikely]] {
                return 0;
            }
            return static_cast< int >( str.size() );
        }() };
        if ( str_len == 0 ) [[unlikely]] {
            return std::pmr::wstring{ resource };
        }
        DWORD flags{ 0 };
        if ( charset == CP_UTF8 ) {
            flags = MB_ERR_INVALID_CHARS;
        }
        const auto size_needed{ MultiByteToWideChar( charset, flags, str.data(), str_len, nullptr, 0 ) };
        if ( size_needed <= 0 ) [[unlikely]] {
            return std::pmr::wstring{ resource };
        }
        std::pmr::wstring result{ static_cast< std::size_t >( size_needed ), L'\0', resource };
        if ( !MultiByteToWideChar( charset, flags, str.data(), str_len, result.data(), size_needed ) ) [[unlikely]] {
            return std::pmr::wstring{ resource };
        }
        return result;
    }
    namespace details_
    {
        struct scoped_handle final
        {
            HANDLE value{ nullptr };
            [[nodiscard]] auto get() const noexcept
            {
                return value;
            }
            auto reset( const HANDLE h ) noexcept
            {
                if ( value != nullptr && value != INVALID_HANDLE_VALUE ) [[likely]] {
                    CloseHandle( value );
                }
                value = h;
            }
            [[nodiscard]] auto put() noexcept
            {
                reset( nullptr );
                return &value;
            }
            scoped_handle() noexcept = default;
            scoped_handle( const HANDLE h ) noexcept
              : value{ h }
            { }
            scoped_handle( const scoped_handle& )                    = delete;
            auto operator=( const scoped_handle& ) -> scoped_handle& = delete;
            scoped_handle( scoped_handle&& other ) noexcept
              : value{ other.value }
            {
                other.value = nullptr;
            }
            auto operator=( scoped_handle&& other ) noexcept -> scoped_handle&
            {
                if ( this != &other ) [[likely]] {
                    reset( other.value );
                    other.value = nullptr;
                }
                return *this;
            }
            ~scoped_handle() noexcept
            {
                if ( value != nullptr && value != INVALID_HANDLE_VALUE ) [[likely]] {
                    CloseHandle( value );
                }
            }
        };
        struct scoped_sc_handle final
        {
            SC_HANDLE value{ nullptr };
            [[nodiscard]] auto get() const noexcept
            {
                return value;
            }
            void reset( const SC_HANDLE h ) noexcept
            {
                if ( value != nullptr ) [[likely]] {
                    CloseServiceHandle( value );
                }
                value = h;
            }
            [[nodiscard]] auto put() noexcept
            {
                reset( nullptr );
                return &value;
            }
            scoped_sc_handle() noexcept = default;
            scoped_sc_handle( const SC_HANDLE h ) noexcept
              : value{ h }
            { }
            scoped_sc_handle( const scoped_sc_handle& )                    = delete;
            auto operator=( const scoped_sc_handle& ) -> scoped_sc_handle& = delete;
            scoped_sc_handle( scoped_sc_handle&& other ) noexcept
              : value{ other.value }
            {
                other.value = nullptr;
            }
            auto operator=( scoped_sc_handle&& other ) noexcept -> scoped_sc_handle&
            {
                if ( this != &other ) [[likely]] {
                    reset( other.value );
                    other.value = nullptr;
                }
                return *this;
            }
            ~scoped_sc_handle() noexcept
            {
                if ( value != nullptr ) [[likely]] {
                    CloseServiceHandle( value );
                }
            }
        };
        struct scoped_hkey final
        {
            HKEY value{ nullptr };
            [[nodiscard]] auto get() const noexcept
            {
                return value;
            }
            auto reset( const HKEY h ) noexcept
            {
                if ( value != nullptr ) [[likely]] {
                    RegCloseKey( value );
                }
                value = h;
            }
            [[nodiscard]] auto put() noexcept
            {
                reset( nullptr );
                return &value;
            }
            scoped_hkey() noexcept = default;
            scoped_hkey( const HKEY h ) noexcept
              : value{ h }
            { }
            scoped_hkey( const scoped_hkey& )                    = delete;
            auto operator=( const scoped_hkey& ) -> scoped_hkey& = delete;
            scoped_hkey( scoped_hkey&& other ) noexcept
              : value{ other.value }
            {
                other.value = nullptr;
            }
            auto operator=( scoped_hkey&& other ) noexcept -> scoped_hkey&
            {
                if ( this != &other ) [[likely]] {
                    reset( other.value );
                    other.value = nullptr;
                }
                return *this;
            }
            ~scoped_hkey() noexcept
            {
                if ( value != nullptr ) [[likely]] {
                    RegCloseKey( value );
                }
            }
        };
        struct scoped_psid final
        {
            PSID value{ nullptr };
            [[nodiscard]] auto get() const noexcept
            {
                return value;
            }
            auto reset( const PSID p ) noexcept
            {
                if ( value != nullptr ) {
                    FreeSid( value );
                }
                value = p;
            }
            [[nodiscard]] auto put() noexcept
            {
                reset( nullptr );
                return &value;
            }
            scoped_psid() noexcept = default;
            scoped_psid( const PSID p ) noexcept
              : value{ p }
            { }
            scoped_psid( const scoped_psid& )                    = delete;
            auto operator=( const scoped_psid& ) -> scoped_psid& = delete;
            scoped_psid( scoped_psid&& other ) noexcept
              : value{ other.value }
            {
                other.value = nullptr;
            }
            auto operator=( scoped_psid&& other ) noexcept -> scoped_psid&
            {
                if ( this != &other ) [[likely]] {
                    reset( other.value );
                    other.value = nullptr;
                }
                return *this;
            }
            ~scoped_psid() noexcept
            {
                if ( value != nullptr ) [[likely]] {
                    FreeSid( value );
                }
            }
        };
        [[nodiscard]] inline auto stop_service_and_dependencies(
          const SC_HANDLE scm, const SC_HANDLE service, std::pmr::memory_resource* const resource ) noexcept -> DWORD
        {
            using namespace std::chrono_literals;
            DWORD result{ ERROR_SUCCESS };
            SERVICE_STATUS status{};
            DWORD bytes_needed{ 0 };
            if ( !QueryServiceConfigW( service, nullptr, 0, &bytes_needed ) && GetLastError() == ERROR_INSUFFICIENT_BUFFER )
              [[likely]]
            {
                std::pmr::vector< BYTE > buffer( bytes_needed, resource );
                const auto config{ reinterpret_cast< LPQUERY_SERVICE_CONFIGW >( buffer.data() ) };
                if ( QueryServiceConfigW( service, config, bytes_needed, &bytes_needed ) && config->lpDependencies != nullptr
                     && *config->lpDependencies != L'\0' ) [[likely]]
                {
                    auto dependency{ config->lpDependencies };
                    while ( *dependency != L'\0' ) [[likely]] {
                        scoped_sc_handle dependency_service{ OpenServiceW( scm, dependency, SERVICE_STOP | SERVICE_QUERY_STATUS ) };
                        if ( dependency_service.get() != nullptr ) [[likely]] {
                            const auto dep_result{ stop_service_and_dependencies( scm, dependency_service.get(), resource ) };
                            if ( dep_result != ERROR_SUCCESS && result == ERROR_SUCCESS ) [[unlikely]] {
                                result = dep_result;
                            }
                        }
                        dependency += std::wcslen( dependency ) + 1;
                    }
                }
            }
            if ( ControlService( service, SERVICE_CONTROL_STOP, &status ) ) [[likely]] {
                bool query_success{ true };
                while ( query_success && status.dwCurrentState == SERVICE_STOP_PENDING ) {
                    query_success = QueryServiceStatus( service, &status );
                    std::this_thread::sleep_for( 5ms );
                }
                if ( !query_success || status.dwCurrentState != SERVICE_STOPPED ) [[unlikely]] {
                    result = ERROR_SERVICE_REQUEST_TIMEOUT;
                }
            } else if ( const auto err{ GetLastError() }; err != ERROR_SERVICE_NOT_ACTIVE ) [[unlikely]] {
                result = err;
            }
            return result;
        }
        [[nodiscard]] inline auto start_service_and_dependencies(
          const SC_HANDLE scm, const SC_HANDLE service, std::pmr::memory_resource* const resource ) noexcept -> DWORD
        {
            DWORD result{ ERROR_SUCCESS };
            DWORD bytes_needed{ 0 };
            if ( !QueryServiceConfigW( service, nullptr, 0, &bytes_needed ) && GetLastError() == ERROR_INSUFFICIENT_BUFFER )
              [[likely]]
            {
                std::pmr::vector< BYTE > buffer( bytes_needed, resource );
                const auto config{ reinterpret_cast< LPQUERY_SERVICE_CONFIGW >( buffer.data() ) };
                if ( QueryServiceConfigW( service, config, bytes_needed, &bytes_needed ) && config->lpDependencies != nullptr
                     && *config->lpDependencies != L'\0' ) [[likely]]
                {
                    auto dependency{ config->lpDependencies };
                    while ( *dependency != L'\0' ) [[likely]] {
                        if ( *dependency != L'@' ) [[likely]] {
                            scoped_sc_handle dependency_service{
                              OpenServiceW( scm, dependency, SERVICE_START | SERVICE_QUERY_STATUS ) };
                            if ( dependency_service.get() != nullptr ) [[likely]] {
                                SERVICE_STATUS status{};
                                if ( !QueryServiceStatus( dependency_service.get(), &status )
                                     || ( status.dwCurrentState != SERVICE_RUNNING && status.dwCurrentState != SERVICE_START_PENDING ) )
                                  [[likely]]
                                {
                                    const auto dep_result{
                                      start_service_and_dependencies( scm, dependency_service.get(), resource ) };
                                    if ( dep_result != ERROR_SUCCESS && result == ERROR_SUCCESS ) [[unlikely]] {
                                        result = dep_result;
                                    }
                                }
                            }
                        }
                        dependency += std::wcslen( dependency ) + 1;
                    }
                }
            }
            if ( result == ERROR_SUCCESS && !StartServiceW( service, 0, nullptr ) ) [[likely]] {
                if ( const auto err{ GetLastError() }; err != ERROR_SERVICE_ALREADY_RUNNING ) [[unlikely]] {
                    result = err;
                }
            }
            return result;
        }
    }
    [[nodiscard]] inline auto set_privilege( const HANDLE proc, const wchar_t* const privilege, const bool is_enabled ) noexcept
    {
        details_::scoped_handle token;
        TOKEN_PRIVILEGES tp{};
        tp.PrivilegeCount = 0;
        LUID local_uid;
        if ( !OpenProcessToken( proc, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, token.put() ) ) [[unlikely]] {
            return GetLastError();
        }
        if ( !LookupPrivilegeValueW( nullptr, privilege, &local_uid ) ) [[unlikely]] {
            return GetLastError();
        }
        tp.PrivilegeCount             = 1;
        tp.Privileges[ 0 ].Luid       = local_uid;
        tp.Privileges[ 0 ].Attributes = is_enabled ? SE_PRIVILEGE_ENABLED : 0;
        if ( !AdjustTokenPrivileges( token.get(), FALSE, &tp, sizeof( TOKEN_PRIVILEGES ), nullptr, nullptr ) ) [[unlikely]] {
            return GetLastError();
        }
        return static_cast< DWORD >( ERROR_SUCCESS );
    }
    [[nodiscard]] inline auto terminate_process_by_pid( const DWORD pid ) noexcept
    {
        details_::scoped_handle proc_handle{ OpenProcess( PROCESS_TERMINATE, FALSE, pid ) };
        if ( proc_handle.get() == nullptr ) [[unlikely]] {
            return static_cast< LONG >( ERROR_ACCESS_DENIED );
        }
        if ( !TerminateProcess( proc_handle.get(), 1 ) ) [[unlikely]] {
            return static_cast< LONG >( GetLastError() );
        }
        return static_cast< LONG >( ERROR_SUCCESS );
    }
    [[nodiscard]] inline auto terminate_process_by_name( const std::wstring_view proc_name ) noexcept
    {
        details_::scoped_handle proc_snapshot{ CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 ) };
        if ( proc_snapshot.get() == INVALID_HANDLE_VALUE ) [[unlikely]] {
            return static_cast< LONG >( ERROR_NOT_FOUND );
        }
        PROCESSENTRY32W proc_entry{};
        proc_entry.dwSize = sizeof( PROCESSENTRY32W );
        bool is_found{ false };
        LONG status{ ERROR_NOT_FOUND };
        if ( Process32FirstW( proc_snapshot.get(), &proc_entry ) ) [[likely]] {
            do {
                if ( _wcsicmp( proc_entry.szExeFile, proc_name.data() ) == 0 ) {
                    is_found = true;
                    if ( terminate_process_by_pid( proc_entry.th32ProcessID ) == ERROR_SUCCESS ) [[likely]] {
                        status = ERROR_SUCCESS;
                    }
                }
            } while ( Process32NextW( proc_snapshot.get(), &proc_entry ) );
        }
        return is_found ? ( status == ERROR_SUCCESS ? ERROR_SUCCESS : ERROR_ACCESS_DENIED ) : ERROR_NOT_FOUND;
    }
    template < typename Range >
        requires requires( const Range& range ) {
            { *range.begin() } -> std::convertible_to< std::wstring_view >;
            range.begin() != range.end();
            range.empty();
        }
    [[nodiscard]] inline auto terminate_process_by_names( const Range& proc_names ) noexcept
    {
        if ( proc_names.empty() ) [[unlikely]] {
            return static_cast< LONG >( ERROR_SUCCESS );
        }
        details_::scoped_handle proc_snapshot{ CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 ) };
        if ( proc_snapshot.get() == INVALID_HANDLE_VALUE ) [[unlikely]] {
            return static_cast< LONG >( ERROR_NOT_FOUND );
        }
        PROCESSENTRY32W proc_entry{};
        proc_entry.dwSize = sizeof( PROCESSENTRY32W );
        LONG status{ ERROR_NOT_FOUND };
        if ( Process32FirstW( proc_snapshot.get(), &proc_entry ) ) [[likely]] {
            do {
                for ( const auto& proc_name : proc_names ) {
                    if ( _wcsicmp( proc_entry.szExeFile, proc_name.data() ) == 0 ) {
                        if ( terminate_process_by_pid( proc_entry.th32ProcessID ) == ERROR_SUCCESS ) [[likely]] {
                            status = ERROR_SUCCESS;
                        }
                        break;
                    }
                }
            } while ( Process32NextW( proc_snapshot.get(), &proc_entry ) );
        }
        return status == ERROR_SUCCESS ? ERROR_SUCCESS : ERROR_ACCESS_DENIED;
    }
    [[nodiscard]] inline auto create_registry_key(
      const HKEY main_key, const std::wstring_view sub_key, const std::wstring_view value_name, const DWORD type,
      const BYTE* const data, const DWORD data_size ) noexcept
    {
        details_::scoped_hkey key_handle;
        auto result{ RegCreateKeyExW(
          main_key, sub_key.data(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, key_handle.put(), nullptr ) };
        if ( result != ERROR_SUCCESS ) [[unlikely]] {
            return result;
        }
        return RegSetValueExW( key_handle.get(), value_name.data(), 0, type, data, data_size );
    }
    [[nodiscard]] inline auto create_registry_key_without_redirect(
      const HKEY main_key, const std::wstring_view sub_key, const std::wstring_view value_name, const DWORD type,
      const BYTE* const data, const DWORD data_size ) noexcept
    {
        details_::scoped_hkey key_handle;
        auto result{ RegCreateKeyExW(
          main_key, sub_key.data(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_WOW64_64KEY, nullptr, key_handle.put(),
          nullptr ) };
        if ( result != ERROR_SUCCESS ) [[unlikely]] {
            return result;
        }
        return RegSetValueExW( key_handle.get(), value_name.data(), 0, type, data, data_size );
    }
    [[nodiscard]] inline auto
      delete_registry_key( const HKEY main_key, const std::wstring_view sub_key, const std::wstring_view value_name ) noexcept
    {
        details_::scoped_hkey key_handle;
        auto result{ RegOpenKeyExW( main_key, sub_key.data(), 0, KEY_SET_VALUE, key_handle.put() ) };
        if ( result != ERROR_SUCCESS ) [[unlikely]] {
            return result;
        }
        return RegDeleteValueW( key_handle.get(), value_name.data() );
    }
    [[nodiscard]] inline auto delete_registry_key_without_redirect(
      const HKEY main_key, const std::wstring_view sub_key, const std::wstring_view value_name ) noexcept
    {
        details_::scoped_hkey key_handle;
        auto result{ RegOpenKeyExW( main_key, sub_key.data(), 0, KEY_SET_VALUE | KEY_WOW64_64KEY, key_handle.put() ) };
        if ( result != ERROR_SUCCESS ) [[unlikely]] {
            return result;
        }
        return RegDeleteValueW( key_handle.get(), value_name.data() );
    }
    [[nodiscard]] inline auto delete_registry_tree( const HKEY main_key, const std::wstring_view sub_key ) noexcept
    {
        return RegDeleteTreeW( main_key, sub_key.data() );
    }
    [[nodiscard]] inline auto delete_registry_tree_without_redirect( const HKEY main_key, const std::wstring_view sub_key ) noexcept
    {
        return RegDeleteKeyExW( main_key, sub_key.data(), KEY_WOW64_64KEY, 0 );
    }
    [[nodiscard]] inline auto set_service_start_type( const std::wstring_view service_name, const DWORD start_type ) noexcept
    {
        details_::scoped_sc_handle scm{ OpenSCManagerW( nullptr, nullptr, SC_MANAGER_CONNECT ) };
        if ( scm.get() == nullptr ) [[unlikely]] {
            return GetLastError();
        }
        details_::scoped_sc_handle service{ OpenServiceW( scm.get(), service_name.data(), SERVICE_CHANGE_CONFIG ) };
        DWORD result{ ERROR_SUCCESS };
        if ( service.get() != nullptr ) [[likely]] {
            if ( !ChangeServiceConfigW(
                   service.get(), SERVICE_NO_CHANGE, start_type, SERVICE_NO_CHANGE, nullptr, nullptr, nullptr, nullptr, nullptr,
                   nullptr, nullptr ) ) [[unlikely]]
            {
                result = GetLastError();
            }
        } else {
            result = GetLastError();
        }
        return result;
    }
    [[nodiscard]] inline auto stop_service_with_dependencies(
      const std::wstring_view service_name, std::pmr::memory_resource* const resource = std::pmr::get_default_resource() ) noexcept
    {
        details_::scoped_sc_handle scm{ OpenSCManagerW( nullptr, nullptr, SC_MANAGER_CONNECT | SC_MANAGER_ENUMERATE_SERVICE ) };
        if ( scm.get() == nullptr ) [[unlikely]] {
            return GetLastError();
        }
        details_::scoped_sc_handle service{
          OpenServiceW( scm.get(), service_name.data(), SERVICE_STOP | SERVICE_QUERY_STATUS | SERVICE_ENUMERATE_DEPENDENTS ) };
        DWORD result{ ERROR_SUCCESS };
        if ( service.get() != nullptr ) [[likely]] {
            result = details_::stop_service_and_dependencies( scm.get(), service.get(), resource );
        } else {
            result = GetLastError();
        }
        return result;
    }
    [[nodiscard]] inline auto start_service_with_dependencies(
      const std::wstring_view service_name, std::pmr::memory_resource* const resource = std::pmr::get_default_resource() ) noexcept
    {
        details_::scoped_sc_handle scm{ OpenSCManagerW( nullptr, nullptr, SC_MANAGER_CONNECT ) };
        if ( scm.get() == nullptr ) [[unlikely]] {
            return GetLastError();
        }
        details_::scoped_sc_handle service{
          OpenServiceW( scm.get(), service_name.data(), SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_QUERY_CONFIG ) };
        DWORD result{ ERROR_SUCCESS };
        if ( service.get() != nullptr ) [[likely]] {
            result = details_::start_service_and_dependencies( scm.get(), service.get(), resource );
        } else {
            result = GetLastError();
        }
        return result;
    }
    [[nodiscard]] inline auto is_run_as_admin() noexcept
    {
        BOOL is_admin{ FALSE };
        details_::scoped_psid admins_group;
        SID_IDENTIFIER_AUTHORITY nt_authority{ SECURITY_NT_AUTHORITY };
        if ( AllocateAndInitializeSid(
               &nt_authority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, admins_group.put() )
             == TRUE ) [[likely]]
        {
            CheckTokenMembership( nullptr, admins_group.get(), &is_admin );
        }
        return static_cast< bool >( is_admin );
    }
    inline auto clone_self() noexcept
    {
        std::array< wchar_t, MAX_PATH > file_path;
        GetModuleFileNameW( nullptr, file_path.data(), MAX_PATH );
        ShellExecuteW( nullptr, L"open", file_path.data(), nullptr, nullptr, SW_SHOWNORMAL );
    }
    inline auto clone_self_as_admin() noexcept
    {
        std::array< wchar_t, MAX_PATH > file_path;
        GetModuleFileNameW( nullptr, file_path.data(), MAX_PATH );
        ShellExecuteW( nullptr, L"runas", file_path.data(), nullptr, nullptr, SW_SHOWNORMAL );
    }
    class console final
    {
      public:
        HWND window_handle{ GetConsoleWindow() };
        HANDLE std_input_handle{ GetStdHandle( STD_INPUT_HANDLE ) };
        HANDLE std_output_handle{ GetStdHandle( STD_OUTPUT_HANDLE ) };
        HANDLE std_error_handle{ GetStdHandle( STD_ERROR_HANDLE ) };
        [[nodiscard]] auto get_state() noexcept
        {
            WINDOWPLACEMENT wp;
            wp.length = sizeof( WINDOWPLACEMENT );
            GetWindowPlacement( window_handle, &wp );
            return wp.showCmd;
        }
        auto&& set_state( this auto&& self, const UINT state ) noexcept
        {
            ShowWindow( self.window_handle, state );
            return self;
        }
        auto&& forced_show( this auto&& self ) noexcept
        {
            const auto thread_id{ GetCurrentThreadId() };
            const auto window_thread_proc_id{ GetWindowThreadProcessId( self.window_handle, nullptr ) };
            AttachThreadInput( thread_id, window_thread_proc_id, TRUE );
            SetWindowPos( self.window_handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
            SetForegroundWindow( self.window_handle );
            AttachThreadInput( thread_id, window_thread_proc_id, FALSE );
            return self;
        }
        template < typename ChronoRep, typename ChronoPeriod >
        [[noreturn]] auto
          forced_show_forever( this auto&& self, const std::chrono::duration< ChronoRep, ChronoPeriod > sleep_duration ) noexcept
        {
            const auto thread_id{ GetCurrentThreadId() };
            const auto window_thread_proc_id{ GetWindowThreadProcessId( self.window_handle, nullptr ) };
            for ( ;; ) {
                AttachThreadInput( thread_id, window_thread_proc_id, TRUE );
                SetWindowPos( self.window_handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
                SetForegroundWindow( self.window_handle );
                AttachThreadInput( thread_id, window_thread_proc_id, FALSE );
                std::this_thread::sleep_for( sleep_duration );
            }
        }
        template < typename ChronoRep, typename ChronoPeriod, std::invocable F >
        auto&& forced_show_until(
          this auto&& self, const std::chrono::duration< ChronoRep, ChronoPeriod > sleep_duration, F&& condition_checker ) noexcept
        {
            const auto thread_id{ GetCurrentThreadId() };
            const auto window_thread_proc_id{ GetWindowThreadProcessId( self.window_handle, nullptr ) };
            while ( !std::forward< F >( condition_checker )() ) {
                AttachThreadInput( thread_id, window_thread_proc_id, TRUE );
                SetWindowPos( self.window_handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
                SetForegroundWindow( self.window_handle );
                AttachThreadInput( thread_id, window_thread_proc_id, FALSE );
                std::this_thread::sleep_for( sleep_duration );
            }
            return self;
        }
        auto&& cancel_forced_show( this auto&& self ) noexcept
        {
            SetWindowPos( self.window_handle, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
            return self;
        }
        auto&& fix_size( this auto&& self, const bool is_enable ) noexcept
        {
            SetWindowLongPtrW(
              self.window_handle, GWL_STYLE,
              is_enable
                ? GetWindowLongPtrW( self.window_handle, GWL_STYLE ) & ~WS_SIZEBOX
                : GetWindowLongPtrW( self.window_handle, GWL_STYLE ) | WS_SIZEBOX );
            return self;
        }
        auto&& enable_context_menu( this auto&& self, const bool is_enable ) noexcept
        {
            SetWindowLongPtrW(
              self.window_handle, GWL_STYLE,
              is_enable
                ? GetWindowLongPtrW( self.window_handle, GWL_STYLE ) | WS_SYSMENU
                : GetWindowLongPtrW( self.window_handle, GWL_STYLE ) & ~WS_SYSMENU );
            return self;
        }
        auto&& enable_window_minimize_ctrl( this auto&& self, const bool is_enable ) noexcept
        {
            SetWindowLongPtrW(
              self.window_handle, GWL_STYLE,
              is_enable
                ? GetWindowLongPtrW( self.window_handle, GWL_STYLE ) | WS_MINIMIZEBOX
                : GetWindowLongPtrW( self.window_handle, GWL_STYLE ) & ~WS_MINIMIZEBOX );
            return self;
        }
        auto&& enable_window_maximize_ctrl( this auto&& self, const bool is_enable ) noexcept
        {
            SetWindowLongPtrW(
              self.window_handle, GWL_STYLE,
              is_enable
                ? GetWindowLongPtrW( self.window_handle, GWL_STYLE ) | WS_MAXIMIZEBOX
                : GetWindowLongPtrW( self.window_handle, GWL_STYLE ) & ~WS_MAXIMIZEBOX );
            return self;
        }
        auto&& enable_window_close_ctrl( this auto&& self, const bool is_enable ) noexcept
        {
            EnableMenuItem(
              GetSystemMenu( self.window_handle, FALSE ), SC_CLOSE,
              is_enable ? MF_BYCOMMAND | MF_ENABLED : MF_BYCOMMAND | MF_DISABLED | MF_GRAYED );
            return self;
        }
        auto&& press_any_key_to_continue( this auto&& self ) noexcept
        {
            DWORD mode;
            if ( !GetConsoleMode( self.std_input_handle, &mode ) ) [[unlikely]] {
                return self;
            }
            SetConsoleMode( self.std_input_handle, ENABLE_EXTENDED_FLAGS | ( mode & ~ENABLE_QUICK_EDIT_MODE ) );
            FlushConsoleInputBuffer( self.std_input_handle );
            INPUT_RECORD record;
            DWORD events;
            do {
                ReadConsoleInputW( self.std_input_handle, &record, 1, &events );
            } while ( record.EventType != KEY_EVENT || !record.Event.KeyEvent.bKeyDown );
            SetConsoleMode( self.std_input_handle, mode );
            return self;
        }
        auto&& ignore_exit_signal( this auto&& self, const bool is_ignore ) noexcept
        {
            SetConsoleCtrlHandler( nullptr, static_cast< WINBOOL >( is_ignore ) );
            return self;
        }
        auto&& enable_virtual_terminal_processing( this auto&& self, const bool is_enable ) noexcept
        {
            DWORD mode;
            GetConsoleMode( self.std_output_handle, &mode );
            is_enable ? mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING : mode &= ~ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode( self.std_output_handle, mode );
            return self;
        }
        auto&& clear( this auto&& self, std::pmr::memory_resource* const resource = std::pmr::get_default_resource() )
        {
            CONSOLE_SCREEN_BUFFER_INFO info;
            GetConsoleScreenBufferInfo( self.std_output_handle, &info );
            constexpr COORD top_left{ 0, 0 };
            const auto area{ info.dwSize.X * info.dwSize.Y };
            DWORD written;
            SetConsoleCursorPosition( self.std_output_handle, top_left );
            std::print( "{}", std::pmr::string{ static_cast< std::size_t >( area ), ' ', resource } );
            FillConsoleOutputAttribute( self.std_output_handle, info.wAttributes, area, top_left, &written );
            SetConsoleCursorPosition( self.std_output_handle, top_left );
            return self;
        }
        [[nodiscard]] auto get_size() const noexcept
        {
            CONSOLE_SCREEN_BUFFER_INFO info;
            GetConsoleScreenBufferInfo( std_output_handle, &info );
            return info.dwSize;
        }
        auto&& set_size(
          this auto&& self, const SHORT width, const SHORT height,
          std::pmr::memory_resource* const resource = std::pmr::get_default_resource() )
        {
            SMALL_RECT wrt{ 0, 0, static_cast< SHORT >( width - 1 ), static_cast< SHORT >( height - 1 ) };
            ShowWindow( self.window_handle, SW_SHOWNORMAL );
            SetConsoleScreenBufferSize( self.std_output_handle, { width, height } );
            SetConsoleWindowInfo( self.std_output_handle, TRUE, &wrt );
            SetConsoleScreenBufferSize( self.std_output_handle, { width, height } );
            SetConsoleWindowInfo( self.std_output_handle, TRUE, &wrt );
            self.clear( resource );
            return self;
        }
        auto&& set_title( this auto&& self, const char* const title ) noexcept
        {
            SetConsoleTitleA( title );
            return self;
        }
        auto&& set_title( this auto&& self, const wchar_t* const title ) noexcept
        {
            SetConsoleTitleW( title );
            return self;
        }
        auto&& set_charset( this auto&& self, const UINT charset_id ) noexcept
        {
            SetConsoleOutputCP( charset_id );
            SetConsoleCP( charset_id );
            return self;
        }
        auto&& set_translucency( this auto&& self, const BYTE value ) noexcept
        {
            SetLayeredWindowAttributes( self.window_handle, RGB( 0, 0, 0 ), value, LWA_ALPHA );
            return self;
        }
        auto&& show_cursor( this auto&& self, const bool is_shown ) noexcept
        {
            CONSOLE_CURSOR_INFO cursor_data;
            GetConsoleCursorInfo( self.std_output_handle, &cursor_data );
            cursor_data.bVisible = static_cast< WINBOOL >( is_shown );
            SetConsoleCursorInfo( self.std_output_handle, &cursor_data );
            return self;
        }
        auto&& lock_text( this auto&& self, const bool is_locked ) noexcept
        {
            DWORD attrs;
            if ( !GetConsoleMode( self.std_input_handle, &attrs ) ) [[unlikely]] {
                return self;
            }
            switch ( is_locked ) {
                case false :
                    attrs |= ENABLE_QUICK_EDIT_MODE;
                    attrs |= ENABLE_INSERT_MODE;
                    break;
                case true :
                    attrs &= ~ENABLE_QUICK_EDIT_MODE;
                    attrs &= ~ENABLE_INSERT_MODE;
                    break;
            }
            attrs |= ENABLE_MOUSE_INPUT;
            attrs |= ENABLE_LINE_INPUT;
            SetConsoleMode( self.std_input_handle, attrs );
            return self;
        }
    };
#else
# error "must be compiled on the windows os"
#endif
}
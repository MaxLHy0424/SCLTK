#pragma once
#include <windows.h>
#if true
# include <ntdef.h>
# include <tlhelp32.h>
# include <winternl.h>
#endif
#include <chrono>
#include <concepts>
#include <functional>
#include <memory_resource>
#include <optional>
#include <ranges>
#include <thread>
#include <type_traits>
#include <utility>
#include "pointer.hpp"
namespace cpp_utils
{
#if defined( _WIN32 ) || defined( _WIN64 )
    namespace mouse
    {
        inline constexpr DWORD button_left{ FROM_LEFT_1ST_BUTTON_PRESSED };
        inline constexpr DWORD button_middle{ FROM_LEFT_2ND_BUTTON_PRESSED };
        inline constexpr DWORD button_right{ RIGHTMOST_BUTTON_PRESSED };
        inline constexpr DWORD click{ 0x0000 };
        inline constexpr DWORD click_double{ DOUBLE_CLICK };
        inline constexpr DWORD move{ MOUSE_MOVED };
        inline constexpr DWORD wheel_horizontal{ MOUSE_HWHEELED };
        inline constexpr DWORD wheel{ MOUSE_WHEELED };
    }
    namespace keyboard
    {
        inline constexpr DWORD right_alt_press{ RIGHT_ALT_PRESSED };
        inline constexpr DWORD left_alt_press{ LEFT_ALT_PRESSED };
        inline constexpr DWORD right_ctrl_press{ RIGHT_CTRL_PRESSED };
        inline constexpr DWORD left_ctrl_press{ LEFT_CTRL_PRESSED };
        inline constexpr DWORD shift_press{ SHIFT_PRESSED };
        inline constexpr DWORD num_lock_on{ NUMLOCK_ON };
        inline constexpr DWORD scroll_lock_on{ SCROLLLOCK_ON };
        inline constexpr DWORD caps_lock_on{ CAPSLOCK_ON };
        inline constexpr DWORD enhanced_key{ ENHANCED_KEY };
    }
    namespace console_handle_flag
    {
        inline constexpr DWORD std_input{ STD_INPUT_HANDLE };
        inline constexpr DWORD std_output{ STD_OUTPUT_HANDLE };
        inline constexpr DWORD std_error{ STD_ERROR_HANDLE };
    }
    namespace console_text
    {
        inline constexpr WORD foreground_red{ FOREGROUND_RED };
        inline constexpr WORD foreground_green{ FOREGROUND_GREEN };
        inline constexpr WORD foreground_blue{ FOREGROUND_BLUE };
        inline constexpr WORD foreground_white{ FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE };
        inline constexpr WORD foreground_intensity{ FOREGROUND_INTENSITY };
        inline constexpr WORD background_red{ BACKGROUND_RED };
        inline constexpr WORD background_green{ BACKGROUND_GREEN };
        inline constexpr WORD background_blue{ BACKGROUND_BLUE };
        inline constexpr WORD background_white{ BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE };
        inline constexpr WORD background_intensity{ BACKGROUND_INTENSITY };
        inline constexpr WORD lvb_leading_byte{ COMMON_LVB_LEADING_BYTE };
        inline constexpr WORD lvb_trailing_byte{ COMMON_LVB_TRAILING_BYTE };
        inline constexpr WORD lvb_grid_horizontal{ COMMON_LVB_GRID_HORIZONTAL };
        inline constexpr WORD lvb_grid_lvertical{ COMMON_LVB_GRID_LVERTICAL };
        inline constexpr WORD lvb_grid_rvertical{ COMMON_LVB_GRID_RVERTICAL };
        inline constexpr WORD lvb_reverse_video{ COMMON_LVB_REVERSE_VIDEO };
        inline constexpr WORD lvb_underscore{ COMMON_LVB_UNDERSCORE };
        inline constexpr WORD lvb_sbcsdbcs{ COMMON_LVB_SBCSDBCS };
    }
    namespace window_state
    {
        inline constexpr UINT hide{ SW_HIDE };
        inline constexpr UINT show{ SW_SHOW };
        inline constexpr UINT show_without_activating{ SW_SHOWNA };
        inline constexpr UINT show_default{ SW_SHOWDEFAULT };
        inline constexpr UINT show_normal{ SW_SHOWNORMAL };
        inline constexpr UINT show_normal_without_activating{ SW_SHOWNOACTIVATE };
        inline constexpr UINT minimize{ SW_SHOWMINIMIZED };
        inline constexpr UINT minimize_and_activate_next_window_with_z_order{ SW_MINIMIZE };
        inline constexpr UINT minimize_without_activating{ SW_SHOWMINNOACTIVE };
        inline constexpr UINT minimize_force{ SW_FORCEMINIMIZE };
        inline constexpr UINT maximize{ SW_SHOWMAXIMIZED };
        inline constexpr UINT restore{ SW_RESTORE };
    }
    namespace service_flag
    {
        inline constexpr DWORD auto_start{ SERVICE_AUTO_START };
        inline constexpr DWORD boot_start{ SERVICE_BOOT_START };
        inline constexpr DWORD demand_start{ SERVICE_DEMAND_START };
        inline constexpr DWORD system_start{ SERVICE_SYSTEM_START };
        inline constexpr DWORD disabled_start{ SERVICE_DISABLED };
    }
    namespace registry_flag
    {
        inline constexpr DWORD binary_type{ REG_BINARY };
        inline constexpr DWORD dword_type{ REG_DWORD };
        inline constexpr DWORD dword_big_endian_type{ REG_DWORD_BIG_ENDIAN };
        inline constexpr DWORD dword_little_endian_type{ REG_DWORD_LITTLE_ENDIAN };
        inline constexpr DWORD qword_type{ REG_QWORD };
        inline constexpr DWORD qword_little_endian{ REG_QWORD_LITTLE_ENDIAN };
        inline constexpr DWORD expandable_string_type{ REG_EXPAND_SZ };
        inline constexpr DWORD multi_string_type{ REG_MULTI_SZ };
        inline constexpr DWORD string_type{ REG_SZ };
        inline constexpr DWORD link_type{ REG_LINK };
        inline constexpr DWORD none_type{ REG_NONE };
    }
    [[nodiscard]] inline auto to_string(
      const std::wstring_view str, const UINT charset,
      std::pmr::memory_resource* const resource = std::pmr::get_default_resource() ) -> std::optional< std::pmr::string >
    {
        if ( str.empty() || str.size() > static_cast< std::size_t >( INT_MAX ) ) [[unlikely]] {
            return std::nullopt;
        }
        const auto str_len{ static_cast< int >( str.size() ) };
        const auto flags{ static_cast< DWORD >( charset == CP_UTF8 ? WC_ERR_INVALID_CHARS : 0 ) };
        const auto size_needed{ WideCharToMultiByte( charset, flags, str.data(), str_len, nullptr, 0, nullptr, nullptr ) };
        if ( size_needed == 0 ) [[unlikely]] {
            return std::nullopt;
        }
        std::pmr::string result{ static_cast< std::size_t >( size_needed ), '\0', resource };
        if ( WideCharToMultiByte( charset, flags, str.data(), str_len, result.data(), size_needed, nullptr, nullptr ) != size_needed )
          [[unlikely]]
        {
            return std::nullopt;
        }
        return result;
    }
    [[nodiscard]] inline auto to_wstring(
      const std::string_view str, const UINT charset,
      std::pmr::memory_resource* const resource = std::pmr::get_default_resource() ) -> std::optional< std::pmr::wstring >
    {
        if ( str.empty() || str.size() > static_cast< std::size_t >( INT_MAX ) ) [[unlikely]] {
            return std::nullopt;
        }
        const auto str_len{ static_cast< int >( str.size() ) };
        const auto flags{ static_cast< DWORD >( charset == CP_UTF8 ? MB_ERR_INVALID_CHARS : 0 ) };
        const auto size_needed{ MultiByteToWideChar( charset, flags, str.data(), str_len, nullptr, 0 ) };
        if ( size_needed <= 0 ) [[unlikely]] {
            return std::nullopt;
        }
        std::pmr::wstring result{ static_cast< std::size_t >( size_needed ), L'\0', resource };
        if ( !MultiByteToWideChar( charset, flags, str.data(), str_len, result.data(), size_needed ) ) [[unlikely]] {
            return std::nullopt;
        }
        return result;
    }
    namespace details_
    {
        struct handle_closer final
        {
            static inline auto operator()( const HANDLE h ) noexcept
            {
                CloseHandle( h );
            }
        };
        struct null_legacy_handle_checker final
        {
            static inline auto operator()( const HANDLE h ) noexcept
            {
                return h == INVALID_HANDLE_VALUE;
            }
        };
    }
    using scoped_handle = std::unique_ptr< std::remove_pointer_t< HANDLE >, details_::handle_closer >;
    using scoped_legacy_handle
      = unique_ptr_ex< std::remove_pointer_t< HANDLE >, details_::null_legacy_handle_checker, details_::handle_closer >;
    namespace details_
    {
        using scoped_sc_handle = std::unique_ptr< std::remove_pointer_t< SC_HANDLE >, decltype( []( const SC_HANDLE h ) static noexcept
        {
            CloseServiceHandle( h );
        } ) >;
        using scoped_reg_key_handle = std::unique_ptr< std::remove_pointer_t< HKEY >, decltype( []( const HKEY h ) static noexcept
        {
            RegCloseKey( h );
        } ) >;
        using scoped_sid_handle = std::unique_ptr< std::remove_pointer_t< PSID >, decltype( []( const PSID p ) static noexcept
        {
            FreeSid( p );
        } ) >;
        template < typename F >
        [[nodiscard]] inline auto
          with_service( const std::wstring_view service_name, const DWORD scm_access, const DWORD service_access, F&& func ) noexcept
        {
            scoped_sc_handle scm{ OpenSCManagerW( nullptr, nullptr, scm_access ) };
            if ( scm == nullptr ) [[unlikely]] {
                return GetLastError();
            }
            scoped_sc_handle svc{ OpenServiceW( scm.get(), service_name.data(), service_access ) };
            if ( svc == nullptr ) [[unlikely]] {
                return GetLastError();
            }
            return func( scm.get(), svc.get() );
        }
        template < typename F >
        [[nodiscard]] inline auto for_each_dependency( const wchar_t* deps, F&& func ) noexcept
        {
            DWORD result{ ERROR_SUCCESS };
            auto current{ deps };
            while ( *current != L'\0' ) [[likely]] {
                if ( const auto r{ func( current ) }; r != ERROR_SUCCESS && result == ERROR_SUCCESS ) [[unlikely]] {
                    result = r;
                }
                current += std::wcslen( current ) + 1;
            }
            return result;
        }
        template < typename F >
        [[nodiscard]] inline auto
          with_service_dependencies( const SC_HANDLE service, F&& func, std::pmr::memory_resource* const resource ) noexcept
        {
            constexpr DWORD stack_buffer_size{ 8192 };
            std::array< BYTE, stack_buffer_size > stack_buffer{};
            DWORD bytes_needed{ 0 };
            const auto stack_config{ reinterpret_cast< LPQUERY_SERVICE_CONFIGW >( stack_buffer.data() ) };
            if ( QueryServiceConfigW( service, stack_config, stack_buffer_size, &bytes_needed ) ) [[likely]] {
                if ( stack_config->lpDependencies && *stack_config->lpDependencies != L'\0' ) {
                    return func( stack_config->lpDependencies );
                }
                return static_cast< DWORD >( ERROR_SUCCESS );
            }
            if ( GetLastError() != ERROR_INSUFFICIENT_BUFFER ) [[unlikely]] {
                return GetLastError();
            }
            std::pmr::vector< BYTE > heap_buffer( bytes_needed, resource );
            const auto heap_config{ reinterpret_cast< LPQUERY_SERVICE_CONFIGW >( heap_buffer.data() ) };
            if ( !QueryServiceConfigW( service, heap_config, bytes_needed, &bytes_needed ) ) [[unlikely]] {
                return static_cast< DWORD >( ERROR_SUCCESS );
            }
            if ( heap_config->lpDependencies && *heap_config->lpDependencies != L'\0' ) {
                return func( heap_config->lpDependencies );
            }
            return static_cast< DWORD >( ERROR_SUCCESS );
        }
        [[nodiscard]] inline auto stop_service_and_dependencies(
          const SC_HANDLE scm, const SC_HANDLE service, std::pmr::memory_resource* const resource ) noexcept -> DWORD
        {
            auto result{ with_service_dependencies( service, [ & ]( const wchar_t* deps ) noexcept
            {
                return for_each_dependency( deps, [ & ]( const wchar_t* dep_name ) noexcept
                {
                    const scoped_sc_handle dep_svc{ OpenServiceW( scm, dep_name, SERVICE_STOP | SERVICE_QUERY_STATUS ) };
                    if ( dep_svc == nullptr ) [[unlikely]] {
                        return static_cast< DWORD >( ERROR_SUCCESS );
                    }
                    return stop_service_and_dependencies( scm, dep_svc.get(), resource );
                } );
            }, resource ) };
            SERVICE_STATUS status{};
            if ( ControlService( service, SERVICE_CONTROL_STOP, &status ) ) [[likely]] {
                using namespace std::chrono_literals;
                bool query_ok{ true };
                while ( query_ok && status.dwCurrentState == SERVICE_STOP_PENDING ) {
                    query_ok = QueryServiceStatus( service, &status );
                    std::this_thread::sleep_for( 50ms );
                }
                if ( !query_ok || status.dwCurrentState != SERVICE_STOPPED ) [[unlikely]] {
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
            const auto result{ with_service_dependencies( service, [ & ]( const wchar_t* deps ) noexcept -> DWORD
            {
                return for_each_dependency( deps, [ & ]( const wchar_t* dep_name ) noexcept -> DWORD
                {
                    if ( *dep_name == L'@' ) [[unlikely]] {
                        return ERROR_SUCCESS;
                    }
                    const scoped_sc_handle dep_svc{ OpenServiceW( scm, dep_name, SERVICE_START | SERVICE_QUERY_STATUS ) };
                    if ( dep_svc == nullptr ) [[unlikely]] {
                        return ERROR_SUCCESS;
                    }
                    SERVICE_STATUS status{};
                    if ( QueryServiceStatus( dep_svc.get(), &status )
                         && ( status.dwCurrentState == SERVICE_RUNNING || status.dwCurrentState == SERVICE_START_PENDING ) )
                    {
                        return ERROR_SUCCESS;
                    }
                    return start_service_and_dependencies( scm, dep_svc.get(), resource );
                } );
            }, resource ) };
            if ( result != ERROR_SUCCESS ) [[unlikely]] {
                return result;
            }
            if ( !StartServiceW( service, 0, nullptr ) ) [[unlikely]] {
                if ( const auto err{ GetLastError() }; err != ERROR_SERVICE_ALREADY_RUNNING ) [[unlikely]] {
                    return err;
                }
            }
            return ERROR_SUCCESS;
        }
    }
    [[nodiscard]] inline auto set_privilege( const HANDLE proc, const wchar_t* const privilege, const bool is_enabled ) noexcept
    {
        scoped_handle token;
        if ( !OpenProcessToken( proc, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, std::out_ptr( token ) ) ) [[unlikely]] {
            return GetLastError();
        }
        LUID local_uid{};
        if ( !LookupPrivilegeValueW( nullptr, privilege, &local_uid ) ) [[unlikely]] {
            return GetLastError();
        }
        TOKEN_PRIVILEGES tp{};
        tp.PrivilegeCount             = 1;
        tp.Privileges[ 0 ].Luid       = local_uid;
        tp.Privileges[ 0 ].Attributes = is_enabled ? SE_PRIVILEGE_ENABLED : 0;
        if ( !AdjustTokenPrivileges( token.get(), FALSE, &tp, sizeof( TOKEN_PRIVILEGES ), nullptr, nullptr ) ) [[unlikely]] {
            return GetLastError();
        }
        return static_cast< DWORD >( ERROR_SUCCESS );
    }
    class process_snapshot final
    {
      private:
        using nt_open_process_t_      = NTSTATUS( NTAPI* )( PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PCLIENT_ID );
        using nt_terminate_process_t_ = NTSTATUS( NTAPI* )( HANDLE, NTSTATUS );
        using nt_suspend_process_t_   = NTSTATUS( NTAPI* )( HANDLE );
        using nt_resume_process_t_    = NTSTATUS( NTAPI* )( HANDLE );
        nt_open_process_t_ nt_open_process_{ nullptr };
        nt_terminate_process_t_ nt_terminate_process_{ nullptr };
        nt_suspend_process_t_ nt_suspend_process_{ nullptr };
        nt_resume_process_t_ nt_resume_process_{ nullptr };
        scoped_legacy_handle snapshot_{};
      public:
        [[nodiscard]] auto valid() const noexcept
        {
            return nt_open_process_ != nullptr && nt_terminate_process_ != nullptr && nt_suspend_process_ != nullptr
                && nt_resume_process_ != nullptr && snapshot_.get() != INVALID_HANDLE_VALUE;
        }
        [[nodiscard]] auto get_nt_terminate_process() const noexcept
        {
            return nt_terminate_process_;
        }
        [[nodiscard]] auto get_nt_suspend_process() const noexcept
        {
            return nt_suspend_process_;
        }
        [[nodiscard]] auto get_nt_resume_process() const noexcept
        {
            return nt_resume_process_;
        }
        [[nodiscard]] auto wrapped_nt_open_process( const DWORD pid, const ACCESS_MASK desired_ccess ) const noexcept
        {
            CLIENT_ID client_id{ .UniqueProcess{ reinterpret_cast< HANDLE >( pid ) }, .UniqueThread{ nullptr } };
            OBJECT_ATTRIBUTES obj_attrs{
              .Length{ sizeof( OBJECT_ATTRIBUTES ) },
              .RootDirectory{ nullptr },
              .ObjectName{ nullptr },
              .Attributes{ OBJ_CASE_INSENSITIVE },
              .SecurityDescriptor{ nullptr },
              .SecurityQualityOfService{ nullptr } };
            scoped_handle proc_handle{};
            nt_open_process_( std::out_ptr( proc_handle ), desired_ccess, &obj_attrs, &client_id );
            return proc_handle;
        }
        [[nodiscard]] auto refresh() noexcept
        {
            const auto new_snapshot{ CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 ) };
            if ( new_snapshot == INVALID_HANDLE_VALUE ) [[unlikely]] {
                return false;
            }
            snapshot_.reset( new_snapshot );
            return true;
        }
        template < typename F >
            requires requires( F&& f, const PROCESSENTRY32W& proc_entry ) {
                { f( proc_entry ) } noexcept -> std::convertible_to< bool >;
            }
        [[nodiscard]] auto iterate( F&& func ) const noexcept
        {
            const auto snapshot_value{ snapshot_.get() };
            if ( snapshot_value == INVALID_HANDLE_VALUE ) [[unlikely]] {
                return false;
            }
            PROCESSENTRY32W proc_entry{};
            proc_entry.dwSize = sizeof( PROCESSENTRY32W );
            if ( !Process32FirstW( snapshot_value, &proc_entry ) ) [[unlikely]] {
                return false;
            }
            auto result{ true };
            do {
                result = func( std::as_const( proc_entry ) );
            } while ( Process32NextW( snapshot_value, &proc_entry ) );
            return result;
        }
        [[nodiscard]] auto terminate_by_pid( const DWORD pid ) const noexcept
        {
            const auto proc_handle{ wrapped_nt_open_process( pid, PROCESS_TERMINATE ) };
            if ( proc_handle == nullptr ) [[unlikely]] {
                return false;
            }
            return NT_SUCCESS( nt_terminate_process_( proc_handle.get(), 0 ) );
        }
        [[nodiscard]] auto terminate_by_name( const std::wstring_view name ) const noexcept
        {
            return iterate( [ & ]( const PROCESSENTRY32W& proc_entry ) noexcept
            {
                if ( _wcsicmp( proc_entry.szExeFile, name.data() ) == 0 ) {
                    return terminate_by_pid( proc_entry.th32ProcessID );
                }
                return true;
            } );
        }
        template < typename Range >
            requires requires( Range&& range ) {
                { *range.begin() } -> std::convertible_to< std::wstring_view >;
                range.begin() != range.end();
                range.empty();
            }
        [[nodiscard]] auto terminate_by_names( Range&& names ) const noexcept
        {
            if ( names.empty() ) [[unlikely]] {
                return true;
            }
            return iterate( [ & ]( const PROCESSENTRY32W& proc_entry ) noexcept
            {
                for ( const auto& name : names ) {
                    if ( _wcsicmp( proc_entry.szExeFile, name.data() ) == 0 ) {
                        return terminate_by_pid( proc_entry.th32ProcessID );
                    }
                }
                return true;
            } );
        }
        [[nodiscard]] auto suspend_by_pid( const DWORD pid ) const noexcept
        {
            const auto proc_handle{ wrapped_nt_open_process( pid, PROCESS_SUSPEND_RESUME ) };
            if ( proc_handle == nullptr ) [[unlikely]] {
                return false;
            }
            return NT_SUCCESS( nt_suspend_process_( proc_handle.get() ) );
        }
        [[nodiscard]] auto suspend_by_name( const std::wstring_view name ) const noexcept
        {
            return iterate( [ & ]( const PROCESSENTRY32W& proc_entry ) noexcept
            {
                if ( _wcsicmp( proc_entry.szExeFile, name.data() ) == 0 ) {
                    return suspend_by_pid( proc_entry.th32ProcessID );
                }
                return true;
            } );
        }
        template < typename Range >
            requires requires( Range&& range ) {
                { *range.begin() } -> std::convertible_to< std::wstring_view >;
                range.begin() != range.end();
                range.empty();
            }
        [[nodiscard]] auto suspend_by_names( Range&& names ) const noexcept
        {
            if ( names.empty() ) [[unlikely]] {
                return true;
            }
            return iterate( [ & ]( const PROCESSENTRY32W& proc_entry ) noexcept
            {
                for ( const auto& name : names ) {
                    if ( _wcsicmp( proc_entry.szExeFile, name.data() ) == 0 ) {
                        return suspend_by_pid( proc_entry.th32ProcessID );
                    }
                }
                return true;
            } );
        }
        [[nodiscard]] auto resume_by_pid( const DWORD pid ) const noexcept
        {
            const auto proc_handle{ wrapped_nt_open_process( pid, PROCESS_SUSPEND_RESUME ) };
            if ( proc_handle == nullptr ) [[unlikely]] {
                return false;
            }
            return NT_SUCCESS( nt_resume_process_( proc_handle.get() ) );
        }
        [[nodiscard]] auto resume_by_name( const std::wstring_view name ) const noexcept
        {
            return iterate( [ & ]( const PROCESSENTRY32W& proc_entry ) noexcept
            {
                if ( _wcsicmp( proc_entry.szExeFile, name.data() ) == 0 ) {
                    return resume_by_pid( proc_entry.th32ProcessID );
                }
                return true;
            } );
        }
        template < typename Range >
            requires requires( Range&& range ) {
                { *range.begin() } -> std::convertible_to< std::wstring_view >;
                range.begin() != range.end();
                range.empty();
            }
        [[nodiscard]] auto resume_by_names( Range&& names ) const noexcept
        {
            if ( names.empty() ) [[unlikely]] {
                return true;
            }
            return iterate( [ & ]( const PROCESSENTRY32W& proc_entry ) noexcept
            {
                for ( const auto& name : names ) {
                    if ( _wcsicmp( proc_entry.szExeFile, name.data() ) == 0 ) {
                        return resume_by_pid( proc_entry.th32ProcessID );
                    }
                }
                return true;
            } );
        }
        auto operator=( const process_snapshot& ) -> process_snapshot& = delete;
        auto operator=( process_snapshot&& ) -> process_snapshot&      = delete;
        process_snapshot() noexcept
        {
            const auto ntdll_handle{ GetModuleHandleW( L"ntdll.dll" ) };
            nt_open_process_ = std::bit_cast< nt_open_process_t_ >( GetProcAddress( ntdll_handle, "NtOpenProcess" ) );
            nt_terminate_process_
              = std::bit_cast< nt_terminate_process_t_ >( GetProcAddress( ntdll_handle, "NtTerminateProcess" ) );
            nt_suspend_process_ = std::bit_cast< nt_suspend_process_t_ >( GetProcAddress( ntdll_handle, "NtSuspendProcess" ) );
            nt_resume_process_  = std::bit_cast< nt_resume_process_t_ >( GetProcAddress( ntdll_handle, "NtResumeProcess" ) );
            ( void ) refresh();
        }
        process_snapshot( const process_snapshot& ) = delete;
        process_snapshot( process_snapshot&& )      = delete;
        ~process_snapshot()                         = default;
    };
    [[nodiscard]] inline auto create_registry_key(
      const HKEY main_key, const std::wstring_view sub_key, const std::wstring_view value_name, const DWORD type,
      const BYTE* const data, const DWORD data_size ) noexcept
    {
        details_::scoped_reg_key_handle key_handle;
        if ( const auto result{ RegCreateKeyExW(
               main_key, sub_key.data(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, std::out_ptr( key_handle ),
               nullptr ) };
             result != ERROR_SUCCESS ) [[unlikely]]
        {
            return result;
        }
        return RegSetValueExW( key_handle.get(), value_name.data(), 0, type, data, data_size );
    }
    [[nodiscard]] inline auto create_registry_key_without_redirect(
      const HKEY main_key, const std::wstring_view sub_key, const std::wstring_view value_name, const DWORD type,
      const BYTE* const data, const DWORD data_size ) noexcept
    {
        details_::scoped_reg_key_handle key_handle;
        if ( const auto result{ RegCreateKeyExW(
               main_key, sub_key.data(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_WOW64_64KEY, nullptr,
               std::out_ptr( key_handle ), nullptr ) };
             result != ERROR_SUCCESS ) [[unlikely]]
        {
            return result;
        }
        return RegSetValueExW( key_handle.get(), value_name.data(), 0, type, data, data_size );
    }
    [[nodiscard]] inline auto
      delete_registry_key( const HKEY main_key, const std::wstring_view sub_key, const std::wstring_view value_name ) noexcept
    {
        details_::scoped_reg_key_handle key_handle;
        if ( const auto result{ RegOpenKeyExW( main_key, sub_key.data(), 0, KEY_SET_VALUE, std::out_ptr( key_handle ) ) };
             result != ERROR_SUCCESS ) [[unlikely]]
        {
            return result;
        }
        return RegDeleteValueW( key_handle.get(), value_name.data() );
    }
    [[nodiscard]] inline auto delete_registry_key_without_redirect(
      const HKEY main_key, const std::wstring_view sub_key, const std::wstring_view value_name ) noexcept
    {
        details_::scoped_reg_key_handle key_handle;
        if ( const auto result{
               RegOpenKeyExW( main_key, sub_key.data(), 0, KEY_SET_VALUE | KEY_WOW64_64KEY, std::out_ptr( key_handle ) ) };
             result != ERROR_SUCCESS ) [[unlikely]]
        {
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
        details_::scoped_reg_key_handle key_handle;
        if ( const auto result{ RegOpenKeyExW(
               main_key, nullptr, 0, KEY_WRITE | KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE | KEY_WOW64_64KEY,
               std::out_ptr( key_handle ) ) };
             result != ERROR_SUCCESS ) [[unlikely]]
        {
            return result;
        }
        return RegDeleteTreeW( key_handle.get(), sub_key.data() );
    }
    [[nodiscard]] inline auto set_service_start_type( const std::wstring_view service_name, const DWORD start_type ) noexcept
    {
        return details_::with_service(
          service_name, SC_MANAGER_CONNECT, SERVICE_CHANGE_CONFIG,
          [ start_type ]( const SC_HANDLE, const SC_HANDLE svc ) noexcept -> DWORD
        {
            if ( !ChangeServiceConfigW(
                   svc, SERVICE_NO_CHANGE, start_type, SERVICE_NO_CHANGE, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                   nullptr ) ) [[unlikely]]
            {
                return GetLastError();
            }
            return ERROR_SUCCESS;
        } );
    }
    [[nodiscard]] inline auto stop_service_with_dependencies(
      const std::wstring_view service_name, std::pmr::memory_resource* const resource = std::pmr::get_default_resource() )
    {
        return details_::with_service(
          service_name, SC_MANAGER_CONNECT | SC_MANAGER_ENUMERATE_SERVICE,
          SERVICE_STOP | SERVICE_QUERY_STATUS | SERVICE_ENUMERATE_DEPENDENTS,
          [ resource ]( const SC_HANDLE scm, const SC_HANDLE svc ) noexcept -> DWORD
        {
            return details_::stop_service_and_dependencies( scm, svc, resource );
        } );
    }
    [[nodiscard]] inline auto start_service_with_dependencies(
      const std::wstring_view service_name, std::pmr::memory_resource* const resource = std::pmr::get_default_resource() )
    {
        return details_::with_service(
          service_name, SC_MANAGER_CONNECT, SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_QUERY_CONFIG,
          [ resource ]( const SC_HANDLE scm, const SC_HANDLE svc ) noexcept -> DWORD
        {
            return details_::start_service_and_dependencies( scm, svc, resource );
        } );
    }
    [[nodiscard]] inline auto is_run_as_admin() noexcept
    {
        SID_IDENTIFIER_AUTHORITY nt_authority{ SECURITY_NT_AUTHORITY };
        details_::scoped_sid_handle admins_group;
        if ( !AllocateAndInitializeSid(
               &nt_authority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0,
               std::out_ptr( admins_group ) ) ) [[unlikely]]
        {
            return false;
        }
        BOOL is_admin{ FALSE };
        CheckTokenMembership( nullptr, admins_group.get(), &is_admin );
        return static_cast< bool >( is_admin );
    }
    inline auto clone_self() noexcept
    {
        std::array< wchar_t, MAX_PATH > file_path{};
        GetModuleFileNameW( nullptr, file_path.data(), MAX_PATH );
        ShellExecuteW( nullptr, L"open", file_path.data(), nullptr, nullptr, SW_SHOWNORMAL );
    }
    inline auto clone_self_as_admin() noexcept
    {
        std::array< wchar_t, MAX_PATH > file_path{};
        GetModuleFileNameW( nullptr, file_path.data(), MAX_PATH );
        ShellExecuteW( nullptr, L"runas", file_path.data(), nullptr, nullptr, SW_SHOWNORMAL );
    }
#else
# error "must be compiled on the windows os"
#endif
}
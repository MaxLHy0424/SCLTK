#define WINVER       0x0601
#define _WIN32_WINNT 0x0601
#include <winsock2.h>
#define NOCOMM
#define NOSOUND
#define NORPC
#include <cpp_utils/const_string.hpp>
#include <cpp_utils/windows_console.hpp>
#include <initguid.h>
#include <iphlpapi.h>
#include <setupapi.h>
#include <tre/regex.h>
#include <wincrypt.h>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <random>
#ifdef SCLTK_LEGACY
# include "../meta/legacy/info.h"
#else
# include "../meta/mainline/info.h"
#endif
DEFINE_GUID( GUID_DEVCLASS_NET, 0x4d36e972, 0xe325, 0x11ce, 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18 );
namespace scltk
{
    using namespace std::chrono_literals;
    using namespace std::string_view_literals;
    using namespace cpp_utils::const_string_literals;
    using ui_func_args_type = cpp_utils::console_ui::func_args;
    constexpr SHORT console_width{ 50 };
    constexpr SHORT console_height{ 25 };
    constexpr UINT charset_id{ 936 };
    constexpr const auto& config_file_name{ L"" INFO_SHORT_NAME ".conf" };
    constexpr auto func_back{ cpp_utils::console_ui::func_back };
    constexpr auto func_exit{ cpp_utils::console_ui::func_exit };
    template < cpp_utils::const_string Title, std::size_t NewLineCount >
    constexpr auto make_title_text{ cpp_utils::concat_const_string(
      cpp_utils::make_repeated_const_string< ' ', ( static_cast< std::size_t >( console_width ) - Title.size() + 1 ) / 2 >(),
      Title, cpp_utils::make_repeated_const_string< '\n', NewLineCount >() ) };
    template < cpp_utils::const_string Text >
    constexpr auto make_item_text{ cpp_utils::concat_const_string( " > "_cs, Text, " "_cs ) };
    const cpp_utils::console con;
    const auto unsynced_mem_pool{ [] static noexcept
    {
        static std::pmr::unsynchronized_pool_resource pool{
          std::pmr::pool_options{ .max_blocks_per_chunk{ 1024 }, .largest_required_pool_block{ 4096 } },
          std::pmr::new_delete_resource()
        };
        std::pmr::set_default_resource( &pool );
        return &pool;
    }() };
    cpp_utils::process_snapshot proc_snapshot;
    constexpr auto quit() noexcept
    {
        return func_exit;
    }
    auto disable_hotkey() noexcept
    {
        DWORD attrs;
        GetConsoleMode( con.std_input_handle, &attrs );
        attrs &= ~ENABLE_PROCESSED_INPUT;
        SetConsoleMode( con.std_input_handle, attrs );
    }
    auto enable_privileges() noexcept
    {
        const auto current_process{ GetCurrentProcess() };
        ( void ) cpp_utils::set_privilege( current_process, L"" SE_DEBUG_NAME, true );
        ( void ) cpp_utils::set_privilege( current_process, L"" SE_SHUTDOWN_NAME, true );
    }
    auto generate_window_title()
    {
        constexpr auto chars_dict{ cpp_utils::invoke_to_array< [] static noexcept
        {
            std::vector< wchar_t > dict;
            for ( auto ch{ L'A' }; ch <= L'Z'; ++ch ) {
                dict.emplace_back( ch );
            }
            for ( auto ch{ L'a' }; ch <= L'z'; ++ch ) {
                dict.emplace_back( ch );
            }
            for ( auto ch{ L'0' }; ch <= L'9'; ++ch ) {
                dict.emplace_back( ch );
            }
            dict.append_range( LR"(`~!@#$%^&*+-_=()[]{}/\|;:'",.<>?)"sv );
            return dict;
        } >() };
        constexpr auto title_length{ 32uz };
        std::array< wchar_t, title_length + 1 > title;
        std::mt19937_64 gen{ std::random_device{}() };
        std::uniform_int_distribution< std::size_t > dist{ 0uz, chars_dict.size() - 1uz };
        for ( auto i{ 0uz }; i < title_length; ++i ) {
            title[ i ] = chars_dict[ dist( gen ) ];
        }
        title.back() = L'\0';
        return title;
    }
    namespace details_
    {
        class scoped_wregex final
        {
          private:
            std::pmr::wstring pattern_{};
            regex_t rx_{};
            bool valid_{};
            auto cleanup_() noexcept
            {
                if ( valid_ ) [[likely]] {
                    regfree( &rx_ );
                    valid_ = false;
                }
            }
          public:
            auto valid() const noexcept
            {
                return valid_;
            }
            const auto& get_pattern() const noexcept
            {
                return pattern_;
            }
            auto match( const wchar_t* const text ) const noexcept
            {
                if ( !valid_ ) [[unlikely]] {
                    return false;
                }
                return regwexec( &rx_, text, 0, nullptr, 0 ) == 0;
            }
            auto operator=( const scoped_wregex& ) -> scoped_wregex& = delete;
            auto operator=( scoped_wregex&& other ) noexcept -> scoped_wregex&
            {
                if ( this != &other ) {
                    cleanup_();
                    rx_          = other.rx_;
                    valid_       = other.valid_;
                    other.rx_    = regex_t{};
                    other.valid_ = false;
                }
                return *this;
            }
            scoped_wregex( const std::wstring_view pattern ) noexcept
              : pattern_( pattern, unsynced_mem_pool )
            {
                valid_ = ( tre_regwcomp( &rx_, pattern_.data(), REG_EXTENDED | REG_NOSUB ) == 0 );
                if ( !valid_ ) [[unlikely]] {
                    rx_ = {};
                }
            }
            scoped_wregex( const scoped_wregex& ) = delete;
            scoped_wregex( scoped_wregex&& other ) noexcept
              : rx_( other.rx_ )
              , valid_( other.valid_ )
            {
                other.rx_    = {};
                other.valid_ = false;
            }
            ~scoped_wregex() noexcept
            {
                cleanup_();
            }
        };
        template < cpp_utils::const_wstring... Items >
        using make_const_wstring_list_t = cpp_utils::type_list< cpp_utils::value_identity< Items >... >;
        using win32_file_path_buffer_t  = std::array< wchar_t, MAX_PATH >;
        using scoped_cert_store = std::unique_ptr< std::remove_pointer_t< HCERTSTORE >, decltype( []( const HCERTSTORE h ) static noexcept
        {
            CertCloseStore( h, 0 );
        } ) >;
        using scoped_cert_context
          = std::unique_ptr< std::remove_pointer_t< PCCERT_CONTEXT >, decltype( []( const PCCERT_CONTEXT h ) static noexcept
        {
            CertFreeCertificateContext( h );
        } ) >;
        class file_locker final
        {
          private:
            HANDLE file_handle_;
            bool locked;
          public:
            auto operator=( const file_locker& ) -> file_locker& = delete;
            auto operator=( file_locker&& ) -> file_locker&      = delete;
            file_locker( const HANDLE file_handle ) noexcept
              : file_handle_{ file_handle }
              , locked{ false }
            {
                OVERLAPPED overlapped{};
                if ( LockFileEx( file_handle_, LOCKFILE_EXCLUSIVE_LOCK, 0, 0xFFFFFFFF, 0xFFFFFFFF, &overlapped ) ) [[likely]] {
                    locked = true;
                }
            }
            file_locker( const file_locker& )     = delete;
            file_locker( file_locker&& ) noexcept = delete;
            ~file_locker() noexcept
            {
                if ( locked ) [[likely]] {
                    OVERLAPPED overlapped{};
                    UnlockFileEx( file_handle_, 0, 0xFFFFFFFF, 0xFFFFFFFF, &overlapped );
                }
            }
        };
#ifdef SCLTK_LEGACY
        class wow64_file_redirect_guard final
        {
          private:
            HMODULE kernel32_dll{ GetModuleHandleW( L"kernel32.dll" ) };
            PVOID old_value{ nullptr };
            bool disabled{ false };
          public:
            auto operator=( const wow64_file_redirect_guard& ) -> wow64_file_redirect_guard& = delete;
            auto operator=( wow64_file_redirect_guard&& ) -> wow64_file_redirect_guard&      = delete;
            wow64_file_redirect_guard() noexcept
            {
                if ( kernel32_dll == nullptr ) [[unlikely]] {
                    return;
                }
                const auto fn_disable{ std::bit_cast< BOOL( WINAPI* )( PVOID* ) >(
                  GetProcAddress( kernel32_dll, "Wow64DisableWow64FsRedirection" ) ) };
                if ( fn_disable != nullptr ) {
                    disabled = fn_disable( &old_value );
                }
            }
            wow64_file_redirect_guard( const wow64_file_redirect_guard& ) = delete;
            wow64_file_redirect_guard( wow64_file_redirect_guard&& )      = delete;
            ~wow64_file_redirect_guard() noexcept
            {
                if ( !disabled ) [[unlikely]] {
                    return;
                }
                if ( kernel32_dll == nullptr ) [[unlikely]] {
                    return;
                }
                const auto fn_revert{ std::bit_cast< BOOL( WINAPI* )( PVOID ) >(
                  GetProcAddress( kernel32_dll, "Wow64RevertWow64FsRedirection" ) ) };
                if ( fn_revert != nullptr ) {
                    fn_revert( old_value );
                }
            }
        };
#endif
        constexpr auto is_capital_case( const wchar_t ch ) noexcept
        {
            return ch >= L'A' && ch <= L'Z';
        };
        constexpr auto is_lower_case( const wchar_t ch ) noexcept
        {
            return ch >= L'a' && ch <= L'z';
        };
        constexpr auto is_in_alphabet( const wchar_t ch ) noexcept
        {
            return is_capital_case( ch ) || is_lower_case( ch );
        };
        constexpr auto is_number( const wchar_t ch ) noexcept
        {
            return ch >= L'0' && ch <= L'9';
        };
        template < cpp_utils::character CharT >
        constexpr auto is_whitespace( const CharT ch ) noexcept
        {
            switch ( ch ) {
                case static_cast< CharT >( '\t' ) :
                case static_cast< CharT >( '\v' ) :
                case static_cast< CharT >( '\f' ) :
                case static_cast< CharT >( ' ' ) : return true;
            }
            return false;
        }
        template < cpp_utils::character CharT, typename... Args >
            requires(
              ( std::same_as< std::decay_t< Args >, std::basic_string< CharT > >
                || std::same_as< std::decay_t< Args >, std::pmr::basic_string< CharT > >
                || std::same_as< std::decay_t< Args >, std::basic_string_view< CharT > > )
              && ... )
        auto concat_string( Args&&... strings )
        {
            std::pmr::basic_string< CharT > result;
            result.reserve( ( std::forward< Args >( strings ).size() + ... ) );
            ( result.append( std::forward< Args >( strings ) ), ... );
            return result;
        }
        auto press_any_key_to_return() noexcept
        {
            cpp_utils::print( cpp_utils::no_formatting, "\n\n 请按任意键返回."sv );
            con.press_any_key_to_continue();
        }
        auto get_proc_path( const cpp_utils::scoped_handle& proc_handle ) noexcept
          -> std::optional< std::pair< win32_file_path_buffer_t, DWORD > >
        {
            std::pair< win32_file_path_buffer_t, DWORD > result{ {}, MAX_PATH };
            if ( !QueryFullProcessImageNameW( proc_handle.get(), 0, result.first.data(), &result.second ) ) [[unlikely]] {
                return std::nullopt;
            }
            return result;
        }
        auto get_sign_name( const win32_file_path_buffer_t& path ) -> std::optional< std::pmr::wstring >
        {
            scoped_cert_store cert_store{ nullptr };
            DWORD encoding{ 0 };
            DWORD content_type{ 0 };
            DWORD format_type{ 0 };
            if ( !CryptQueryObject(
                   CERT_QUERY_OBJECT_FILE, path.data(), CERT_QUERY_CONTENT_FLAG_PKCS7_SIGNED_EMBED, CERT_QUERY_FORMAT_FLAG_BINARY,
                   0, &encoding, &content_type, &format_type, std::out_ptr( cert_store ), nullptr, nullptr )
                 || cert_store == nullptr ) [[unlikely]]
            {
                return std::nullopt;
            }
            scoped_cert_context cert{ nullptr };
            while ( cert.reset( CertEnumCertificatesInStore( cert_store.get(), cert.get() ) ), cert != nullptr ) [[likely]] {
                const auto name_len{ CertGetNameStringW( cert.get(), CERT_NAME_SIMPLE_DISPLAY_TYPE, 0, nullptr, nullptr, 0 ) };
                if ( name_len < 2 ) [[unlikely]] {
                    continue;
                }
                std::pmr::wstring name_buf( name_len, L'\0', unsynced_mem_pool );
                CertGetNameStringW( cert.get(), CERT_NAME_SIMPLE_DISPLAY_TYPE, 0, nullptr, name_buf.data(), name_len );
                return name_buf;
            }
            return std::nullopt;
        }
        auto is_cbms_daemon( const PROCESSENTRY32W& proc_entry ) noexcept
        {
            constexpr auto extension_name{ L".exe"sv };
            constexpr auto is_number_or_in_alphabet{ []( const wchar_t ch ) static noexcept
            {
                return is_in_alphabet( ch ) || is_number( ch );
            } };
            if ( const std::wstring_view file_name{ proc_entry.szExeFile };
                 file_name.size() != 6 + extension_name.size() && file_name.size() != 7 + extension_name.size()
                 && std::ranges::all_of( file_name.subview( 0, file_name.size() - extension_name.size() ), is_number_or_in_alphabet ) )
            {
                return false;
            }
            const auto proc_handle{
              proc_snapshot.wrapped_nt_open_process( proc_entry.th32ProcessID, PROCESS_QUERY_LIMITED_INFORMATION ) };
            if ( proc_handle == nullptr ) [[unlikely]] {
                return false;
            }
            const auto proc_path{ get_proc_path( proc_handle ) };
            if ( !proc_path.has_value() ) {
                return false;
            }
            const auto& [ buffer, size ]{ proc_path.value() };
            for ( const auto original_token :
                  std::wstring_view{ buffer.data(), size } | std::views::drop( L"C:\\"sv.size() ) | std::views::split( L'\\' ) )
            {
                const std::wstring_view token{ original_token.begin(), original_token.end() };
                if ( token == L"yesok_CBCS"sv ) {
                    return true;
                }
                if ( is_in_alphabet( token.front() ) && std::ranges::all_of( token.subview( 1 ), is_number ) ) {
                    return true;
                }
            }
            return false;
        }
        auto is_jfglzs_daemon( const PROCESSENTRY32W& proc_entry ) noexcept
        {
            constexpr auto extension_name_size{ L".exe"sv.size() };
            std::wstring_view name{ proc_entry.szExeFile };
            if ( name == L"zmserv.exe"sv || name == L"syszm.exe"sv ) {
                return false;
            }
            if ( name.size() != 3 + extension_name_size && name.size() != 5 + extension_name_size
                 && name.size() != 7 + extension_name_size && name.size() != 10 + extension_name_size )
            {
                return false;
            }
            if ( !std::ranges::all_of( name.substr( 0, name.size() - 4 ), is_lower_case ) ) {
                return false;
            }
            const auto proc_handle{
              proc_snapshot.wrapped_nt_open_process( proc_entry.th32ProcessID, PROCESS_QUERY_LIMITED_INFORMATION ) };
            if ( proc_handle == nullptr ) [[unlikely]] {
                return false;
            }
            const auto proc_path{ get_proc_path( proc_handle ) };
            if ( !proc_path.has_value() ) {
                return false;
            }
            const auto& [ buffer, size ]{ proc_path.value() };
            std::wstring_view path_view{ buffer.data(), size };
            if ( path_view.starts_with( LR"(C:\Program Files\)"sv ) ) {
                path_view.remove_prefix( LR"(C:\Program Files\)"sv.size() );
                path_view.remove_suffix( name.size() + 1 );
                if ( path_view.size() != 3 && path_view.size() != 4 ) {
                    return false;
                }
                if ( !name.contains( path_view ) ) {
                    return false;
                }
                return true;
            }
            if ( path_view.starts_with( LR"(C:\Program Files (x86)\)"sv ) ) {
                path_view.remove_prefix( LR"(C:\Program Files (x86)\)"sv.size() );
                path_view.remove_suffix( name.size() + 1 );
                if ( path_view.size() != 3 && path_view.size() != 4 ) {
                    return false;
                }
                if ( !name.contains( path_view ) ) {
                    return false;
                }
                return true;
            }
            if ( path_view.starts_with( LR"(C:\)"sv ) && is_lower_case( path_view[ 3 ] ) ) {
                path_view.remove_prefix( LR"(C:\)"sv.size() + 1 );
                path_view.remove_suffix( name.size() + 1 );
                if ( std::ranges::all_of( path_view, is_number ) ) {
                    return true;
                }
                return false;
            }
            return false;
        }
        auto terminate_jfglzs_servs() noexcept
        {
            ( void ) proc_snapshot.terminate_by_names( std::array{ L"syszm.exe"sv, L"zmserv.exe"sv } );
        }
        auto is_workwin( const PROCESSENTRY32W& proc_entry )
        {
            const auto proc_handle{
              proc_snapshot.wrapped_nt_open_process( proc_entry.th32ProcessID, PROCESS_QUERY_LIMITED_INFORMATION ) };
            if ( proc_handle.get() == nullptr ) [[unlikely]] {
                return false;
            }
            const auto proc_path{ get_proc_path( proc_handle ) };
            if ( !proc_path.has_value() ) {
                return false;
            }
            const auto& [ path, _ ]{ proc_path.value() };
            const auto sign_name{ get_sign_name( path ) };
            if ( !sign_name.has_value() ) {
                return false;
            }
            if ( sign_name.value().contains( L"Nanjing Wangya Computer"sv ) ) {
                return true;
            }
            return false;
        }
        constexpr auto default_extra_proc_matcher{ []( const PROCESSENTRY32W& ) static noexcept
        {
            return false;
        } };
        constexpr auto default_helper{ [] static noexcept { } };
    }
    template <
      cpp_utils::const_string DisplayName, cpp_utils::same_as_type_list ProcNames, cpp_utils::same_as_type_list ServNames,
      auto ExtraProcMatcher = details_::default_extra_proc_matcher, auto CrackHelper = details_::default_helper,
      auto RestoreHelper = details_::default_helper >
        requires requires( const PROCESSENTRY32W& proc_entry ) {
            { ExtraProcMatcher( proc_entry ) } -> std::convertible_to< bool >;
            CrackHelper();
            RestoreHelper();
        }
    struct compile_time_rule_node final
    {
        static constexpr auto display_name{ DisplayName };
        static constexpr auto extra_proc_matcher{ ExtraProcMatcher };
        static constexpr auto crack_helper{ CrackHelper };
        static constexpr auto restore_helper{ RestoreHelper };
        using proc_names = ProcNames;
        using serv_names = ServNames;
    };
    using builtin_rules = cpp_utils::type_list<
      compile_time_rule_node<
        "海米计算机批量维护系统",
        details_::make_const_wstring_list_t<
          L"CBMS_Client.exe", L"susetup.exe", L"suerver.exe", L"tsvchqst.exe", L"snntime.exe", L"svchqst.exe", L"svch0st.exe",
          L"nssm.exe" >,
        details_::make_const_wstring_list_t< L"suerver", L"svch0st", L"svchqst", L"snntime" >, details_::is_cbms_daemon >,
      compile_time_rule_node<
        "机房管理助手",
        details_::make_const_wstring_list_t<
          L"jfglzs.exe", L"jfglzsn.exe", L"jfglzsp.exe", L"przs.exe", L"udwchk.exe", L"jcctzx.exe" >,
        details_::make_const_wstring_list_t< L"zmserv" >, details_::is_jfglzs_daemon, details_::terminate_jfglzs_servs >,
      compile_time_rule_node<
        "极域电子教室",
        details_::make_const_wstring_list_t<
          L"StudentMain.exe", L"DispcapHelper.exe", L"VRCwPlayer.exe", L"InstHelpApp.exe", L"InstHelpApp64.exe",
          L"TDOvrSet.exe", L"GATESRV.exe", L"ProcHelper64.exe", L"MasterHelper.exe", L"config-service.exe", L"gate-service.exe",
          L"network-service.exe", L"service-manager.exe", L"Student.exe", L"student-service.exe", L"sys-cmd-service.exe" >,
        details_::make_const_wstring_list_t< L"STUDSRV", L"TDKeybd", L"TDNetFilter", L"TDFileFilter", L"CMSGateSVC" > >,
      compile_time_rule_node<
        "联想智能云教室",
        details_::make_const_wstring_list_t<
          L"WfbsPnpInstall.exe", L"WFBSMon.exe", L"WFBSMlogon.exe", L"WFBSSvrLogShow.exe", L"ResetIp.exe", L"FuncForWIN64.exe",
          L"Fireware.exe", L"BCDBootCopy.exe", L"refreship.exe", L"WFDeskShow.exe", L"lenovoLockScreen.exe",
          L"PortControl64.exe", L"DesktopCheck.exe", L"DeploymentManager.exe", L"DeploymentAgent.exe", L"XYNTService.exe" >,
        details_::make_const_wstring_list_t< L"BSAgentSvr", L"tvnserver", L"WFBSMlogon" > >,
      compile_time_rule_node<
        "红蜘蛛多媒体网络教室", details_::make_const_wstring_list_t< L"rscheck.exe", L"checkrs.exe", L"REDAgent.exe" >,
        details_::make_const_wstring_list_t< L"appcheck2", L"checkapp2" > >,
      compile_time_rule_node< "伽卡他卡电子教室", details_::make_const_wstring_list_t< L"Student.exe", L"Smonitor.exe" >,
                              details_::make_const_wstring_list_t< L"Smsvc" > >,
      compile_time_rule_node<
        "锐捷云教室",
        details_::make_const_wstring_list_t<
          L"CMLauncher.exe", L"CMExternal.exe", L"ClassManagerAppSrv.exe", L"ClassManagerApp.exe", L"ClassManagerCmd.exe",
          L"Thin_Client_Config_Tool.exe", L"RemoteTch.exe", L"RCD_AutoUpdate.exe", L"rccdaemon.exe" >,
        details_::make_const_wstring_list_t< L"FileZilla Server", L"rccdaemon" > >,
      compile_time_rule_node<
        "凌波网络教室", details_::make_const_wstring_list_t< L"sbkup.exe", L"wsf.exe", L"NCStu.exe", L"NCCmn.dll" >,
        details_::make_const_wstring_list_t< L"Windows Application and Components Data Backup Support Service" > >,
      compile_time_rule_node<
        "传奇电子教室",
        details_::make_const_wstring_list_t<
          L"Student.exe", L"PsGhost.exe", L"secprocess.exe", L"SECService.exe", L"MirrorProInfo.exe", L"ClickShow.exe",
          L"GtSRun.exe" >,
        details_::make_const_wstring_list_t< L"SECService" > >,
      compile_time_rule_node<
        "管鲍电子教室 & GZYZ",
        details_::make_const_wstring_list_t<
          L"CRMSPre.exe", L"Student.exe", L"StudentTools.exe", L"EcrSetup.exe", L"ExamClient.exe", L"AnyTimeSrv.exe",
          L"NetLockerInstall.exe", L"NetLockerInstall64.exe", L"LockerManage64.exe" >,
        details_::make_const_wstring_list_t< L"AnytimeSrv" > >,
      compile_time_rule_node<
        "WeeBack",
        details_::make_const_wstring_list_t<
          L"TimerExitWindows.exe", L"termapps.exe", L"sdpi.exe", L"session_helper.exe", L"scrsender.exe", L"PMonitorNO.exe",
          L"notify-send.exe", L"moviesender.exe", L"moviereceiver.exe", L"audiosender.exe", L"audioreceiver.exe",
          L"mppgsvrc-ec.exe", L"mppgsvrc-ec-guard.exe", L"mppgclient-ec.exe", L"lock_screen.exe", L"krtc-client.exe",
          L"FileFilterTipError.exe", L"dyws_agent.exe", L"NdisFilterInstall.exe" >,
        details_::make_const_wstring_list_t< L"eClass Client Service", L"BrDevfer", L"ProcessProtect" > >,
      compile_time_rule_node<
        "Veyon",
        details_::make_const_wstring_list_t<
          L"veyon-worker.exe", L"veyon-configurator.exe", L"veyon-server.exe", L"veyon-cli.exe", L"veyon-wcli.exe",
          L"veyon-service.exe" >,
        details_::make_const_wstring_list_t< L"VeyonService" > >,
      compile_time_rule_node<
        "WorkWin", cpp_utils::type_list<>, cpp_utils::type_list<>, details_::is_workwin, details_::default_helper, [] static noexcept
    {
        cpp_utils::print( cpp_utils::no_formatting, "\n (i) \"WorkWin\" 无需恢复, 请直接启动软件.\n\n"sv );
    } > >;
    struct runtime_rule_node final
    {
        using regex_item_type  = std::pmr::vector< details_::scoped_wregex >;
        using string_item_type = std::pmr::vector< std::pmr::wstring >;
        regex_item_type proc_names{ unsynced_mem_pool };
        regex_item_type proc_paths{ unsynced_mem_pool };
        regex_item_type proc_signs{ unsynced_mem_pool };
        string_item_type serv_names{ unsynced_mem_pool };
        string_item_type crack_helpers{ unsynced_mem_pool };
        string_item_type restore_helpers{ unsynced_mem_pool };
    };
    runtime_rule_node custom_rules;
    namespace details_
    {
        template < bool Stateful, cpp_utils::const_string RawName >
        class config_node_interface
        {
          public:
            static constexpr auto is_stateful{ Stateful };
            static constexpr auto raw_name{ []
            {
                if constexpr ( Stateful ) {
                    return RawName;
                } else {
                    return ""_cs;
                }
            }() };
            auto load( this auto&& self, const std::string_view line )
            {
                if constexpr ( is_stateful ) {
                    self.load_( line );
                }
            }
            auto reload( this auto&& self, const std::string_view line )
            {
                using child_type = std::decay_t< decltype( self ) >;
                if constexpr ( is_stateful && requires( child_type d ) { d.reload_( line ); } ) {
                    self.reload_( line );
                } else {
                    self.load( line );
                }
            }
            auto sync( this auto&& self, std::ofstream& out )
            {
                if constexpr ( is_stateful ) {
                    out << cpp_utils::value_identity_v< cpp_utils::concat_const_string( "["_cs, raw_name, "]\n"_cs ) >.view();
                    self.sync_( out );
                }
            }
            auto before_load( this auto&& self )
            {
                using child_type = std::decay_t< decltype( self ) >;
                if constexpr ( requires( child_type d ) { d.before_load_(); } ) {
                    self.before_load_();
                }
            }
            auto after_load( this auto&& self )
            {
                using child_type = std::decay_t< decltype( self ) >;
                if constexpr ( requires( child_type d ) { d.after_load_(); } ) {
                    self.after_load_();
                }
            }
            auto init_ui( this auto&& self, cpp_utils::console_ui& parent_ui )
            {
                using child_type = std::decay_t< decltype( self ) >;
                if constexpr ( requires( child_type d ) { d.init_ui_( parent_ui ); } ) {
                    self.init_ui_( parent_ui );
                }
            }
            consteval auto ui_count( this auto&& self ) noexcept
            {
                using child_type = std::decay_t< decltype( self ) >;
                if constexpr ( requires {
                                   { child_type::ui_count_ } -> std::convertible_to< std::size_t >;
                               } )
                {
                    return cpp_utils::value_identity< static_cast< std::size_t >( child_type::ui_count_ ) >{};
                } else {
                    return cpp_utils::value_identity< 0uz >{};
                }
            }
        };
        template < typename T >
        struct is_stateful_config_node final
        {
            static constexpr auto value{ T::is_stateful };
        };
        template < typename T >
        struct is_valid_config_node final
        {
            static constexpr auto value{ requires {
                { T::is_stateful } -> std::convertible_to< bool >;
                cpp_utils::concat_const_string( T::raw_name );
            } && std::is_default_constructible_v< T > && std::is_same_v< std::decay_t< T >, T > };
        };
        template < cpp_utils::const_string RawName, cpp_utils::const_string DisplayName >
        struct option_info final
        {
            static constexpr auto raw_name{ RawName };
            static constexpr auto display_name{ DisplayName };
        };
        template < typename >
        struct is_option_info final : std::false_type
        { };
        template < cpp_utils::const_string RawName, cpp_utils::const_string DisplayName >
        struct is_option_info< option_info< RawName, DisplayName > > final : std::true_type
        { };
        template < typename... Options >
            requires( is_option_info< Options >::value && ... )
        struct options_info_table final
        {
            using base_type      = cpp_utils::type_list< Options... >;
            using raw_names_type = cpp_utils::type_list< cpp_utils::value_identity< Options::raw_name >... >;
            static consteval auto is_valid() noexcept
            {
                return raw_names_type::unique::size == raw_names_type::size
                    && []< cpp_utils::const_string... Items >(
                         const cpp_utils::type_list< cpp_utils::value_identity< Items >... > ) static constexpr noexcept
                {
                    return ( std::ranges::all_of( Items.view(), []( const char ch ) static constexpr noexcept
                    {
                        return !is_whitespace< char >( ch ) && ch != '\r' && ch != '\n';
                    } ) && ... );
                }( raw_names_type{} );
            }
            template < cpp_utils::const_string RawName >
            static consteval auto contains() noexcept
            {
                return raw_names_type::template contains< cpp_utils::value_identity< RawName > >;
            }
            template < cpp_utils::const_string RawName >
            static consteval auto index_of() noexcept
            {
                return raw_names_type::template find_first< cpp_utils::value_identity< RawName > >;
            }
        };
        template < typename >
        struct is_valid_options_info_table final : std::false_type
        { };
        template < typename... Options >
        struct is_valid_options_info_table< options_info_table< Options... > > final
          : std::conditional_t< options_info_table< Options... >::is_valid(), std::true_type, std::false_type >
        { };
        template < cpp_utils::const_string RawName, cpp_utils::const_string DisplayName, bool Atomic, typename OptionsInfoTable >
            requires( is_valid_options_info_table< OptionsInfoTable >::value == true )
        class options_config_node final : public config_node_interface< true, RawName >
        {
            friend config_node_interface< true, RawName >;
          private:
            using info_table_base_type_ = typename OptionsInfoTable::base_type;
            using value_type_           = std::conditional_t< Atomic, std::atomic_flag, bool >;
            std::array< value_type_, info_table_base_type_::size > data_{};
            static constexpr auto suffix_true_{ ": true"sv };
            static constexpr auto suffix_false_{ ": false"sv };
            static auto get_value_( const value_type_& value ) noexcept
            {
                if constexpr ( std::is_same_v< value_type_, std::atomic_flag > ) {
                    return value.test( std::memory_order_acquire );
                } else {
                    return value;
                }
            }
            static auto set_value_( value_type_& obj, const bool val ) noexcept
            {
                if constexpr ( std::is_same_v< value_type_, std::atomic_flag > ) {
                    switch ( val ) {
                        case false : obj.clear( std::memory_order_release ); break;
                        case true : ( void ) obj.test_and_set( std::memory_order_release ); break;
                    }
                } else {
                    obj = val;
                }
            }
            static auto set_value_then_notify_all_( value_type_& obj, const bool val ) noexcept
            {
                set_value_( obj, val );
                if constexpr ( std::is_same_v< value_type_, std::atomic_flag > ) {
                    obj.notify_all();
                }
            }
            auto load_( std::string_view line ) noexcept
            {
                bool value;
                if ( line.size() > suffix_true_.size() && line.ends_with( suffix_true_ ) ) [[likely]] {
                    line.remove_suffix( suffix_true_.size() );
                    value = true;
                } else if ( line.size() > suffix_false_.size() && line.ends_with( suffix_false_ ) ) [[likely]] {
                    line.remove_suffix( suffix_false_.size() );
                    value = false;
                } else {
                    return;
                }
                [ & ]< std::size_t... Is >( const std::index_sequence< Is... > ) noexcept
                {
                    (
                      [ & ]< std::size_t I > noexcept
                    {
                        if ( info_table_base_type_::template at< I >::raw_name.view() == line ) {
                            set_value_( std::get< I >( data_ ), value );
                            return true;
                        }
                        return false;
                    }.template operator()< Is >()
                      || ... );
                }( std::make_index_sequence< info_table_base_type_::size >{} );
            }
            static auto reload_( const std::string_view ) noexcept
            { }
            auto sync_( std::ofstream& out )
            {
                [ & ]< std::size_t... Is >( const std::index_sequence< Is... > )
                {
                    ( ( out << info_table_base_type_::template at< Is >::raw_name.view()
                            << ( get_value_( std::get< Is >( data_ ) ) == true ? suffix_true_ : suffix_false_ ) << '\n' ),
                      ... );
                }( std::make_index_sequence< info_table_base_type_::size >{} );
            }
            static auto make_flip_button_text_( const bool current_value ) noexcept
            {
                return current_value == true ? " > 禁用 "sv : " > 启用 "sv;
            }
            static auto flip_item_value_( const ui_func_args_type args, value_type_& value ) noexcept
            {
                const auto value_to_set{ !get_value_( value ) };
                set_value_then_notify_all_( value, value_to_set );
                args.parent_ui.set_text( args.node_index, make_flip_button_text_( value_to_set ) );
                return func_back;
            }
            static auto make_option_editor_ui_( std::array< value_type_, info_table_base_type_::size >& data_ )
            {
                cpp_utils::console_ui ui{ con, unsynced_mem_pool };
                ui.reserve( 2 + data_.size() * 2 )
                  .add_back( make_title_text< "[ 配  置 ]", 1 >.view() )
                  .add_back(
                    " < 返回 ", quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity );
                [ & ]< std::size_t... Is >( const std::index_sequence< Is... > )
                {
                    ( ui.add_back(
                          cpp_utils::value_identity_v< cpp_utils::concat_const_string(
                            "\n[ "_cs, info_table_base_type_::template at< Is >::display_name, " ]\n"_cs ) >.view() )
                        .add_back(
                          make_flip_button_text_( get_value_( std::get< Is >( data_ ) ) ),
                          std::bind_back< flip_item_value_ >( std::ref( std::get< Is >( data_ ) ) ),
                          cpp_utils::console_text::foreground_red | cpp_utils::console_text::foreground_green ),
                      ... );
                }( std::make_index_sequence< info_table_base_type_::size >{} );
                ui.show();
                return func_back;
            }
            auto init_ui_( cpp_utils::console_ui& ui )
            {
                ui.add_back( make_item_text< DisplayName >.view(), std::bind_back< make_option_editor_ui_ >( std::ref( data_ ) ) );
            }
            static constexpr auto ui_count_{ 1uz };
          public:
            template < cpp_utils::const_string OptionRawName >
                requires( OptionsInfoTable::template contains< OptionRawName >() )
            constexpr auto&& at( this auto&& self ) noexcept
            {
                return std::get< OptionsInfoTable::template index_of< OptionRawName >() >( self.data_ );
            }
            auto operator=( const options_config_node& ) -> options_config_node&     = delete;
            auto operator=( options_config_node&& ) noexcept -> options_config_node& = delete;
            options_config_node() noexcept                                           = default;
            options_config_node( const options_config_node& )                        = delete;
            options_config_node( options_config_node&& ) noexcept                    = delete;
            ~options_config_node() noexcept                                          = default;
        };
    }
    class options_title_ui final : public details_::config_node_interface< false, "options_title_ui" >
    {
        friend options_title_ui::config_node_interface;
      private:
        static auto init_ui_( cpp_utils::console_ui& ui )
        {
            ui.add_back( "\n[ 选项 ]\n" );
        }
        static constexpr auto ui_count_{ 1uz };
      public:
        options_title_ui() noexcept  = default;
        ~options_title_ui() noexcept = default;
    };
    using crack_restore_config = details_::options_config_node<
      "crack_restore", "破解与恢复", false,
      details_::options_info_table<
        details_::option_info< "crack_when_launching", "启动时破解" >,
        details_::option_info< "suspend_process", "挂起进程" > > >;
    using window_config = details_::options_config_node<
      "window", "窗口显示", true, details_::options_info_table< details_::option_info< "forced_show", "置顶窗口" > > >;
    class custom_rules_config final : public details_::config_node_interface< true, "custom_rules" >
    {
        friend custom_rules_config::config_node_interface;
      private:
        template < cpp_utils::const_wstring Flag, auto& Items >
            requires( std::ranges::none_of( Flag.view(), details_::is_whitespace< wchar_t > ) )
        struct custom_rule_binding_ final
        {
            static constexpr auto flag{ Flag };
            static constexpr auto&& items{ Items };
        };
        using custom_rule_bindings_ = cpp_utils::type_list<
          custom_rule_binding_< L"proc_name:"_cs, custom_rules.proc_names >,
          custom_rule_binding_< L"proc_path:"_cs, custom_rules.proc_paths >,
          custom_rule_binding_< L"proc_sign:"_cs, custom_rules.proc_signs >,
          custom_rule_binding_< L"serv_name:"_cs, custom_rules.serv_names >,
          custom_rule_binding_< L"crack_helper:"_cs, custom_rules.crack_helpers >,
          custom_rule_binding_< L"restore_helper:"_cs, custom_rules.restore_helpers > >;
        static auto load_( const std::string_view unconverted_line )
        {
            const auto converted{ cpp_utils::to_wstring( unconverted_line, CP_UTF8, unsynced_mem_pool ) };
            if ( !converted.has_value() ) [[unlikely]] {
                return;
            }
            const auto& line{ converted.value() };
            [ & ]< std::size_t... Is >( const std::index_sequence< Is... > )
            {
                ( [ & ]
                {
                    using current_binding = custom_rule_bindings_::at< Is >;
                    if ( line.starts_with( current_binding::flag.view() ) ) [[likely]] {
                        current_binding::items.emplace_back( std::ranges::find_if_not(
                          line.subview( current_binding::flag.view().size() ), details_::is_whitespace< wchar_t > ) );
                        return true;
                    }
                    return false;
                }() || ... );
            }( std::make_index_sequence< custom_rule_bindings_::size >{} );
        }
        static auto sync_( std::ofstream& out )
        {
            [ & ]< std::size_t... Is >( const std::index_sequence< Is... > )
            {
                ( [ & ]
                {
                    using current_binding = custom_rule_bindings_::at< Is >;
                    const auto converted{ cpp_utils::to_string( current_binding::flag.view(), CP_UTF8, unsynced_mem_pool ) };
                    if ( !converted.has_value() ) [[unlikely]] {
                        return;
                    }
                    const auto& flag{ converted.value() };
                    for ( const auto& item : current_binding::items ) {
                        const auto line{ cpp_utils::to_string( [ & ] noexcept -> decltype( auto )
                        {
                            if constexpr ( std::is_same_v< std::decay_t< decltype( item ) >, details_::scoped_wregex > ) {
                                return item.get_pattern();
                            } else {
                                return item;
                            }
                        }(), CP_UTF8, unsynced_mem_pool ) };
                        if ( line.has_value() ) [[likely]] {
                            out << flag << ' ' << line.value() << '\n';
                        }
                    }
                }(), ... );
            }( std::make_index_sequence< custom_rule_bindings_::size >{} );
        }
        static auto before_load_() noexcept
        {
            []< std::size_t... Is >( const std::index_sequence< Is... > ) static noexcept
            {
                ( []() static noexcept
                {
                    using current_binding = custom_rule_bindings_::at< Is >;
                    current_binding::items.clear();
                }(), ... );
            }( std::make_index_sequence< custom_rule_bindings_::size >{} );
        }
        static auto show_help_info_()
        {
            cpp_utils::console_ui ui{ con, unsynced_mem_pool };
            ui.reserve( 3 )
              .add_back( make_title_text< "[ 配  置 ]", 1 >.view() )
              .add_back( " < 返回 ", quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
              .add_back(
                "\n 自定义规则格式为 <flag>:{可选的若干空格}<item>\n"
                " 说明内容带有 (RX) 的 <flag>, 使用正则表达式\n"
                " (调用 libtre, 使用 POSIX ERE 语法, 大小写敏感).\n"
                " 合法的 <flag> 如下 (大小写敏感):\n"
                "  proc_name - 进程名称 (RX).\n"
                "   示例: ^abc_client[0-9]{10}\\.exe$\n"
                "  proc_path - 进程的文件的路径 (RX).\n"
                "   示例: ^C:\\\\[a-z][0-9]{9}\\\\[a-z]{10}\\.exe$\n"
                "  proc_sign - 进程的文件的数字签名的签名者 (RX).\n"
                "   示例: ^ABC eClass [a-z]{5}$\n"
                "  serv_name - 服务名称.\n"
                "   示例: abc_eclass\n"
                "  crack_helper - 破解时执行的程序的命令行.\n"
                "   示例: \"abc toolkit.exe\" crack\n"
                "  restore_helper - 恢复时执行的程序的命令行.\n"
                "   示例: \"abc toolkit.exe\" restore"sv )
              .show();
            return func_back;
        }
        static auto init_ui_( cpp_utils::console_ui& ui )
        {
            ui.add_back( "\n[ 自定义规则 ]\n" ).add_back( " > 查看帮助信息 ", show_help_info_ );
        }
        static constexpr auto ui_count_{ 2uz };
      public:
        custom_rules_config() noexcept  = default;
        ~custom_rules_config() noexcept = default;
    };
    using config_nodes_type = cpp_utils::type_list< options_title_ui, crack_restore_config, window_config, custom_rules_config >;
    static_assert( config_nodes_type::all_of< details_::is_valid_config_node > );
    static_assert( config_nodes_type::unique::size == config_nodes_type::size );
    config_nodes_type::apply< std::tuple > config_nodes{};
    namespace details_
    {
        auto get_config_node_raw_name_by_tag( std::string_view str ) noexcept
        {
            str.remove_prefix( 1 );
            str.remove_suffix( 1 );
            const auto head{ std::ranges::find_if_not( str, is_whitespace< char > ) };
            const auto tail{ std::ranges::find_if_not( str | std::views::reverse, is_whitespace< char > ).base() };
            if ( head >= tail ) [[unlikely]] {
                return std::string_view{};
            }
            return std::string_view{ head, tail };
        }
    }
    auto load_config( const bool is_reload )
    {
        std::ifstream config_file{ config_file_name, std::ios::in };
        if ( !config_file.good() ) [[unlikely]] {
            return;
        }
        const details_::file_locker _{ config_file.native_handle() };
        std::apply( []( auto&... config_node ) static
        {
            ( config_node.before_load(), ... );
        }, config_nodes );
        std::pmr::string line;
        using stateful_config_nodes_type = config_nodes_type::filter< details_::is_stateful_config_node >;
        using config_node_recorder_type
          = stateful_config_nodes_type::transform< std::add_pointer >::add_front< std::monostate >::apply< std::variant >;
        config_node_recorder_type current_config_node;
        while ( std::getline( config_file, line ) ) {
            const auto parsed_begin{ std::ranges::find_if_not( line, details_::is_whitespace< char > ) };
            const auto parsed_end{ std::ranges::find_if_not( line | std::views::reverse, details_::is_whitespace< char > ).base() };
            if ( parsed_begin >= parsed_end ) [[unlikely]] {
                continue;
            }
            const std::string_view parsed_line{ parsed_begin, parsed_end };
            if ( parsed_line.front() == '#' ) {
                continue;
            }
            if ( parsed_line.front() == '[' && parsed_line.back() == ']' && parsed_line.size() > "[]"sv.size() ) [[likely]] {
                current_config_node = std::monostate{};
                const auto current_raw_name{ details_::get_config_node_raw_name_by_tag( parsed_line ) };
                std::apply( [ & ]( auto&... config_node ) noexcept
                {
                    ( [ & ]< typename T >( T& current_node ) noexcept
                    {
                        if constexpr ( stateful_config_nodes_type::contains< T > ) {
                            if ( T::raw_name.view() == current_raw_name ) {
                                current_config_node = &current_node;
                                return true;
                            }
                        }
                        return false;
                    }( config_node ) || ... );
                }, config_nodes );
                continue;
            }
            if ( is_reload ) {
                current_config_node.visit( [ & ]< typename T >( const T node_ptr )
                {
                    if constexpr ( !std::is_same_v< T, std::monostate > ) {
                        node_ptr->reload( parsed_line );
                    }
                } );
            } else {
                current_config_node.visit( [ & ]< typename T >( const T node_ptr )
                {
                    if constexpr ( !std::is_same_v< T, std::monostate > ) {
                        node_ptr->load( parsed_line );
                    }
                } );
            }
        }
        std::apply( []( auto&... config_node ) static
        {
            ( config_node.after_load(), ... );
        }, config_nodes );
    }
    namespace details_
    {
        auto show_config_parsing_rules()
        {
            cpp_utils::console_ui ui{ con, unsynced_mem_pool };
            ui.reserve( 3 )
              .add_back( make_title_text< "[ 配  置 ]", 1 >.view() )
              .add_back( " < 返回 ", quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
              .add_back(
                "\n 配置以行作为单位解析.\n\n"
                " 以 # 开头的行是注释, 不进行解析.\n\n"
                " 各个配置项在配置文件中由不同标签区分,\n"
                " 标签的格式为 [<标签名>],\n"
                " <标签名> 与中括号间可以有若干空格.\n\n"
                " 如果匹配不到配置项,\n"
                " 则当前读取的标签到下一标签之间的内容都将被忽略.\n\n"
                " 解析时会忽略每行前导和末尾的空白字符.\n"
                " 如果当前行不是标签, 则该行将由上一个标签处理." )
              .show();
            return func_back;
        }
        auto sync_config()
        {
            cpp_utils::print( cpp_utils::no_formatting, 
              cpp_utils::value_identity_v< cpp_utils::concat_const_string(
                make_title_text< "[ 配  置 ]", 2 >, " -> 同步配置.\n\n"_cs ) >.view() );
            load_config( true );
            constexpr auto header{
              u8"# " INFO_FULL_NAME "\n# " INFO_GIT_TAG " (" INFO_GIT_BRANCH " " INFO_GIT_HASH ")\n# 本文件编码为 UTF-8。\n" };
            constexpr auto header_size{ std::char_traits< char8_t >::length( header ) * sizeof( char8_t ) };
            std::ofstream config_file{ config_file_name, std::ios::out | std::ios::trunc };
            if ( config_file.good() ) [[likely]] {
                const details_::file_locker _{ config_file.native_handle() };
                config_file.write( reinterpret_cast< const char* >( header ), header_size );
                std::apply( [ & ]( auto&... config_node )
                {
                    ( config_node.sync( config_file ), ... );
                }, config_nodes );
                config_file.flush();
            }
            static constexpr auto final_message{ [] static consteval noexcept
            {
                constexpr auto msg_start{ " (i) 同步配置"_cs };
                return std::array{
                  cpp_utils::value_identity_v< cpp_utils::concat_const_string( msg_start, "失败."_cs ) >.view(),
                  cpp_utils::value_identity_v< cpp_utils::concat_const_string( msg_start, "成功."_cs ) >.view() };
            }() };
            cpp_utils::print( cpp_utils::no_formatting, final_message[ static_cast< std::size_t >( config_file.good() ) ] );
            press_any_key_to_return();
            return func_back;
        }
        auto open_config_file()
        {
            cpp_utils::print( cpp_utils::no_formatting, 
              cpp_utils::value_identity_v< cpp_utils::concat_const_string(
                make_title_text< "[ 配  置 ]", 2 >,  " -> 尝试打开配置文件.\n\n"_cs ) >.view() );
            constexpr auto cmd_init{
              cpp_utils::concat_const_string( L"notepad.exe "_cs, cpp_utils::const_wstring{ config_file_name } ).data() };
            std::error_code ec;
            bool success{ false };
            if ( std::filesystem::exists( config_file_name, ec ) ) {
                auto cmd{ cmd_init };
                STARTUPINFOW startup_info{};
                PROCESS_INFORMATION proc_info;
                startup_info.cb = sizeof( startup_info );
                if ( CreateProcessW( nullptr, cmd.data(), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &startup_info, &proc_info ) )
                  [[likely]]
                {
                    CloseHandle( proc_info.hProcess );
                    CloseHandle( proc_info.hThread );
                    success = true;
                }
            }
            static constexpr auto final_message{ [] static consteval noexcept
            {
                constexpr auto msg_start{ " (i) 打开配置文件"_cs };
                return std::array{
                  cpp_utils::value_identity_v< cpp_utils::concat_const_string( msg_start, "失败."_cs ) >.view(),
                  cpp_utils::value_identity_v< cpp_utils::concat_const_string( msg_start, "成功."_cs ) >.view() };
            }() };
            cpp_utils::print( cpp_utils::no_formatting, final_message[ static_cast< std::size_t >( success ) ] );
            press_any_key_to_return();
            return func_back;
        }
    }
    auto config_ui()
    {
        std::apply( []( auto&... nodes ) static
        {
            cpp_utils::console_ui ui{ con, unsynced_mem_pool };
            ui.reserve( 5 + ( decltype( nodes.ui_count() )::value + ... ) )
              .add_back( make_title_text< "[ 配  置 ]", 1 >.view() )
              .add_back( " < 返回 ", quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
              .add_back( " > 查看解析规则 ", details_::show_config_parsing_rules )
              .add_back( " > 同步配置 ", details_::sync_config )
              .add_back( " > 打开配置文件 ", details_::open_config_file );
            ( nodes.init_ui( ui ), ... );
            ui.show();
        }, config_nodes );
        return func_back;
    }
    auto info()
    {
        cpp_utils::console_ui ui{ con, unsynced_mem_pool };
        ui.reserve( 3 )
          .add_back( make_title_text< "[ 关  于 ]", 1 >.view() )
          .add_back( " < 返回 ", quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
          .add_back(
            "\n[ 软件名 ]\n\n " INFO_FULL_NAME "\n (aka " INFO_SHORT_NAME ")\n\n[ 软件版本 ]\n\n 标签: " INFO_GIT_TAG
            "\n 分支: " INFO_GIT_BRANCH "\n 提交: " INFO_GIT_HASH "\n\n[ 许可证与版权 ]\n\n " INFO_LICENSE
            "\n\n " INFO_COPYRIGHT "\n\n[ 开源仓库 ]\n\n " INFO_REPO_URL )
          .show();
        return func_back;
    }
    namespace details_
    {
        template < cpp_utils::const_string Description, void ( *Func )() noexcept >
        struct func_item final
        {
            static constexpr auto description{ Description };
            static auto execute() noexcept
            {
                cpp_utils::print( cpp_utils::no_formatting, make_title_text< "[ 工 具 箱 ]", 2 >.view() );
                Func();
                cpp_utils::print( cpp_utils::no_formatting, "\n (i) 操作已完成."sv );
                press_any_key_to_return();
                return func_back;
            }
        };
        auto launch_cmd()
        {
            STARTUPINFOW startup_info{};
            PROCESS_INFORMATION proc_info;
            wchar_t cmd[]{ L"cmd.exe" };
            startup_info.cb = sizeof( startup_info );
            if ( CreateProcessW( nullptr, cmd, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &startup_info, &proc_info ) )
              [[likely]]
            {
                con.set_size( 120, 30, unsynced_mem_pool )
                  .fix_size( false )
                  .enable_window_maximize_ctrl( true )
                  .show_cursor( true )
                  .lock_text( false );
                SetConsoleScreenBufferSize( con.std_output_handle, { 120, std::numeric_limits< SHORT >::max() - 1 } );
                WaitForSingleObject( proc_info.hProcess, INFINITE );
                CloseHandle( proc_info.hProcess );
                CloseHandle( proc_info.hThread );
                con.set_charset( charset_id )
                  .set_size( console_width, console_height, unsynced_mem_pool )
                  .fix_size( true )
                  .enable_window_maximize_ctrl( false );
            }
            return func_back;
        }
        auto relaunch_explorer() noexcept
        {
            cpp_utils::print( cpp_utils::no_formatting, " -> 终止进程.\n"sv );
            if ( proc_snapshot.refresh() && proc_snapshot.valid() ) {
                ( void ) proc_snapshot.terminate_by_name( L"explorer.exe"sv );
            }
            cpp_utils::print( cpp_utils::no_formatting, " -> 启动进程.\n"sv );
            wchar_t cmd[]{ L"explorer.exe" };
            STARTUPINFOW startup_info{};
            PROCESS_INFORMATION proc_info{};
            startup_info.cb = sizeof( startup_info );
            if ( CreateProcessW( nullptr, cmd, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &startup_info, &proc_info ) )
              [[likely]]
            {
                CloseHandle( proc_info.hProcess );
                CloseHandle( proc_info.hThread );
            }
        }
        auto logoff() noexcept
        {
            cpp_utils::print(
              cpp_utils::no_formatting,
              " (i) 该功能可能丢失未保存的文件数据,\n"
              "     请确认文件均已保存!\n\n"
              " 请按任意键继续."sv );
            con.press_any_key_to_continue();
            cpp_utils::print( cpp_utils::no_formatting, "\n\n (i) 3s 后将注销当前用户账户.\n"sv );
            std::thread{ [] static noexcept
            {
                std::this_thread::sleep_for( 3s );
                ExitWindowsEx( EWX_LOGOFF, 0 );
            } }
              .detach();
        }
        using scoped_reg_key = std::unique_ptr< std::remove_pointer_t< HKEY >, decltype( []( const HKEY h ) static noexcept
        {
            RegCloseKey( h );
        } ) >;
        [[nodiscard]] auto is_sub_key_empty( const HKEY sub_key ) noexcept
        {
            wchar_t name[ 256 ];
            DWORD index{ 0 };
            auto size{ static_cast< DWORD >( std::size( name ) ) };
            while ( RegEnumValueW( sub_key, index, name, &size, nullptr, nullptr, nullptr, nullptr ) == ERROR_SUCCESS ) {
                if ( size != 0 ) {
                    return false;
                }
                ++index;
                size = static_cast< DWORD >( std::size( name ) );
            }
            index = 0;
            size  = static_cast< DWORD >( std::size( name ) );
            while ( RegEnumKeyExW( sub_key, index, name, &size, nullptr, nullptr, nullptr, nullptr ) == ERROR_SUCCESS ) {
                return false;
            }
            return true;
        }
        auto process_debugger_values( const HKEY root_key ) noexcept
        {
            wchar_t sub_key_name[ 256 ];
            DWORD index{ 0 };
            auto size{ static_cast< DWORD >( std::size( sub_key_name ) ) };
            while ( RegEnumKeyExW( root_key, index, sub_key_name, &size, nullptr, nullptr, nullptr, nullptr ) == ERROR_SUCCESS )
            {
                scoped_reg_key sub_key;
                if ( RegOpenKeyExW( root_key, sub_key_name, 0, KEY_QUERY_VALUE | KEY_SET_VALUE, std::out_ptr( sub_key ) )
                     == ERROR_SUCCESS )
                {
                    wchar_t value_data[ 1024 ];
                    DWORD data_size{ sizeof( value_data ) };
                    DWORD type{ 0 };
                    if ( RegQueryValueExW( sub_key.get(), L"Debugger", nullptr, &type, reinterpret_cast< LPBYTE >( value_data ), &data_size )
                           == ERROR_SUCCESS
                         && type == REG_SZ )
                    {
                        RegDeleteValueW( sub_key.get(), L"Debugger" );
                    }
                }
                ++index;
                size = static_cast< DWORD >( std::size( sub_key_name ) );
            }
        }
        auto process_empty_keys( const HKEY root_key ) noexcept
        {
            DWORD sub_key_count{ 0 };
            if ( RegQueryInfoKeyW(
                   root_key, nullptr, nullptr, nullptr, &sub_key_count, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr )
                 != ERROR_SUCCESS )
            {
                return;
            }
            wchar_t sub_key_name[ 256 ];
            for ( DWORD idx{ sub_key_count }; idx > 0; --idx ) {
                auto size{ static_cast< DWORD >( std::size( sub_key_name ) ) };
                if ( RegEnumKeyExW( root_key, idx - 1, sub_key_name, &size, nullptr, nullptr, nullptr, nullptr ) != ERROR_SUCCESS )
                {
                    continue;
                }
                scoped_reg_key sub_key;
                if ( RegOpenKeyExW( root_key, sub_key_name, 0, KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS, std::out_ptr( sub_key ) )
                     == ERROR_SUCCESS )
                {
                    if ( is_sub_key_empty( sub_key.get() ) ) {
                        RegDeleteKeyExW( root_key, sub_key_name, KEY_WOW64_64KEY, 0 );
                    }
                }
            }
        }
        auto process_ifeo_path( const wchar_t* const path ) noexcept
        {
            scoped_reg_key root_key;
            if ( RegOpenKeyExW(
                   HKEY_LOCAL_MACHINE, path, 0,
                   KEY_READ | KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE | KEY_SET_VALUE | DELETE | KEY_WOW64_64KEY,
                   std::out_ptr( root_key ) )
                 != ERROR_SUCCESS ) [[unlikely]]
            {
                return;
            }
            process_debugger_values( root_key.get() );
            process_empty_keys( root_key.get() );
        }
        auto cleanup_hijacked_debuggers() noexcept
        {
            process_ifeo_path( LR"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options)" );
            process_ifeo_path( LR"(SOFTWARE\WOW6432Node\Microsoft\Windows NT\CurrentVersion\Image File Execution Options)" );
        }
        auto restore_os_settings() noexcept
        {
            constexpr std::array policy_key_regs{
              LR"(Software\Policies\Microsoft\Windows\System)"sv, LR"(Software\Policies\Microsoft\Internet Explorer)"sv,
              LR"(Software\Policies\Microsoft\MMC)"sv, LR"(Software\Microsoft\Windows\CurrentVersion\Policies\System)"sv,
              LR"(Software\Microsoft\Windows\CurrentVersion\Policies\Explorer)"sv };
            constexpr std::pair< std::wstring_view, std::wstring_view > policy_value_regs[]{
              {LR"(SOFTWARE\Policies\Microsoft\Windows NT\SystemRestore)"sv, L"DisableSR"sv   },
              {LR"(Control Panel\Desktop)"sv,                                L"AutoEndTasks"sv}
            };
            constexpr std::pair< std::wstring_view, std::wstring_view > need_enabled_regs[]{
              {LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Advanced)"sv,                       L"ShowTaskViewButton"sv},
              {LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Advanced\Folder\Hidden\SHOWALL)"sv, L"CheckedValue"sv      }
            };
            cpp_utils::print( cpp_utils::no_formatting, " -> 撤销映像劫持.\n"sv );
            cleanup_hijacked_debuggers();
            cpp_utils::print( cpp_utils::no_formatting, " -> 撤销功能禁用.\n"sv );
            for ( const auto& policy_reg : policy_key_regs ) {
                ( void ) cpp_utils::delete_registry_tree_without_redirect( HKEY_CURRENT_USER, policy_reg );
            }
            for ( const auto& [ key, value ] : policy_value_regs ) {
                ( void ) cpp_utils::delete_registry_value_without_redirect( HKEY_LOCAL_MACHINE, key, value );
            }
            constexpr DWORD need_enabled_reg_value{ 1 };
            for ( const auto& [ key, value ] : need_enabled_regs ) {
                ( void ) cpp_utils::create_registry_value_without_redirect(
                  HKEY_LOCAL_MACHINE, key, value, cpp_utils::registry_flag::dword_type,
                  reinterpret_cast< const BYTE* >( &need_enabled_reg_value ), sizeof( need_enabled_reg_value ) );
            }
            cpp_utils::print( cpp_utils::no_formatting, " -> 撤销按键禁用 (注销当前用户账户后生效).\n"sv );
            ( void ) cpp_utils::delete_registry_value_without_redirect(
              HKEY_LOCAL_MACHINE, LR"(SYSTEM\CurrentControlSet\Control\Keyboard Layout)"sv, L"Scancode Map"sv );
            ( void ) cpp_utils::delete_registry_value_without_redirect(
              HKEY_CURRENT_USER, LR"(Software\Microsoft\Windows\CurrentVersion\Explorer\Advanced)"sv, L"DisabledHotkeys"sv );
            cpp_utils::print( cpp_utils::no_formatting, " -> 恢复 USB 存储器服务.\n"sv );
            constexpr DWORD start_type{ 3 };
            ( void ) cpp_utils::create_registry_value_without_redirect(
              HKEY_LOCAL_MACHINE, LR"(SYSTEM\CurrentControlSet\Services\USBSTOR)"sv, L"Start"sv,
              cpp_utils::registry_flag::dword_type, reinterpret_cast< const BYTE* >( &start_type ), sizeof( start_type ) );
        }
        auto reset_firewall_rules() noexcept
        {
            cpp_utils::print( cpp_utils::no_formatting, " -> 重置防火墙规则.\n"sv );
            STARTUPINFOW startup_info{};
            PROCESS_INFORMATION proc_info{};
            SECURITY_ATTRIBUTES sec_attrib{ sizeof( sec_attrib ), nullptr, TRUE };
            startup_info.cb = sizeof( startup_info );
            const auto nul_file_handle{
              CreateFileW( L"NUL", GENERIC_WRITE, FILE_SHARE_WRITE, &sec_attrib, OPEN_EXISTING, 0, nullptr ) };
            if ( nul_file_handle == INVALID_HANDLE_VALUE ) [[unlikely]] {
                return;
            }
            startup_info.dwFlags    = STARTF_USESTDHANDLES;
            startup_info.hStdInput  = con.std_input_handle;
            startup_info.hStdOutput = nul_file_handle;
            startup_info.hStdError  = nul_file_handle;
            wchar_t cmd[]{ L"netsh.exe advfirewall reset" };
            const auto success{ CreateProcessW(
              nullptr, cmd, nullptr, nullptr, TRUE, CREATE_NO_WINDOW | CREATE_UNICODE_ENVIRONMENT, nullptr, nullptr,
              &startup_info, &proc_info ) };
            CloseHandle( nul_file_handle );
            if ( success ) [[likely]] {
                WaitForSingleObject( proc_info.hProcess, INFINITE );
                CloseHandle( proc_info.hProcess );
                CloseHandle( proc_info.hThread );
            }
        }
        auto reset_hosts() noexcept
        {
            cpp_utils::print( cpp_utils::no_formatting, " -> 重置 Hosts.\n"sv );
#ifdef SCLTK_LEGACY
            const wow64_file_redirect_guard _;
#endif
            const auto hosts_path{ [] static
            {
                win32_file_path_buffer_t result;
                GetWindowsDirectoryW( result.data(), MAX_PATH );
                std::ranges::copy( LR"(\System32\drivers\etc\hosts)", std::ranges::find( result, L'\0' ) );
                return std::filesystem::path{ result.data() };
            }() };
            std::error_code ec;
            const auto original_perms{ std::filesystem::status( hosts_path, ec ).permissions() };
            if ( ec ) [[unlikely]] {
                return;
            }
            std::filesystem::permissions( hosts_path, std::filesystem::perms::all, std::filesystem::perm_options::replace, ec );
            std::filesystem::remove( hosts_path, ec );
            std::ofstream{ hosts_path, std::ios::out }.close();
            std::filesystem::permissions( hosts_path, original_perms, std::filesystem::perm_options::replace, ec );
        }
        extern auto clear_winhttp_proxy() noexcept -> void;
        extern auto clear_wininet_proxy() noexcept -> void;
        auto reset_network_proxy() noexcept
        {
            cpp_utils::print( cpp_utils::no_formatting, " -> 重置网络代理.\n"sv );
            clear_winhttp_proxy();
            clear_wininet_proxy();
        }
        using scoped_module_handle = std::unique_ptr< std::remove_pointer_t< HMODULE >, decltype( []( const HMODULE h )
        {
            FreeLibrary( h );
        } ) >;
        auto flush_dns() noexcept
        {
            cpp_utils::print( cpp_utils::no_formatting, " -> 刷新 DNS 缓存.\n"sv );
            const scoped_module_handle dnsapi{ LoadLibraryW( L"dnsapi.dll" ) };
            if ( dnsapi == nullptr ) [[unlikely]] {
                return;
            }
            const auto dns_flush_resolver_cache{
              std::bit_cast< BOOL( WINAPI* )() noexcept >( GetProcAddress( dnsapi.get(), "DnsFlushResolverCache" ) ) };
            if ( dns_flush_resolver_cache != nullptr ) [[likely]] {
                dns_flush_resolver_cache();
            }
        }
        auto set_device_state( const HDEVINFO device_info, SP_DEVINFO_DATA* const p_device_info_data, const bool enabled ) noexcept
        {
            SP_PROPCHANGE_PARAMS pcp;
            pcp.ClassInstallHeader.cbSize          = sizeof( SP_CLASSINSTALL_HEADER );
            pcp.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
            pcp.StateChange                        = enabled ? DICS_ENABLE : DICS_DISABLE;
            pcp.Scope                              = DICS_FLAG_GLOBAL;
            pcp.HwProfile                          = 0;
            if ( !SetupDiSetClassInstallParamsW( device_info, p_device_info_data, &pcp.ClassInstallHeader, sizeof( pcp ) ) )
              [[unlikely]]
            {
                return FALSE;
            }
            return SetupDiCallClassInstaller( DIF_PROPERTYCHANGE, device_info, p_device_info_data );
        }
        auto relaunch_network_adapters() noexcept
        {
            cpp_utils::print( cpp_utils::no_formatting, " -> 重启网络适配器 (可能失败).\n"sv );
            NET_LUID target_local_uid{};
            wchar_t target_name[ 256 ]{};
            ULONG out_buffer_length{};
            GetAdaptersAddresses( AF_UNSPEC, 0, nullptr, nullptr, &out_buffer_length );
            auto addresses{ static_cast< PIP_ADAPTER_ADDRESSES >( unsynced_mem_pool->allocate( out_buffer_length ) ) };
            if ( GetAdaptersAddresses( AF_UNSPEC, 0, nullptr, addresses, &out_buffer_length ) == NO_ERROR ) [[likely]] {
                auto current{ addresses };
                while ( current != nullptr ) {
                    if ( ( current->IfType == IF_TYPE_ETHERNET_CSMACD || current->IfType == IF_TYPE_IEEE80211 )
                         && current->OperStatus == IfOperStatusUp )
                    {
                        target_local_uid = current->Luid;
                        wcscpy_s( target_name, 256, current->Description );
                        break;
                    }
                    current = current->Next;
                }
            }
            unsynced_mem_pool->deallocate( addresses, out_buffer_length );
            if ( target_local_uid.Value == 0 ) [[unlikely]] {
                return;
            }
            const auto device_info{ SetupDiGetClassDevsW( &GUID_DEVCLASS_NET, nullptr, nullptr, DIGCF_PRESENT ) };
            if ( device_info == INVALID_HANDLE_VALUE ) [[unlikely]] {
                return;
            }
            SP_DEVINFO_DATA device_info_data;
            device_info_data.cbSize = sizeof( SP_DEVINFO_DATA );
            bool found{ false };
            for ( DWORD i{ 0 }; SetupDiEnumDeviceInfo( device_info, i, &device_info_data ); ++i ) {
                wchar_t buffer[ 256 ];
                if ( !SetupDiGetDeviceRegistryPropertyW(
                       device_info, &device_info_data, SPDRP_FRIENDLYNAME, nullptr, reinterpret_cast< PBYTE >( buffer ),
                       sizeof( buffer ), nullptr )
                     && !SetupDiGetDeviceRegistryPropertyW(
                       device_info, &device_info_data, SPDRP_DEVICEDESC, nullptr, reinterpret_cast< PBYTE >( buffer ),
                       sizeof( buffer ), nullptr ) )
                {
                    continue;
                }
                if ( wcscmp( buffer, target_name ) == 0 ) {
                    found = true;
                    break;
                }
            }
            if ( !found ) [[unlikely]] {
                SetupDiDestroyDeviceInfoList( device_info );
                return;
            }
            if ( set_device_state( device_info, &device_info_data, FALSE ) ) {
                std::this_thread::sleep_for( 3s );
                set_device_state( device_info, &device_info_data, TRUE );
            }
            SetupDiDestroyDeviceInfoList( device_info );
        }
        auto fix_network() noexcept
        {
            reset_firewall_rules();
            reset_hosts();
            reset_network_proxy();
            flush_dns();
            relaunch_network_adapters();
        }
        auto reset_jfglzs_config() noexcept
        {
            cpp_utils::print( cpp_utils::no_formatting, " -> 删除密码.\n"sv );
            ( void ) cpp_utils::delete_registry_value_without_redirect( HKEY_CURRENT_USER, L"Software"sv, L"n"sv );
            cpp_utils::print( cpp_utils::no_formatting, " -> 删除配置.\n"sv );
            ( void ) cpp_utils::delete_registry_tree_without_redirect( HKEY_CURRENT_USER, LR"(Software\jfglzs)"sv );
            cpp_utils::print( cpp_utils::no_formatting, " -> 删除自启动项.\n"sv );
            constexpr std::array autorun_items{ L"jfglzs"sv, L"jfglzsn"sv, L"jfglzsp"sv, L"prozs"sv, L"przs"sv };
            constexpr std::array notification_items{ L"StartupTNotijfglzsn"sv, L"StartupTNotiprozs"sv };
            for ( const auto& autorun_item : autorun_items ) {
                ( void ) cpp_utils::delete_registry_value_without_redirect(
                  HKEY_LOCAL_MACHINE, LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\Run)"sv, autorun_item );
                ( void ) cpp_utils::delete_registry_value_without_redirect(
                  HKEY_LOCAL_MACHINE, LR"(SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Run)"sv, autorun_item );
            }
            for ( const auto& notification_item : notification_items ) {
                ( void ) cpp_utils::delete_registry_value_without_redirect(
                  HKEY_CURRENT_USER, LR"(Software\Microsoft\Windows\CurrentVersion\RunNotification)"sv, notification_item );
            }
            cpp_utils::print( cpp_utils::no_formatting, " -> 删除备份.\n"sv );
            std::error_code ec;
            std::filesystem::remove_all( LR"(C:\Windows\jf)"sv, ec );
        }
        auto reset_common_web_browsers_policy() noexcept
        {
            cpp_utils::print( cpp_utils::no_formatting, " -> 删除注册表.\n"sv );
            constexpr std::array regs{
              LR"(SOFTWARE\Policies\Google\Chrome)"sv,   LR"(SOFTWARE\WOW6432Node\Policies\Google\Chrome)"sv,
              LR"(SOFTWARE\Policies\Microsoft\Edge)"sv,  LR"(SOFTWARE\WOW6432Node\Policies\Microsoft\Edge)"sv,
              LR"(SOFTWARE\Policies\Mozilla\Firefox)"sv, LR"(SOFTWARE\WOW6432Node\Policies\Mozilla\Firefox)"sv };
            for ( const auto& reg : regs ) {
                ( void ) cpp_utils::delete_registry_tree_without_redirect( HKEY_LOCAL_MACHINE, reg );
            }
        }
    }
    auto toolkit()
    {
        using funcs = cpp_utils::type_list<
          details_::func_item< "重启资源管理器", details_::relaunch_explorer >,
          details_::func_item< "注销当前用户账户", details_::logoff >,
          details_::func_item< "恢复操作系统设置", details_::restore_os_settings >,
          details_::func_item< "修复网络访问", details_::fix_network >,
          details_::func_item< "重置 \"机房管理助手\" 配置", details_::reset_jfglzs_config >,
          details_::func_item< "重置 Chrome & Edge & Firefox 管理策略", details_::reset_common_web_browsers_policy > >;
        cpp_utils::console_ui ui{ con, unsynced_mem_pool };
        ui.reserve( 4 + funcs::size )
          .add_back( make_title_text< "[ 工 具 箱 ]", 1 >.view() )
          .add_back( " < 返回 ", quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
          .add_back(
            "\n[ 快捷工具 ]\n\n"
            " (i) 请破解电子教室软件后再使用此处功能.\n" )
          .add_back( " > 启动命令提示符 ", details_::launch_cmd );
        [ & ]< typename... Items >( const cpp_utils::type_list< Items... > )
        {
            ( ui.add_back( make_item_text< Items::description >.view(), Items::execute ), ... );
        }( funcs{} );
        ui.show();
        return func_back;
    }
    namespace details_
    {
        enum class rule_executor_mode : bool
        {
            crack,
            restore
        };
        auto current_rule_executor_mode{ rule_executor_mode::crack };
    }
    template < typename... Backends >
        requires requires( const PROCESSENTRY32W& proc_entry ) {
            requires cpp_utils::as_concept< ( sizeof...( Backends ) != 0 ) >;
            { ( Backends::invoke_fn_is_target_proc || ... ) } -> std::convertible_to< bool >;
            { ( Backends::is_target_proc( proc_entry ) || ... ) } -> std::convertible_to< bool >;
            { ( Backends::invoke_fn_get_estimated_proc_handles_numbers || ... ) } -> std::convertible_to< bool >;
            { ( Backends::get_estimated_proc_handles_numbers() + ... ) } -> std::convertible_to< std::size_t >;
            { ( Backends::invoke_fn_enable_and_start_servs || ... ) } -> std::convertible_to< bool >;
            ( Backends::enable_and_start_servs(), ... );
            { ( Backends::invoke_fn_disable_and_stop_servs || ... ) } -> std::convertible_to< bool >;
            ( Backends::disable_and_stop_servs(), ... );
            { ( Backends::invoke_fn_crack_helper || ... ) } -> std::convertible_to< bool >;
            ( Backends::crack_helper(), ... );
            { ( Backends::invoke_fn_restore_helper || ... ) } -> std::convertible_to< bool >;
            ( Backends::restore_helper(), ... );
        }
    struct rule_executor final
    {
        static auto crack()
        {
            cpp_utils::print( cpp_utils::no_formatting, make_title_text< "[ 破  解 ]", 2 >.view() );
            constexpr const auto& crack_restore_config_node{ std::get< crack_restore_config >( config_nodes ) };
            constexpr const auto& enabled_suspend_process{ crack_restore_config_node.at< "suspend_process" >() };
            ( void ) proc_snapshot.refresh();
            if ( !proc_snapshot.valid() ) [[unlikely]] {
                cpp_utils::print( cpp_utils::no_formatting, " (!) 进程快照初始化错误!\n"sv );
                return;
            }
            if constexpr ( ( Backends::invoke_fn_is_target_proc || ... ) ) {
                std::pmr::vector< cpp_utils::scoped_handle > proc_handles( unsynced_mem_pool );
                proc_handles.reserve( (
                  []< typename Backend >() static
                {
                    if constexpr ( Backend::invoke_fn_get_estimated_proc_handles_numbers ) {
                        return Backend::get_estimated_proc_handles_numbers();
                    } else {
                        return 0uz;
                    }
                }.template operator()< Backends >()
                  + ... ) );
                ( void ) proc_snapshot.iterate( [ & ]( const PROCESSENTRY32W& proc_entry )
                {
                    if ( !(
                           [ & ]< typename Backend >
                    {
                        if constexpr ( Backend::invoke_fn_is_target_proc ) {
                            return Backend::is_target_proc( proc_entry );
                        } else {
                            return false;
                        }
                    }.template operator()< Backends >()
                           || ... ) )
                    {
                        return true;
                    }
                    auto proc_handle{ proc_snapshot.wrapped_nt_open_process(
                      proc_entry.th32ProcessID, PROCESS_TERMINATE | PROCESS_SUSPEND_RESUME ) };
                    if ( proc_handle != nullptr ) [[likely]] {
                        proc_handles.emplace_back( std::move( proc_handle ) );
                    }
                    return true;
                } );
                if ( enabled_suspend_process ) {
                    cpp_utils::print( cpp_utils::no_formatting, " -> 挂起进程.\n"sv );
                    for ( const auto& proc_handle : proc_handles ) {
                        proc_snapshot.get_nt_suspend_process()( proc_handle.get() );
                    }
                }
                cpp_utils::print( cpp_utils::no_formatting, " -> 终止进程.\n"sv );
                for ( const auto& proc_handle : proc_handles ) {
                    proc_snapshot.get_nt_terminate_process()( proc_handle.get(), 0 );
                }
            }
            if constexpr ( ( Backends::invoke_fn_disable_and_stop_servs || ... ) ) {
                cpp_utils::print( cpp_utils::no_formatting, " -> 禁用并停止服务.\n"sv );
                (
                  []< typename Backend >() static
                {
                    if constexpr ( Backend::invoke_fn_disable_and_stop_servs ) {
                        Backend::disable_and_stop_servs();
                    }
                }.template operator()< Backends >(),
                  ... );
            }
            if constexpr ( ( Backends::invoke_fn_crack_helper || ... ) ) {
                (
                  []< typename Backend >() static
                {
                    if constexpr ( Backend::invoke_fn_crack_helper ) {
                        Backend::crack_helper();
                    }
                }.template operator()< Backends >(),
                  ... );
            }
        }
        static auto restore()
        {
            cpp_utils::print( cpp_utils::no_formatting, make_title_text< "[ 恢  复 ]", 2 >.view() );
            if constexpr ( ( Backends::invoke_fn_enable_and_start_servs || ... ) ) {
                cpp_utils::print( cpp_utils::no_formatting, " -> 启用并启动服务.\n"sv );
                (
                  []< typename Backend >() static
                {
                    if constexpr ( Backend::invoke_fn_enable_and_start_servs ) {
                        Backend::enable_and_start_servs();
                    }
                }.template operator()< Backends >(),
                  ... );
            }
            if constexpr ( ( Backends::invoke_fn_restore_helper || ... ) ) {
                (
                  []< typename Backend >() static
                {
                    if constexpr ( Backend::invoke_fn_restore_helper ) {
                        Backend::restore_helper();
                    }
                }.template operator()< Backends >(),
                  ... );
            }
        }
        static auto entry()
        {
            switch ( details_::current_rule_executor_mode ) {
                case details_::rule_executor_mode::crack : crack(); break;
                case details_::rule_executor_mode::restore : restore(); break;
            }
            cpp_utils::print( cpp_utils::no_formatting, "\n (i) 操作已完成."sv );
            details_::press_any_key_to_return();
            return func_back;
        }
    };
    template < typename... BuiltinRuleNodes >
    struct builtin_rules_executor_backend final
    {
        using default_extra_proc_matcher_type = decltype( details_::default_extra_proc_matcher );
        using default_helper_type             = decltype( details_::default_helper );
        static constexpr auto proc_names{
          []< cpp_utils::const_wstring... ProcNames >(
            const cpp_utils::type_list< cpp_utils::value_identity< ProcNames >... > ) static consteval noexcept
        {
            return std::array< std::wstring_view, sizeof...( ProcNames ) >{ ProcNames.view()... };
        }( typename cpp_utils::type_list_concat_t< typename BuiltinRuleNodes::proc_names... >::unique{} ) };
        static constexpr auto serv_names{
          []< cpp_utils::const_wstring... ServNames >(
            const cpp_utils::type_list< cpp_utils::value_identity< ServNames >... > ) static consteval noexcept
        {
            return std::array< std::wstring_view, sizeof...( ServNames ) >{ ServNames.view()... };
        }( typename cpp_utils::type_list_concat_t< typename BuiltinRuleNodes::serv_names... >::unique{} ) };
        static constexpr auto invoke_fn_is_target_proc{ !proc_names.empty() };
        static auto is_target_proc( const PROCESSENTRY32W& proc_entry )
        {
            for ( const auto& proc_name : proc_names ) {
                if ( _wcsicmp( proc_entry.szExeFile, proc_name.data() ) == 0 ) {
                    return true;
                }
            }
            if ( (
                   [ & ]< typename Node >
            {
                if constexpr ( !std::is_same_v< decltype( Node::extra_proc_matcher ), default_extra_proc_matcher_type > ) {
                    return Node::extra_proc_matcher( proc_entry );
                } else {
                    return false;
                }
            }.template operator()< BuiltinRuleNodes >()
                   || ... ) )
            {
                return true;
            }
            return false;
        }
        static constexpr auto invoke_fn_get_estimated_proc_handles_numbers{ true };
        static consteval auto get_estimated_proc_handles_numbers() noexcept
        {
            return proc_names.size() * 1.5;
        }
        static constexpr auto invoke_fn_enable_and_start_servs{ !serv_names.empty() };
        static auto enable_and_start_servs() noexcept
        {
            for ( const auto& serv : serv_names ) {
                ( void ) cpp_utils::set_service_start_type( serv, cpp_utils::service_flag::auto_start );
                ( void ) cpp_utils::start_service_with_dependencies( serv, unsynced_mem_pool );
            }
        }
        static constexpr auto invoke_fn_disable_and_stop_servs{ !serv_names.empty() };
        static auto disable_and_stop_servs() noexcept
        {
            for ( const auto& serv : serv_names ) {
                ( void ) cpp_utils::set_service_start_type( serv, cpp_utils::service_flag::disabled_start );
                ( void ) cpp_utils::stop_service_with_dependencies( serv, unsynced_mem_pool );
            }
        }
        static constexpr auto invoke_fn_crack_helper{
          ( !std::is_same_v< decltype( BuiltinRuleNodes::crack_helper ), default_helper_type > || ... ) };
        static auto crack_helper()
        {
            (
              []< typename Node >() static
            {
                if constexpr ( !std::is_same_v< decltype( Node::crack_helper ), default_helper_type > ) {
                    Node::crack_helper();
                }
            }.template operator()< BuiltinRuleNodes >(),
              ... );
        }
        static constexpr auto invoke_fn_restore_helper{
          ( !std::is_same_v< decltype( BuiltinRuleNodes::restore_helper ), default_helper_type > || ... ) };
        static auto restore_helper()
        {
            (
              []< typename Node >() static
            {
                if constexpr ( !std::is_same_v< decltype( Node::restore_helper ), default_helper_type > ) {
                    Node::restore_helper();
                }
            }.template operator()< BuiltinRuleNodes >(),
              ... );
        }
    };
    struct custom_rule_executor_backend final
    {
        static constexpr auto invoke_fn_is_target_proc{ true };
        static auto is_target_proc( const PROCESSENTRY32W& proc_entry )
        {
            for ( const auto& proc_name_rx : custom_rules.proc_names ) {
                if ( proc_name_rx.match( proc_entry.szExeFile ) ) {
                    return true;
                }
            }
            const auto proc_handle{
              proc_snapshot.wrapped_nt_open_process( proc_entry.th32ProcessID, PROCESS_QUERY_LIMITED_INFORMATION ) };
            if ( proc_handle == nullptr ) {
                return false;
            }
            const auto proc_path{ details_::get_proc_path( proc_handle ) };
            if ( !proc_path.has_value() ) {
                return false;
            }
            const auto& [ proc_path_buffer, _ ]{ proc_path.value() };
            for ( const auto& proc_path_rx : custom_rules.proc_paths ) {
                if ( proc_path_rx.match( proc_path_buffer.data() ) ) {
                    return true;
                }
            }
            const auto proc_sign{ details_::get_sign_name( proc_path_buffer ) };
            if ( !proc_sign.has_value() ) {
                return false;
            }
            for ( const auto& proc_sign_rx : custom_rules.proc_signs ) {
                if ( proc_sign_rx.match( proc_sign.value().data() ) ) {
                    return true;
                }
            }
            return false;
        }
        static constexpr auto invoke_fn_get_estimated_proc_handles_numbers{ true };
        static auto get_estimated_proc_handles_numbers() noexcept
        {
            return custom_rules.proc_names.size() * 2;
        }
        static constexpr auto invoke_fn_enable_and_start_servs{ true };
        static auto enable_and_start_servs() noexcept
        {
            for ( const auto& serv_name : custom_rules.serv_names ) {
                ( void ) cpp_utils::set_service_start_type( serv_name, cpp_utils::service_flag::auto_start );
                ( void ) cpp_utils::start_service_with_dependencies( serv_name, unsynced_mem_pool );
            }
        }
        static constexpr auto invoke_fn_disable_and_stop_servs{ true };
        static auto disable_and_stop_servs() noexcept
        {
            for ( const auto& serv_name : custom_rules.serv_names ) {
                ( void ) cpp_utils::set_service_start_type( serv_name, cpp_utils::service_flag::disabled_start );
                ( void ) cpp_utils::stop_service_with_dependencies( serv_name, unsynced_mem_pool );
            }
        }
        static auto execute_helpers_( const std::pmr::vector< std::pmr::wstring >& helpers ) noexcept
        {
            cpp_utils::print( cpp_utils::no_formatting, " -> 执行自定义辅助程序.\n"sv );
            for ( const auto& helper : helpers ) {
                std::pmr::wstring cmd{ helper, unsynced_mem_pool };
                STARTUPINFOW startup_info{};
                PROCESS_INFORMATION proc_info{};
                startup_info.cb = sizeof( startup_info );
                if ( CreateProcessW( nullptr, cmd.data(), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &startup_info, &proc_info ) )
                  [[likely]]
                {
                    WaitForSingleObject( proc_info.hProcess, INFINITE );
                    CloseHandle( proc_info.hProcess );
                    CloseHandle( proc_info.hThread );
                }
            }
        }
        static constexpr auto invoke_fn_crack_helper{ true };
        static auto crack_helper() noexcept
        {
            execute_helpers_( custom_rules.crack_helpers );
        }
        static constexpr auto invoke_fn_restore_helper{ true };
        static auto restore_helper() noexcept
        {
            execute_helpers_( custom_rules.restore_helpers );
        }
    };
    using all_rules = rule_executor< builtin_rules::apply< builtin_rules_executor_backend >, custom_rule_executor_backend >;
    auto make_executor_mode_ui_text() noexcept
    {
        switch ( details_::current_rule_executor_mode ) {
            case details_::rule_executor_mode::crack : return "[ 破解 (点击切换) ]\n"sv;
            case details_::rule_executor_mode::restore : return "[ 恢复 (点击切换) ]\n"sv;
        }
    }
    auto flip_executor_mode( const ui_func_args_type args ) noexcept
    {
        switch ( details_::current_rule_executor_mode ) {
            case details_::rule_executor_mode::crack :
                details_::current_rule_executor_mode = details_::rule_executor_mode::restore;
                break;
            case details_::rule_executor_mode::restore :
                details_::current_rule_executor_mode = details_::rule_executor_mode::crack;
                break;
        }
        args.parent_ui.set_text( args.node_index, make_executor_mode_ui_text() );
        return func_back;
    }
    namespace details_
    {
        auto forced_show() noexcept
        {
            constexpr const auto& enabled{ std::get< window_config >( config_nodes ).at< "forced_show" >() };
            constexpr auto sleep_duration{ 50ms };
            constexpr auto condition_checker{ [] static noexcept
            {
                if ( enabled.test( std::memory_order_acquire ) == false ) {
                    con.cancel_forced_show();
                    enabled.wait( false, std::memory_order_acquire );
                }
                return false;
            } };
            con.forced_show_until( sleep_duration, condition_checker );
        }
    }
    auto create_parallel_tasks() noexcept
    {
        constexpr std::array parallel_tasks{ details_::forced_show };
        for ( const auto& parallel_task : parallel_tasks ) {
            std::thread{ parallel_task }.detach();
        }
    }
    namespace details_
    {
        auto crack_when_launching() noexcept
        {
            if ( std::get< crack_restore_config >( config_nodes ).at< "crack_when_launching" >() ) {
                con.clear( unsynced_mem_pool );
                all_rules::crack();
                cpp_utils::print(
                  cpp_utils::no_formatting,
                  "\n (i) 已执行全部破解规则,"
                  "\n     请按任意键进入主页." );
                con.press_any_key_to_continue();
            }
        }
    }
    auto do_extra_prep_tasks() noexcept
    {
        constexpr std::array tasks{ details_::crack_when_launching };
        for ( const auto& task : tasks ) {
            task();
        }
    }
    auto show_homepage_ui()
    {
        cpp_utils::console_ui ui{ scltk::con, scltk::unsynced_mem_pool };
        ui.reserve( 9 + scltk::builtin_rules::size )
          .add_back( scltk::make_title_text< "[ 主  页 ]", 1 >.view() )
          .add_back( " < 退出 ", scltk::quit, cpp_utils::console_text::foreground_red | cpp_utils::console_text::foreground_intensity )
          .add_back( " > 关于 ", scltk::info )
          .add_back( " > 配置 ", scltk::config_ui )
          .add_back( " > 工具箱\n", scltk::toolkit )
          .add_back(
            scltk::make_executor_mode_ui_text(), scltk::flip_executor_mode,
            cpp_utils::console_text::foreground_red | cpp_utils::console_text::foreground_green )
          .add_back( " > 全部执行 ", scltk::all_rules::entry )
          .add_back( " > * 自定义 * ", scltk::rule_executor< scltk::custom_rule_executor_backend >::entry );
        [ & ]< typename... Nodes >( const cpp_utils::type_list< Nodes... > )
        {
            ( ui.add_back(
                scltk::make_item_text< Nodes::display_name >.view(),
                scltk::rule_executor< scltk::builtin_rules_executor_backend< Nodes > >::entry ),
              ... );
        }( scltk::builtin_rules{} );
        ui.show();
    }
}
auto main() -> int
{
    using namespace std::string_view_literals;
    scltk::con.set_charset( scltk::charset_id )
      .set_title( scltk::generate_window_title().data() )
      .ignore_exit_signal( true )
      .show_cursor( false )
      .fix_size( true )
      .lock_text( true )
      .set_size( scltk::console_width, scltk::console_height, scltk::unsynced_mem_pool )
      .enable_window_maximize_ctrl( false )
      .enable_window_minimize_ctrl( false )
      .enable_window_close_ctrl( false );
    cpp_utils::print( cpp_utils::no_formatting, " -> 准备就绪."sv );
    scltk::disable_hotkey();
    scltk::enable_privileges();
    scltk::load_config( false );
    scltk::create_parallel_tasks();
    scltk::do_extra_prep_tasks();
    scltk::show_homepage_ui();
}
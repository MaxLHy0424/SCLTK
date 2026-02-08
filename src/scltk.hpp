#pragma once
#define WINVER       0x0601
#define _WIN32_WINNT 0x0601
#define NOCOMM
#include <cpp_utils/const_string.hpp>
#include <cpp_utils/math.hpp>
#include <cpp_utils/meta.hpp>
#include <cpp_utils/windows_app_tools.hpp>
#include <cpp_utils/windows_console_ui.hpp>
#include <filesystem>
#include <fstream>
#include "info.hpp"
namespace scltk
{
    using namespace std::chrono_literals;
    using namespace std::string_view_literals;
    inline constexpr SHORT console_width{ 50 };
    inline constexpr SHORT console_height{ 25 };
    inline constexpr UINT charset_id{ 936 };
    inline constexpr auto default_thread_sleep_time{ 200ms };
    inline constexpr auto config_file_name{ L"config.ini" };
    inline constexpr auto func_back{ cpp_utils::console_ui::func_back };
    inline constexpr auto func_exit{ cpp_utils::console_ui::func_exit };
    inline const cpp_utils::console con;
    inline const auto unsynced_mem_pool{ [] static noexcept
    {
        static std::pmr::unsynchronized_pool_resource pool{
          std::pmr::pool_options{ .max_blocks_per_chunk{ 1024 }, .largest_required_pool_block{ 4096 } },
          std::pmr::new_delete_resource()
        };
        std::pmr::set_default_resource( &pool );
        return static_cast< std::pmr::memory_resource* >( &pool );
    }() };
    static_assert( default_thread_sleep_time.count() != 0 );
    using ui_func_args_t = cpp_utils::console_ui::func_args;
    inline auto quit() noexcept
    {
        return func_exit;
    }
    inline auto relaunch() noexcept
    {
        cpp_utils::clone_self();
        return func_exit;
    }
    template < cpp_utils::const_string Title, std::size_t NewLineCount >
    inline constexpr auto make_title_text{ cpp_utils::concat_const_string(
      cpp_utils::make_repeated_const_string< ' ', ( static_cast< std::size_t >( console_width ) - Title.size() + 1 ) / 2 >(),
      Title, cpp_utils::make_repeated_const_string< '\n', NewLineCount >() ) };
    template < cpp_utils::const_string Text >
    inline constexpr auto make_button_text{
      cpp_utils::concat_const_string( cpp_utils::const_string{ " > " }, Text, cpp_utils::const_string{ " " } ) };
    namespace details
    {
        inline auto press_any_key_to_return() noexcept
        {
            std::print( "\n\n 请按任意键返回." );
            con.press_any_key_to_continue();
        }
        template < cpp_utils::character CharT >
        inline constexpr auto is_whitespace( const CharT ch ) noexcept
        {
            switch ( ch ) {
                case static_cast< CharT >( '\t' ) :
                case static_cast< CharT >( '\v' ) :
                case static_cast< CharT >( '\f' ) :
                case static_cast< CharT >( ' ' ) : return true;
            }
            return false;
        }
        template < typename, typename >
        struct sort_const_string_comp;
        template < cpp_utils::basic_const_string A, cpp_utils::basic_const_string B >
            requires std::same_as< typename decltype( A )::char_t, typename decltype( B )::char_t >
        struct sort_const_string_comp< cpp_utils::value_identity< A >, cpp_utils::value_identity< B > > final
        {
            static inline constexpr auto value{ A.view() < B.view() };
        };
        template < cpp_utils::const_wstring... Items >
        using make_ordered_const_wstring_list_t =
          typename cpp_utils::type_list< cpp_utils::value_identity< Items >... >::template sort< sort_const_string_comp >;
        inline constexpr auto empty_lambda{ [] static noexcept { } };
        inline auto terminate_jfglzs_daemon() noexcept
        {
            constexpr auto close_handle{ []( const HANDLE handle ) static noexcept { CloseHandle( handle ); } };
            using handle_wrapper_t = const std::unique_ptr< std::remove_pointer_t< HANDLE >, decltype( close_handle ) >;
            handle_wrapper_t process_snapshot{ CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 ) };
            if ( process_snapshot.get() == INVALID_HANDLE_VALUE ) {
                return;
            }
            constexpr auto is_lower_case{ []( const wchar_t ch ) static noexcept { return ch >= L'a' && ch <= L'z'; } };
            constexpr auto needle{ L"Program Files"sv };
            const std::boyer_moore_horspool_searcher searcher{ needle.begin(), needle.end() };
            PROCESSENTRY32W process_entry{};
            process_entry.dwSize = sizeof( process_entry );
            if ( Process32FirstW( process_snapshot.get(), &process_entry ) ) {
                do {
                    std::wstring_view name{ process_entry.szExeFile };
                    if ( name.size() != L"xxxxx.exe"sv.size() ) {
                        continue;
                    }
                    name.remove_suffix( L".exe"sv.size() );
                    if ( std::ranges::all_of( name, is_lower_case ) ) {
                        handle_wrapper_t process_handle{ OpenProcess(
                          PROCESS_TERMINATE | PROCESS_QUERY_LIMITED_INFORMATION, FALSE, process_entry.th32ProcessID ) };
                        if ( process_handle == nullptr ) {
                            continue;
                        }
                        DWORD size{ MAX_PATH };
                        std::array< wchar_t, MAX_PATH > buffer{};
                        QueryFullProcessImageNameW( process_handle.get(), 0, buffer.data(), &size );
                        if ( std::search( buffer.begin(), buffer.end(), searcher ) != buffer.end() ) {
                            TerminateProcess( process_handle.get(), 1 );
                        }
                    }
                } while ( Process32NextW( process_snapshot.get(), &process_entry ) );
            }
        }
        inline auto terminate_yjhlq_daemon() noexcept
        {
            constexpr auto close_handle{ []( const HANDLE handle ) static noexcept { CloseHandle( handle ); } };
            using handle_wrapper_t = const std::unique_ptr< std::remove_pointer_t< HANDLE >, decltype( close_handle ) >;
            handle_wrapper_t process_snapshot{ CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 ) };
            if ( process_snapshot.get() == INVALID_HANDLE_VALUE ) {
                return;
            }
            constexpr auto needle{ LR"(ProgramData\Microsoft\Windows\Caches)"sv };
            const std::boyer_moore_horspool_searcher searcher{ needle.begin(), needle.end() };
            PROCESSENTRY32W process_entry{};
            process_entry.dwSize = sizeof( process_entry );
            if ( Process32FirstW( process_snapshot.get(), &process_entry ) ) {
                do {
                    if ( std::wstring_view{ process_entry.szExeFile }.starts_with( L"runtime_"sv ) ) {
                        handle_wrapper_t process_handle{ OpenProcess(
                          PROCESS_TERMINATE | PROCESS_QUERY_LIMITED_INFORMATION, FALSE, process_entry.th32ProcessID ) };
                        if ( process_handle == nullptr ) {
                            continue;
                        }
                        DWORD size{ MAX_PATH };
                        std::array< wchar_t, MAX_PATH > buffer{};
                        QueryFullProcessImageNameW( process_handle.get(), 0, buffer.data(), &size );
                        if ( std::search( buffer.begin(), buffer.end(), searcher ) != buffer.end() ) {
                            TerminateProcess( process_handle.get(), 1 );
                        }
                    }
                } while ( Process32NextW( process_snapshot.get(), &process_entry ) );
            }
        }
    }
    template < cpp_utils::const_string DisplayName, cpp_utils::same_as_type_list Procs, cpp_utils::same_as_type_list Servs,
               std::invocable auto CrackHelper = details::empty_lambda, std::invocable auto RestoreHelper = details::empty_lambda >
    struct compile_time_rule_node final
    {
        static inline constexpr auto display_name{ DisplayName };
        static inline constexpr auto crack_helper{ CrackHelper };
        static inline constexpr auto restore_helper{ RestoreHelper };
        using procs = Procs;
        using servs = Servs;
    };
    struct runtime_rule_node final
    {
        using item_t = std::pmr::vector< std::pmr::wstring >;
        item_t procs{ unsynced_mem_pool };
        item_t servs{ unsynced_mem_pool };
        item_t crack_helpers{ unsynced_mem_pool };
        item_t restore_helpers{ unsynced_mem_pool };
    };
    using builtin_rules_t = cpp_utils::type_list<
      compile_time_rule_node<
        "机房管理助手",
        details::make_ordered_const_wstring_list_t<
          L"yz.exe", L"jfglzs.exe", L"jfglzsn.exe", L"jfglzsp.exe", L"przs.exe", L"zmserv.exe", L"zmsrv.exe" >,
        details::make_ordered_const_wstring_list_t< L"zmserv" >, details::terminate_jfglzs_daemon >,
      compile_time_rule_node<
        "市一中伊金霍洛校区机房管理程序",
        details::make_ordered_const_wstring_list_t< L"ComputerClassroom_Client.exe", L"MonitorProcess.exe" >,
        details::make_ordered_const_wstring_list_t<>, details::terminate_yjhlq_daemon >,
      compile_time_rule_node<
        "极域电子教室",
        details::make_ordered_const_wstring_list_t<
          L"StudentMain.exe", L"DispcapHelper.exe", L"VRCwPlayer.exe", L"InstHelpApp.exe", L"InstHelpApp64.exe",
          L"TDOvrSet.exe", L"GATESRV.exe", L"ProcHelper64.exe", L"MasterHelper.exe" >,
        details::make_ordered_const_wstring_list_t< L"STUDSRV" >,
        [] static noexcept
    {
        ( void ) cpp_utils::stop_service_with_dependencies( L"TDNetFilter" );
        ( void ) cpp_utils::stop_service_with_dependencies( L"TDFileFilter" );
    },
        [] static noexcept
    {
        ( void ) cpp_utils::start_service_with_dependencies( L"TDNetFilter" );
        ( void ) cpp_utils::start_service_with_dependencies( L"TDFileFilter" );
    } >,
      compile_time_rule_node<
        "联想智能云教室",
        details::make_ordered_const_wstring_list_t<
          L"vncviewer.exe", L"tvnserver32.exe", L"WfbsPnpInstall.exe", L"WFBSMon.exe", L"WFBSMlogon.exe", L"WFBSSvrLogShow.exe",
          L"ResetIp.exe", L"FuncForWIN64.exe", L"Fireware.exe", L"BCDBootCopy.exe", L"refreship.exe", L"WFDeskShow.exe",
          L"lenovoLockScreen.exe", L"PortControl64.exe", L"DesktopCheck.exe", L"DeploymentManager.exe", L"DeploymentAgent.exe",
          L"XYNTService.exe" >,
        details::make_ordered_const_wstring_list_t< L"BSAgentSvr", L"tvnserver", L"WFBSMlogon" > >,
      compile_time_rule_node<
        "红蜘蛛多媒体网络教室",
        details::make_ordered_const_wstring_list_t<
          L"rscheck.exe", L"checkrs.exe", L"REDAgent.exe", L"PerformanceCheck.exe", L"edpaper.exe", L"Adapter.exe",
          L"repview.exe", L"FormatPaper.exe" >,
        details::make_ordered_const_wstring_list_t< L"appcheck2", L"checkapp2" > >,
      compile_time_rule_node< "伽卡他卡电子教室", details::make_ordered_const_wstring_list_t< L"Student.exe", L"Smonitor.exe" >,
                              details::make_ordered_const_wstring_list_t< L"Smsvc" > >,
      compile_time_rule_node<
        "凌波网络教室", details::make_ordered_const_wstring_list_t< L"sbkup.exe", L"wsf.exe", L"NCStu.exe", L"NCCmn.dll" >,
        details::make_ordered_const_wstring_list_t< L"Windows Application and Components Data Backup Support Service" > >,
      compile_time_rule_node<
        "Veyon",
        details::make_ordered_const_wstring_list_t<
          L"veyon-worker.exe", L"veyon-configurator.exe", L"veyon-server.exe", L"veyon-cli.exe", L"veyon-wcli.exe",
          L"veyon-master.exe", L"veyon-service.exe" >,
        details::make_ordered_const_wstring_list_t< L"VeyonService" > > >;
    runtime_rule_node custom_rules;
    namespace details
    {
        template < cpp_utils::const_string RawName >
        struct config_node_raw_name
        {
            static inline constexpr auto raw_name{ RawName };
        };
        class config_node_impl;
        template < typename T >
        struct is_parsable_config_node final
        {
            static inline constexpr auto value{ ( std::is_base_of_v< config_node_impl, T > && requires { T::raw_name; } ) };
        };
        template < typename T >
        inline constexpr auto is_parsable_config_node_v{ is_parsable_config_node< T >::value };
        class config_node_impl
        {
          public:
            auto load( this auto&& self, const std::string_view line )
            {
                using child_t = std::decay_t< decltype( self ) >;
                if constexpr ( is_parsable_config_node_v< child_t > ) {
                    self.load_( line );
                }
            }
            auto reload( this auto&& self, const std::string_view line )
            {
                using child_t = std::decay_t< decltype( self ) >;
                if constexpr ( is_parsable_config_node_v< child_t > && requires( child_t obj ) { obj.reload_( line ); } ) {
                    self.reload_( line );
                } else {
                    self.load( line );
                }
            }
            auto sync( this auto&& self, std::ofstream& out )
            {
                using child_t = std::decay_t< decltype( self ) >;
                if constexpr ( is_parsable_config_node_v< child_t > ) {
                    out << cpp_utils::value_identity_v< cpp_utils::concat_const_string(
                      cpp_utils::const_string{ "[" }, child_t::raw_name, cpp_utils::const_string{ "]\n" } ) >.view();
                    self.sync_( out );
                }
            }
            auto before_load( this auto&& self )
            {
                using child_t = std::decay_t< decltype( self ) >;
                if constexpr ( is_parsable_config_node_v< child_t > && requires( child_t obj ) { obj.before_load_(); } ) {
                    self.before_load_();
                }
            }
            auto after_load( this auto&& self )
            {
                using child_t = std::decay_t< decltype( self ) >;
                if constexpr ( is_parsable_config_node_v< child_t > && requires( child_t obj ) { obj.after_load_(); } ) {
                    self.after_load_();
                }
            }
            auto init_ui( this auto&& self, cpp_utils::console_ui& parent_ui )
            {
                using child_t = std::decay_t< decltype( self ) >;
                if constexpr ( requires( child_t obj ) { obj.init_ui_( parent_ui ); } ) {
                    self.init_ui_( parent_ui );
                }
            }
            consteval auto request_ui_count( this auto&& self ) noexcept
            {
                using child_t = std::decay_t< decltype( self ) >;
                if constexpr ( requires( child_t obj ) {
                                   { child_t::ui_count_ } -> std::convertible_to< std::size_t >;
                               } )
                {
                    return std::integral_constant< std::size_t, child_t::ui_count_ >{};
                } else {
                    return std::integral_constant< std::size_t, 0uz >{};
                }
            }
        };
        template < cpp_utils::const_string RawName, cpp_utils::const_string DisplayName >
        struct option_info final
        {
            static inline constexpr auto raw_name{ RawName };
            static inline constexpr auto display_name{ DisplayName };
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
            using base_t      = cpp_utils::type_list< Options... >;
            using raw_names_t = cpp_utils::type_list< cpp_utils::value_identity< Options::raw_name >... >;
            static consteval auto is_valid()
            {
                return raw_names_t::unique::size == raw_names_t::size;
            }
            template < cpp_utils::const_string RawName >
            static consteval auto contains()
            {
                return raw_names_t::template contains< cpp_utils::value_identity< RawName > >;
            }
            template < cpp_utils::const_string RawName >
            static consteval auto index_of()
            {
                return raw_names_t::template find_first< cpp_utils::value_identity< RawName > >;
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
        class options_config_node final
          : public config_node_raw_name< RawName >
          , public config_node_impl
        {
            friend config_node_impl;
          private:
            using info_table_base_t_ = typename OptionsInfoTable::base_t;
            using value_t_           = std::conditional_t< Atomic, std::atomic< bool >, bool >;
            std::array< value_t_, info_table_base_t_::size > data_{};
            static inline constexpr auto str_enabled_{ ": enabled"sv };
            static inline constexpr auto str_disabled_{ ": disabled"sv };
            auto load_( std::string_view line )
            {
                bool value;
                if ( line.size() > str_enabled_.size() && line.ends_with( str_enabled_ ) ) {
                    line.remove_suffix( str_enabled_.size() );
                    value = true;
                } else if ( line.size() > str_disabled_.size() && line.ends_with( str_disabled_ ) ) {
                    line.remove_suffix( str_disabled_.size() );
                    value = false;
                } else {
                    return;
                }
                [ & ]< std::size_t... Is >( const std::index_sequence< Is... > ) noexcept
                {
                    (
                      [ & ]< std::size_t I > noexcept
                    {
                        if ( info_table_base_t_::template at< I >::raw_name.view() == line ) {
                            std::get< I >( data_ ) = value;
                            return true;
                        }
                        return false;
                    }.template operator()< Is >()
                      || ... );
                }( std::make_index_sequence< info_table_base_t_::size >{} );
            }
            static auto reload_( const std::string_view ) noexcept
            { }
            auto sync_( std::ofstream& out )
            {
                [ & ]< std::size_t... Is >( const std::index_sequence< Is... > )
                {
                    ( ( out << info_table_base_t_::template at< Is >::raw_name.view()
                            << ( std::get< Is >( data_ ) == true ? str_enabled_ : str_disabled_ ) << '\n' ),
                      ... );
                }( std::make_index_sequence< info_table_base_t_::size >{} );
            }
            static auto get_value_( const value_t_& value )
            {
                if constexpr ( std::is_same_v< value_t_, std::atomic< bool > > ) {
                    return value.load( std::memory_order_acquire );
                } else {
                    return value;
                }
            }
            static auto& set_value_( value_t_& obj, const bool val )
            {
                if constexpr ( std::is_same_v< value_t_, std::atomic< bool > > ) {
                    obj.store( val, std::memory_order_release );
                    return obj;
                } else {
                    return obj = val;
                }
            }
            static auto make_flip_button_text_( const value_t_& value )
            {
                return get_value_( value ) == true ? " > 禁用 "sv : " > 启用 "sv;
            }
            static auto flip_item_value_( const ui_func_args_t args, value_t_& value )
            {
                args.parent_ui.set_text( args.node_index, make_flip_button_text_( set_value_( value, !get_value_( value ) ) ) );
                return func_back;
            }
            static auto make_option_editor_ui_( std::array< value_t_, info_table_base_t_::size >& data_ )
            {
                cpp_utils::console_ui ui{ con, unsynced_mem_pool };
                ui.reserve( 2 + data_.size() * 2 )
                  .add_back( make_title_text< "[ 配  置 ]", 2 >.view() )
                  .add_back(
                    " < 返回 "sv, quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity );
                [ & ]< std::size_t... Is >( const std::index_sequence< Is... > )
                {
                    ( ui.add_back(
                          cpp_utils::value_identity_v< cpp_utils::concat_const_string(
                            cpp_utils::const_string{ "\n[ " }, info_table_base_t_::template at< Is >::display_name,
                            cpp_utils::const_string{ " ]\n" } ) >.view() )
                        .add_back(
                          make_flip_button_text_( std::get< Is >( data_ ) ),
                          std::bind_back( flip_item_value_, std::ref( std::get< Is >( data_ ) ) ),
                          cpp_utils::console_text::foreground_red | cpp_utils::console_text::foreground_green ),
                      ... );
                }( std::make_index_sequence< info_table_base_t_::size >{} );
                ui.show();
                return func_back;
            }
            auto init_ui_( cpp_utils::console_ui& ui )
            {
                ui.add_back( make_button_text< DisplayName >.view(), std::bind_back( make_option_editor_ui_, std::ref( data_ ) ) );
            }
            static inline constexpr auto ui_count_{ 1uz };
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
    class options_title_ui final : public details::config_node_impl
    {
        friend details::config_node_impl;
      private:
        static auto init_ui_( cpp_utils::console_ui& ui )
        {
            ui.add_back( "\n[ 选项 ]\n"sv );
        }
        static inline constexpr auto ui_count_{ 1uz };
      public:
        options_title_ui() noexcept  = default;
        ~options_title_ui() noexcept = default;
    };
    using crack_restore_config = details::options_config_node<
      "crack_restore", "破解与恢复", false,
      details::options_info_table<
        details::option_info< "hijack_procs", "劫持进程" >,
        details::option_info< "set_servs_start_type", "设置服务启动类型" > > >;
    using window_config = details::options_config_node<
      "window", "窗口显示", true,
      details::options_info_table<
        details::option_info< "force_show", "置顶窗口 (非实时)" >,
        details::option_info< "simple_titlebar", "极简标题栏 (非实时)" >,
        details::option_info< "translucent", "半透明 (非实时)" > > >;
    using performance_config = details::options_config_node<
      "performance", "性能", false,
      details::options_info_table< details::option_info< "no_hot_reload", "禁用非实时热重载 (下次启动时生效)" > > >;
    class custom_rules_config final
      : public details::config_node_impl
      , public details::config_node_raw_name< "custom_rules" >
    {
        friend details::config_node_impl;
      private:
        static inline constexpr auto flag_proc_{ L"proc:"sv };
        static inline constexpr auto flag_serv_{ L"serv:"sv };
        static inline constexpr auto flag_crack_helper_{ L"crack_helper:"sv };
        static inline constexpr auto flag_restore_helper_{ L"restore_helper:"sv };
        static_assert( []( auto... strings ) static consteval noexcept {
            return ( std::ranges::none_of( strings, details::is_whitespace< wchar_t > ) && ... );
        }( flag_proc_, flag_serv_, flag_crack_helper_, flag_restore_helper_ ) );
        static auto load_( const std::string_view unconverted_line )
        {
            const auto converted_line{ cpp_utils::to_wstring( unconverted_line, charset_id ) };
            const std::wstring_view line{ converted_line };
            if ( line.size() > flag_proc_.size() && line.starts_with( flag_proc_ ) ) {
                custom_rules.procs.emplace_back(
                  std::ranges::find_if_not( line.substr( flag_proc_.size() ), details::is_whitespace< wchar_t > ) );
                return;
            }
            if ( line.size() > flag_serv_.size() && line.starts_with( flag_serv_ ) ) {
                custom_rules.servs.emplace_back(
                  std::ranges::find_if_not( line.substr( flag_serv_.size() ), details::is_whitespace< wchar_t > ) );
                return;
            }
            if ( line.size() > flag_crack_helper_.size() && line.starts_with( flag_crack_helper_ ) ) {
                custom_rules.crack_helpers.emplace_back(
                  std::ranges::find_if_not( line.substr( flag_crack_helper_.size() ), details::is_whitespace< wchar_t > ) );
                return;
            }
            if ( line.size() > flag_restore_helper_.size() && line.starts_with( flag_restore_helper_ ) ) {
                custom_rules.restore_helpers.emplace_back(
                  std::ranges::find_if_not( line.substr( flag_restore_helper_.size() ), details::is_whitespace< wchar_t > ) );
                return;
            }
        }
        static auto sync_( std::ofstream& out )
        {
            const auto flag_proc{ cpp_utils::to_string( flag_proc_, charset_id, unsynced_mem_pool ) };
            const auto flag_serv{ cpp_utils::to_string( flag_serv_, charset_id, unsynced_mem_pool ) };
            const auto flag_crack_helper{ cpp_utils::to_string( flag_crack_helper_, charset_id, unsynced_mem_pool ) };
            const auto flag_restore_helper{ cpp_utils::to_string( flag_restore_helper_, charset_id, unsynced_mem_pool ) };
            for ( const auto& proc : custom_rules.procs ) {
                out << flag_proc << ' ' << cpp_utils::to_string( proc, charset_id, unsynced_mem_pool ) << '\n';
            }
            for ( const auto& serv : custom_rules.servs ) {
                out << flag_serv << ' ' << cpp_utils::to_string( serv, charset_id, unsynced_mem_pool ) << '\n';
            }
            for ( const auto& crack_helper : custom_rules.crack_helpers ) {
                out << flag_crack_helper << ' ' << cpp_utils::to_string( crack_helper, charset_id, unsynced_mem_pool ) << '\n';
            }
            for ( const auto& restore_helper : custom_rules.restore_helpers ) {
                out << flag_restore_helper << ' ' << cpp_utils::to_string( restore_helper, charset_id, unsynced_mem_pool ) << '\n';
            }
        }
        static auto before_load_() noexcept
        {
            custom_rules.procs.clear();
            custom_rules.servs.clear();
            custom_rules.crack_helpers.clear();
            custom_rules.restore_helpers.clear();
        }
        static auto show_help_info_()
        {
            cpp_utils::console_ui ui{ con, unsynced_mem_pool };
            ui.reserve( 3 )
              .add_back( make_title_text< "[ 配  置 ]", 2 >.view() )
              .add_back( " < 返回 "sv, quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
              .add_back(
                "\n 自定义规则格式为 <flag>: <item>\n"
                " <flag> 后的冒号与 <item> 之间可以有若干空白字符.\n"
                " 不符合格式的规则将会被忽略.\n"
                " <item> 的类型由 <flag> 决定.\n"
                " 其中, <flag> 有如下选项:\n"
                " proc - Windows 进程名称.\n"
                " serv - Windows 服务的服务名称.\n"
                " crack_helper - 破解时执行的程序的命令行.\n"
                " restore_helper - 恢复时执行的程序的命令行.\n\n"
                " 使用示例:\n"
                " [custom_rules]\n"
                " proc: abc_frontend.exe\n"
                " proc: abc_backend.com\n"
                " serv: abc_connect\n"
                " serv: abc_proc_defender\n"
                " crack_helper: \"abc helper.exe\" crack\n"
                " restore_helper: \"abc helper.exe\" restore"sv )
              .show();
            return func_back;
        }
        static auto init_ui_( cpp_utils::console_ui& ui )
        {
            ui.add_back( "\n[ 自定义规则 ]\n"sv ).add_back( " > 查看帮助信息 "sv, show_help_info_ );
        }
        static inline constexpr auto ui_count_{ 2uz };
      public:
        custom_rules_config() noexcept  = default;
        ~custom_rules_config() noexcept = default;
    };
    namespace details
    {
        template < typename T >
        struct is_valid_config_node final
        {
            static inline constexpr auto is_valid_type{ std::is_same_v< std::decay_t< T >, T > };
            static inline constexpr auto has_traits{
              std::is_base_of_v< config_node_impl, T > && std::is_default_constructible_v< T > };
            static inline constexpr auto value{ is_valid_type && has_traits };
            is_valid_config_node()           = delete;
            ~is_valid_config_node() noexcept = delete;
        };
    }
    using config_nodes_t
      = cpp_utils::type_list< options_title_ui, crack_restore_config, window_config, performance_config, custom_rules_config >;
    static_assert( config_nodes_t::all_of< details::is_valid_config_node > );
    static_assert( config_nodes_t::unique::size == config_nodes_t::size );
    inline config_nodes_t::apply< std::tuple > config_nodes{};
    namespace details
    {
        inline auto get_config_node_raw_name_by_tag( std::string_view str ) noexcept
        {
            str.remove_prefix( 1 );
            str.remove_suffix( 1 );
            const auto head{ std::ranges::find_if_not( str, is_whitespace< char > ) };
            const auto tail{ std::ranges::find_if_not( str | std::views::reverse, is_whitespace< char > ).base() };
            if ( head >= tail ) {
                return std::string_view{};
            }
            return std::string_view{ head, tail };
        }
    }
    inline auto load_config( const bool is_reload )
    {
        std::ifstream config_file{ config_file_name, std::ios::in };
        if ( !config_file.good() ) {
            return;
        }
        std::apply( []< typename... Ts >( Ts&... config_node ) static { ( config_node.before_load(), ... ); }, config_nodes );
        std::pmr::string line;
        using parsable_config_nodes_t = config_nodes_t::filter< details::is_parsable_config_node >;
        using config_node_recorder_t
          = parsable_config_nodes_t::transform< std::add_pointer >::add_front< std::monostate >::apply< std::variant >;
        config_node_recorder_t current_config_node;
        while ( std::getline( config_file, line ) ) {
            const auto parsed_begin{ std::ranges::find_if_not( line, details::is_whitespace< char > ) };
            const auto parsed_end{ std::ranges::find_if_not( line | std::views::reverse, details::is_whitespace< char > ).base() };
            if ( parsed_begin >= parsed_end ) {
                continue;
            }
            const std::string_view parsed_line{ parsed_begin, parsed_end };
            if ( parsed_line.front() == '#' ) {
                continue;
            }
            if ( parsed_line.front() == '[' && parsed_line.back() == ']' && parsed_line.size() > "[]"sv.size() ) {
                current_config_node = std::monostate{};
                std::apply( [ & ]( auto&... config_node ) noexcept
                {
                    const auto current_raw_name{ details::get_config_node_raw_name_by_tag( parsed_line ) };
                    ( [ & ]< typename T >( T& current_node ) noexcept
                    {
                        if constexpr ( parsable_config_nodes_t::contains< T > ) {
                            if ( T::raw_name.view() == current_raw_name ) {
                                current_config_node = std::addressof( current_node );
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
        std::apply( []< typename... Ts >( Ts&... config_node ) static { ( config_node.after_load(), ... ); }, config_nodes );
    }
    namespace details
    {
        inline auto show_config_parsing_rules()
        {
            cpp_utils::console_ui ui{ con, unsynced_mem_pool };
            ui.reserve( 3 )
              .add_back( make_title_text< "[ 配  置 ]", 2 >.view() )
              .add_back( " < 返回 "sv, quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
              .add_back(
                "\n 配置以行作为单位解析.\n\n"
                " 以 # 开头的行是注释, 不进行解析.\n\n"
                " 各个配置项在配置文件中由不同标签区分,\n"
                " 标签的格式为 [<标签名>],\n"
                " <标签名> 与中括号间可以有若干空格.\n\n"
                " 如果匹配不到配置项,\n"
                " 则当前读取的标签到下一标签之间的内容都将被忽略.\n\n"
                " 解析时会忽略每行前导和末尾的空白字符.\n"
                " 如果当前行不是标签, 则该行将由上一个标签处理."sv )
              .show();
            return func_back;
        }
        inline auto sync_config()
        {
            std::print(
              cpp_utils::value_identity_v< cpp_utils::concat_const_string(
                make_title_text< "[ 配  置 ]", 3 >, cpp_utils::const_string{ " -> 同步配置.\n\n" } ) >.view() );
            load_config( true );
            std::ofstream config_file_stream{ config_file_name, std::ios::out | std::ios::trunc };
            config_file_stream << "# " INFO_FULL_NAME "\n# " INFO_VERSION "\n";
            std::apply( [ & ]( auto&... config_node ) { ( config_node.sync( config_file_stream ), ... ); }, config_nodes );
            config_file_stream.flush();
            std::print( " (i) 同步配置{}.", config_file_stream.good() ? "成功" : "失败" );
            press_any_key_to_return();
            return func_back;
        }
        inline auto open_config_file()
        {
            std::print(
              cpp_utils::value_identity_v< cpp_utils::concat_const_string(
                make_title_text< "[ 配  置 ]", 3 >, cpp_utils::const_string{ " -> 尝试打开配置文件.\n\n" } ) >.view() );
            std::print(
              " (i) 打开配置文件{}.",
              std::bit_cast< INT_PTR >( ShellExecuteW( nullptr, L"open", config_file_name, nullptr, nullptr, SW_SHOWNORMAL ) ) > 32
                ? "成功"
                : "失败" );
            press_any_key_to_return();
            return func_back;
        }
    }
    inline auto config_ui()
    {
        std::apply( []( auto&... nodes ) static
        {
            cpp_utils::console_ui ui{ con, unsynced_mem_pool };
            constexpr auto reserved_size{ 5 + ( decltype( nodes.request_ui_count() )::value + ... ) };
            ui.reserve( reserved_size )
              .add_back( make_title_text< "[ 配  置 ]", 2 >.view() )
              .add_back( " < 返回\n"sv, quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
              .add_back( " > 查看解析规则 "sv, details::show_config_parsing_rules )
              .add_back( " > 同步配置 "sv, details::sync_config )
              .add_back( " > 打开配置文件 "sv, details::open_config_file );
            ( nodes.init_ui( ui ), ... );
            ui.show();
        }, config_nodes );
        return func_back;
    }
    inline auto info()
    {
        cpp_utils::console_ui ui{ con, unsynced_mem_pool };
        ui.reserve( 3 )
          .add_back( make_title_text< "[ 关  于 ]", 2 >.view() )
          .add_back( " < 返回 "sv, quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
          .add_back(
            "\n[ 名称 ]\n\n " INFO_FULL_NAME " (" INFO_SHORT_NAME ")\n\n[ 版本 ]\n\n " INFO_VERSION
            "\n\n[ 许可证与开发者 ]\n\n " INFO_LICENSE "\n " INFO_COPYRIGHT "\n\n 开源仓库:\n " INFO_REPO_URL
            "\n\n 开发者 GnuPG 公钥指纹:\n " INFO_GPG_KEY ""sv )
          .show();
        return func_back;
    }
    namespace details
    {
        template < cpp_utils::const_string Description, void ( *Func )() noexcept >
        struct func_item final
        {
            static inline constexpr auto description{ Description };
            static auto execute() noexcept
            {
                std::print( make_title_text< "[ 工 具 箱 ]", 3 >.view() );
                Func();
                std::print( "\n (i) 操作已完成." );
                press_any_key_to_return();
                return func_back;
            }
        };
        inline auto launch_cmd( const ui_func_args_t args )
        {
            STARTUPINFOW startup{};
            PROCESS_INFORMATION proc;
            wchar_t cmd[]{ L"cmd.exe" };
            startup.cb = sizeof( startup );
            if ( CreateProcessW( nullptr, cmd, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &startup, &proc ) ) {
                con.set_title( L"" INFO_SHORT_NAME " - 命令提示符" )
                  .set_size( 120, 30, unsynced_mem_pool )
                  .fix_size( false )
                  .enable_window_maximize_ctrl( true );
                args.parent_ui.set_constraints( false, false );
                SetConsoleScreenBufferSize( con.std_output_handle, { 120, std::numeric_limits< SHORT >::max() - 1 } );
                WaitForSingleObject( proc.hProcess, INFINITE );
                CloseHandle( proc.hProcess );
                CloseHandle( proc.hThread );
                con.set_charset( charset_id )
                  .set_title( L"" INFO_SHORT_NAME )
                  .set_size( console_width, console_height, unsynced_mem_pool )
                  .fix_size( true )
                  .enable_window_maximize_ctrl( false );
            }
            return func_back;
        }
        inline auto restore_os_components() noexcept
        {
            using reg_dirs_t = make_ordered_const_wstring_list_t<
              LR"(Software\Policies\Microsoft\Windows\System)", LR"(Software\Microsoft\Windows\CurrentVersion\Policies\System)",
              LR"(Software\Microsoft\Windows\CurrentVersion\Policies\Explorer)", LR"(Software\Policies\Microsoft\MMC)" >;
            using procs_t = make_ordered_const_wstring_list_t<
              L"tasklist", L"taskkill", L"ntsd", L"sc", L"net", L"reg", L"cmd", L"taskmgr", L"perfmon", L"regedit", L"mmc",
              L"dism", L"sfc", L"netsh", L"sethc", L"sidebar", L"shvlzm", L"winmine", L"bckgzm", L"Chess", L"chkrzm",
              L"FreeCell", L"Hearts", L"Magnify", L"Mahjong", L"Minesweeper", L"PurblePlace", L"Solitaire", L"SpiderSolitaire" >;
            std::print( " -> 撤销功能禁用.\n" );
            []< cpp_utils::const_wstring... Items >( const cpp_utils::type_list< cpp_utils::value_identity< Items >... > ) static noexcept
            { ( ( void ) cpp_utils::delete_registry_tree( HKEY_CURRENT_USER, Items.view() ), ... ); }( reg_dirs_t{} );
            ( void ) cpp_utils::delete_registry_key(
              HKEY_LOCAL_MACHINE, LR"(SOFTWARE\Policies\Microsoft\Windows NT\SystemRestore)"sv, L"DisableSR"sv );
            std::print( " -> 撤销映像劫持.\n" );
            []< cpp_utils::const_wstring... Items >( const cpp_utils::type_list< cpp_utils::value_identity< Items >... > ) static noexcept
            {
                ( ( void ) cpp_utils::delete_registry_tree(
                    HKEY_LOCAL_MACHINE,
                    cpp_utils::value_identity_v< cpp_utils::concat_const_string(
                      cpp_utils::const_wstring{ LR"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\)" },
                      Items, cpp_utils::const_wstring{ L".exe" } ) >.view() ),
                  ... );
            }( procs_t{} );
        }
        inline auto reset_firewall_rules() noexcept
        {
            std::print( " -> 重置防火墙规则.\n" );
            STARTUPINFOW si{};
            si.cb = sizeof( si );
            PROCESS_INFORMATION proc_info{};
            SECURITY_ATTRIBUTES sec_attrib{ sizeof( sec_attrib ), nullptr, TRUE };
            const auto nul_file_handle{
              CreateFileW( L"NUL", GENERIC_WRITE, FILE_SHARE_WRITE, &sec_attrib, OPEN_EXISTING, 0, nullptr ) };
            if ( nul_file_handle == INVALID_HANDLE_VALUE ) {
                return;
            }
            si.dwFlags    = STARTF_USESTDHANDLES;
            si.hStdInput  = GetStdHandle( STD_INPUT_HANDLE );
            si.hStdOutput = nul_file_handle;
            si.hStdError  = nul_file_handle;
            wchar_t cmd[]{ L"netsh.exe advfirewall reset" };
            const auto has_created_process_successfully{ CreateProcessW(
              nullptr, cmd, nullptr, nullptr, TRUE, CREATE_NO_WINDOW | CREATE_UNICODE_ENVIRONMENT, nullptr, nullptr, &si,
              &proc_info ) };
            CloseHandle( nul_file_handle );
            if ( has_created_process_successfully ) {
                WaitForSingleObject( proc_info.hProcess, INFINITE );
                CloseHandle( proc_info.hProcess );
                CloseHandle( proc_info.hThread );
            }
        }
        inline auto reset_hosts() noexcept
        {
            std::print( " -> 重置 Hosts.\n" );
            constexpr auto default_content{
              "# Copyright (c) 1993-2009 Microsoft Corp.\n"
              "#\n"
              "# This is a sample HOSTS file used by Microsoft TCP/IP for Windows.\n"
              "#\n"
              "# This file contains the mappings of IP addresses to host names. Each\n"
              "# entry should be kept on an individual line. The IP address should\n"
              "# be placed in the first column followed by the corresponding host name.\n"
              "# The IP address and the host name should be separated by at least one\n"
              "# space.\n"
              "#\n"
              "# Additionally, comments (such as these) may be inserted on individual\n"
              "# lines or following the machine name denoted by a '#' symbol.\n"
              "#\n"
              "# For example:\n"
              "#\n"
              "#      102.54.94.97     rhino.acme.com          # source server\n"
              "#       38.25.63.10     x.acme.com              # x client host\n"
              "\n"
              "# localhost name resolution is handled within DNS itself.\n"
              "# 127.0.0.1       localhost\n"
              "# ::1             localhost\n"sv };
            static constexpr auto reset_hosts_error_message{ "\n (!) 重置 Hosts 失败.\n\n" };
            constexpr auto has_error{ []( const std::error_code& ec ) static
            {
                if ( ec ) {
                    std::print( reset_hosts_error_message );
                    return true;
                }
                return false;
            } };
            const auto hosts_path{ [] static
            {
                std::array< wchar_t, MAX_PATH > result;
                GetWindowsDirectoryW( result.data(), MAX_PATH );
                std::ranges::copy( LR"(\System32\drivers\etc\hosts)", std::ranges::find( result, L'\0' ) );
                return std::filesystem::path{ result.data() };
            }() };
            std::error_code ec;
            const auto original_perms{ std::filesystem::status( hosts_path, ec ).permissions() };
            if ( has_error( ec ) ) {
                return;
            }
            std::filesystem::permissions( hosts_path, std::filesystem::perms::all, std::filesystem::perm_options::replace, ec );
            std::filesystem::remove( hosts_path, ec );
            std::ofstream file{ hosts_path, std::ios::out | std::ios::trunc | std::ios::binary };
            file.write( default_content.data(), default_content.size() ).flush();
            if ( !file.good() ) {
                std::print( reset_hosts_error_message );
            }
            std::filesystem::permissions( hosts_path, original_perms, std::filesystem::perm_options::replace, ec );
        }
        inline auto flush_dns() noexcept
        {
            constexpr auto flush_dns_error_message{ "\n (!) 刷新 DNS 失败.\n\n" };
            std::print( " -> 刷新 DNS 缓存.\n" );
            const auto dnsapi{ LoadLibraryW( L"dnsapi.dll" ) };
            if ( dnsapi == nullptr ) {
                std::print( flush_dns_error_message );
                return;
            }
            const auto dns_flush_resolver_cache{
              std::bit_cast< BOOL( WINAPI* )() noexcept >( GetProcAddress( dnsapi, "DnsFlushResolverCache" ) ) };
            if ( dns_flush_resolver_cache == nullptr ) {
                std::print( flush_dns_error_message );
                return;
            }
            if ( !dns_flush_resolver_cache() ) {
                std::print( flush_dns_error_message );
            }
            FreeLibrary( dnsapi );
        }
        inline auto reset_partial_network_settings() noexcept
        {
            reset_firewall_rules();
            reset_hosts();
            flush_dns();
        }
        inline auto reset_jfglzs_config() noexcept
        {
            std::print(
              " (i) 请破解 \"机房管理助手\" 后使用此功能.\n\n"
              " -> 删除密码.\n" );
            ( void ) cpp_utils::delete_registry_key( HKEY_CURRENT_USER, L"Software"sv, L"n"sv );
            std::print( " -> 删除配置.\n" );
            ( void ) cpp_utils::delete_registry_tree( HKEY_CURRENT_USER, LR"(Software\jfglzs)"sv );
            std::print( " -> 删除自启动项.\n" );
            using autorun_items_t = make_ordered_const_wstring_list_t< L"jfglzs", L"jfglzsn", L"jfglzsp", L"prozs", L"przs" >;
            []< cpp_utils::const_wstring... Items >( const cpp_utils::type_list< cpp_utils::value_identity< Items >... > ) static noexcept
            {
                ( ( void ) cpp_utils::delete_registry_key(
                    HKEY_LOCAL_MACHINE, LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\Run)", Items.view() ),
                  ... );
            }( autorun_items_t{} );
        }
        inline auto relaunch_explorer() noexcept
        {
            std::print( " -> 终止进程.\n" );
            ( void ) cpp_utils::terminate_process_by_name( L"explorer.exe"sv );
            std::print( " -> 启动进程.\n" );
            ( void ) ShellExecuteW( nullptr, L"open", L"explorer.exe", nullptr, nullptr, SW_HIDE );
        }
        inline auto restore_usb_device_access() noexcept
        {
            std::print( " -> 写入注册表.\n" );
            constexpr DWORD start_type{ 3 };
            ( void ) cpp_utils::create_registry_key(
              HKEY_LOCAL_MACHINE, LR"(SYSTEM\CurrentControlSet\Services\USBSTOR)"sv, L"Start"sv,
              cpp_utils::registry_flag::dword_type, std::bit_cast< const BYTE* >( &start_type ), sizeof( start_type ) );
        }
        inline auto reset_google_chrome_policy() noexcept
        {
            std::print( " -> 删除注册表.\n" );
            ( void ) cpp_utils::delete_registry_tree( HKEY_LOCAL_MACHINE, LR"(SOFTWARE\Policies\Google\Chrome)"sv );
        }
        inline auto reset_microsoft_edge_policy() noexcept
        {
            std::print( " -> 删除注册表.\n" );
            ( void ) cpp_utils::delete_registry_tree( HKEY_LOCAL_MACHINE, LR"(SOFTWARE\Policies\Microsoft\Edge)"sv );
        }
    }
    inline auto toolkit()
    {
        using funcs_t = cpp_utils::type_list<
          details::func_item< "重启资源管理器", details::relaunch_explorer >,
          details::func_item< "恢复操作系统组件", details::restore_os_components >,
          details::func_item< "恢复 USB 存储设备访问", details::restore_usb_device_access >,
          details::func_item< "重置部分网络设置", details::reset_partial_network_settings >,
          details::func_item< "重置 \"机房管理助手\" 配置", details::reset_jfglzs_config >,
          details::func_item< "重置 Google Chrome 管理策略", details::reset_google_chrome_policy >,
          details::func_item< "重置 Microsoft Edge 管理策略", details::reset_microsoft_edge_policy > >;
        cpp_utils::console_ui ui{ con, unsynced_mem_pool };
        ui.reserve( 4 + funcs_t::size )
          .add_back( make_title_text< "[ 工 具 箱 ]", 2 >.view() )
          .add_back( " < 返回 "sv, quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
          .add_back( "\n[ 快捷工具 ]\n"sv )
          .add_back( " > 启动命令提示符\n"sv, details::launch_cmd );
        [ & ]< typename... Items >( const cpp_utils::type_list< Items... > )
        { ( ui.add_back( make_button_text< Items::description >.view(), Items::execute ), ... ); }( funcs_t{} );
        ui.show();
        return func_back;
    }
    namespace details
    {
        inline auto set_console_attrs() noexcept
        {
            constexpr const auto& window_options{ std::get< window_config >( config_nodes ) };
            constexpr const auto& is_enable_simple_titlebar{ window_options.at< "simple_titlebar" >() };
            constexpr const auto& is_translucent{ window_options.at< "translucent" >() };
            constexpr const auto& is_no_hot_reload{ std::get< performance_config >( config_nodes ).at< "no_hot_reload" >() };
            if ( is_no_hot_reload ) {
                con.enable_context_menu( !is_enable_simple_titlebar.load( std::memory_order_acquire ) );
                con.set_translucency( is_translucent.load( std::memory_order_acquire ) ? 230 : 255 );
                return;
            }
            while ( true ) {
                con.enable_context_menu( !is_enable_simple_titlebar.load( std::memory_order_acquire ) );
                con.set_translucency( is_translucent.load( std::memory_order_acquire ) ? 230 : 255 );
                std::this_thread::sleep_for( default_thread_sleep_time );
            }
        }
        inline auto force_show() noexcept
        {
            constexpr const auto& is_no_hot_reload{ std::get< performance_config >( config_nodes ).at< "no_hot_reload" >() };
            constexpr const auto& is_force_show{ std::get< window_config >( config_nodes ).at< "force_show" >() };
            if ( is_no_hot_reload && !is_force_show.load( std::memory_order_acquire ) ) {
                return;
            }
            constexpr auto sleep_time{ 50ms };
            if ( is_no_hot_reload ) {
                con.force_show_forever( sleep_time );
            }
            while ( true ) {
                if ( !is_force_show.load( std::memory_order_acquire ) ) {
                    con.cancel_force_show();
                    std::this_thread::sleep_for( default_thread_sleep_time );
                    continue;
                }
                con.force_show();
                std::this_thread::sleep_for( sleep_time );
            }
        }
        inline constexpr std::array parallel_tasks{ set_console_attrs, force_show };
    }
    inline auto create_parallel_tasks() noexcept
    {
        std::apply( []( auto&... funcs ) static noexcept { ( std::thread{ funcs }.detach(), ... ); }, details::parallel_tasks );
    }
    namespace details
    {
        enum class rule_executor_mode : bool
        {
            crack,
            restore
        };
        inline auto current_rule_executor_mode{ rule_executor_mode::crack };
    }
    template < typename... Backends >
        requires requires {
            requires cpp_utils::as_concept< ( sizeof...( Backends ) != 0 ) >;
            ( Backends::hijack_procs(), ... );
            ( Backends::disable_servs(), ... );
            ( Backends::stop_servs(), ... );
            ( Backends::kill_procs(), ... );
            ( Backends::crack_helper(), ... );
            ( Backends::undo_hijack_procs(), ... );
            ( Backends::enable_servs(), ... );
            ( Backends::start_servs(), ... );
            ( Backends::restore_helper(), ... );
        }
    struct rule_executor final
    {
        static auto crack()
        {
            constexpr const auto& options{ std::get< crack_restore_config >( config_nodes ) };
            constexpr const auto& can_hijack_procs{ options.at< "hijack_procs" >() };
            constexpr const auto& can_set_serv_startup_types{ options.at< "set_servs_start_type" >() };
            if ( can_hijack_procs ) {
                std::print( " -> 劫持进程.\n" );
                ( Backends::hijack_procs(), ... );
            }
            if ( can_set_serv_startup_types ) {
                std::print( " -> 禁用服务.\n" );
                ( Backends::disable_servs(), ... );
            }
            std::print( " -> 停止服务.\n" );
            ( Backends::stop_servs(), ... );
            std::print( " -> 终止进程.\n" );
            ( Backends::kill_procs(), ... );
            std::print( " -> 执行扩展操作.\n" );
            ( Backends::crack_helper(), ... );
        }
        static auto restore()
        {
            constexpr const auto& options{ std::get< crack_restore_config >( config_nodes ) };
            constexpr const auto& can_hijack_procs{ options.at< "hijack_procs" >() };
            constexpr const auto& can_set_serv_startup_types{ options.at< "set_servs_start_type" >() };
            if ( can_hijack_procs ) {
                std::print( " -> 撤销劫持.\n" );
                ( Backends::undo_hijack_procs(), ... );
            }
            if ( can_set_serv_startup_types ) {
                std::print( " -> 启用服务.\n" );
                ( Backends::enable_servs(), ... );
            }
            std::print( " -> 启动服务.\n" );
            ( Backends::start_servs(), ... );
            std::print( " -> 执行扩展操作.\n" );
            ( Backends::restore_helper(), ... );
        }
        static auto operator()()
        {
            switch ( details::current_rule_executor_mode ) {
                case details::rule_executor_mode::crack : std::print( make_title_text< "[ 破  解 ]", 3 >.view() ); break;
                case details::rule_executor_mode::restore : std::print( make_title_text< "[ 恢  复 ]", 3 >.view() ); break;
                default : std::unreachable();
            }
            switch ( details::current_rule_executor_mode ) {
                case details::rule_executor_mode::crack : crack(); break;
                case details::rule_executor_mode::restore : restore(); break;
                default : std::unreachable();
            }
            std::print( "\n (i) 操作已完成." );
            details::press_any_key_to_return();
            return func_back;
        }
    };
    template < typename BuiltinRuleNode >
    struct builtin_rules_executor_backend final
    {
        static auto hijack_procs() noexcept
        {
            []< cpp_utils::const_wstring... Procs >( const cpp_utils::type_list< cpp_utils::value_identity< Procs >... > ) static noexcept
            {
                constexpr const wchar_t data[]{ L"nul" };
                ( ( void ) cpp_utils::create_registry_key(
                    HKEY_LOCAL_MACHINE,
                    cpp_utils::value_identity_v< cpp_utils::concat_const_string(
                      cpp_utils::const_wstring{ LR"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\)" },
                      Procs ) >.view(),
                    L"Debugger", cpp_utils::registry_flag::string_type, std::bit_cast< const BYTE* >( +data ), sizeof( data ) ),
                  ... );
            }( typename BuiltinRuleNode::procs{} );
        }
        static auto disable_servs() noexcept
        {
            []< cpp_utils::const_wstring... Servs >( const cpp_utils::type_list< cpp_utils::value_identity< Servs >... > ) static noexcept
            {
                ( ( void ) cpp_utils::set_service_start_type( Servs.view(), cpp_utils::service_flag::disabled_start ), ... );
            }( typename BuiltinRuleNode::servs{} );
        }
        static auto stop_servs() noexcept
        {
            []< cpp_utils::const_wstring... Servs >( const cpp_utils::type_list< cpp_utils::value_identity< Servs >... > ) static noexcept
            {
                ( ( void ) cpp_utils::stop_service_with_dependencies( Servs.view(), unsynced_mem_pool ), ... );
            }( typename BuiltinRuleNode::servs{} );
        }
        static auto kill_procs() noexcept
        {
            []< cpp_utils::const_wstring... Procs >( const cpp_utils::type_list< cpp_utils::value_identity< Procs >... > ) static noexcept
            {
                constexpr std::array names{ cpp_utils::value_identity_v< cpp_utils::concat_const_string( Procs ) >.view()... };
                ( void ) cpp_utils::terminate_process_by_names( names );
            }( typename BuiltinRuleNode::procs{} );
        }
        static auto crack_helper()
        {
            BuiltinRuleNode::crack_helper();
        }
        static auto undo_hijack_procs() noexcept
        {
            []< cpp_utils::const_wstring... Procs >( const cpp_utils::type_list< cpp_utils::value_identity< Procs >... > ) static noexcept
            {
                ( ( void ) cpp_utils::delete_registry_tree(
                    HKEY_LOCAL_MACHINE,
                    cpp_utils::value_identity_v< cpp_utils::concat_const_string(
                      cpp_utils::const_wstring{ LR"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\)" },
                      Procs ) >.view() ),
                  ... );
            }( typename BuiltinRuleNode::procs{} );
        }
        static auto enable_servs() noexcept
        {
            []< cpp_utils::const_wstring... Servs >( const cpp_utils::type_list< cpp_utils::value_identity< Servs >... > ) static noexcept
            {
                ( ( void ) cpp_utils::set_service_start_type( Servs.view(), cpp_utils::service_flag::auto_start ), ... );
            }( typename BuiltinRuleNode::servs{} );
        }
        static auto start_servs() noexcept
        {
            []< cpp_utils::const_wstring... Servs >( const cpp_utils::type_list< cpp_utils::value_identity< Servs >... > ) static noexcept
            {
                ( ( void ) cpp_utils::start_service_with_dependencies( Servs.view(), unsynced_mem_pool ), ... );
            }( typename BuiltinRuleNode::servs{} );
        }
        static auto restore_helper()
        {
            BuiltinRuleNode::restore_helper();
        }
    };
    struct custom_rule_executor_backend final
    {
        static auto hijack_procs()
        {
            constexpr const wchar_t data[]{ L"nul" };
            for ( const auto& proc : custom_rules.procs ) {
                ( void ) cpp_utils::create_registry_key(
                  HKEY_LOCAL_MACHINE,
                  std::format( LR"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\{})", proc ),
                  L"Debugger", cpp_utils::registry_flag::string_type, std::bit_cast< const BYTE* >( +data ), sizeof( data ) );
            }
        }
        static auto disable_servs() noexcept
        {
            for ( const auto& serv : custom_rules.servs ) {
                ( void ) cpp_utils::set_service_start_type( serv, cpp_utils::service_flag::disabled_start );
            }
        }
        static auto stop_servs() noexcept
        {
            for ( const auto& serv : custom_rules.servs ) {
                ( void ) cpp_utils::stop_service_with_dependencies( serv, unsynced_mem_pool );
            }
        }
        static auto kill_procs()
        {
            ( void ) cpp_utils::terminate_process_by_names( custom_rules.procs );
        }
        static auto execute_helpers_( const std::pmr::vector< std::pmr::wstring >& helpers ) noexcept
        {
            std::error_code ec;
            for ( const auto& helper : helpers ) {
                STARTUPINFOW startup{};
                startup.cb = sizeof( startup );
                PROCESS_INFORMATION proc{};
                std::pmr::wstring cmd{ helper, unsynced_mem_pool };
                if ( CreateProcessW( nullptr, cmd.data(), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &startup, &proc ) ) {
                    WaitForSingleObject( proc.hProcess, INFINITE );
                    CloseHandle( proc.hProcess );
                    CloseHandle( proc.hThread );
                }
            }
        }
        static auto crack_helper() noexcept
        {
            execute_helpers_( custom_rules.crack_helpers );
        }
        static auto undo_hijack_procs()
        {
            for ( const auto& proc : custom_rules.procs ) {
                ( void ) cpp_utils::delete_registry_tree(
                  HKEY_LOCAL_MACHINE,
                  std::format( LR"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\{})", proc ) );
            }
        }
        static auto enable_servs() noexcept
        {
            for ( const auto& serv : custom_rules.servs ) {
                ( void ) cpp_utils::set_service_start_type( serv, cpp_utils::service_flag::auto_start );
            }
        }
        static auto start_servs() noexcept
        {
            for ( const auto& serv : custom_rules.servs ) {
                ( void ) cpp_utils::start_service_with_dependencies( serv, unsynced_mem_pool );
            }
        }
        static auto restore_helper() noexcept
        {
            execute_helpers_( custom_rules.restore_helpers );
        }
    };
    using all_rules_t
      = builtin_rules_t::apply_each< builtin_rules_executor_backend >::add_front< custom_rule_executor_backend >::apply< rule_executor >;
    inline auto make_executor_mode_ui_text() noexcept
    {
        switch ( details::current_rule_executor_mode ) {
            case details::rule_executor_mode::crack : return "[ 破解 (点击切换) ]\n"sv;
            case details::rule_executor_mode::restore : return "[ 恢复 (点击切换) ]\n"sv;
            default : std::unreachable();
        }
    }
    inline auto flip_executor_mode( const ui_func_args_t args ) noexcept
    {
        switch ( details::current_rule_executor_mode ) {
            case details::rule_executor_mode::crack :
                details::current_rule_executor_mode = details::rule_executor_mode::restore;
                break;
            case details::rule_executor_mode::restore :
                details::current_rule_executor_mode = details::rule_executor_mode::crack;
                break;
            default : std::unreachable();
        }
        args.parent_ui.set_text( args.node_index, make_executor_mode_ui_text() );
        return func_back;
    }
}
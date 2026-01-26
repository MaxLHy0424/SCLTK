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
#include <limits>
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
    using ui_func_args = cpp_utils::console_ui::func_args;
    inline auto quit() noexcept
    {
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
        template < cpp_utils::const_wstring... Items >
        using make_const_wstring_list = cpp_utils::type_list< cpp_utils::value_identity< Items >... >;
        inline auto terminate_jfglzs_daemon() noexcept
        {
            constexpr auto close_handle{ []( const HANDLE handle ) static noexcept { CloseHandle( handle ); } };
            using raii_handle = const std::unique_ptr< std::remove_pointer_t< HANDLE >, decltype( close_handle ) >;
            raii_handle process_snapshot{ CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 ), close_handle };
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
                        raii_handle process_handle{
                          OpenProcess( PROCESS_TERMINATE | PROCESS_QUERY_LIMITED_INFORMATION, FALSE, process_entry.th32ProcessID ),
                          close_handle };
                        if ( process_handle.get() == nullptr ) {
                            continue;
                        }
                        DWORD size{ MAX_PATH };
                        std::array< wchar_t, MAX_PATH > buffer{};
                        QueryFullProcessImageNameW( process_handle.get(), 0, buffer.data(), &size );
                        if ( std::search( buffer.begin(), buffer.end(), searcher ) == buffer.end() ) {
                            continue;
                        }
                        TerminateProcess( process_handle.get(), 1 );
                    }
                } while ( Process32NextW( process_snapshot.get(), &process_entry ) );
            }
        }
    }
    template <
      cpp_utils::const_string DisplayName, cpp_utils::same_as_type_list Execs, cpp_utils::same_as_type_list Servs,
      std::invocable auto CrackHelper = []() static noexcept { }, std::invocable auto RestoreHelper = []() static noexcept { } >
    struct compile_time_rule_node final
    {
        static inline constexpr auto display_name{ DisplayName };
        static inline constexpr auto crack_helper{ CrackHelper };
        static inline constexpr auto restore_helper{ RestoreHelper };
        using execs = Execs;
        using servs = Servs;
    };
    struct runtime_rule_node final
    {
        std::pmr::vector< std::pmr::wstring > execs{ unsynced_mem_pool };
        std::pmr::vector< std::pmr::wstring > servs{ unsynced_mem_pool };
    };
    using builtin_rules = cpp_utils::type_list<
      compile_time_rule_node<
        "机房管理助手",
        details::make_const_wstring_list< L"yz", L"jfglzs", L"jfglzsn", L"jfglzsp", L"przs", L"zmserv", L"zmsrv" >,
        details::make_const_wstring_list< L"zmserv" >, details::terminate_jfglzs_daemon >,
      compile_time_rule_node<
        "极域电子教室",
        details::make_const_wstring_list< L"StudentMain", L"DispcapHelper", L"VRCwPlayer", L"InstHelpApp", L"InstHelpApp64",
                                          L"TDOvrSet", L"GATESRV", L"ProcHelper64", L"MasterHelper" >,
        details::make_const_wstring_list< L"STUDSRV" >,
        [] static noexcept
    {
        cpp_utils::stop_service_with_dependencies( L"TDNetFilter" );
        cpp_utils::stop_service_with_dependencies( L"TDFileFilter" );
    },
        [] static noexcept
    {
        cpp_utils::start_service_with_dependencies( L"TDNetFilter" );
        cpp_utils::start_service_with_dependencies( L"TDFileFilter" );
    } >,
      compile_time_rule_node<
        "联想智能云教室",
        details::make_const_wstring_list<
          L"vncviewer", L"tvnserver32", L"WfbsPnpInstall", L"WFBSMon", L"WFBSMlogon", L"WFBSSvrLogShow", L"ResetIp",
          L"FuncForWIN64", L"CertMgr", L"Fireware", L"BCDBootCopy", L"refreship", L"lenovoLockScreen", L"PortControl64",
          L"DesktopCheck", L"DeploymentManager", L"DeploymentAgent", L"XYNTService" >,
        details::make_const_wstring_list< L"BSAgentSvr", L"tvnserver", L"WFBSMlogon" > >,
      compile_time_rule_node<
        "红蜘蛛多媒体网络教室",
        details::make_const_wstring_list<
          L"rscheck", L"checkrs", L"REDAgent", L"PerformanceCheck", L"edpaper", L"Adapter", L"repview", L"FormatPaper" >,
        details::make_const_wstring_list< L"appcheck2", L"checkapp2" > >,
      compile_time_rule_node<
        "市一中伊金霍洛校区机房管理程序",
        details::make_const_wstring_list< L"ComputerClassroom_Client", L"MonitorProcess", L"00 PowerRun_x64", L"PowerRun" >,
        details::make_const_wstring_list<> >,
      compile_time_rule_node<
        "Veyon",
        details::make_const_wstring_list< L"veyon-worker", L"veyon-configurator", L"veyon-server", L"veyon-cli", L"veyon-wcli",
                                          L"veyon-master", L"veyon-service" >,
        details::make_const_wstring_list< L"VeyonService" > > >;
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
            static consteval auto impl() noexcept
            {
                return std::is_base_of_v< config_node_impl, T > && requires { T::raw_name; };
            }
            static inline constexpr auto value{ impl() };
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
        template < cpp_utils::const_string RawName, cpp_utils::const_string DisplayName, bool Atomic, cpp_utils::const_string... Items >
            requires( sizeof...( Items ) % 2 == 0 && sizeof...( Items ) != 0 )
        class basic_options_config_node
          : public config_node_impl
          , public config_node_raw_name< RawName >
        {
            friend config_node_impl;
          private:
            using item_list_ = cpp_utils::type_list< cpp_utils::value_identity< Items >... >;
            using value_t_   = std::conditional_t< Atomic, std::atomic< bool >, bool >;
            std::array< value_t_, sizeof...( Items ) / 2 > data_{};
            static constexpr auto str_of_the_enabled{ ": enabled"sv };
            static constexpr auto str_of_the_disabled{ ": disabled"sv };
            auto load_( const std::string_view line )
            {
                std::string_view key;
                bool value;
                if ( line.ends_with( str_of_the_enabled ) ) {
                    key   = line.substr( 0, line.size() - str_of_the_enabled.size() );
                    value = true;
                } else if ( line.ends_with( str_of_the_disabled ) ) {
                    key   = line.substr( 0, line.size() - str_of_the_disabled.size() );
                    value = false;
                } else {
                    return;
                }
                [ & ]< std::size_t... Is >( const std::index_sequence< Is... > ) noexcept
                {
                    (
                      [ & ]< std::size_t I >() noexcept
                    {
                        if ( item_list_::template at< I * 2 >::value.view() == key ) {
                            std::get< I >( data_ ) = value;
                            return true;
                        }
                        return false;
                    }.template operator()< Is >()
                      || ... );
                }( std::make_index_sequence< sizeof...( Items ) / 2 >{} );
            }
            static auto reload_( const std::string_view ) noexcept
            { }
            auto sync_( std::ofstream& out )
            {
                [ & ]< std::size_t... Is >( const std::index_sequence< Is... > )
                {
                    ( ( out << item_list_::template at< Is * 2 >::value.c_str()
                            << ( std::get< Is >( data_ ) == true ? str_of_the_enabled : str_of_the_disabled ) << '\n' ),
                      ... );
                }( std::make_index_sequence< sizeof...( Items ) / 2 >{} );
            }
            static auto get_value( const value_t_& value )
            {
                if constexpr ( std::is_same_v< value_t_, std::atomic< bool > > ) {
                    return value.load( std::memory_order_acquire );
                } else {
                    return value;
                }
            }
            static auto& set_value( value_t_& obj, bool val )
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
                return get_value( value ) == true ? " > 禁用 "sv : " > 启用 "sv;
            }
            static auto flip_item_value_( const ui_func_args args, value_t_& value )
            {
                args.parent_ui.set_text( args.node_index, make_flip_button_text_( set_value( value, !get_value( value ) ) ) );
                return func_back;
            }
            static auto make_option_editor_ui_( std::array< value_t_, sizeof...( Items ) / 2 >& data_ )
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
                            cpp_utils::const_string{ "\n[ " }, item_list_::template at< Is * 2 + 1 >::value,
                            cpp_utils::const_string{ " ]\n" } ) >.view() )
                        .add_back(
                          make_flip_button_text_( std::get< Is >( data_ ) ),
                          std::bind_back( flip_item_value_, std::ref( std::get< Is >( data_ ) ) ),
                          cpp_utils::console_text::foreground_red | cpp_utils::console_text::foreground_green ),
                      ... );
                }( std::make_index_sequence< sizeof...( Items ) / 2 >{} );
                ui.show();
                return func_back;
            }
            auto init_ui_( cpp_utils::console_ui& ui )
            {
                ui.add_back( make_button_text< DisplayName >.view(), std::bind_back( make_option_editor_ui_, std::ref( data_ ) ) );
            }
            static inline constexpr auto ui_count_{ 1uz };
          public:
            template < cpp_utils::const_string Key >
            auto&& at( this auto&& self ) noexcept
            {
                static_assert( item_list_::template contains< cpp_utils::value_identity< Key > > );
                return std::get< item_list_::template find_first< cpp_utils::value_identity< Key > > / 2 >( self.data_ );
            }
            auto operator=( const basic_options_config_node& ) -> basic_options_config_node&     = delete;
            auto operator=( basic_options_config_node&& ) noexcept -> basic_options_config_node& = delete;
            basic_options_config_node() noexcept                                                 = default;
            basic_options_config_node( const basic_options_config_node& )                        = delete;
            basic_options_config_node( basic_options_config_node&& ) noexcept                    = delete;
            ~basic_options_config_node() noexcept                                                = default;
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
    class crack_restore_config final
      : public details::basic_options_config_node<
          "crack_restore", "破解与恢复", false, "hijack_execs", "劫持可执行文件", "set_serv_startup_types", "设置服务启动类型" >
    { };
    class window_config final
      : public details::basic_options_config_node<
          "window", "窗口显示", true, "force_show", "置顶窗口 (非实时)", "simple_titlebar", "极简标题栏 (非实时)",
          "translucent", "半透明 (非实时)" >
    { };
    class performance_config final
      : public details::basic_options_config_node<
          "performance", "性能", false, "no_hot_reload", "禁用非实时热重载 (下次启动时生效)" >
    { };
    class custom_rules_config final
      : public details::config_node_impl
      , public details::config_node_raw_name< "custom_rules" >
    {
        friend details::config_node_impl;
      private:
        static inline constexpr auto flag_exec{ L"exec:"sv };
        static inline constexpr auto flag_serv{ L"serv:"sv };
        static_assert( []( auto... strings ) consteval
        { return ( std::ranges::none_of( strings, details::is_whitespace< wchar_t > ) && ... ); }( flag_exec, flag_serv ) );
        static auto load_( const std::string_view unconverted_line )
        {
            const auto converted_line{ cpp_utils::to_wstring( unconverted_line, charset_id ) };
            const std::wstring_view line{ converted_line };
            if ( line.size() > flag_exec.size() && line.starts_with( flag_exec ) ) {
                custom_rules.execs.emplace_back(
                  std::ranges::find_if_not( line.substr( flag_exec.size() ), details::is_whitespace< wchar_t > ) );
                return;
            }
            if ( line.size() > flag_serv.size() && line.starts_with( flag_serv ) ) {
                custom_rules.servs.emplace_back(
                  std::ranges::find_if_not( line.substr( flag_serv.size() ), details::is_whitespace< wchar_t > ) );
                return;
            }
        }
        static auto sync_( std::ofstream& out )
        {
            const auto flag_exec_ansi{ cpp_utils::to_string( flag_exec, charset_id, unsynced_mem_pool ) };
            const auto flag_serv_ansi{ cpp_utils::to_string( flag_serv, charset_id, unsynced_mem_pool ) };
            for ( const auto& exec : custom_rules.execs ) {
                out << flag_exec_ansi << ' ' << cpp_utils::to_string( exec, charset_id, unsynced_mem_pool ) << '\n';
            }
            for ( const auto& serv : custom_rules.servs ) {
                out << flag_serv_ansi << ' ' << cpp_utils::to_string( serv, charset_id, unsynced_mem_pool ) << '\n';
            }
        }
        static auto before_load_() noexcept
        {
            custom_rules.execs.clear();
            custom_rules.servs.clear();
        }
        static auto show_help_info_()
        {
            cpp_utils::console_ui ui{ con, unsynced_mem_pool };
            ui.reserve( 3 )
              .add_back( make_title_text< "[ 配  置 ]", 2 >.view() )
              .add_back( " < 返回 "sv, quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
              .add_back(
                "\n 自定义规则格式为 <flag>: <item>\n"
                " 其中, <flag> 可为 exec 或 serv,\n"
                " 分别表示以 .exe 为文件扩展名的可执行文件的名称\n"
                " 和某个 Windows 服务的服务名称.\n"
                " <flag> 后的冒号与 <item> 之间可以有若干空白字符.\n"
                " <item> 的类型由 <flag> 决定.\n"
                " 如果 <item> 为空, 该项规则将会被忽略.\n"
                " 如果自定义规则不符合格式, 则会被忽略.\n"
                " 本配置项不对自定义规则的正确性进行检测,\n"
                " 在修改自定义规则时, 请仔细检查.\n\n"
                " 使用示例:\n\n"
                " [custom_rules]\n"
                " exec: abc_frontend\n"
                " exec: abc_backend\n"
                " serv: abc_connect\n"
                " serv: abc_proc_defender"sv )
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
            static inline constexpr auto value{
              std::is_final_v< T > && std::is_same_v< std::decay_t< T >, T > && std::is_base_of_v< config_node_impl, T >
              && std::is_default_constructible_v< T > };
            is_valid_config_node()           = delete;
            ~is_valid_config_node() noexcept = delete;
        };
    }
    using config_node_types
      = cpp_utils::type_list< options_title_ui, crack_restore_config, window_config, performance_config, custom_rules_config >;
    static_assert( config_node_types::all_of< details::is_valid_config_node > );
    static_assert( config_node_types::unique::size == config_node_types::size );
    inline config_node_types::apply< std::tuple > config_nodes{};
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
    inline auto load_config( const bool is_reload = false )
    {
        std::ifstream config_file{ config_file_name, std::ios::in };
        if ( !config_file.good() ) {
            return;
        }
        std::apply( []< typename... Ts >( Ts&... config_node ) static { ( config_node.before_load(), ... ); }, config_nodes );
        std::pmr::string line;
        using parsable_config_node_types = config_node_types::filter< details::is_parsable_config_node >;
        using config_node_recorder
          = parsable_config_node_types::transform< std::add_pointer >::add_front< std::monostate >::apply< std::variant >;
        config_node_recorder current_config_node;
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
                        if constexpr ( parsable_config_node_types::contains< T > ) {
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
            current_config_node.visit( [ & ]< typename T >( const T node_ptr )
            {
                if constexpr ( !std::is_same_v< T, std::monostate > ) {
                    if ( is_reload ) {
                        node_ptr->reload( parsed_line );
                    } else {
                        node_ptr->load( parsed_line );
                    }
                }
            } );
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
                " 以 # 开头的行是注释, 不进行解析\n\n"
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
            "\n\n 构建时间: " INFO_BUILD_TIME "\n 编译工具: " INFO_COMPILER " " INFO_ARCH
            "\n\n[ 许可证与版权 ]\n\n " INFO_LICENSE "\n " INFO_COPYRIGHT "\n\n[ 仓库 ]\n\n " INFO_REPO_URL ""sv )
          .show();
        return func_back;
    }
    namespace details
    {
        inline auto launch_cmd( const ui_func_args args )
        {
            STARTUPINFOW startup{};
            PROCESS_INFORMATION proc;
            wchar_t name[]{ L"cmd.exe" };
            startup.cb = sizeof( startup );
            if ( CreateProcessW( nullptr, name, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &startup, &proc ) ) {
                con.set_title( L"" INFO_SHORT_NAME " - 命令提示符" );
                con.set_size( 120, 30, unsynced_mem_pool );
                con.fix_size( false );
                con.enable_window_maximize_ctrl( true );
                args.parent_ui.set_constraints( false, false );
                SetConsoleScreenBufferSize( con.std_output_handle, { 120, std::numeric_limits< SHORT >::max() - 1 } );
                WaitForSingleObject( proc.hProcess, INFINITE );
                CloseHandle( proc.hProcess );
                CloseHandle( proc.hThread );
                con.set_charset( charset_id );
                con.set_title( L"" INFO_SHORT_NAME );
                con.set_size( console_width, console_height, unsynced_mem_pool );
                con.fix_size( true );
                con.enable_window_maximize_ctrl( false );
            }
            return func_back;
        }
        inline auto restore_os_components() noexcept
        {
            std::print( " -> 尝试恢复.\n" );
            using reg_dirs = make_const_wstring_list<
              LR"(Software\Policies\Microsoft\Windows\System)", LR"(Software\Microsoft\Windows\CurrentVersion\Policies\System)",
              LR"(Software\Microsoft\Windows\CurrentVersion\Policies\Explorer)", LR"(Software\Policies\Microsoft\MMC)" >;
            using execs = make_const_wstring_list<
              L"tasklist", L"taskkill", L"ntsd", L"sc", L"net", L"reg", L"cmd", L"taskmgr", L"perfmon", L"regedit", L"mmc",
              L"dism", L"sfc", L"sethc", L"sidebar", L"shvlzm", L"winmine", L"bckgzm", L"Chess", L"chkrzm", L"FreeCell",
              L"Hearts", L"Magnify", L"Mahjong", L"Minesweeper", L"PurblePlace", L"Solitaire", L"SpiderSolitaire" >;
            []< cpp_utils::const_wstring... Items >( const cpp_utils::type_list< cpp_utils::value_identity< Items >... > ) static noexcept
            { ( RegDeleteTreeW( HKEY_CURRENT_USER, Items.c_str() ), ... ); }( reg_dirs{} );
            []< cpp_utils::const_wstring... Items >( const cpp_utils::type_list< cpp_utils::value_identity< Items >... > ) static noexcept
            {
                ( RegDeleteTreeW(
                    HKEY_CURRENT_USER,
                    cpp_utils::value_identity_v< cpp_utils::concat_const_string(
                      cpp_utils::const_wstring{ LR"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\)" },
                      Items, cpp_utils::const_wstring{ L".exe" } ) >.c_str() ),
                  ... );
            }( execs{} );
        }
        inline auto reset_hosts() noexcept
        {
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
            constexpr auto has_error{ []( const std::error_code& ec ) static
            {
                if ( ec ) {
                    std::print( "\n (!) 重置失败.\n\n" );
                    return true;
                }
                return false;
            } };
            std::error_code ec;
            std::print( " -> 检查文件是否存在.\n" );
            const auto hosts_path{ [] static
            {
                std::array< wchar_t, MAX_PATH > result;
                GetWindowsDirectoryW( result.data(), MAX_PATH );
                std::ranges::copy( LR"(\System32\drivers\etc\hosts)", std::ranges::find( result, L'\0' ) );
                return std::filesystem::path{ result.data() };
            }() };
            const auto is_exists_hosts_file{ std::filesystem::exists( hosts_path, ec ) };
            if ( has_error( ec ) || !is_exists_hosts_file ) {
                return;
            }
            std::print( " -> 获取原文件权限.\n" );
            const auto original_perms{ std::filesystem::status( hosts_path, ec ).permissions() };
            if ( has_error( ec ) ) {
                return;
            }
            std::print( " -> 设置文件权限.\n" );
            std::filesystem::permissions( hosts_path, std::filesystem::perms::all, std::filesystem::perm_options::replace, ec );
            if ( has_error( ec ) ) {
                return;
            }
            std::print( " -> 重置文件.\n" );
            std::ofstream file{ hosts_path, std::ios::out | std::ios::trunc | std::ios::binary };
            file.write( default_content.data(), default_content.size() ).flush();
            if ( !file.good() ) {
                std::print( "\n (!) 重置失败, 无法写入.\n\n" );
            }
            std::print( " -> 恢复文件权限.\n" );
            std::filesystem::permissions( hosts_path, original_perms, std::filesystem::perm_options::replace, ec );
            has_error( ec );
            std::print( " -> 刷新 DNS 缓存.\n" );
            const auto dnsapi{ LoadLibraryW( L"dnsapi.dll" ) };
            if ( dnsapi == nullptr ) {
                std::print( "\n (!) 刷新 DNS 失败.\n\n" );
                return;
            }
            const auto dns_flush_resolver_cache{
              std::bit_cast< BOOL( WINAPI* )() noexcept >( GetProcAddress( dnsapi, "DnsFlushResolverCache" ) ) };
            if ( dns_flush_resolver_cache != nullptr ) {
                const auto result{ dns_flush_resolver_cache() };
                if ( !result ) {
                    std::print( "\n (!) 刷新 DNS 失败.\n\n" );
                }
            }
            FreeLibrary( dnsapi );
        }
        inline auto reset_jfglzs_config() noexcept
        {
            std::print(
              " (i) 请破解 \"机房管理助手\" 后使用此功能.\n\n"
              " -> 删除密码.\n" );
            cpp_utils::delete_registry_key( HKEY_CURRENT_USER, L"Software"sv, L"n"sv );
            std::print( " -> 删除配置.\n" );
            cpp_utils::delete_registry_tree( HKEY_CURRENT_USER, LR"(Software\jfglzs)"sv );
            std::print( " -> 删除自启动项.\n" );
            using autorun_items = make_const_wstring_list< L"jfglzs", L"jfglzsn", L"jfglzsp", L"prozs", L"przs" >;
            []< cpp_utils::const_wstring... Items >( const cpp_utils::type_list< cpp_utils::value_identity< Items >... > ) static noexcept
            {
                ( cpp_utils::delete_registry_key(
                    HKEY_LOCAL_MACHINE, LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\Run)", Items.view() ),
                  ... );
            }( autorun_items{} );
        }
        template < cpp_utils::const_string Description, cpp_utils::const_string Command >
        struct cmd_item final
        {
            static inline constexpr auto description{ Description };
            static auto execute() noexcept
            {
                constexpr auto dividing_line{ cpp_utils::make_repeated_const_string< '-', console_width >() };
                std::print(
                  cpp_utils::value_identity_v< cpp_utils::concat_const_string(
                    make_title_text< "[ 工 具 箱 ]", 3 >, cpp_utils::const_string{ " -> 执行操作系统命令.\n\n" },
                    dividing_line, cpp_utils::const_string{ "\n\n" } ) >.view() );
                std::system( Command.c_str() );
                std::print(
                  cpp_utils::value_identity_v< cpp_utils::concat_const_string(
                    cpp_utils::const_string{ "\n" }, dividing_line, cpp_utils::const_string{ "\n\n (i) 操作已完成." } ) >.view() );
                press_any_key_to_return();
                return func_back;
            }
        };
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
    }
    inline auto toolkit()
    {
        using funcs = cpp_utils::type_list<
          details::func_item< "恢复操作系统组件", details::restore_os_components >,
          details::func_item< "重置 Hosts", details::reset_hosts >,
          details::func_item< R"(重置 "机房管理助手" 配置)", details::reset_jfglzs_config > >;
        using cmds = cpp_utils::type_list<
          details::cmd_item< "重启资源管理器", R"(taskkill.exe /f /im explorer.exe && timeout.exe /t 3 /nobreak && start explorer.exe)" >,
          details::cmd_item< "恢复 USB 设备访问", R"(reg.exe add "HKLM\SYSTEM\CurrentControlSet\Services\USBSTOR" /f /t reg_dword /v Start /d 3)" >,
          details::cmd_item< "重置 Google Chrome 管理策略", R"(reg.exe delete "HKLM\SOFTWARE\Policies\Google\Chrome" /f)" >,
          details::cmd_item< "重置 Microsoft Edge 管理策略", R"(reg.exe delete "HKLM\SOFTWARE\Policies\Microsoft\Edge" /f)" > >;
        cpp_utils::console_ui ui{ con, unsynced_mem_pool };
        ui.reserve( 5 + funcs::size + cmds::size )
          .add_back( make_title_text< "[ 工 具 箱 ]", 2 >.view() )
          .add_back( " < 返回 "sv, quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
          .add_back( "\n[ 高级工具 ]\n"sv )
          .add_back( " > 启动命令提示符 "sv, details::launch_cmd );
        [ & ]< typename... Items >( const cpp_utils::type_list< Items... > )
        { ( ui.add_back( make_button_text< Items::description >.view(), Items::execute ), ... ); }( funcs{} );
        ui.add_back( "\n[ 快捷命令 ]\n"sv );
        [ & ]< typename... Items >( const cpp_utils::type_list< Items... > )
        { ( ui.add_back( make_button_text< Items::description >.view(), Items::execute ), ... ); }( cmds{} );
        ui.show();
        return func_back;
    }
    namespace details
    {
        inline auto set_console_attrs() noexcept
        {
            const auto& window_options{ std::get< window_config >( config_nodes ) };
            const auto& is_enable_simple_titlebar{ window_options.at< "simple_titlebar" >() };
            const auto& is_translucent{ window_options.at< "translucent" >() };
            const auto& is_no_hot_reload{ std::get< performance_config >( config_nodes ).at< "no_hot_reload" >() };
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
            const auto& is_no_hot_reload{ std::get< performance_config >( config_nodes ).at< "no_hot_reload" >() };
            const auto& is_force_show{ std::get< window_config >( config_nodes ).at< "force_show" >() };
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
        inline constexpr std::array threads_func{ set_console_attrs, force_show };
    }
    inline auto create_threads() noexcept
    {
        for ( const auto func : details::threads_func ) {
            std::thread{ func }.detach();
        }
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
            ( Backends::hijack_execs(), ... );
            ( Backends::disable_servs(), ... );
            ( Backends::stop_servs(), ... );
            ( Backends::kill_execs(), ... );
            ( Backends::crack_helper(), ... );
            ( Backends::undo_hijack_execs(), ... );
            ( Backends::enable_servs(), ... );
            ( Backends::start_servs(), ... );
            ( Backends::restore_helper(), ... );
        }
    struct rule_executor final
    {
        static auto crack()
        {
            const auto& options{ std::get< crack_restore_config >( config_nodes ) };
            const auto can_hijack_execs{ options.at< "hijack_execs" >() };
            const auto can_set_serv_startup_types{ options.at< "set_serv_startup_types" >() };
            if ( can_hijack_execs ) {
                std::print( " -> 劫持文件.\n" );
                ( Backends::hijack_execs(), ... );
            }
            if ( can_set_serv_startup_types ) {
                std::print( " -> 禁用服务.\n" );
                ( Backends::disable_servs(), ... );
            }
            std::print( " -> 停止服务.\n" );
            ( Backends::stop_servs(), ... );
            std::print( " -> 终止进程.\n" );
            ( Backends::kill_execs(), ... );
            std::print( " -> 执行扩展操作.\n" );
            ( Backends::crack_helper(), ... );
        }
        static auto restore()
        {
            const auto& options{ std::get< crack_restore_config >( config_nodes ) };
            const auto can_hijack_execs{ options.at< "hijack_execs" >() };
            const auto can_set_serv_startup_types{ options.at< "set_serv_startup_types" >() };
            if ( can_hijack_execs ) {
                std::print( " -> 撤销劫持.\n" );
                ( Backends::undo_hijack_execs(), ... );
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
                case details::rule_executor_mode::crack : std::print( "                    [ 破  解 ]\n\n\n" ); break;
                case details::rule_executor_mode::restore : std::print( "                    [ 恢  复 ]\n\n\n" ); break;
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
        static auto hijack_execs() noexcept
        {
            []< cpp_utils::const_wstring... Execs >( const cpp_utils::type_list< cpp_utils::value_identity< Execs >... > ) static noexcept
            {
                constexpr const wchar_t data[]{ L"nul" };
                ( cpp_utils::create_registry_key(
                    HKEY_LOCAL_MACHINE,
                    cpp_utils::value_identity_v< cpp_utils::concat_const_string(
                      cpp_utils::const_wstring{ LR"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\)" },
                      Execs, cpp_utils::const_wstring{ L".exe" } ) >.view(),
                    L"Debugger", cpp_utils::registry_flag::string_type, std::bit_cast< const BYTE* >( +data ), sizeof( data ) ),
                  ... );
            }( typename BuiltinRuleNode::execs{} );
        }
        static auto disable_servs() noexcept
        {
            []< cpp_utils::const_wstring... Servs >( const cpp_utils::type_list< cpp_utils::value_identity< Servs >... > ) static noexcept
            {
                ( cpp_utils::set_service_status( Servs.view(), cpp_utils::service_flag::disabled_start ), ... );
            }( typename BuiltinRuleNode::servs{} );
        }
        static auto stop_servs() noexcept
        {
            []< cpp_utils::const_wstring... Servs >( const cpp_utils::type_list< cpp_utils::value_identity< Servs >... > ) static noexcept
            {
                ( cpp_utils::stop_service_with_dependencies( Servs.view(), unsynced_mem_pool ), ... );
            }( typename BuiltinRuleNode::servs{} );
        }
        static auto kill_execs() noexcept
        {
            []< cpp_utils::const_wstring... Execs >( const cpp_utils::type_list< cpp_utils::value_identity< Execs >... > ) static noexcept
            {
                ( cpp_utils::terminate_process_by_name( cpp_utils::value_identity_v< cpp_utils::concat_const_string( Execs, cpp_utils::const_wstring{ L".exe" } ) >.view() ), ... );
            }( typename BuiltinRuleNode::execs{} );
        }
        static auto crack_helper()
        {
            BuiltinRuleNode::crack_helper();
        }
        static auto undo_hijack_execs() noexcept
        {
            []< cpp_utils::const_wstring... Execs >( const cpp_utils::type_list< cpp_utils::value_identity< Execs >... > ) static noexcept
            {
                ( cpp_utils::delete_registry_tree(
                    HKEY_LOCAL_MACHINE,
                    cpp_utils::value_identity_v< cpp_utils::concat_const_string(
                      cpp_utils::const_wstring{ LR"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\)" },
                      Execs, cpp_utils::const_wstring{ L".exe" }) >.view() ),
                  ... );
            }( typename BuiltinRuleNode::execs{} );
        }
        static auto enable_servs() noexcept
        {
            []< cpp_utils::const_wstring... Servs >( const cpp_utils::type_list< cpp_utils::value_identity< Servs >... > ) static noexcept
            {
                ( cpp_utils::set_service_status( Servs.view(), cpp_utils::service_flag::auto_start ), ... );
            }( typename BuiltinRuleNode::servs{} );
        }
        static auto start_servs() noexcept
        {
            []< cpp_utils::const_wstring... Servs >( const cpp_utils::type_list< cpp_utils::value_identity< Servs >... > ) static noexcept
            {
                ( cpp_utils::start_service_with_dependencies( Servs.view(), unsynced_mem_pool ), ... );
            }( typename BuiltinRuleNode::servs{} );
        }
        static auto restore_helper()
        {
            BuiltinRuleNode::restore_helper();
        }
    };
    struct custom_rule_executor_backend final
    {
        static auto hijack_execs()
        {
            constexpr const wchar_t data[]{ L"nul" };
            for ( const auto& exec : custom_rules.execs ) {
                cpp_utils::create_registry_key(
                  HKEY_LOCAL_MACHINE,
                  std::format( LR"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\{}.exe)", exec ),
                  L"Debugger", cpp_utils::registry_flag::string_type, std::bit_cast< const BYTE* >( +data ), sizeof( data ) );
            }
        }
        static auto disable_servs() noexcept
        {
            for ( const auto& serv : custom_rules.servs ) {
                cpp_utils::set_service_status( serv, cpp_utils::service_flag::disabled_start );
            }
        }
        static auto stop_servs() noexcept
        {
            for ( const auto& serv : custom_rules.servs ) {
                cpp_utils::stop_service_with_dependencies( serv, unsynced_mem_pool );
            }
        }
        static auto kill_execs()
        {
            for ( const auto& exec : custom_rules.execs ) {
                cpp_utils::terminate_process_by_name( std::format( L"{}.exe", exec ) );
            }
        }
        static auto crack_helper() noexcept
        { }
        static auto undo_hijack_execs()
        {
            for ( const auto& exec : custom_rules.execs ) {
                cpp_utils::delete_registry_tree(
                  HKEY_LOCAL_MACHINE,
                  std::format( LR"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\{}.exe)", exec ) );
            }
        }
        static auto enable_servs() noexcept
        {
            for ( const auto& serv : custom_rules.servs ) {
                cpp_utils::set_service_status( serv, cpp_utils::service_flag::auto_start );
            }
        }
        static auto start_servs() noexcept
        {
            for ( const auto& serv : custom_rules.servs ) {
                cpp_utils::start_service_with_dependencies( serv, unsynced_mem_pool );
            }
        }
        static auto restore_helper() noexcept
        { }
    };
    inline consteval auto execute_all_rules() noexcept
    {
        using builtin_rules_executor_backends = builtin_rules::apply_each< builtin_rules_executor_backend >;
        return builtin_rules_executor_backends::add_front< custom_rule_executor_backend >::apply< rule_executor >{};
    }
    inline auto make_executor_mode_ui_text() noexcept
    {
        switch ( details::current_rule_executor_mode ) {
            case details::rule_executor_mode::crack : return "[ 破解 (点击切换) ]\n"sv;
            case details::rule_executor_mode::restore : return "[ 恢复 (点击切换) ]\n"sv;
            default : std::unreachable();
        }
    }
    inline auto flip_executor_mode( const ui_func_args args ) noexcept
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
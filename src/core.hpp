#pragma once
#include <cpp_utils/const_string.hpp>
#include <cpp_utils/math.hpp>
#include <cpp_utils/meta.hpp>
#include <cpp_utils/multithread.hpp>
#include <cpp_utils/windows_app_tools.hpp>
#include <cpp_utils/windows_console_ui.hpp>
#include <fstream>
#include <limits>
#include "info.hpp"
namespace core
{
    using namespace std::chrono_literals;
    using namespace std::string_view_literals;
    inline constexpr SHORT console_width{ 50 };
    inline constexpr SHORT console_height{ 25 };
    inline constexpr UINT charset_id{ 936 };
    inline constexpr auto default_thread_sleep_time{ 1s };
    inline constexpr auto config_file_name{ "config.ini" };
    inline constexpr auto func_back{ cpp_utils::console_ui::func_back };
    inline constexpr auto func_exit{ cpp_utils::console_ui::func_exit };
    inline constexpr auto diving_line{ cpp_utils::make_repeated_const_string< '-', console_width >() };
    inline const auto window_handle{ GetConsoleWindow() };
    inline const auto std_input_handle{ GetStdHandle( STD_INPUT_HANDLE ) };
    inline const auto std_output_handle{ GetStdHandle( STD_OUTPUT_HANDLE ) };
    static_assert( default_thread_sleep_time.count() != 0 );
    using ui_func_args = cpp_utils::console_ui::func_args;
    inline auto quit() noexcept
    {
        return func_exit;
    }
    inline auto relaunch() noexcept
    {
        cpp_utils::relaunch_as_admin( EXIT_SUCCESS );
        return func_exit;
    }
    namespace details
    {
        inline auto wait_to_return() noexcept
        {
            std::print( "\n\n" );
            for ( unsigned short t{ 3 }; t > 0; --t ) {
                std::print( " {} 秒后返回...\r", t );
                std::this_thread::sleep_for( 1s );
            }
        }
        inline auto is_space( const char ch ) noexcept
        {
            switch ( ch ) {
                case '\f' :
                case '\v' :
                case '\t' :
                case ' ' : return true;
            }
            return false;
        }
    }
    struct rule_node final
    {
        using item_t      = std::string;
        using container_t = std::deque< item_t >;
        const char* shown_name{};
        container_t execs{};
        container_t servs{};
        auto empty() const noexcept
        {
            return execs.empty() && servs.empty();
        }
    };
    inline rule_node custom_rules;
    inline const std::array< rule_node, 4 > builtin_rules{
      { { "学生机房管理助手", { "jfglzs", "przs", "zmserv", "vprtt", "oporn" }, { "zmserv" } },
       { "极域电子教室",
          { "StudentMain", "DispcapHelper", "VRCwPlayer", "InstHelpApp", "InstHelpApp64", "TDOvrSet", "GATESRV", "ProcHelper64",
            "MasterHelper" },
          { "TDNetFilter", "TDFileFilter", "STUDSRV" } },
       { "联想智能云教室",
          { "vncviewer", "tvnserver32", "WfbsPnpInstall", "WFBSMon", "WFBSMlogon", "WFBSSvrLogShow", "ResetIp", "FuncForWIN64",
            "CertMgr", "Fireware", "BCDBootCopy", "refreship", "lenovoLockScreen", "PortControl64", "DesktopCheck",
            "DeploymentManager", "DeploymentAgent", "XYNTService" },
          { "BSAgentSvr", "tvnserver", "WFBSMlogon" } },
       { "红蜘蛛多媒体网络教室",
          { "rscheck", "checkrs", "REDAgent", "PerformanceCheck", "edpaper", "Adapter", "repview", "FormatPaper" },
          { "appcheck2", "checkapp2" } } }
    };
    namespace details
    {
        class config_node_impl
        {
          public:
            const char* raw_name;
            auto load( this auto&& self, const bool is_reload, std::string& line )
            {
                self.load_( is_reload, line );
            }
            auto sync( this auto&& self, std::ofstream& out )
            {
                self.sync_( out );
            }
            auto prepare_reload( this auto&& self )
            {
                using child_t = std::decay_t< decltype( self ) >;
                if constexpr ( requires( child_t obj ) { obj.prepare_reload_(); } ) {
                    self.prepare_reload_();
                }
            }
            auto edit_ui( this auto&& self, cpp_utils::console_ui& parent_ui )
            {
                using child_t = std::decay_t< decltype( self ) >;
                if constexpr ( requires( child_t obj ) { obj.edit_ui_( parent_ui ); } ) {
                    self.edit_ui_( parent_ui );
                }
            }
        };
    }
    class options final : public details::config_node_impl
    {
        friend details::config_node_impl;
      public:
        class item final
        {
          private:
            std::atomic< bool > value_{ false };
          public:
            const char* raw_name;
            const char* shown_name;
            auto set( const bool value ) noexcept
            {
                value_.store( value, std::memory_order_release );
            }
            auto get() const noexcept
            {
                return value_.load( std::memory_order_acquire );
            }
            operator bool() const noexcept
            {
                return get();
            }
            auto operator=( const item& ) -> item&     = delete;
            auto operator=( item&& ) noexcept -> item& = delete;
            item( const char* const raw_name, const char* const shown_name ) noexcept
              : raw_name{ raw_name }
              , shown_name{ shown_name }
            { }
            item( const item& src ) noexcept
              : value_{ src }
              , raw_name{ src.raw_name }
              , shown_name{ src.shown_name }
            { }
            item( item&& src ) noexcept
              : value_{ src }
              , raw_name{ src.raw_name }
              , shown_name{ src.shown_name }
            { }
            ~item() noexcept = default;
        };
        class category final
        {
          private:
            static auto is_same_raw_name_( const item& item, const std::string_view raw_name )
            {
                return raw_name == item.raw_name;
            }
          public:
            const char* raw_name;
            const char* shown_name;
            std::vector< item > items;
            const auto& operator[]( const std::string_view raw_name ) const noexcept
            {
                const auto result{ std::ranges::find_if( items, std::bind_back( is_same_raw_name_, raw_name ) ) };
                if constexpr ( cpp_utils::is_debugging_build ) {
                    if ( result == items.end() ) {
                        std::print( "'{}' does not exist!", raw_name );
                        std::terminate();
                    }
                }
                return *result;
            }
            auto& operator[]( const std::string_view raw_name ) noexcept
            {
                return const_cast< item& >( std::as_const( *this )[ raw_name ] );
            }
        };
      private:
        std::vector< category > categories_{
          { { "crack_restore",
              "破解与恢复",
              { { "hijack_execs", "劫持可执行文件" },
                { "set_serv_startup_types", "设置服务启动类型" },
                { "fast_mode", "快速模式" } } },
           { "window",
              "窗口显示",
              { { "keep_window_top", "* 置顶窗口" }, { "simple_titlebar", "* 极简标题栏" }, { "translucent", "* 半透明" } } },
           { "misc", "杂项", { { "no_optional_hot_reload", "** 禁用标 * 选项热重载" } } } }
        };
        static constexpr auto format_string_{ "{}.{}: {}" };
        static auto make_swith_button_text_( const auto is_enable )
        {
            return is_enable ? " > 禁用 "sv : " > 启用 "sv;
        }
        auto load_( const bool is_reload, const std::string& line )
        {
            if ( is_reload ) {
                return;
            }
            for ( auto& category : categories_ ) {
                for ( auto& item : category.items ) {
                    if ( line == std::format( format_string_, category.raw_name, item.raw_name, true ) ) {
                        item.set( true );
                    } else if ( line == std::format( format_string_, category.raw_name, item.raw_name, false ) ) {
                        item.set( false );
                    }
                }
            }
        }
        auto sync_( std::ofstream& out )
        {
            for ( const auto& category : categories_ ) {
                for ( const auto& item : category.items ) {
                    out << std::format( format_string_, category.raw_name, item.raw_name, item.get() ) << '\n';
                }
            }
        }
        static auto set_item_value_( ui_func_args args, item& item )
        {
            item.set( !item.get() );
            args.parent_ui.edit_text( args.node_index, make_swith_button_text_( item ) );
            return func_back;
        }
        static auto make_category_ui_( category& category )
        {
            cpp_utils::console_ui ui;
            ui.add_back( "                    [ 配  置 ]\n\n" )
              .add_back(
                std::format( " < 折叠 {} ", category.shown_name ), quit,
                cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity );
            for ( auto& item : category.items ) {
                ui.add_back( std::format( "\n[ {} ]\n", item.shown_name ) )
                  .add_back(
                    make_swith_button_text_( item ), std::bind_back( set_item_value_, std::ref( item ) ),
                    cpp_utils::console_text::foreground_red | cpp_utils::console_text::foreground_green );
            }
            ui.show();
            return func_back;
        }
        auto edit_ui_( cpp_utils::console_ui& ui )
        {
            ui.add_back(
              "\n[ 选项 ]\n\n"
              " (i) 选项相关信息可参阅文档. 标 * 选项自动热重载.\n"
              "     标 ** 选项无法热重载. 其余选项实时热重载.\n" );
            for ( auto& category : categories_ ) {
                ui.add_back(
                  std::format( " > {} ", category.shown_name ), std::bind_back( make_category_ui_, std::ref( category ) ),
                  cpp_utils::console_text::foreground_red | cpp_utils::console_text::foreground_green );
            }
        }
        static auto is_same_raw_name_( const category& category, const std::string_view raw_name )
        {
            return raw_name == category.raw_name;
        }
      public:
        const auto& operator[]( const std::string_view raw_name ) const noexcept
        {
            const auto result{ std::ranges::find_if( categories_, std::bind_back( is_same_raw_name_, raw_name ) ) };
            if constexpr ( cpp_utils::is_debugging_build ) {
                if ( result == categories_.end() ) {
                    std::print( "'{}' does not exist!", raw_name );
                    std::terminate();
                }
            }
            return *result;
        }
        auto& operator[]( const std::string_view raw_name ) noexcept
        {
            return const_cast< category& >( std::as_const( *this )[ raw_name ] );
        }
        options() noexcept
          : config_node_impl{ "options" }
        { }
        ~options() noexcept = default;
    };
    class customized_rules final : public details::config_node_impl
    {
        friend details::config_node_impl;
      private:
        static constexpr auto suffix_exec{ "exec: "sv };
        static constexpr auto suffix_serv{ "serv: "sv };
        static auto load_( const bool, const std::string_view line )
        {
            if ( line.size() > suffix_exec.size() && line.starts_with( suffix_exec ) ) {
                custom_rules.execs.emplace_back( std::ranges::find_if_not( line.substr( suffix_exec.size() ), details::is_space ) );
                return;
            }
            if ( line.size() > suffix_serv.size() && line.starts_with( suffix_serv ) ) {
                custom_rules.servs.emplace_back( std::ranges::find_if_not( line.substr( suffix_serv.size() ), details::is_space ) );
                return;
            }
        }
        static auto sync_( std::ofstream& out )
        {
            for ( const auto& exec : custom_rules.execs ) {
                out << suffix_exec << exec << '\n';
            }
            for ( const auto& serv : custom_rules.servs ) {
                out << suffix_serv << serv << '\n';
            }
        }
        static auto prepare_reload_() noexcept
        {
            custom_rules.execs.clear();
            custom_rules.servs.clear();
        }
        static auto show_help_info_()
        {
            cpp_utils::console_ui ui;
            ui.add_back( "                    [ 配  置 ]\n\n" )
              .add_back( " < 返回 ", quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
              .add_back(
                "\n 自定义规则格式为 <flag>: <item>\n"
                " 其中, <flag> 可为 exec 或 serv,\n"
                " 分别表示以 .exe 为文件扩展名的可执行文件\n"
                " 和某个 Windows 服务的服务名称.\n"
                " <flag> 后的冒号与 <item> 间至少有一个空格.\n"
                " <item> 的类型由 <flag> 决定.\n"
                " 如果 <item> 为空, 该项规则将会被忽略.\n"
                " 如果自定义规则不符合格式, 则会被忽略.\n"
                " 本配置项不对自定义规则的正确性进行检测,\n"
                " 在修改自定义规则时, 请仔细检查.\n"
                " 更多信息请参阅文档.\n\n"
                " 使用示例:\n\n"
                " [customized_rules]\n"
                " exec: abc_client_gui\n"
                " exec: abc_client_server\n"
                " exec: abc_protect_server\n"
                " serv: abc_network\n"
                " serv: abc_diag_track\n" )
              .show();
            return func_back;
        }
        static auto edit_ui_( cpp_utils::console_ui& ui )
        {
            ui.add_back( "\n[ 自定义规则 ]\n" ).add_back( " > 查看帮助信息 ", show_help_info_ );
        }
      public:
        customized_rules() noexcept
          : config_node_impl{ "customized_rules" }
        { }
        ~customized_rules() noexcept = default;
    };
    namespace details
    {
        template < typename T >
        struct is_valid_config_node final
        {
            static constexpr auto value{ std::is_base_of_v< config_node_impl, T > };
        };
    }
    using config_node_types = cpp_utils::type_list< options, customized_rules >;
    inline config_node_types::apply< std::tuple > config_nodes{};
    static_assert( config_node_types::all_of< details::is_valid_config_node > );
    static_assert( config_node_types::all_of< std::is_default_constructible > );
    static_assert( config_node_types::unique::size == config_node_types::size );
    auto& options_set{ std::get< options >( config_nodes ) };
    namespace details
    {
        inline const auto& is_no_optional_hot_reload{ options_set[ "misc" ][ "no_optional_hot_reload" ] };
        inline auto get_config_node_raw_name_by_tag( std::string_view str ) noexcept
        {
            str = str.substr( 1, str.size() - 2 );
            const auto head{ std::ranges::find_if_not( str, is_space ) };
            const auto tail{ std::ranges::find_if_not( str.rbegin(), str.rend(), is_space ).base() };
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
        std::apply( []( auto&... config_node ) { ( config_node.prepare_reload(), ... ); }, config_nodes );
        std::string line;
        config_node_types::transform< std::add_pointer >::prepend< std::monostate >::apply< std::variant > current_config_node;
        while ( std::getline( config_file, line ) ) {
            if ( line.empty() ) {
                continue;
            }
            if ( line.front() == '#' ) {
                continue;
            }
            line.erase( std::ranges::find_if_not( line.rbegin(), line.rend(), details::is_space ).base(), line.end() );
            if ( line.front() == '[' && line.back() == ']' && line.size() > "[]"sv.size() ) {
                current_config_node = std::monostate{};
                std::apply( [ & ]( auto&... config_node )
                {
                    ( [ & ]( auto& current_node ) noexcept
                    {
                        if ( details::get_config_node_raw_name_by_tag( line ) == current_node.raw_name ) {
                            current_config_node = &current_node;
                        }
                    }( config_node ), ... );
                }, config_nodes );
                continue;
            }
            current_config_node.visit( [ & ]< typename T >( const T node_ptr )
            {
                if constexpr ( !std::is_same_v< T, std::monostate > ) {
                    node_ptr->load( is_reload, line );
                }
            } );
        }
    }
    namespace details
    {
        inline auto show_config_parsing_rules()
        {
            cpp_utils::console_ui ui;
            ui.add_back( "                    [ 配  置 ]\n\n" )
              .add_back( " < 返回 ", quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
              .add_back(
                "\n 配置以行作为单位解析.\n\n"
                " 以 # 开头的行是注释.\n\n"
                " 各个配置项在配置文件中由不同标签区分,\n"
                " 标签的格式为 [<标签名>],\n"
                " <标签名> 与中括号间可以有若干空格.\n\n"
                " 如果匹配不到配置项,\n"
                " 则当前读取的标签到下一标签之间的内容都将被忽略.\n\n"
                " 解析时会忽略每行末尾的空白字符.\n"
                " 不会忽略每行的前导空白字符.\n"
                " 如果当前行不是标签, 则该行将由上一个标签处理.\n\n"
                " 更多信息请参阅文档." )
              .show();
            return func_back;
        }
        inline auto sync_config()
        {
            std::print(
              "                    [ 配  置 ]\n\n\n"
              " -> 正在同步配置...\n" );
            load_config( true );
            std::ofstream config_file_stream{ config_file_name, std::ios::out | std::ios::trunc };
            config_file_stream << "# " INFO_FULL_NAME "\n# " INFO_VERSION "\n";
            std::apply( [ & ]( auto&... config_node )
            {
                ( ( config_file_stream << std::format( "[{}]\n", config_node.raw_name ), config_node.sync( config_file_stream ) ),
                  ... );
            }, config_nodes );
            config_file_stream.flush();
            std::print( "\n (i) 同步配置{}.", config_file_stream.good() ? "成功" : "失败" );
            details::wait_to_return();
            return func_back;
        }
        inline auto open_config_file()
        {
            std::print(
              "                    [ 配  置 ]\n\n\n"
              " -> 正在尝试打开配置文件...\n\n" );
            const auto is_success{
              std::bit_cast< INT_PTR >( ShellExecuteA( nullptr, "open", config_file_name, nullptr, nullptr, SW_SHOWNORMAL ) ) > 32 };
            std::print( " (i) 打开配置文件{}.", is_success ? "成功" : "失败" );
            details::wait_to_return();
            return func_back;
        }
    }
    inline auto config_ui()
    {
        cpp_utils::console_ui ui;
        ui.add_back( "                    [ 配  置 ]\n\n" )
          .add_back( " < 返回 ", quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
          .add_back( " > 查看解析规则", details::show_config_parsing_rules )
          .add_back( " > 同步配置 ", details::sync_config )
          .add_back( " > 打开配置文件 ", details::open_config_file );
        std::apply( [ & ]( auto&... config_node ) { ( config_node.edit_ui( ui ), ... ); }, config_nodes );
        ui.show();
        return func_back;
    }
    namespace details
    {
        inline auto visit_repo_webpage()
        {
            std::thread{ ShellExecuteA, nullptr, "open", INFO_REPO_URL, nullptr, nullptr, SW_SHOWNORMAL }.detach();
            return func_back;
        }
    }
    inline auto info()
    {
        cpp_utils::console_ui ui;
        ui.add_back( "                    [ 关  于 ]\n\n" )
          .add_back( " < 返回 ", quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
          .add_back(
            "\n[ 名称 ]\n\n " INFO_FULL_NAME " (" INFO_SHORT_NAME ")\n\n[ 版本 ]\n\n " INFO_VERSION
            "\n\n 构建时间: " INFO_BUILD_TIME "\n 编译工具: " INFO_COMPILER " " INFO_ARCH
            "\n\n[ 许可证与版权 ]\n\n " INFO_LICENSE "\n " INFO_COPYRIGHT "\n\n[ 仓库 ]\n" )
          .add_back(
            " " INFO_REPO_URL " ", details::visit_repo_webpage,
            cpp_utils::console_text::foreground_white | cpp_utils::console_text::foreground_intensity
              | cpp_utils::console_text::lvb_underscore )
          .show();
        return func_back;
    }
    namespace details
    {
        inline auto launch_cmd( ui_func_args args )
        {
            cpp_utils::set_current_console_title( INFO_SHORT_NAME " - 命令提示符" );
            cpp_utils::set_console_size( window_handle, std_output_handle, 120, 30 );
            cpp_utils::fix_window_size( window_handle, false );
            cpp_utils::enable_window_maximize_ctrl( window_handle, true );
            args.parent_ui.set_limits( false, false );
            SetConsoleScreenBufferSize( std_output_handle, COORD{ 120, std::numeric_limits< SHORT >::max() - 1 } );
            STARTUPINFOA startup_info{};
            startup_info.cb = sizeof( startup_info );
            PROCESS_INFORMATION process_info;
            if ( CreateProcessA(
                   nullptr, const_cast< LPSTR >( "cmd.exe" ), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &startup_info,
                   &process_info ) )
            {
                WaitForSingleObject( process_info.hProcess, INFINITE );
                CloseHandle( process_info.hProcess );
                CloseHandle( process_info.hThread );
            }
            cpp_utils::set_current_console_charset( charset_id );
            cpp_utils::set_current_console_title( INFO_SHORT_NAME );
            cpp_utils::set_console_size( window_handle, std_output_handle, console_width, console_height );
            cpp_utils::fix_window_size( window_handle, true );
            cpp_utils::enable_window_maximize_ctrl( window_handle, false );
            return func_back;
        }
        inline auto restore_os_components()
        {
            std::print(
              "                   [ 工 具 箱 ]\n\n\n"
              " -> 正在尝试恢复...\n\n" );
            constexpr std::array reg_dirs{
              R"(Software\Policies\Microsoft\Windows\System)", R"(Software\Microsoft\Windows\CurrentVersion\Policies\System)",
              R"(Software\Microsoft\Windows\CurrentVersion\Policies\Explorer)", R"(Software\Policies\Microsoft\MMC)" };
            constexpr std::array execs{
              "taskkill", "ntsd", "sc", "net", "reg", "cmd", "taskmgr", "perfmon", "regedit", "mmc", "dism", "sfc" };
            for ( const auto reg_dir : reg_dirs ) {
                RegDeleteTreeA( HKEY_CURRENT_USER, reg_dir );
            }
            for ( const auto exec : execs ) {
                RegDeleteTreeA(
                  cpp_utils::registry::hkey_local_machine,
                  std::format( R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\{}.exe)", exec ).c_str() );
            }
            std::print( " (i) 操作已完成." );
            details::wait_to_return();
            return func_back;
        }
        struct cmd_item final
        {
            const char* description;
            const char* command;
        };
        inline auto execute_cmd( const cmd_item& item )
        {
            std::print(
              "                   [ 工 具 箱 ]\n\n\n"
              " -> 正在执行操作系统命令...\n\n{}\n\n",
              diving_line.data() );
            std::system( item.command );
            return func_exit;
        }
        inline auto make_cmd_executor_ui( const cmd_item& item )
        {
            cpp_utils::console_ui ui;
            ui.add_back(
                "                   [ 工 具 箱 ]\n\n\n"
                " (i) 是否继续执行?\n" )
              .add_back(
                " > 是, 继续 ", std::bind_back( execute_cmd, std::cref( item ) ),
                cpp_utils::console_text::foreground_red | cpp_utils::console_text::foreground_intensity )
              .add_back( " > 否, 返回 ", quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
              .show();
            return func_back;
        }
    }
    inline auto toolkit()
    {
        constexpr std::array< details::cmd_item, 5 > common_cmds{
          { { "重启资源管理器", R"(taskkill.exe /f /im explorer.exe && timeout.exe /t 3 /nobreak && start C:\Windows\explorer.exe)" },
           { "重启至高级选项", "shutdown.exe /r /o /f /t 0" },
           { "恢复 USB 设备访问", R"(reg.exe add "HKLM\SYSTEM\CurrentControlSet\Services\USBSTOR" /f /t reg_dword /v Start /d 3)" },
           { "恢复 Google Chrome 离线游戏", R"(reg.exe delete "HKLM\SOFTWARE\Policies\Google\Chrome" /f /v AllowDinosaurEasterEgg)" },
           { "恢复 Microsoft Edge 离线游戏", R"(reg.exe delete "HKLM\SOFTWARE\Policies\Microsoft\Edge" /f /v AllowSurfGame)" } }
        };
        cpp_utils::console_ui ui;
        ui.add_back( "                   [ 工 具 箱 ]\n\n" )
          .add_back( " < 返回 ", quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
          .add_back( " > 命令提示符 ", details::launch_cmd )
          .add_back( " > 恢复操作系统组件 ", details::restore_os_components )
          .add_back( "\n[ 常用命令 ]\n" );
        for ( const auto& common_cmd : common_cmds ) {
            ui.add_back(
              std::format( " > {} ", common_cmd.description ),
              std::bind_back( details::make_cmd_executor_ui, std::cref( common_cmd ) ) );
        }
        ui.show();
        return func_back;
    }
    inline auto set_console_attrs()
    {
        const auto& window_options{ options_set[ "window" ] };
        const auto& is_enable_simple_titlebar{ window_options[ "simple_titlebar" ] };
        const auto& is_translucent{ window_options[ "translucent" ] };
        auto core_op{ [ & ]
        {
            cpp_utils::enable_window_menu( window_handle, !is_enable_simple_titlebar );
            cpp_utils::set_window_translucency( window_handle, is_translucent ? 230 : 255 );
        } };
        if ( details::is_no_optional_hot_reload ) {
            core_op();
            return;
        }
        while ( true ) {
            core_op();
            std::this_thread::sleep_for( default_thread_sleep_time );
        }
    }
    inline auto keep_window_top()
    {
        const auto& is_keep_window_top{ options_set[ "window" ][ "keep_window_top" ] };
        if ( details::is_no_optional_hot_reload && !is_keep_window_top ) {
            return;
        }
        constexpr auto sleep_time{ 100ms };
        const auto current_thread_id{ GetCurrentThreadId() };
        const auto current_window_thread_process_id{ GetWindowThreadProcessId( window_handle, nullptr ) };
        if ( details::is_no_optional_hot_reload ) {
            cpp_utils::loop_keep_window_top( window_handle, current_thread_id, current_window_thread_process_id, sleep_time );
            return;
        }
        while ( true ) {
            if ( !is_keep_window_top ) {
                cpp_utils::cancel_top_window( window_handle );
                std::this_thread::sleep_for( default_thread_sleep_time );
                continue;
            }
            cpp_utils::keep_window_top( window_handle, current_thread_id, current_window_thread_process_id );
            std::this_thread::sleep_for( sleep_time );
        }
    }
    namespace details
    {
        using rule_item_const_ref_t = cpp_utils::add_const_lvalue_reference_t< rule_node::item_t >;
        inline constexpr auto default_executing_sleep_time{ 50ms };
        static_assert( default_executing_sleep_time.count() != 0 );
        inline const auto& option_crack_restore{ options_set[ "crack_restore" ] };
        inline const auto& is_hijack_execs{ option_crack_restore[ "hijack_execs" ] };
        inline const auto& is_set_serv_startup_types{ option_crack_restore[ "set_serv_startup_types" ] };
        inline const auto& is_enable_fast_mode{ option_crack_restore[ "fast_mode" ] };
        enum class rule_executing : bool
        {
            crack,
            restore
        };
        inline auto executor_mode{ rule_executing::crack };
        inline auto make_progress( const std::size_t now, const std::size_t total, const std::size_t digits_of_total ) noexcept
        {
            return std::format( "({}{}/{})", std::string( digits_of_total - cpp_utils::count_digits( now ), ' ' ), now, total );
        }
        inline auto for_each_wrapper( const rule_node::container_t container, void ( *func )( rule_item_const_ref_t ) )
        {
            static const auto nproc_for_executing{ std::max< unsigned >( std::thread::hardware_concurrency(), 4 ) };
            cpp_utils::parallel_for_each( nproc_for_executing, container.begin(), container.end(), func );
        }
        inline auto hijack_exec( rule_item_const_ref_t exec ) noexcept
        {
            cpp_utils::create_registry_key< charset_id >(
              cpp_utils::registry::hkey_local_machine,
              std::format( R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\{}.exe)", exec ).c_str(),
              "Debugger", cpp_utils::registry::string_type, reinterpret_cast< const BYTE* >( L"nul" ), sizeof( L"nul" ) );
        }
        inline auto disable_serv( rule_item_const_ref_t serv ) noexcept
        {
            cpp_utils::set_service_status< charset_id >( serv.c_str(), cpp_utils::service::disabled_start );
        }
        inline auto kill_exec( rule_item_const_ref_t exec ) noexcept
        {
            cpp_utils::kill_process_by_name< charset_id >( std::format( "{}.exe", exec ).c_str() );
        }
        inline auto stop_serv( rule_item_const_ref_t serv ) noexcept
        {
            cpp_utils::stop_service_with_dependencies< charset_id >( serv.c_str() );
        }
        inline auto undo_hijack_exec( rule_item_const_ref_t exec ) noexcept
        {
            cpp_utils::delete_registry_tree< charset_id >(
              cpp_utils::registry::hkey_local_machine,
              std::format( R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\{}.exe)", exec ).c_str() );
        }
        inline auto enable_serv( rule_item_const_ref_t serv ) noexcept
        {
            cpp_utils::set_service_status< charset_id >( serv.c_str(), cpp_utils::service::auto_start );
        }
        inline auto start_serv( rule_item_const_ref_t serv ) noexcept
        {
            cpp_utils::start_service_with_dependencies< charset_id >( serv.c_str() );
        }
        inline auto default_crack( const rule_node& rules )
        {
            std::print( "{}\n\n", diving_line.data() );
            const auto& execs{ rules.execs };
            const auto& servs{ rules.servs };
            std::size_t finished_count{ 0 };
            const auto total_count{
              execs.size() * ( details::is_hijack_execs ? 2 : 1 ) + servs.size() * ( details::is_set_serv_startup_types ? 2 : 1 ) };
            const auto digits_of_total{ cpp_utils::count_digits( total_count ) };
            if ( details::is_hijack_execs ) {
                for ( const auto& exec : execs ) {
                    std::print(
                      "{} 劫持文件 {}.exe.\n", details::make_progress( ++finished_count, total_count, digits_of_total ), exec );
                    hijack_exec( exec );
                    std::this_thread::sleep_for( default_executing_sleep_time );
                }
            }
            if ( details::is_set_serv_startup_types ) {
                for ( const auto& serv : servs ) {
                    std::print( "{} 禁用服务 {}.\n", details::make_progress( ++finished_count, total_count, digits_of_total ), serv );
                    disable_serv( serv );
                    std::this_thread::sleep_for( default_executing_sleep_time );
                }
            }
            for ( const auto& exec : execs ) {
                std::print( "{} 终止进程 {}.exe.\n", details::make_progress( ++finished_count, total_count, digits_of_total ), exec );
                kill_exec( exec );
                std::this_thread::sleep_for( default_executing_sleep_time );
            }
            for ( const auto& serv : servs ) {
                std::print( "{} 停止服务 {}.\n", details::make_progress( ++finished_count, total_count, digits_of_total ), serv );
                stop_serv( serv );
                std::this_thread::sleep_for( default_executing_sleep_time );
            }
            std::print( "\n{}\n\n", diving_line.data() );
        }
        inline auto fast_crack( const rule_node& rules )
        {
            const auto& execs{ rules.execs };
            const auto& servs{ rules.servs };
            using thread_t = std::thread;
            std::array< thread_t, 4 > threads;
            if ( details::is_hijack_execs ) {
                threads[ 0 ] = thread_t{ for_each_wrapper, std::cref( execs ), hijack_exec };
            }
            if ( details::is_set_serv_startup_types ) {
                threads[ 1 ] = thread_t{ for_each_wrapper, std::cref( servs ), disable_serv };
            }
            threads[ 2 ] = thread_t{ for_each_wrapper, std::cref( execs ), kill_exec };
            threads[ 3 ] = thread_t{ for_each_wrapper, std::cref( servs ), stop_serv };
            for ( auto& thread : threads ) {
                if ( thread.joinable() ) {
                    thread.join();
                }
            }
        }
        inline auto default_restore( const rule_node& rules )
        {
            std::print( "{}\n\n", diving_line.data() );
            const auto& execs{ rules.execs };
            const auto& servs{ rules.servs };
            std::size_t finished_count{ 0 };
            const auto total_count{
              ( details::is_hijack_execs ? execs.size() : 0 ) + servs.size() * ( details::is_set_serv_startup_types ? 2 : 1 ) };
            const auto digits_of_total{ cpp_utils::count_digits( total_count ) };
            if ( details::is_hijack_execs ) {
                for ( const auto& exec : execs ) {
                    std::print(
                      "{} 撤销劫持 {}.exe.\n", details::make_progress( ++finished_count, total_count, digits_of_total ), exec );
                    undo_hijack_exec( exec );
                    std::this_thread::sleep_for( default_executing_sleep_time );
                }
            }
            if ( details::is_set_serv_startup_types ) {
                for ( const auto& serv : servs ) {
                    std::print( "{} 启用服务 {}.\n", details::make_progress( ++finished_count, total_count, digits_of_total ), serv );
                    enable_serv( serv );
                    std::this_thread::sleep_for( default_executing_sleep_time );
                }
            }
            for ( const auto& serv : servs ) {
                std::print( "{} 启动服务 {}.\n", details::make_progress( ++finished_count, total_count, digits_of_total ), serv );
                start_serv( serv );
                std::this_thread::sleep_for( default_executing_sleep_time );
            }
            std::print( "\n{}\n\n", diving_line.data() );
        }
        inline auto fast_restore( const rule_node& rules )
        {
            const auto& execs{ rules.execs };
            const auto& servs{ rules.servs };
            if ( details::is_hijack_execs ) {
                for_each_wrapper( execs, undo_hijack_exec );
            }
            if ( details::is_set_serv_startup_types ) {
                for_each_wrapper( servs, enable_serv );
            }
            for_each_wrapper( servs, start_serv );
        }
    }
    inline auto execute_rules( const rule_node& rules )
    {
        switch ( details::executor_mode ) {
            case details::rule_executing::crack : std::print( "                    [ 破  解 ]\n\n\n" ); break;
            case details::rule_executing::restore : std::print( "                    [ 恢  复 ]\n\n\n" ); break;
            default : std::unreachable();
        }
        if ( rules.empty() ) {
            std::print( " (i) 规则为空." );
            details::wait_to_return();
            return func_back;
        }
        if ( details::executor_mode == details::rule_executing::restore && !details::is_hijack_execs && rules.servs.empty() ) {
            std::print( " (!) 当前配置下无可用恢复操作." );
            details::wait_to_return();
            return func_back;
        }
        std::print( " -> 正在执行...\n\n" );
        std::array< void ( * )( const rule_node& ), 2 > f;
        switch ( details::executor_mode ) {
            case details::rule_executing::crack : f = { details::default_crack, details::fast_crack }; break;
            case details::rule_executing::restore : f = { details::default_restore, details::fast_restore }; break;
            default : std::unreachable();
        }
        f[ details::is_enable_fast_mode ]( rules );
        std::print( " (i) 操作已完成." );
        details::wait_to_return();
        return func_back;
    }
    inline auto make_executor_mode_ui_text()
    {
        switch ( details::executor_mode ) {
            case details::rule_executing::crack : return "[ 破解 (点击切换) ]"sv;
            case details::rule_executing::restore : return "[ 恢复 (点击切换) ]"sv;
            default : std::unreachable();
        }
    }
    inline auto change_executor_mode( ui_func_args args )
    {
        switch ( details::executor_mode ) {
            case details::rule_executing::crack : details::executor_mode = details::rule_executing::restore; break;
            case details::rule_executing::restore : details::executor_mode = details::rule_executing::crack; break;
            default : std::unreachable();
        }
        args.parent_ui.edit_text( args.node_index, make_executor_mode_ui_text() );
        return func_back;
    }
    inline auto execute_all_rules()
    {
        std::print( " -> 正在准备数据...\n" );
        rule_node total;
        total.execs.append_range( custom_rules.execs );
        total.servs.append_range( custom_rules.servs );
        for ( const auto& builtin_rule : builtin_rules ) {
            total.execs.append_range( builtin_rule.execs );
            total.servs.append_range( builtin_rule.servs );
        }
        cpp_utils::clear_console( std_output_handle );
        return execute_rules( total );
    }
}
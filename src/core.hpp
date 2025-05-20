#pragma once
#include <fstream>
#include "cpp_utils/multithread.hpp"
#include "cpp_utils/pointer.hpp"
#include "cpp_utils/windows_app_tools.hpp"
#include "cpp_utils/windows_console_ui.hpp"
#include "info.hpp"
namespace core {
    using namespace std::chrono_literals;
    using namespace std::string_view_literals;
    using size_type = cpp_utils::size_type;
    inline constexpr SHORT console_width{ 50 };
    inline constexpr SHORT console_height{ 25 };
    inline constexpr UINT charset_id{ 65001 };
    inline constexpr auto default_thread_sleep_time{ 1s };
    inline constexpr auto config_file_name{ "config.ini" };
    inline constexpr auto func_back{ cpp_utils::console_ui::func_back };
    inline constexpr auto func_exit{ cpp_utils::console_ui::func_exit };
    inline const auto nproc{ std::thread::hardware_concurrency() };
    inline const auto window_handle{ GetConsoleWindow() };
    inline const auto std_input_handle{ GetStdHandle( STD_INPUT_HANDLE ) };
    inline const auto std_output_handle{ GetStdHandle( STD_OUTPUT_HANDLE ) };
    using ui_func_args = cpp_utils::console_ui::func_args;
    inline auto exit( ui_func_args )
    {
        return func_exit;
    }
    inline auto relaunch( ui_func_args )
    {
        cpp_utils::relaunch_as_admin( EXIT_SUCCESS, nullptr );
        return func_exit;
    }
    inline auto wait()
    {
        std::print( "\n\n" );
        for ( auto i{ 3 }; i > 0; --i ) {
            std::print( " {}s 后返回.\r", i );
            std::this_thread::sleep_for( 1s );
        }
    }
    struct option_container final {
        struct node final {
            struct item final {
              private:
                std::atomic_flag value_{ ATOMIC_FLAG_INIT };
              public:
                const char *const self_name;
                const char *const shown_name;
                auto enable()
                {
                    value_.test_and_set( std::memory_order_release );
                }
                auto disable()
                {
                    value_.clear( std::memory_order_release );
                }
                auto get() const
                {
                    return value_.test( std::memory_order_acquire );
                }
                operator bool() const
                {
                    return get();
                }
                auto operator=( const item & ) -> item & = delete;
                auto operator=( item && ) -> item &      = delete;
                item( const char *const _self_name, const char *const _shown_name )
                  : self_name{ _self_name }
                  , shown_name{ _shown_name }
                { }
                item( const item &_src )
                  : value_{ _src }
                  , self_name{ _src.self_name }
                  , shown_name{ _src.shown_name }
                { }
                item( item &&_src )
                  : value_{ _src }
                  , self_name{ _src.self_name }
                  , shown_name{ _src.shown_name }
                { }
                ~item() = default;
            };
            const char *const self_name;
            const char *const shown_name;
            std::vector< item > items;
            auto &operator[]( const std::string_view _self_name )
            {
                for ( auto &item : items ) {
                    if ( _self_name == item.self_name ) {
                        return item;
                    }
                }
                std::unreachable();
            }
            const auto &operator[]( const std::string_view _self_name ) const
            {
                for ( const auto &item : items ) {
                    if ( _self_name == item.self_name ) {
                        return item;
                    }
                }
                std::unreachable();
            }
            auto operator=( const node & ) -> node & = delete;
            auto operator=( node && ) -> node &      = delete;
            node( const char *const _self_name, const char *const _shown_name, std::vector< item > _items )
              : self_name{ _self_name }
              , shown_name{ _shown_name }
              , items{ std::move( _items ) }
            { }
            node( const node & ) = default;
            node( node && )      = default;
            ~node()              = default;
        };
        std::vector< node > nodes;
        auto &operator[]( const std::string_view _self_name )
        {
            for ( auto &node : nodes ) {
                if ( _self_name == node.self_name ) {
                    return node;
                }
            }
            std::unreachable();
        }
        const auto &operator[]( const std::string_view _self_name ) const
        {
            for ( const auto &node : nodes ) {
                if ( _self_name == node.self_name ) {
                    return node;
                }
            }
            std::unreachable();
        }
        option_container( std::vector< node > _nodes )
          : nodes{ std::move( _nodes ) }
        { }
    };
    inline option_container options{
      { { { "crack_restore",
            "破解 & 恢复",
            { { "hijack_execs", "劫持可执行文件" },
              { "set_serv_startup_types", "设置服务启动类型" },
              { "parallel_op", "并行操作 (预览版)" },
              { "fix_os_env", "* 修复操作系统环境" } } },
          { "window",
            "窗口显示",
            { { "keep_window_top", "* 置顶窗口" }, { "minimalist_titlebar", "* 极简标题栏" }, { "translucent", "* 半透明" } } },
          { "perf", "性能", { { "disable_x_option_hot_reload", "** 禁用标 * 选项热重载" } } } } } };
    inline const auto &is_disable_x_option_hot_reload{ options[ "perf" ][ "disable_x_option_hot_reload" ] };
    struct rule_node final {
        const char *const shown_name;
        std::deque< std::string > execs;
        std::deque< std::string > servs;
        auto empty() const noexcept
        {
            return execs.empty() && servs.empty();
        }
        auto operator=( const rule_node & ) -> rule_node & = delete;
        auto operator=( rule_node && ) -> rule_node &      = delete;
        rule_node( const char *const _shown_name, decltype( execs ) _execs, decltype( execs ) _servs )
          : shown_name{ _shown_name }
          , execs{ std::move( _execs ) }
          , servs{ std::move( _servs ) }
        { }
        rule_node( const rule_node & ) = default;
        rule_node( rule_node && )      = default;
        ~rule_node()                   = default;
    };
    inline rule_node custom_rules{ "自定义", {}, {} };
    inline const rule_node builtin_rules[]{
      {"极域电子教室",
       { "StudentMain.exe", "DispcapHelper.exe", "VRCwPlayer.exe", "InstHelpApp.exe", "InstHelpApp64.exe", "TDOvrSet.exe",
          "GATESRV.exe", "ProcHelper64.exe", "MasterHelper.exe" },
       { "TDNetFilter", "TDFileFilter", "STUDSRV" }},
      {"联想智能云教室",
       { "vncviewer.exe", "tvnserver32.exe", "WfbsPnpInstall.exe", "WFBSMon.exe", "WFBSMlogon.exe", "WFBSSvrLogShow.exe",
          "ResetIp.exe", "FuncForWIN64.exe", "CertMgr.exe", "Fireware.exe", "BCDBootCopy.exe", "refreship.exe",
          "lenovoLockScreen.exe", "PortControl64.exe", "DesktopCheck.exe", "DeploymentManager.exe", "DeploymentAgent.exe",
          "XYNTService.exe" },
       { "BSAgentSvr", "tvnserver", "WFBSMlogon" } },
      {"红蜘蛛多媒体网络教室",
       { "rscheck.exe", "checkrs.exe", "REDAgent.exe", "PerformanceCheck.exe", "edpaper.exe", "Adapter.exe", "repview.exe",
          "FormatPaper.exe" },
       { "appcheck2", "checkapp2" }                }
    };
    class basic_config_node {
      public:
        const cpp_utils::raw_pointer_wrapper< const char * > self_name;
        virtual auto load( const bool, std::string & ) -> void = 0;
        virtual auto sync( std::ofstream & ) -> void           = 0;
        virtual auto prepare_reload() -> void { }
        virtual auto ui( cpp_utils::console_ui & ) -> void { }
        basic_config_node( const char *const _self_name ) noexcept
          : self_name{ _self_name }
        { }
        virtual ~basic_config_node() noexcept = default;
    };
    class option_op final : public basic_config_node {
      private:
        static constexpr auto format_string_{ R"("{}.{}": {})" };
        static auto make_swith_button_text_( const auto _is_enabled )
        {
            return std::format( " > {}用 ", _is_enabled ? "禁" : "启" );
        }
      public:
        virtual auto load( const bool _is_reload, std::string &_line ) -> void override final
        {
            if ( _is_reload ) {
                return;
            }
            for ( auto &node : options.nodes ) {
                for ( auto &item : node.items ) {
                    if ( _line == std::format( format_string_, node.self_name, item.self_name, true ) ) {
                        item.enable();
                    } else if ( _line == std::format( format_string_, node.self_name, item.self_name, false ) ) {
                        item.disable();
                    }
                }
            }
        }
        virtual auto sync( std::ofstream &_out ) -> void override final
        {
            for ( const auto &node : options.nodes ) {
                for ( const auto &item : node.items ) {
                    _out << std::format( format_string_, node.self_name, item.self_name, item.get() ) << '\n';
                }
            }
        }
        virtual auto ui( cpp_utils::console_ui &_ui ) -> void override final
        {
            constexpr auto option_ctrl_color{ cpp_utils::console_text::foreground_red | cpp_utils::console_text::foreground_green };
            using item_type = option_container::node::item;
            class option_setter final {
              private:
                item_type &item_;
              public:
                auto operator()( ui_func_args _args )
                {
                    constexpr void ( item_type::*fn[] )(){ &item_type::enable, &item_type::disable };
                    ( item_.*fn[ static_cast< size_type >( item_.get() ) ] )();
                    _args.parent_ui.edit_text( _args.node_index, make_swith_button_text_( item_ ) );
                    return func_back;
                }
                option_setter( item_type &_item ) noexcept
                  : item_{ _item }
                { }
                ~option_setter() noexcept = default;
            };
            class option_shower final {
              private:
                option_container::node &node_;
              public:
                auto operator()( ui_func_args )
                {
                    cpp_utils::console_ui ui;
                    ui.add_back( "                    [ 配  置 ]\n\n" )
                      .add_back(
                        std::format( R"( < 折叠 "{}" )", node_.shown_name ), exit,
                        cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity );
                    for ( auto &item : node_.items ) {
                        ui.add_back( std::format( "\n[ {} ]\n", item.shown_name ) )
                          .add_back( make_swith_button_text_( item ), option_setter{ item }, option_ctrl_color );
                    }
                    ui.show();
                    return func_back;
                }
                option_shower( option_container::node &_node ) noexcept
                  : node_{ _node }
                { }
                ~option_shower() noexcept = default;
            };
            _ui.add_back( std::format(
              "\n[ 选项 ]\n\n"
              " (i) 选项相关信息可参阅文档.\n"
              "     标 * 选项热重载每 {} 自动执行, 可禁用.\n"
              "     标 ** 选项无法热重载. 其余选项可实时热重载.\n",
              default_thread_sleep_time ) );
            for ( auto &node : options.nodes ) {
                _ui.add_back( std::format( " > {} ", node.shown_name ), option_shower{ node }, option_ctrl_color );
            }
        }
        option_op() noexcept
          : basic_config_node{ "options" }
        { }
        virtual ~option_op() noexcept = default;
    };
    class custom_rules_execs_op final : public basic_config_node {
      public:
        virtual auto load( const bool, std::string &_line ) -> void override final
        {
            custom_rules.execs.emplace_back( std::move( _line ) );
        }
        virtual auto sync( std::ofstream &_out ) -> void override final
        {
            for ( const auto &exec : custom_rules.execs ) {
                _out << exec << '\n';
            }
        }
        virtual auto prepare_reload() -> void override final
        {
            custom_rules.execs.clear();
        }
        custom_rules_execs_op() noexcept
          : basic_config_node{ "custom_rules_execs" }
        { }
        virtual ~custom_rules_execs_op() noexcept = default;
    };
    class custom_rules_servs_op final : public basic_config_node {
      public:
        virtual auto load( const bool, std::string &_line ) -> void override final
        {
            custom_rules.servs.emplace_back( std::move( _line ) );
        }
        virtual auto sync( std::ofstream &_out ) -> void override final
        {
            for ( const auto &serv : custom_rules.servs ) {
                _out << serv << '\n';
            }
        }
        virtual auto prepare_reload() -> void override final
        {
            custom_rules.servs.clear();
        }
        custom_rules_servs_op() noexcept
          : basic_config_node{ "custom_rules_servs" }
        { }
        virtual ~custom_rules_servs_op() noexcept = default;
    };
    using basic_config_node_smart_ptr = std::unique_ptr< basic_config_node >;
    inline basic_config_node_smart_ptr config_nodes[]{
      std::make_unique< option_op >(), std::make_unique< custom_rules_execs_op >(), std::make_unique< custom_rules_servs_op >() };
    inline auto info( ui_func_args )
    {
        auto visit_repo_webpage{ []( ui_func_args ) static
        {
            ShellExecuteA( nullptr, "open", INFO_REPO_URL, nullptr, nullptr, SW_SHOWNORMAL );
            return func_back;
        } };
        cpp_utils::console_ui ui;
        ui.add_back( "                    [ 关  于 ]\n\n" )
          .add_back( " < 返回 ", exit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
          .add_back(
            "\n[ 名称 ]\n\n " INFO_FULL_NAME " (缩写: " INFO_SHORT_NAME ")\n\n[ 版本 ]\n\n " INFO_VERSION
            "\n (时区: " INFO_TIME_ZONE ")\n 提交时间: " INFO_GIT_DATE "\n 构建时间: " INFO_BUILD_TIME
            "\n\n[ 许可证 & 版权 ]\n\n " INFO_LICENSE "\n " INFO_COPYRIGHT "\n\n[ 仓库 ]\n" )
          .add_back(
            " " INFO_REPO_URL " ", visit_repo_webpage,
            cpp_utils::console_text::default_attrs | cpp_utils::console_text::foreground_intensity
              | cpp_utils::console_text::lvb_underscore )
          .show();
        return func_back;
    }
    inline auto toolkit( ui_func_args )
    {
        auto launch_cmd{ []( ui_func_args _args ) static
        {
            cpp_utils::set_console_title( INFO_SHORT_NAME " - 命令提示符" );
            cpp_utils::set_console_size( window_handle, std_output_handle, 120, 30 );
            cpp_utils::fix_window_size( window_handle, false );
            cpp_utils::enable_window_maximize_ctrl( window_handle, true );
            _args.parent_ui.set_limits( false, false );
            SetConsoleScreenBufferSize( std_output_handle, COORD{ 127, SHRT_MAX - 1 } );
            std::system( "cmd.exe" );
            cpp_utils::set_console_charset( charset_id );
            cpp_utils::set_console_title( INFO_SHORT_NAME );
            cpp_utils::set_console_size( window_handle, std_output_handle, console_width, console_height );
            cpp_utils::fix_window_size( window_handle, true );
            cpp_utils::enable_window_maximize_ctrl( window_handle, false );
            return func_back;
        } };
        class cmd_executor final {
          private:
            const char *const ( &item_ )[ 2 ];
          public:
            auto operator()( ui_func_args )
            {
                auto execute{ [ this ]( ui_func_args )
                {
                    std::print(
                      "                   [ 工 具 箱 ]\n\n\n"
                      " -> 正在执行操作系统命令...\n{}\n",
                      std::string( console_width, '-' ) );
                    std::system( item_[ 1 ] );
                    return func_exit;
                } };
                cpp_utils::console_ui ui;
                ui.add_back( "                   [ 工 具 箱 ]\n\n" )
                  .add_back( std::format( " (i) 是否执行 \"{}\"?\n", item_[ 0 ] ) )
                  .add_back( " > 是, 继续执行 ", execute, cpp_utils::console_text::foreground_red | cpp_utils::console_text::foreground_intensity )
                  .add_back( " > 否, 立即返回 ", exit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
                  .show();
                return func_back;
            }
            cmd_executor( const char *const ( &_item )[ 2 ] ) noexcept
              : item_{ _item }
            { }
            cmd_executor( const cmd_executor & ) noexcept = default;
            cmd_executor( cmd_executor && ) noexcept      = default;
            ~cmd_executor() noexcept                      = default;
        };
        constexpr const char *common_cmds[][ 2 ]{
          {"重启资源管理器",               R"(taskkill.exe /f /im explorer.exe && timeout.exe /t 3 /nobreak && start C:\Windows\explorer.exe)"},
          {"重启至高级选项",               "shutdown.exe /r /o /f /t 15"                                                                      },
          {"恢复 USB 设备访问",            R"(reg.exe add "HKLM\SYSTEM\CurrentControlSet\Services\USBSTOR" /f /t reg_dword /v Start /d 3)"    },
          {"恢复 Google Chrome 离线游戏",  R"(reg.exe delete "HKLM\SOFTWARE\Policies\Google\Chrome" /f /v AllowDinosaurEasterEgg)"            },
          {"恢复 Microsoft Edge 离线游戏", R"(reg.exe delete "HKLM\SOFTWARE\Policies\Microsoft\Edge" /f /v AllowSurfGame)"                    }
        };
        cpp_utils::console_ui ui;
        ui.add_back( "                   [ 工 具 箱 ]\n\n" )
          .add_back( " < 返回 ", exit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
          .add_back( " > 命令提示符 ", launch_cmd )
          .add_back( "\n[ 常用操作 ]\n" );
        for ( const auto &common_cmd : common_cmds ) {
            ui.add_back( std::format( " > {} ", common_cmd[ 0 ] ), cmd_executor{ common_cmd } );
        }
        ui.show();
        return func_back;
    }
    inline auto set_console_attrs( const std::stop_token _msg )
    {
        const auto &window_options{ options[ "window" ] };
        const auto &is_enable_minimalist_titlebar{ window_options[ "minimalist_titlebar" ] };
        const auto &is_translucent{ window_options[ "translucent" ] };
        if ( is_disable_x_option_hot_reload ) {
            cpp_utils::enable_window_menu( window_handle, !is_enable_minimalist_titlebar );
            cpp_utils::set_window_translucency( window_handle, is_translucent ? 230 : 255 );
            return;
        }
        while ( !_msg.stop_requested() ) {
            cpp_utils::enable_window_menu( window_handle, !is_enable_minimalist_titlebar );
            cpp_utils::set_window_translucency( window_handle, is_translucent ? 230 : 255 );
            std::this_thread::sleep_for( default_thread_sleep_time );
        }
    }
    inline auto keep_window_top( const std::stop_token _msg )
    {
        const auto &is_keep_window_top{ options[ "window" ][ "keep_window_top" ] };
        if ( is_disable_x_option_hot_reload && !is_keep_window_top ) {
            return;
        }
        constexpr auto sleep_time{ 100ms };
        const auto current_thread_id{ GetCurrentThreadId() };
        const auto current_window_thread_process_id{ GetWindowThreadProcessId( window_handle, nullptr ) };
        if ( is_disable_x_option_hot_reload ) {
            cpp_utils::loop_keep_window_top(
              window_handle, current_thread_id, current_window_thread_process_id, sleep_time, []( const std::stop_token _msg ) static
            { return !_msg.stop_requested(); }, _msg );
            cpp_utils::cancel_top_window( window_handle );
            return;
        }
        while ( !_msg.stop_requested() ) {
            if ( !is_keep_window_top ) [[unlikely]] {
                cpp_utils::cancel_top_window( window_handle );
                std::this_thread::sleep_for( default_thread_sleep_time );
                continue;
            }
            cpp_utils::keep_window_top( window_handle, current_thread_id, current_window_thread_process_id );
            std::this_thread::sleep_for( sleep_time );
        }
        cpp_utils::cancel_top_window( window_handle );
    }
    inline auto fix_os_env( const std::stop_token _msg )
    {
        const auto &is_fix_os_env{ options[ "crack_restore" ][ "fix_os_env" ] };
        if ( is_disable_x_option_hot_reload && !is_fix_os_env ) {
            return;
        }
        constexpr const char *reg_dirs[]{
          R"(Software\Policies\Microsoft\Windows\System)", R"(Software\Microsoft\Windows\CurrentVersion\Policies\System)",
          R"(Software\Microsoft\Windows\CurrentVersion\Policies\Explorer)" };
        constexpr const char *execs[]{
          "taskkill.exe", "sc.exe",      "net.exe", "reg.exe",  "cmd.exe", "taskmgr.exe",
          "perfmon.exe",  "regedit.exe", "mmc.exe", "dism.exe", "sfc.exe" };
        constexpr auto number_of_execs{ sizeof( execs ) / sizeof( const char * ) };
        std::string reg_hijacked_execs[ number_of_execs ];
        for ( const auto i : std::ranges::iota_view{ decltype( number_of_execs ){ 0 }, number_of_execs } ) {
            reg_hijacked_execs[ i ]
              = std::format( R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\{})", execs[ i ] );
        }
        auto engine{ [ & ]()
        {
            for ( const auto &reg_dir : reg_dirs ) {
                RegDeleteTreeA( HKEY_CURRENT_USER, reg_dir );
            }
            for ( const auto &reg_hijacked_exec : reg_hijacked_execs ) {
                RegDeleteTreeA( HKEY_LOCAL_MACHINE, reg_hijacked_exec.c_str() );
            }
            std::this_thread::sleep_for( 1s );
        } };
        if ( is_disable_x_option_hot_reload ) {
            while ( !_msg.stop_requested() ) {
                engine();
            }
            return;
        }
        while ( !_msg.stop_requested() ) {
            if ( !is_fix_os_env ) [[unlikely]] {
                std::this_thread::sleep_for( default_thread_sleep_time );
                continue;
            }
            engine();
        }
    }
    inline auto load_config( const bool _is_reload = false )
    {
        std::ifstream config_file{ config_file_name, std::ios::in };
        if ( !config_file.good() ) {
            return;
        }
        if ( _is_reload ) {
            for ( auto &config_node : config_nodes ) {
                config_node->prepare_reload();
            }
        }
        std::string line;
        std::string_view line_view;
        basic_config_node *node_ptr{};
        while ( std::getline( config_file, line ) ) {
            line_view = line;
            if ( line_view.empty() ) {
                continue;
            }
            if ( line_view.front() == '#' ) {
                continue;
            }
            if ( line_view.size() >= sizeof( "[  ]" ) && line_view.substr( 0, sizeof( "[ " ) - 1 ) == "[ "
                 && line_view.substr( line_view.size() - sizeof( " ]" ) + 1, line_view.size() ) == " ]" )
            {
                line_view = line_view.substr( sizeof( "[ " ) - 1, line_view.size() - sizeof( " ]" ) - 1 );
                for ( auto &config_node : config_nodes ) {
                    if ( line_view == config_node->self_name.get() ) {
                        node_ptr = config_node.get();
                        break;
                    }
                    node_ptr = nullptr;
                }
                continue;
            }
            if ( node_ptr != nullptr ) {
                node_ptr->load( _is_reload, line );
            }
        }
    }
    inline auto config_ui( ui_func_args )
    {
        auto sync{ []( ui_func_args ) static
        {
            std::print(
              "                    [ 配  置 ]\n\n\n"
              " -> 正在同步配置...\n" );
            load_config( true );
            std::ofstream config_file_stream{ config_file_name, std::ios::out | std::ios::trunc };
            config_file_stream << "# " INFO_FULL_NAME "\n# " INFO_VERSION "\n";
            for ( auto &config_node : config_nodes ) {
                config_file_stream << std::format( "[ {} ]\n", config_node->self_name.get() );
                config_node->sync( config_file_stream );
            }
            config_file_stream << std::flush;
            const auto is_good{ config_file_stream.good() };
            std::print( "\n ({}) 同步配置{}.", is_good ? 'i' : '!', is_good ? "成功" : "失败" );
            wait();
            return func_back;
        } };
        auto open_file{ []( ui_func_args ) static
        {
            if ( std::ifstream{ config_file_name, std::ios::in }.good() ) {
                std::print(
                  "                    [ 配  置 ]\n\n\n"
                  " -> 正在打开配置文件..." );
                ShellExecuteA( nullptr, "open", config_file_name, nullptr, nullptr, SW_SHOWNORMAL );
                return func_back;
            }
            std::print(
              "                    [ 配  置 ]\n\n\n"
              " (!) 无法打开配置文件." );
            wait();
            return func_back;
        } };
        cpp_utils::console_ui ui;
        ui.add_back( "                    [ 配  置 ]\n\n" )
          .add_back( " < 返回 ", exit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
          .add_back( " > 同步配置 ", sync )
          .add_back( " > 打开配置文件 ", open_file );
        for ( auto &config_node : config_nodes ) {
            config_node->ui( ui );
        }
        ui.show();
        return func_back;
    }
    using exec_const_ref_type
      = cpp_utils::add_const_lvalue_reference_type< std::decay_t< decltype( std::declval< rule_node >().execs[ 0 ] ) > >;
    using serv_const_ref_type
      = cpp_utils::add_const_lvalue_reference_type< std::decay_t< decltype( std::declval< rule_node >().servs[ 0 ] ) > >;
    inline const auto &option_crack_restore{ options[ "crack_restore" ] };
    inline const auto &is_hijack_execs{ option_crack_restore[ "hijack_execs" ] };
    inline const auto &is_set_serv_startup_types{ option_crack_restore[ "set_serv_startup_types" ] };
    inline const auto &is_parallel_op{ option_crack_restore[ "parallel_op" ] };
    class crack final {
      private:
        const rule_node &rules_;
        static auto hijack_exec_( exec_const_ref_type _exec )
        {
            std::system(
              std::format(
                R"(reg.exe add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution options\{}" /f /t reg_sz /v debugger /d "nul")",
                _exec )
                .c_str() );
        }
        static auto disable_serv_( serv_const_ref_type _serv )
        {
            std::system( std::format( R"(sc.exe config "{}" start= disabled)", _serv ).c_str() );
        }
        static auto kill_exec_( exec_const_ref_type _exec )
        {
            std::system( std::format( R"(taskkill.exe /f /im "{}")", _exec ).c_str() );
        }
        static auto stop_serv_( serv_const_ref_type _serv )
        {
            std::system( std::format( R"(net.exe stop "{}" /y)", _serv ).c_str() );
        }
        static auto singlethread_engine_( const rule_node &_rules )
        {
            const auto &execs{ _rules.execs };
            const auto &servs{ _rules.servs };
            if ( is_hijack_execs ) {
                for ( const auto &exec : execs ) {
                    hijack_exec_( exec );
                }
            }
            if ( is_set_serv_startup_types ) {
                for ( const auto &serv : servs ) {
                    disable_serv_( serv );
                }
            }
            for ( const auto &exec : execs ) {
                kill_exec_( exec );
            }
            for ( const auto &serv : servs ) {
                stop_serv_( serv );
            }
        }
        static auto multithread_engine_( const rule_node &_rules )
        {
            const auto &execs{ _rules.execs };
            const auto &servs{ _rules.servs };
            const auto nproc_for_exec_op{ std::ranges::max( nproc / 4, 2U ) };
            cpp_utils::thread_pool threads;
            if ( is_hijack_execs ) {
                threads.add( [ & ]
                { cpp_utils::parallel_for_each_impl( nproc_for_exec_op, execs.begin(), execs.end(), hijack_exec_ ); } );
            }
            if ( is_set_serv_startup_types ) {
                threads.add( [ & ] { cpp_utils::parallel_for_each_impl( nproc, servs.begin(), servs.end(), disable_serv_ ); } );
            }
            threads
              .add( [ & ] { cpp_utils::parallel_for_each_impl( nproc_for_exec_op, execs.begin(), execs.end(), kill_exec_ ); } )
              .add( [ & ] { cpp_utils::parallel_for_each_impl( nproc, servs.begin(), servs.end(), stop_serv_ ); } );
        }
      public:
        auto operator()( ui_func_args )
        {
            std::print( "                    [ 破  解 ]\n\n\n" );
            if ( rules_.empty() ) {
                std::print( " (i) 规则为空." );
                wait();
                return func_back;
            }
            std::print( " -> 正在生成并执行操作系统命令...\n{}\n", std::string( console_width, '-' ) );
            void ( *fn[] )( const rule_node & ){ &crack::singlethread_engine_, &crack::multithread_engine_ };
            fn[ is_parallel_op.get() ]( rules_ );
            return func_back;
        }
        crack( const rule_node &_rules ) noexcept
          : rules_{ _rules }
        { }
        crack( const crack & )     = default;
        crack( crack && ) noexcept = default;
        ~crack() noexcept          = default;
    };
    class restore final {
      private:
        const rule_node &rules_;
        static auto undo_hijack_exec_( exec_const_ref_type _exec )
        {
            std::system(
              std::format(
                R"(reg.exe delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution options\{}" /f)", _exec )
                .c_str() );
        }
        static auto enable_serv_( serv_const_ref_type _serv )
        {
            std::system( std::format( R"(sc.exe config "{}" start= auto)", _serv ).c_str() );
        }
        static auto start_serv_( serv_const_ref_type _serv )
        {
            std::system( std::format( R"(net.exe start "{}")", _serv ).c_str() );
        }
        static auto singlethread_engine_( const rule_node &_rules )
        {
            const auto &execs{ _rules.execs };
            const auto &servs{ _rules.servs };
            if ( is_hijack_execs ) {
                for ( const auto &exec : execs ) {
                    undo_hijack_exec_( exec );
                }
            }
            if ( is_set_serv_startup_types ) {
                for ( const auto &serv : servs ) {
                    enable_serv_( serv );
                }
            }
            for ( const auto &serv : servs ) {
                start_serv_( serv );
            }
        }
        static auto multithread_engine_( const rule_node &_rules )
        {
            const auto &execs{ _rules.execs };
            const auto &servs{ _rules.servs };
            const auto nproc_for_exec_op{ std::ranges::max( nproc / 4, 2U ) };
            if ( is_hijack_execs ) {
                cpp_utils::parallel_for_each_impl( nproc_for_exec_op, execs.begin(), execs.end(), undo_hijack_exec_ );
            }
            if ( is_set_serv_startup_types ) {
                cpp_utils::parallel_for_each_impl( nproc_for_exec_op, servs.begin(), servs.end(), enable_serv_ );
            }
            cpp_utils::parallel_for_each_impl( nproc, servs.begin(), servs.end(), start_serv_ );
        }
      public:
        auto operator()( ui_func_args )
        {
            std::print( "                    [ 恢  复 ]\n\n\n" );
            if ( rules_.empty() ) {
                std::print( " (i) 规则为空." );
                wait();
                return func_back;
            }
            std::print( " -> 正在生成并执行操作系统命令...\n{}\n", std::string( console_width, '-' ) );
            constexpr void ( *fn[] )( const rule_node & ){ &restore::singlethread_engine_, &restore::multithread_engine_ };
            fn[ is_parallel_op.get() ]( rules_ );
            return func_back;
        }
        restore( const rule_node &_rules ) noexcept
          : rules_{ _rules }
        { }
        restore( const restore & )     = default;
        restore( restore && ) noexcept = default;
        ~restore() noexcept            = default;
    };
}
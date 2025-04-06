#pragma once
#include <fstream>
#include "cpp_utils.hpp"
#include "info.hpp"
namespace core {
    using namespace std::chrono_literals;
    using namespace std::string_view_literals;
    using size_type            = cpp_utils::size_type;
    using ansi_char            = cpp_utils::ansi_char;
    using ansi_std_string      = cpp_utils::ansi_std_string;
    using ansi_std_string_view = cpp_utils::ansi_std_string_view;
    inline constexpr SHORT console_width{ 50 };
    inline constexpr SHORT console_height{ 25 };
    inline constexpr UINT charset_id{ 54936 };
    inline const auto window_handle{ GetConsoleWindow() };
    inline const auto std_input_handle{ GetStdHandle( STD_INPUT_HANDLE ) };
    inline const auto std_output_handle{ GetStdHandle( STD_OUTPUT_HANDLE ) };
    inline constexpr auto config_file_name{ "config.ini" };
    inline constexpr auto default_thread_sleep_time{ 1s };
    inline auto exit( cpp_utils::console_ui::func_args )
    {
        return cpp_utils::console_ui::exit;
    }
    inline auto relaunch( cpp_utils::console_ui::func_args )
    {
        cpp_utils::relaunch_as_admin();
        return cpp_utils::console_ui::exit;
    }
    inline auto wait()
    {
        std::print( "\n\n" );
        for ( auto i{ 3s }; i > 0s; --i ) {
            std::print( " {} 后返回.\r", i );
            std::this_thread::sleep_for( 1s );
        }
    }
    struct option_container final {
        struct node final {
            struct item final {
              private:
                std::atomic_flag value_{ ATOMIC_FLAG_INIT };
              public:
                const ansi_char *const self_name;
                const ansi_char *const shown_name;
                auto enable()
                {
                    value_.test_and_set( std::memory_order_acq_rel );
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
                item( const ansi_char *const _self_name, const ansi_char *const _shown_name )
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
            const ansi_char *const self_name;
            const ansi_char *const shown_name;
            std::vector< item > items;
            auto &operator[]( const ansi_std_string_view _self_name )
            {
                for ( auto &item : items ) {
                    if ( _self_name == item.self_name ) {
                        return item;
                    }
                }
                std::abort();
            }
            const auto &operator[]( const ansi_std_string_view _self_name ) const
            {
                for ( const auto &item : items ) {
                    if ( _self_name == item.self_name ) {
                        return item;
                    }
                }
                std::abort();
            }
            auto operator=( const node & ) -> node & = delete;
            auto operator=( node && ) -> node &      = delete;
            node( const ansi_char *const _self_name, const ansi_char *const _shown_name, std::vector< item > _items )
              : self_name{ _self_name }
              , shown_name{ _shown_name }
              , items{ std::move( _items ) }
            { }
            node( const node & ) = default;
            node( node && )      = default;
            ~node()              = default;
        };
        std::vector< node > nodes;
        auto &operator[]( const ansi_std_string_view _self_name )
        {
            for ( auto &node : nodes ) {
                if ( _self_name == node.self_name ) {
                    return node;
                }
            }
            std::abort();
        }
        const auto &operator[]( const ansi_std_string_view _self_name ) const
        {
            for ( const auto &node : nodes ) {
                if ( _self_name == node.self_name ) {
                    return node;
                }
            }
            std::abort();
        }
        auto operator=( const option_container & ) -> option_container & = delete;
        auto operator=( option_container && ) -> option_container &      = delete;
        option_container( std::vector< node > _nodes )
          : nodes{ std::move( _nodes ) }
        { }
        option_container( const option_container & ) = default;
        option_container( option_container && )      = default;
        ~option_container()                          = default;
    };
    inline option_container options{
      { { { "crack_restore",
            "破解/恢复",
            { { "hijack_execs", "劫持可执行文件" },
              { "set_serv_startup_types", "设置服务启动类型" },
              { "fix_os_env", "* 修复操作系统环境" } } },
          { "window",
            "窗口显示",
            { { "keep_window_top", "* 置顶窗口" },
              { "disable_close_ctrl", "* 禁用关闭控件" },
              { "translucent", "* 半透明" },
              { "minimalist_titlebar", "* 极简标题栏" } } },
          { "perf",
            "性能",
            { { "quick_exit_and_relaunch", "** 快速退出与重启" },
              { "disable_x_option_hot_reload", "** 禁用标 * 选项热重载" } } } } } };
    inline const auto &is_disable_x_option_hot_reload{ options[ "perf" ][ "disable_x_option_hot_reload" ] };
    struct rule_node final {
        const ansi_char *const shown_name;
        std::deque< ansi_std_string > execs;
        std::deque< ansi_std_string > servs;
        auto empty() const
        {
            return execs.empty() && servs.empty();
        }
        auto operator=( const rule_node & ) -> rule_node & = delete;
        auto operator=( rule_node && ) -> rule_node &      = delete;
        rule_node( const ansi_char *const _shown_name, std::deque< ansi_std_string > _execs, std::deque< ansi_std_string > _servs )
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
        const ansi_char *const self_name;
        virtual auto load( const bool, ansi_std_string & ) -> void = 0;
        virtual auto prepare_reload() -> void { }
        virtual auto sync( std::ofstream & ) -> void = 0;
        virtual auto ui_panel( cpp_utils::console_ui & ) -> void { }
        virtual auto operator=( const basic_config_node & ) -> basic_config_node & = delete;
        virtual auto operator=( basic_config_node && ) -> basic_config_node &      = delete;
        basic_config_node( const ansi_char *const _self_name )
          : self_name{ _self_name }
        { }
        basic_config_node( const basic_config_node & ) = delete;
        basic_config_node( basic_config_node && )      = delete;
        virtual ~basic_config_node()                   = default;
    };
    class option_op final : public basic_config_node {
      public:
        virtual auto load( const bool _is_reload, ansi_std_string &_line ) -> void override final
        {
            if ( _is_reload ) {
                return;
            }
            for ( auto &node : options.nodes ) {
                for ( auto &item : node.items ) {
                    if ( _line == std::format( R"("{}.{}": "enabled")", node.self_name, item.self_name ) ) {
                        item.enable();
                    } else if ( _line == std::format( R"("{}.{}": "disabled")", node.self_name, item.self_name ) ) {
                        item.disable();
                    }
                }
            }
        }
        virtual auto sync( std::ofstream &_out ) -> void override final
        {
            for ( const auto &node : options.nodes ) {
                for ( const auto &item : node.items ) {
                    _out << std::format(
                      R"("{}.{}": "{}")"
                      "\n",
                      node.self_name, item.self_name, item.get() ? "enabled" : "disabled" );
                }
            }
        }
        virtual auto ui_panel( cpp_utils::console_ui &_ui ) -> void
        {
            constexpr auto option_ctrl_color{ cpp_utils::console_text::foreground_red | cpp_utils::console_text::foreground_green };
            class option_setter final {
              private:
                option_container::node::item &item_;
              public:
                auto operator()( cpp_utils::console_ui::func_args _args )
                {
                    switch ( item_.get() ) {
                        case true : {
                            item_.disable();
                            break;
                        }
                        case false : {
                            item_.enable();
                            break;
                        }
                    }
                    _args.parent_ui.edit_text( _args.node_index, std::format( " > {}用 ", item_.get() ? "禁" : "启" ) );
                    return cpp_utils::console_ui::back;
                }
                auto operator=( const option_setter & ) -> option_setter & = delete;
                auto operator=( option_setter && ) -> option_setter &      = delete;
                option_setter( option_container::node::item &_item )
                  : item_{ _item }
                { }
                option_setter( const option_setter & ) = default;
                option_setter( option_setter && )      = default;
                ~option_setter()                       = default;
            };
            class option_shower final {
              private:
                option_container::node &node_;
              public:
                auto operator()( cpp_utils::console_ui::func_args )
                {
                    std::print( " -> 初始化 UI.\n" );
                    cpp_utils::console_ui ui;
                    ui.add_back( "                    [ 配  置 ]\n\n" )
                      .add_back(
                        std::format( " < 折叠 {} ", node_.shown_name ), exit,
                        cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity );
                    for ( auto &item : node_.items ) {
                        ui.add_back( std::format( "\n[ {} ]\n", item.shown_name ) )
                          .add_back( std::format( " > {}用 ", item.get() ? "禁" : "启" ), option_setter{ item }, option_ctrl_color );
                    }
                    ui.show();
                    return cpp_utils::console_ui::back;
                }
                auto operator=( const option_shower & ) -> option_shower & = delete;
                auto operator=( option_shower && ) -> option_shower &      = delete;
                option_shower( option_container::node &_node )
                  : node_{ _node }
                { }
                option_shower( const option_shower & ) = default;
                option_shower( option_shower && )      = default;
                ~option_shower()                       = default;
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
        virtual auto operator=( const option_op & ) -> option_op & = delete;
        virtual auto operator=( option_op && ) -> option_op &      = delete;
        option_op()
          : basic_config_node{ "options" }
        { }
        option_op( const option_op & ) = delete;
        option_op( option_op && )      = delete;
        virtual ~option_op()           = default;
    };
    class custom_rules_execs_op final : public basic_config_node {
      public:
        virtual auto load( const bool, ansi_std_string &_line ) -> void override final
        {
            custom_rules.execs.emplace_back( std::move( _line ) );
        }
        virtual auto prepare_reload() -> void override final
        {
            custom_rules.execs.clear();
        }
        virtual auto sync( std::ofstream &_out ) -> void override final
        {
            for ( const auto &exec : custom_rules.execs ) {
                _out << exec << '\n';
            }
        }
        virtual auto operator=( const custom_rules_execs_op & ) -> custom_rules_execs_op & = delete;
        virtual auto operator=( custom_rules_execs_op && ) -> custom_rules_execs_op &      = delete;
        custom_rules_execs_op()
          : basic_config_node{ "custom_rules_execs" }
        { }
        custom_rules_execs_op( const custom_rules_execs_op & ) = delete;
        custom_rules_execs_op( custom_rules_execs_op && )      = delete;
        virtual ~custom_rules_execs_op()                       = default;
    };
    class custom_rules_servs_op final : public basic_config_node {
      public:
        virtual auto load( const bool, ansi_std_string &_line ) -> void override final
        {
            custom_rules.servs.emplace_back( std::move( _line ) );
        }
        virtual auto prepare_reload() -> void override final
        {
            custom_rules.servs.clear();
        }
        virtual auto sync( std::ofstream &_out ) -> void override final
        {
            for ( const auto &serv : custom_rules.servs ) {
                _out << serv << '\n';
            }
        }
        virtual auto operator=( const custom_rules_servs_op & ) -> custom_rules_servs_op & = delete;
        virtual auto operator=( custom_rules_servs_op && ) -> custom_rules_servs_op &      = delete;
        custom_rules_servs_op()
          : basic_config_node{ "custom_rules_servs" }
        { }
        custom_rules_servs_op( const custom_rules_servs_op & ) = delete;
        custom_rules_servs_op( custom_rules_servs_op && )      = delete;
        virtual ~custom_rules_servs_op()                       = default;
    };
    using basic_config_node_smart_ptr = std::unique_ptr< basic_config_node >;
    inline basic_config_node_smart_ptr config_nodes[]{
      basic_config_node_smart_ptr{ std::make_unique< option_op >() },
      basic_config_node_smart_ptr{ std::make_unique< custom_rules_execs_op >() },
      basic_config_node_smart_ptr{ std::make_unique< custom_rules_servs_op >() } };
    inline auto info( cpp_utils::console_ui::func_args )
    {
        std::print( " -> 初始化 UI.\n" );
        auto visit_repo_webpage{ []( cpp_utils::console_ui::func_args ) static
        {
            ShellExecuteA( nullptr, "open", INFO_REPO_URL, nullptr, nullptr, SW_SHOWNORMAL );
            return cpp_utils::console_ui::back;
        } };
        cpp_utils::console_ui ui;
        ui.add_back( "                    [ 关  于 ]\n\n" )
          .add_back( " < 返回 ", exit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
          .add_back(
            "\n[ 名称 ]\n\n " INFO_FULL_NAME " (" INFO_SHORT_NAME ")\n\n[ 版本 ]\n\n " INFO_VERSION "\n (时区: " INFO_TIME_ZONE
            ")\n 提交时间: " INFO_GIT_DATE "\n 构建时间: " INFO_BUILD_TIME "\n\n[ 许可证 & 版权 ]\n\n " INFO_LICENSE
            "\n " INFO_COPYRIGHT "\n\n[ 仓库 ]\n" )
          .add_back(
            " " INFO_REPO_URL " ", visit_repo_webpage,
            cpp_utils::console_text::default_set | cpp_utils::console_text::foreground_intensity | cpp_utils::console_text::lvb_underscore )
          .show();
        return cpp_utils::console_ui::back;
    }
    inline auto toolkit( cpp_utils::console_ui::func_args )
    {
        std::print( " -> 初始化 UI.\n" );
        auto launch_cmd{ []( cpp_utils::console_ui::func_args _args ) static
        {
            cpp_utils::set_console_title( INFO_SHORT_NAME " - 命令提示符" );
            cpp_utils::set_console_size( window_handle, std_output_handle, 120, 30 );
            cpp_utils::fix_window_size( window_handle, false );
            cpp_utils::enable_window_maximize_ctrl( window_handle, true );
            _args.parent_ui.lock( false, false );
            SetConsoleScreenBufferSize( std_output_handle, { 127, SHRT_MAX - 1 } );
            std::system( "cmd.exe" );
            cpp_utils::set_console_charset( charset_id );
            cpp_utils::set_console_title( INFO_SHORT_NAME );
            cpp_utils::set_console_size( window_handle, std_output_handle, console_width, console_height );
            cpp_utils::fix_window_size( window_handle, true );
            cpp_utils::enable_window_maximize_ctrl( window_handle, false );
            return cpp_utils::console_ui::back;
        } };
        class cmd_executor final {
          private:
            const ansi_char *const cmd_;
          public:
            auto operator()( cpp_utils::console_ui::func_args )
            {
                std::print(
                  "                   [ 工 具 箱 ]\n\n\n"
                  " -> 执行操作系统命令.\n{}\n",
                  ansi_std_string( console_width, '-' ) );
                std::system( cmd_ );
                return cpp_utils::console_ui::back;
            }
            auto operator=( const cmd_executor & ) -> cmd_executor & = delete;
            auto operator=( cmd_executor && ) -> cmd_executor &      = delete;
            cmd_executor( const ansi_char *const _cmd )
              : cmd_{ _cmd }
            { }
            cmd_executor( const cmd_executor & ) = default;
            cmd_executor( cmd_executor && )      = default;
            ~cmd_executor()                      = default;
        };
        constexpr const ansi_char *common_ops[][ 2 ]{
          {"重启资源管理器",               R"(taskkill.exe /f /im explorer.exe && timeout.exe /t 3 /nobreak && start C:\Windows\explorer.exe)"},
          {"修复操作系统",                 "dism.exe /online /cleanup-image /restorehealth && sfc.exe /scannow"                               },
          {"恢复 USB 设备访问",            R"(reg.exe add "HKLM\SYSTEM\CurrentControlSet\Services\USBSTOR" /f /t reg_dword /v Start /d 3)"    },
          {"恢复 Google Chrome 离线游戏",  R"(reg.exe delete "HKLM\SOFTWARE\Policies\Google\Chrome" /f /v AllowDinosaurEasterEgg)"            },
          {"恢复 Microsoft Edge 离线游戏", R"(reg.exe delete "HKLM\SOFTWARE\Policies\Microsoft\Edge" /f /v AllowSurfGame)"                    }
        };
        cpp_utils::console_ui ui;
        ui.add_back( "                   [ 工 具 箱 ]\n\n" )
          .add_back( " < 返回 ", exit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
          .add_back( " > 命令提示符 ", launch_cmd )
          .add_back( "\n[ 常用操作 ]\n" );
        for ( const auto &common_op : common_ops ) {
            ui.add_back( std::format( " > {} ", common_op[ 0 ] ), cmd_executor{ common_op[ 1 ] } );
        }
        ui.show();
        return cpp_utils::console_ui::back;
    }
    inline auto set_console_attrs( const std::stop_token _msg )
    {
        const auto &window_options{ options[ "window" ] };
        const auto &is_translucent{ window_options[ "translucent" ] };
        const auto &is_disable_close_ctrl{ window_options[ "disable_close_ctrl" ] };
        const auto &is_enable_minimalist_titlebar{ window_options[ "minimalist_titlebar" ] };
        if ( is_disable_x_option_hot_reload ) {
            cpp_utils::set_window_translucency( window_handle, is_translucent ? 230 : 255 );
            cpp_utils::enable_window_close_ctrl( window_handle, !is_disable_close_ctrl );
            cpp_utils::enable_window_menu( window_handle, !is_enable_minimalist_titlebar );
            return;
        }
        while ( !_msg.stop_requested() ) {
            cpp_utils::set_window_translucency( window_handle, is_translucent ? 230 : 255 );
            cpp_utils::enable_window_close_ctrl( window_handle, !is_disable_close_ctrl );
            cpp_utils::enable_window_menu( window_handle, !is_enable_minimalist_titlebar );
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
            if ( !is_keep_window_top ) {
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
        auto engine{ []() static
        {
            constexpr const ansi_char *hkcu_reg_dirs[]{
              R"(Software\Policies\Microsoft\Windows\System)", R"(Software\Microsoft\Windows\CurrentVersion\Policies\System)",
              R"(Software\Microsoft\Windows\CurrentVersion\Policies\Explorer)" };
            constexpr const ansi_char *execs[]{
              "taskkill.exe", "sc.exe",      "net.exe", "reg.exe",  "cmd.exe", "taskmgr.exe",
              "perfmon.exe",  "regedit.exe", "mmc.exe", "dism.exe", "sfc.exe" };
            for ( const auto &reg_dir : hkcu_reg_dirs ) {
                RegDeleteTreeA( HKEY_CURRENT_USER, reg_dir );
            }
            for ( const auto &exec : execs ) {
                RegDeleteTreeA(
                  HKEY_LOCAL_MACHINE,
                  std::format( R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\{})", exec ).c_str() );
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
            if ( !is_fix_os_env ) {
                std::this_thread::sleep_for( default_thread_sleep_time );
                continue;
            }
            engine();
        }
    }
    inline auto load_config( const bool _is_reload )
    {
        std::ifstream config_file{ config_file_name, std::ios::in };
        if ( !config_file.good() ) {
            return;
        }
        if ( _is_reload ) {
            std::print( " -> 准备配置重载.\n" );
            for ( auto &config_node : config_nodes ) {
                config_node->prepare_reload();
            }
        }
        std::print( " -> 加载配置文件.\n" );
        ansi_std_string line;
        ansi_std_string_view line_view;
        basic_config_node *node_ptr{ nullptr };
        while ( std::getline( config_file, line ) ) {
            line_view = line;
            if ( line_view.empty() ) {
                continue;
            }
            if ( line_view.front() == '#' ) {
                continue;
            }
            if ( line_view.size() >= 4 && line_view.substr( 0, sizeof( "[ " ) - 1 ) == "[ "
                 && line_view.substr( line_view.size() - sizeof( " ]" ) + 1, line_view.size() ) == " ]" )
            {
                line_view = line_view.substr( sizeof( "[ " ) - 1, line_view.size() - sizeof( " ]" ) - 1 );
                for ( auto &config_node : config_nodes ) {
                    if ( line_view == config_node->self_name ) {
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
    inline auto edit_config( cpp_utils::console_ui::func_args )
    {
        std::print( " -> 初始化 UI.\n" );
        auto sync{ []( cpp_utils::console_ui::func_args ) static
        {
            std::print( "                    [ 配  置 ]\n\n\n" );
            load_config( true );
            std::print( " -> 保存更改.\n" );
            std::ofstream config_file_stream{ config_file_name, std::ios::out | std::ios::trunc };
            config_file_stream << "# " INFO_FULL_NAME " (" INFO_GIT_TAG ")\n";
            for ( auto &config_node : config_nodes ) {
                config_file_stream << std::format( "[ {} ]\n", config_node->self_name );
                config_node->sync( config_file_stream );
            }
            config_file_stream << std::flush;
            const auto is_good{ config_file_stream.good() };
            std::print( "\n ({}) 同步配置{}.", is_good ? 'i' : '!', is_good ? "成功" : "失败" );
            wait();
            return cpp_utils::console_ui::back;
        } };
        auto open_file{ []( cpp_utils::console_ui::func_args ) static
        {
            if ( std::ifstream{ config_file_name, std::ios::in }.good() ) {
                std::print( " -> 打开配置.\n" );
                ShellExecuteA( nullptr, "open", config_file_name, nullptr, nullptr, SW_SHOWNORMAL );
                return cpp_utils::console_ui::back;
            }
            std::print(
              "                    [ 配  置 ]\n\n\n"
              " (!) 无法打开配置文件." );
            wait();
            return cpp_utils::console_ui::back;
        } };
        cpp_utils::console_ui ui;
        ui.add_back( "                    [ 配  置 ]\n\n" )
          .add_back( " < 返回 ", exit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
          .add_back( " > 同步配置 ", sync )
          .add_back( " > 打开配置文件 ", open_file );
        for ( auto &config_node : config_nodes ) {
            config_node->ui_panel( ui );
        }
        ui.show();
        return cpp_utils::console_ui::back;
    }
    inline const auto &option_crack_restore{ options[ "crack_restore" ] };
    inline const auto &is_hijack_execs{ option_crack_restore[ "hijack_execs" ] };
    inline const auto &is_set_serv_startup_types{ option_crack_restore[ "set_serv_startup_types" ] };
    class crack_with_rules final {
      public:
        const rule_node &rules_;
      public:
        auto operator()( cpp_utils::console_ui::func_args )
        {
            std::print( "                    [ 破  解 ]\n\n\n" );
            if ( rules_.empty() ) {
                std::print( " (i) 规则为空." );
                wait();
                return cpp_utils::console_ui::back;
            }
            std::print( " -> 生成并执行操作系统命令.\n{}\n", ansi_std_string( console_width, '-' ) );
            if ( is_hijack_execs ) {
                for ( const auto &exec : rules_.execs ) {
                    std::system(
                      std::format(
                        R"(reg.exe add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution options\{}" /f /t reg_sz /v debugger /d "nul")",
                        exec )
                        .c_str() );
                }
            }
            if ( is_set_serv_startup_types ) {
                for ( const auto &serv : rules_.servs ) {
                    std::system( std::format( R"(sc.exe config "{}" start= disabled)", serv ).c_str() );
                }
            }
            for ( const auto &exec : rules_.execs ) {
                std::system( std::format( R"(taskkill.exe /f /im "{}")", exec ).c_str() );
            }
            for ( const auto &serv : rules_.servs ) {
                std::system( std::format( R"(net.exe stop "{}" /y)", serv ).c_str() );
            }
            return cpp_utils::console_ui::back;
        }
        auto operator=( const crack_with_rules & ) -> crack_with_rules & = default;
        auto operator=( crack_with_rules && ) -> crack_with_rules &      = default;
        crack_with_rules( const rule_node &_rules )
          : rules_{ _rules }
        { }
        crack_with_rules( const crack_with_rules & ) = default;
        crack_with_rules( crack_with_rules && )      = default;
        ~crack_with_rules()                          = default;
    };
    class restore_with_rules final {
      public:
        const rule_node &rules_;
      public:
        auto operator()( cpp_utils::console_ui::func_args )
        {
            std::print( "                    [ 恢  复 ]\n\n\n" );
            if ( rules_.empty() ) {
                std::print( " (i) 规则为空." );
                wait();
                return cpp_utils::console_ui::back;
            }
            std::print( " -> 生成并执行操作系统命令.\n{}\n", ansi_std_string( console_width, '-' ) );
            if ( is_hijack_execs ) {
                for ( const auto &exec : rules_.execs ) {
                    std::system(
                      std::format(
                        R"(reg.exe delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution options\{}" /f)", exec )
                        .c_str() );
                }
            }
            if ( is_set_serv_startup_types ) {
                for ( const auto &serv : rules_.servs ) {
                    std::system( std::format( R"(sc.exe config "{}" start= auto)", serv ).c_str() );
                }
            }
            for ( const auto &serv : rules_.servs ) {
                std::system( std::format( R"(net.exe start "{}")", serv ).c_str() );
            }
            return cpp_utils::console_ui::back;
        }
        auto operator=( const restore_with_rules & ) -> restore_with_rules & = default;
        auto operator=( restore_with_rules && ) -> restore_with_rules &      = default;
        restore_with_rules( const rule_node &_rules )
          : rules_{ _rules }
        { }
        restore_with_rules( const restore_with_rules & ) = default;
        restore_with_rules( restore_with_rules && )      = default;
        ~restore_with_rules()                            = default;
    };
}
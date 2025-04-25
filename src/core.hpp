#pragma once
#include <fstream>
#include "cpp_utils.hpp"
#include "info.hpp"
namespace core {
    using namespace std::chrono_literals;
    using namespace std::string_view_literals;
    using size_type = cpp_utils::size_type;
    inline constexpr SHORT console_width{ 50 };
    inline constexpr SHORT console_height{ 25 };
    inline constexpr UINT charset_id{ 936 };
    inline const auto cpu_core{ std::thread::hardware_concurrency() };
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
                const char *const self_name;
                const char *const shown_name;
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
                std::abort();
            }
            const auto &operator[]( const std::string_view _self_name ) const
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
            std::abort();
        }
        const auto &operator[]( const std::string_view _self_name ) const
        {
            for ( const auto &node : nodes ) {
                if ( _self_name == node.self_name ) {
                    return node;
                }
            }
            std::abort();
        }
        option_container( std::vector< node > _nodes )
          : nodes{ std::move( _nodes ) }
        { }
    };
    inline option_container options{
      { { { "crack_restore",
            "破解/恢复",
            { { "hijack_execs", "劫持可执行文件" },
              { "set_serv_startup_types", "设置服务启动类型" },
              { "parallel_op", "并行操作 (预览版)" },
              { "fix_os_env", "* 修复操作系统环境" } } },
          { "window",
            "窗口显示",
            { { "keep_window_top", "* 置顶窗口" }, { "minimalist_titlebar", "* 极简标题栏" }, { "translucent", "* 半透明" } } },
          { "perf",
            "性能",
            { { "quick_exit_and_relaunch", "** 快速退出与重启" },
              { "disable_x_option_hot_reload", "** 禁用标 * 选项热重载" } } } } } };
    inline const auto &is_disable_x_option_hot_reload{ options[ "perf" ][ "disable_x_option_hot_reload" ] };
    struct rule_node final {
        const char *const shown_name;
        std::deque< std::string > execs;
        std::deque< std::string > servs;
        auto empty() const
        {
            return execs.empty() && servs.empty();
        }
        auto operator=( const rule_node & ) -> rule_node & = delete;
        auto operator=( rule_node && ) -> rule_node &      = delete;
        rule_node( const char *const _shown_name, std::deque< std::string > _execs, std::deque< std::string > _servs )
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
        const char *const self_name;
        virtual auto load( const bool, std::string & ) -> void = 0;
        virtual auto prepare_reload() -> void { }
        virtual auto sync( std::ofstream & ) -> void = 0;
        virtual auto ui_panel( cpp_utils::console_ui & ) -> void { }
        virtual auto operator=( const basic_config_node & ) -> basic_config_node & = delete;
        virtual auto operator=( basic_config_node && ) -> basic_config_node &      = delete;
        basic_config_node( const char *const _self_name )
          : self_name{ _self_name }
        { }
        basic_config_node( const basic_config_node & ) = delete;
        basic_config_node( basic_config_node && )      = delete;
        virtual ~basic_config_node()                   = default;
    };
    class option_op final : public basic_config_node {
      private:
        static constexpr auto format_string_{ R"("{}.{}": {})" };
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
                option_setter( option_container::node::item &_item )
                  : item_{ _item }
                { }
                ~option_setter() = default;
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
                option_shower( option_container::node &_node )
                  : node_{ _node }
                { }
                ~option_shower() = default;
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
        option_op()
          : basic_config_node{ "options" }
        { }
        virtual ~option_op() = default;
    };
    class custom_rules_execs_op final : public basic_config_node {
      public:
        virtual auto load( const bool, std::string &_line ) -> void override final
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
        custom_rules_execs_op()
          : basic_config_node{ "custom_rules_execs" }
        { }
        virtual ~custom_rules_execs_op() = default;
    };
    class custom_rules_servs_op final : public basic_config_node {
      public:
        virtual auto load( const bool, std::string &_line ) -> void override final
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
        custom_rules_servs_op()
          : basic_config_node{ "custom_rules_servs" }
        { }
        virtual ~custom_rules_servs_op() = default;
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
            SetConsoleScreenBufferSize( std_output_handle, COORD{ 127, SHRT_MAX - 1 } );
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
            const char *const cmd_;
          public:
            auto operator()( cpp_utils::console_ui::func_args )
            {
                std::print(
                  "                   [ 工 具 箱 ]\n\n\n"
                  " -> 执行操作系统命令.\n{}\n",
                  std::string( console_width, '-' ) );
                std::system( cmd_ );
                return cpp_utils::console_ui::back;
            }
            cmd_executor( const char *const _cmd )
              : cmd_{ _cmd }
            { }
            cmd_executor( const cmd_executor & ) = default;
            cmd_executor( cmd_executor && )      = default;
            ~cmd_executor()                      = default;
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
            ui.add_back( std::format( " > {} ", common_cmd[ 0 ] ), cmd_executor{ common_cmd[ 1 ] } );
        }
        ui.show();
        return cpp_utils::console_ui::back;
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
            constexpr const char *hkcu_reg_dirs[]{
              R"(Software\Policies\Microsoft\Windows\System)", R"(Software\Microsoft\Windows\CurrentVersion\Policies\System)",
              R"(Software\Microsoft\Windows\CurrentVersion\Policies\Explorer)" };
            constexpr const char *execs[]{
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
            config_file_stream << "# " INFO_FULL_NAME " (" INFO_VERSION ")\n";
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
        static auto basic_hijack_exec_( exec_const_ref_type _exec )
        {
            std::system(
              std::format(
                R"(reg.exe add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution options\{}" /f /t reg_sz /v debugger /d "nul")",
                _exec )
                .c_str() );
        }
        static auto basic_disable_serv_( serv_const_ref_type _serv )
        {
            std::system( std::format( R"(sc.exe config "{}" start= disabled)", _serv ).c_str() );
        }
        static auto basic_kill_exec_( exec_const_ref_type _exec )
        {
            std::system( std::format( R"(taskkill.exe /f /im "{}")", _exec ).c_str() );
        }
        static auto basic_stop_serv_( serv_const_ref_type _serv )
        {
            std::system( std::format( R"(net.exe stop "{}" /y)", _serv ).c_str() );
        }
      public:
        auto operator()( cpp_utils::console_ui::func_args )
        {
            std::print( "                    [ 破  解 ]\n\n\n" );
            if ( rules_.empty() ) {
                std::print( " (i) 规则为空." );
                wait();
                return cpp_utils::console_ui::back;
            }
            std::print( " -> 生成并执行操作系统命令.\n{}\n", std::string( console_width, '-' ) );
            const auto &execs{ rules_.execs };
            const auto &servs{ rules_.servs };
            switch ( is_parallel_op ) {
                case true : {
                    cpp_utils::thread_pool threads;
                    if ( is_hijack_execs ) {
                        threads.add( [ & ]()
                        { cpp_utils::parallel_for_each( cpu_core, execs.begin(), execs.end(), basic_hijack_exec_ ); } );
                    }
                    if ( is_set_serv_startup_types ) {
                        threads.add( [ & ]()
                        { cpp_utils::parallel_for_each( cpu_core, servs.begin(), servs.end(), basic_disable_serv_ ); } );
                    }
                    threads
                      .add( [ & ]() { cpp_utils::parallel_for_each( cpu_core, execs.begin(), execs.end(), basic_kill_exec_ ); } )
                      .add( [ & ]() { cpp_utils::parallel_for_each( cpu_core, servs.begin(), servs.end(), basic_stop_serv_ ); } );
                    break;
                }
                case false : {
                    if ( is_hijack_execs ) {
                        for ( const auto &exec : execs ) {
                            basic_hijack_exec_( exec );
                        }
                    }
                    if ( is_set_serv_startup_types ) {
                        for ( const auto &serv : servs ) {
                            basic_disable_serv_( serv );
                        }
                    }
                    for ( const auto &exec : execs ) {
                        basic_kill_exec_( exec );
                    }
                    for ( const auto &serv : servs ) {
                        basic_stop_serv_( serv );
                    }
                    break;
                }
            }
            return cpp_utils::console_ui::back;
        }
        crack( const rule_node &_rules )
          : rules_{ _rules }
        { }
        crack( const crack & ) = default;
        crack( crack && )      = default;
        ~crack()               = default;
    };
    class restore final {
      private:
        const rule_node &rules_;
        static auto basic_undo_hijack_exec_( exec_const_ref_type _exec )
        {
            std::system(
              std::format(
                R"(reg.exe delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution options\{}" /f)", _exec )
                .c_str() );
        }
        static auto basic_enable_serv_( serv_const_ref_type _serv )
        {
            std::system( std::format( R"(sc.exe config "{}" start= auto)", _serv ).c_str() );
        }
        static auto basic_start_serv_( serv_const_ref_type _serv )
        {
            std::system( std::format( R"(net.exe start "{}")", _serv ).c_str() );
        }
      public:
        auto operator()( cpp_utils::console_ui::func_args )
        {
            std::print( "                    [ 恢  复 ]\n\n\n" );
            if ( rules_.empty() ) {
                std::print( " (i) 规则为空." );
                wait();
                return cpp_utils::console_ui::back;
            }
            std::print( " -> 生成并执行操作系统命令.\n{}\n", std::string( console_width, '-' ) );
            const auto &execs{ rules_.execs };
            const auto &servs{ rules_.servs };
            switch ( is_parallel_op ) {
                case true : {
                    if ( is_hijack_execs ) {
                        cpp_utils::parallel_for_each( cpu_core, execs.begin(), execs.end(), basic_undo_hijack_exec_ );
                    }
                    if ( is_set_serv_startup_types ) {
                        cpp_utils::parallel_for_each( cpu_core, servs.begin(), servs.end(), basic_enable_serv_ );
                    }
                    cpp_utils::parallel_for_each( cpu_core, servs.begin(), servs.end(), basic_start_serv_ );
                    break;
                }
                case false : {
                    if ( is_hijack_execs ) {
                        for ( const auto &exec : execs ) {
                            basic_undo_hijack_exec_( exec );
                        }
                    }
                    if ( is_set_serv_startup_types ) {
                        for ( const auto &serv : servs ) {
                            basic_enable_serv_( serv );
                        }
                    }
                    for ( const auto &serv : servs ) {
                        basic_start_serv_( serv );
                    }
                    break;
                }
            }
            return cpp_utils::console_ui::back;
        }
        restore( const rule_node &_rules )
          : rules_{ _rules }
        { }
        restore( const restore & ) = default;
        restore( restore && )      = default;
        ~restore()                 = default;
    };
}
#pragma once
#include "cpp_utils.hpp"
#include "info.hpp"
namespace core {
    using namespace std::chrono_literals;
    using namespace std::string_view_literals;
    using size_type        = cpp_utils::size_type;
    using ansi_char        = cpp_utils::ansi_char;
    using ansi_string      = cpp_utils::ansi_string;
    using ansi_string_view = cpp_utils::ansi_string_view;
    using wide_string      = cpp_utils::wide_string;
    template < typename _type_ >
    using type_alloc = cpp_utils::type_alloc< _type_ >;
    inline constexpr auto config_file_name{ "config.ini"sv };
    inline constexpr auto default_thread_sleep_time{ 1s };
    struct option_node final {
        struct sub_key final {
            const ansi_string_view self_name, showed_name;
            bool is_enabled{};
            auto operator=( const sub_key & ) -> sub_key & = delete;
            auto operator=( sub_key && ) -> sub_key &      = delete;
            sub_key( const ansi_string_view _self_name, const ansi_string_view _showed_name )
              : self_name{ _self_name }
              , showed_name{ _showed_name }
            { }
            sub_key( const sub_key & ) = default;
            sub_key( sub_key && )      = default;
            ~sub_key()                 = default;
        };
        struct main_key final {
            const ansi_string_view self_name, showed_name;
            std::vector< sub_key > sub_keys;
            auto &operator[]( const ansi_string_view _self_name )
            {
                for ( auto &key : sub_keys ) {
                    if ( key.self_name == _self_name ) {
                        return key;
                    }
                }
                std::abort();
            }
            auto operator=( const main_key & ) -> main_key & = delete;
            auto operator=( main_key && ) -> main_key &      = delete;
            main_key( const ansi_string_view _self_name, const ansi_string_view _showed_name, std::vector< sub_key > _sub_keys )
              : self_name{ _self_name }
              , showed_name{ _showed_name }
              , sub_keys{ std::move( _sub_keys ) }
            { }
            main_key( const main_key & ) = default;
            main_key( main_key && )      = default;
            ~main_key()                  = default;
        };
        std::vector< main_key > main_keys;
        auto &operator[]( const ansi_string_view _self_name )
        {
            for ( auto &key : main_keys ) {
                if ( key.self_name == _self_name ) {
                    return key;
                }
            }
            std::abort();
        }
        auto operator=( const option_node & ) -> option_node & = delete;
        auto operator=( option_node && ) -> option_node &      = delete;
        option_node( std::vector< main_key > _main_keys )
          : main_keys{ std::move( _main_keys ) }
        { }
        option_node( const option_node & ) = default;
        option_node( option_node && )      = default;
        ~option_node()                     = default;
    };
    inline auto options{ option_node{
      { { "crack_restore",
          "破解/恢复",
          { { "hijack_execs", "劫持可执行文件" }, { "set_serv_startup_types", "设置服务启动类型" } } },
        { "window",
          "窗口显示",
          { { "topmost_show", "置顶显示" }, { "disable_close_ctrl", "禁用关闭控件" }, { "translucency", "半透明化" } } },
        { "other", "其他", { { "fix_os_env", "修复操作系统环境" } } } } } };
    struct rule_node final {
        const ansi_string_view showed_name;
        std::deque< ansi_string > execs, servs;
        auto empty() const
        {
            return execs.empty() && servs.empty();
        }
        auto operator=( const rule_node & ) -> rule_node & = delete;
        auto operator=( rule_node && ) -> rule_node &      = delete;
        rule_node( const ansi_string_view _showed_name, std::deque< ansi_string > _execs, std::deque< ansi_string > _servs )
          : showed_name{ _showed_name }
          , execs{ std::move( _execs ) }
          , servs{ std::move( _servs ) }
        { }
        rule_node( const rule_node & ) = default;
        rule_node( rule_node && )      = default;
        ~rule_node()                   = default;
    };
    inline auto custom_rules{
      rule_node{ "自定义", {}, {} }
    };
    inline const auto builtin_rules{
      std::array{
                 rule_node{
          "极域电子教室",
          { "StudentMain.exe", "DispcapHelper.exe", "VRCwPlayer.exe", "InstHelpApp.exe", "InstHelpApp64.exe", "TDOvrSet.exe",
            "GATESRV.exe", "ProcHelper64.exe", "MasterHelper.exe" },
          { "TDNetFilter", "TDFileFilter", "STUDSRV" } },
                 rule_node{
          "联想智能云教室",
          { "vncviewer.exe", "tvnserver32.exe", "WfbsPnpInstall.exe", "WFBSMon.exe", "WFBSMlogon.exe", "WFBSSvrLogShow.exe",
            "ResetIp.exe", "FuncForWIN64.exe", "CertMgr.exe", "Fireware.exe", "BCDBootCopy.exe", "refreship.exe",
            "lenovoLockScreen.exe", "PortControl64.exe", "DesktopCheck.exe", "DeploymentManager.exe", "DeploymentAgent.exe",
            "XYNTService.exe" },
          { "BSAgentSvr", "tvnserver", "WFBSMlogon" } },
                 rule_node{
          "红蜘蛛多媒体网络教室",
          { "rscheck.exe", "checkrs.exe", "REDAgent.exe", "PerformanceCheck.exe", "edpaper.exe", "Adapter.exe", "repview.exe",
            "FormatPaper.exe" },
          { "appcheck2", "checkapp2" } } }
    };
    class config_item {
      public:
        const ansi_string_view self_name;
        virtual auto load( const bool, const ansi_string_view ) -> void = 0;
        virtual auto reload_init() -> void                              = 0;
        virtual auto sync( ansi_string & ) -> void                      = 0;
        virtual auto operator=( const config_item & ) -> config_item &  = delete;
        virtual auto operator=( config_item && ) -> config_item &       = delete;
        config_item( const ansi_string_view _self_name )
          : self_name{ _self_name }
        { }
        config_item( const config_item & ) = delete;
        config_item( config_item && )      = delete;
        virtual ~config_item()             = default;
    };
    class option_op final : public config_item {
      public:
        virtual auto load( const bool _is_reloaded, const ansi_string_view _line ) -> void override final
        {
            if ( _is_reloaded ) {
                return;
            }
            for ( auto &main_key : options.main_keys ) {
                for ( auto &sub_key : main_key.sub_keys ) {
                    if ( _line == std::format( "{}::{} = true", main_key.self_name, sub_key.self_name ) ) {
                        sub_key.is_enabled = true;
                    } else if ( _line == std::format( "{}::{} = false", main_key.self_name, sub_key.self_name ) ) {
                        sub_key.is_enabled = false;
                    }
                }
            }
        }
        virtual auto reload_init() -> void override final { }
        virtual auto sync( ansi_string &_out ) -> void override final
        {
            for ( const auto &main_key : options.main_keys ) {
                for ( const auto &sub_key : main_key.sub_keys ) {
                    _out.append( std::format(
                      "{}::{} = {}\n", main_key.self_name, sub_key.self_name, sub_key.is_enabled ? "true" : "false" ) );
                }
            }
        }
        virtual auto operator=( const option_op & ) -> option_op & = delete;
        virtual auto operator=( option_op && ) -> option_op &      = delete;
        option_op()
          : config_item{ "options" }
        { }
        option_op( const option_op & ) = delete;
        option_op( option_op && )      = delete;
        virtual ~option_op()           = default;
    };
    class custom_rule_execs_op final : public config_item {
      public:
        virtual auto load( const bool, const ansi_string_view _line ) -> void override final
        {
            custom_rules.execs.emplace_back( _line );
        }
        virtual auto reload_init() -> void override final
        {
            custom_rules.execs.clear();
        }
        virtual auto sync( ansi_string &_out ) -> void override final
        {
            for ( const auto &exec : custom_rules.execs ) {
                _out.append( exec ).push_back( '\n' );
            }
        }
        virtual auto operator=( const custom_rule_execs_op & ) -> custom_rule_execs_op & = delete;
        virtual auto operator=( custom_rule_execs_op && ) -> custom_rule_execs_op &      = delete;
        custom_rule_execs_op()
          : config_item{ "custom_rule_execs" }
        { }
        custom_rule_execs_op( const custom_rule_execs_op & ) = delete;
        custom_rule_execs_op( custom_rule_execs_op && )      = delete;
        virtual ~custom_rule_execs_op()                      = default;
    };
    class custom_rule_servs_op final : public config_item {
      public:
        virtual auto load( const bool, const ansi_string_view _line ) -> void override final
        {
            custom_rules.servs.emplace_back( _line );
        }
        virtual auto reload_init() -> void override final
        {
            custom_rules.servs.clear();
        }
        virtual auto sync( ansi_string &_out ) -> void override final
        {
            for ( const auto &serv : custom_rules.servs ) {
                _out.append( serv ).push_back( '\n' );
            }
        }
        virtual auto operator=( const custom_rule_servs_op & ) -> custom_rule_servs_op & = delete;
        virtual auto operator=( custom_rule_servs_op && ) -> custom_rule_servs_op &      = delete;
        custom_rule_servs_op()
          : config_item{ "custom_rule_servs" }
        { }
        custom_rule_servs_op( const custom_rule_servs_op & ) = delete;
        custom_rule_servs_op( custom_rule_servs_op && )      = delete;
        virtual ~custom_rule_servs_op()                      = default;
    };
    inline auto config_items{
      std::array{
                 std::unique_ptr< config_item >{ new option_op }, std::unique_ptr< config_item >{ new custom_rule_execs_op },
                 std::unique_ptr< config_item >{ new custom_rule_servs_op } }
    };
    template < typename _chrono_type_ >
    inline auto wait( const _chrono_type_ _time )
    {
        for ( auto i{ _time }; i > _chrono_type_{}; --i ) {
            std::print( " {} 后返回.\r", i );
            cpp_utils::perf_sleep( _chrono_type_{ 1 } );
        }
    }
    inline auto quit( cpp_utils::console_ui_ansi::func_args )
    {
        return cpp_utils::console_value::ui_exit;
    }
    inline auto info( cpp_utils::console_ui_ansi::func_args )
    {
        auto visit_repo_webpage{ []( cpp_utils::console_ui_ansi::func_args )
        {
            ShellExecuteA( nullptr, "open", INFO_REPO_URL, nullptr, nullptr, SW_SHOWNORMAL );
            return cpp_utils::console_value::ui_return;
        } };
        auto ui{ cpp_utils::console_ui_ansi{} };
        std::print( " -> 准备用户界面.\n" );
        ui.add_back( "                    [ 信  息 ]\n\n" )
          .add_back( " < 返回 ", quit, cpp_utils::console_value::text_foreground_green | cpp_utils::console_value::text_foreground_intensity )
          .add_back( "\n[名称]\n\n " INFO_NAME " \n\n[版本]\n\n " INFO_VERSION " " )
          .add_back( "\n[仓库]\n" )
          .add_back(
            " " INFO_REPO_URL " ", visit_repo_webpage,
            cpp_utils::console_value::text_default | cpp_utils::console_value::text_common_lvb_underscore )
          .add_back( "\n[许可证]\n\n " INFO_LICENSE " \n\n (C) 2023 - present " INFO_DEVELOPER "." )
          .show();
        return cpp_utils::console_value::ui_return;
    }
    inline auto toolkit( cpp_utils::console_ui_ansi::func_args )
    {
        std::print( " -> 准备用户界面.\n" );
        auto launch_cmd{ []( cpp_utils::console_ui_ansi::func_args _args )
        {
            _args.parent_ui
              .set_console(
                WINDOW_TITLE " - 命令提示符", CODE_PAGE_CODE, 120, 30, false, false,
                options[ "window" ][ "disable_close_ctrl" ].is_enabled ? false : true,
                options[ "window" ][ "translucency" ].is_enabled ? 230 : 255 )
              .lock( false, false );
            SetConsoleScreenBufferSize( GetStdHandle( STD_OUTPUT_HANDLE ), { 128, SHRT_MAX - 1 } );
            std::system( "cmd.exe" );
            _args.parent_ui.set_console(
              WINDOW_TITLE, CODE_PAGE_CODE, WINDOW_WIDTH, WINDOW_HEIGHT, true, false,
              options[ "window" ][ "disable_close_ctrl" ].is_enabled ? false : true,
              options[ "window" ][ "translucency" ].is_enabled ? 230 : 255 );
            return cpp_utils::console_value::ui_return;
        } };
        class cmd_executor final {
          private:
            const ansi_string cmd_;
          public:
            auto operator()( cpp_utils::console_ui_ansi::func_args )
            {
                std::print( " -> 执行操作系统命令.\n{}\n", ansi_string( WINDOW_WIDTH, '-' ) );
                std::system( cmd_.c_str() );
                return cpp_utils::console_value::ui_return;
            }
            auto operator=( const cmd_executor & ) -> cmd_executor & = delete;
            auto operator=( cmd_executor && ) -> cmd_executor &      = delete;
            cmd_executor( const ansi_string_view _cmd )
              : cmd_{ _cmd }
            { }
            cmd_executor( const cmd_executor & ) = default;
            cmd_executor( cmd_executor && )      = default;
            ~cmd_executor()                      = default;
        };
        using common_op_type = std::array< const ansi_char *, 2 >;
        const auto common_ops{
          std::array{
                     common_op_type{
              "重启资源管理器", R"(taskkill.exe /f /im explorer.exe && timeout /t 3 /nobreak && start C:\Windows\explorer.exe)" },
                     common_op_type{
              "恢复 USB 设备访问", R"(reg.exe add "HKLM\SYSTEM\CurrentControlSet\Services\USBSTOR" /f /t reg_dword /v Start /d 3)" },
                     common_op_type{
              "恢复 Google Chrome 离线游戏", R"(reg.exe delete "HKLM\SOFTWARE\Policies\Google\Chrome" /f /v AllowDinosaurEasterEgg)" },
                     common_op_type{
              "恢复 Microsoft Edge 离线游戏", R"(reg.exe delete "HKLM\SOFTWARE\Policies\Microsoft\Edge" /f /v AllowSurfGame)" } }
        };
        auto ui{ cpp_utils::console_ui_ansi{} };
        std::print( " -> 准备用户界面.\n" );
        ui.add_back( "                   [ 工 具 箱 ]\n\n" )
          .add_back( " < 返回 ", quit, cpp_utils::console_value::text_foreground_green | cpp_utils::console_value::text_foreground_intensity )
          .add_back( " > 命令提示符 ", launch_cmd )
          .add_back( "\n[常用操作]\n" );
        for ( const auto &op : common_ops ) {
            ui.add_back( std::format( " > {} ", op[ 0 ] ), cmd_executor{ op[ 1 ] } );
        }
        ui.show();
        return cpp_utils::console_value::ui_return;
    }
    class set_window final : private cpp_utils::multithread_task_ansi {
      private:
        type_alloc< const bool & > is_topmost_shown_, is_disabled_close_ctrl_, is_translucency_;
        auto set_attrs_( const std::stop_token _msg )
        {
            while ( !_msg.stop_requested() ) {
                SetLayeredWindowAttributes( GetConsoleWindow(), RGB( 0, 0, 0 ), is_translucency_ ? 230 : 255, LWA_ALPHA );
                EnableMenuItem(
                  GetSystemMenu( GetConsoleWindow(), FALSE ), SC_CLOSE,
                  is_disabled_close_ctrl_ ? MF_BYCOMMAND | MF_DISABLED | MF_GRAYED : MF_BYCOMMAND | MF_ENABLED );
                cpp_utils::perf_sleep( default_thread_sleep_time );
            }
        }
        auto topmost_show_( const std::stop_token _msg )
        {
            const auto this_window{ GetConsoleWindow() };
            const auto foreground_id{ GetWindowThreadProcessId( this_window, nullptr ) };
            const auto current_id{ GetCurrentThreadId() };
            while ( !_msg.stop_requested() ) {
                if ( !is_topmost_shown_ ) {
                    SetWindowPos( GetConsoleWindow(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
                    cpp_utils::perf_sleep( default_thread_sleep_time );
                    continue;
                }
                AttachThreadInput( current_id, foreground_id, TRUE );
                ShowWindow( this_window, SW_SHOWNORMAL );
                SetForegroundWindow( this_window );
                AttachThreadInput( current_id, foreground_id, FALSE );
                SetWindowPos( this_window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
                cpp_utils::perf_sleep( 100ms );
            }
            SetWindowPos( GetConsoleWindow(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
        }
      public:
        auto operator=( const set_window & ) -> set_window & = delete;
        auto operator=( set_window && ) -> set_window &      = delete;
        set_window( const bool &_is_topmost_shown, const bool &_is_disabled_close_ctrl, const bool &_is_translucency )
          : is_topmost_shown_{ _is_topmost_shown }
          , is_disabled_close_ctrl_{ _is_disabled_close_ctrl }
          , is_translucency_{ _is_translucency }
        {
            std::print( " -> 创建线程: 窗口属性设定.\n" );
            add_task( std::jthread{ set_attrs_, this } );
            std::print( " -> 创建线程: 置顶显示.\n" );
            add_task( std::jthread{ topmost_show_, this } );
        }
        set_window( const set_window & ) = delete;
        set_window( set_window && )      = delete;
        ~set_window()                    = default;
    };
    class fix_os_env final : private cpp_utils::multithread_task_ansi {
      private:
        const bool &is_enabled_;
        auto exec_op_( const std::stop_token _msg )
        {
            constexpr auto hkcu_reg_dirs{
              std::array{
                         R"(Software\Policies\Microsoft\Windows\System)", R"(Software\Microsoft\Windows\CurrentVersion\Policies\System)",
                         R"(Software\Microsoft\Windows\CurrentVersion\Policies\Explorer)" }
            };
            constexpr auto execs{
              std::array{
                         "mode.com", "chcp.com", "ntsd.exe", "taskkill.exe", "sc.exe", "net.exe", "reg.exe", "cmd.exe", "taskmgr.exe",
                         "perfmon.exe", "regedit.exe", "mmc.exe" }
            };
            while ( !_msg.stop_requested() ) {
                if ( !is_enabled_ ) {
                    cpp_utils::perf_sleep( default_thread_sleep_time );
                    continue;
                }
                for ( const auto &reg_dir : hkcu_reg_dirs ) {
                    RegDeleteTreeA( HKEY_CURRENT_USER, reg_dir );
                }
                for ( const auto &exec : execs ) {
                    RegDeleteTreeA(
                      HKEY_LOCAL_MACHINE,
                      std::format( R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\{})", exec ).c_str() );
                }
                cpp_utils::perf_sleep( 1s );
            }
        }
      public:
        auto operator=( const fix_os_env & ) -> fix_os_env & = delete;
        auto operator=( fix_os_env && ) -> fix_os_env &      = delete;
        fix_os_env( const bool &_is_enabled )
          : is_enabled_{ _is_enabled }
        {
            std::print( " -> 创建线程: 修复操作系统环境.\n" );
            add_task( std::jthread{ exec_op_, this } );
        }
        fix_os_env( const fix_os_env & ) = delete;
        fix_os_env( fix_os_env && )      = delete;
        ~fix_os_env()                    = default;
    };
    class config_op final {
      public:
        enum class mod { load, edit };
      private:
        const mod mod_data_;
        static constexpr auto option_button_color{
          cpp_utils::console_value::text_foreground_red | cpp_utils::console_value::text_foreground_green };
        auto load_( const bool _is_reloaded )
        {
            auto config_file{
              std::ifstream{ config_file_name.data(), std::ios::in }
            };
            if ( !config_file.good() ) {
                return;
            }
            if ( _is_reloaded ) {
                std::print( " -> 初始化配置.\n" );
                for ( auto &config_item : config_items ) {
                    config_item->reload_init();
                }
            }
            std::print( " -> 加载配置文件.\n" );
            auto line{ ansi_string{} };
            auto view{ ansi_string_view{} };
            auto config_item_ptr{ ( config_item * ) {} };
            while ( std::getline( config_file, line ) ) {
                view = line;
                if ( view.empty() ) {
                    continue;
                }
                if ( view.front() == '#' ) {
                    continue;
                }
                if ( view.substr( 0, sizeof( "[ " ) - 1 ) == "[ "
                     && view.substr( view.size() - sizeof( " ]" ) + 1, view.size() ) == " ]" )
                {
                    view = view.substr( sizeof( "[ " ) - 1, view.size() - sizeof( " ]" ) - 1 );
                    for ( auto &config_item : config_items ) {
                        if ( config_item->self_name == view ) {
                            config_item_ptr = config_item.get();
                            break;
                        }
                        config_item_ptr = nullptr;
                    }
                    continue;
                }
                if ( config_item_ptr != nullptr ) {
                    config_item_ptr->load( _is_reloaded, view );
                }
            }
            return;
        }
        auto sync()
        {
            std::print( "                    [ 配  置 ]\n\n\n" );
            load_( true );
            std::print( " -> 保存更改.\n" );
            auto config_text{ ansi_string{} };
            for ( auto &config_item : config_items ) {
                config_text.append( std::format( "[ {} ]\n", config_item->self_name ) );
                config_item->sync( config_text );
            }
            auto config_file{
              std::ofstream{ config_file_name.data(), std::ios::out | std::ios::trunc }
            };
            config_file << std::format(
              "# {}\n"
              "# [同步时间] {} (UTC)\n"
              "# [同步版本] {}\n"
              "# [字符编码] {} ({})\n",
              INFO_NAME, std::chrono::system_clock::now(), INFO_VERSION, CODE_PAGE_NAME, CODE_PAGE_CODE )
                        << config_text << std::flush;
            std::print( "\n ({}) 同步配置{}.\n\n", config_file.good() ? 'i' : '!', config_file.good() ? "成功" : "失败" );
            wait( 3s );
            return cpp_utils::console_value::ui_return;
        }
        auto open_file()
        {
            if ( std::ifstream{ config_file_name.data(), std::ios::in }.good() ) {
                std::print( " -> 打开配置.\n" );
                ShellExecuteA( nullptr, "open", config_file_name.data(), nullptr, nullptr, SW_SHOWNORMAL );
                return cpp_utils::console_value::ui_return;
            }
            std::print(
              "                    [ 配  置 ]\n\n\n"
              " (i) 无法读取配置文件.\n\n" );
            wait( 3s );
            return cpp_utils::console_value::ui_return;
        }
        class option_setter final {
          private:
            option_node::sub_key &sub_key_;
            const bool sub_key_value_;
          public:
            auto operator()( cpp_utils::console_ui_ansi::func_args )
            {
                sub_key_.is_enabled = sub_key_value_;
                return cpp_utils::console_value::ui_return;
            }
            auto operator=( const option_setter & ) -> option_setter & = delete;
            auto operator=( option_setter && ) -> option_setter &      = delete;
            option_setter( option_node::sub_key &_sub_key, const bool _sub_key_value )
              : sub_key_{ _sub_key }
              , sub_key_value_{ _sub_key_value }
            { }
            option_setter( const option_setter & ) = default;
            option_setter( option_setter && )      = default;
            ~option_setter()                       = default;
        };
        class option_shower final {
          private:
            option_node::main_key &main_key_;
          public:
            auto operator()( cpp_utils::console_ui_ansi::func_args )
            {
                auto ui{ cpp_utils::console_ui_ansi{} };
                std::print( " -> 准备用户界面.\n" );
                ui.add_back( "                    [ 配  置 ]\n\n" )
                  .add_back(
                    std::format( " < 折叠 {} ", main_key_.showed_name ), quit,
                    cpp_utils::console_value::text_foreground_green | cpp_utils::console_value::text_foreground_intensity );
                for ( auto &sub_key : main_key_.sub_keys ) {
                    ui.add_back( std::format( "\n[{}]\n", sub_key.showed_name ) )
                      .add_back( " > 启用 ", option_setter{ sub_key, true }, option_button_color )
                      .add_back( " > 禁用 ", option_setter{ sub_key, false }, option_button_color );
                }
                ui.show();
                return cpp_utils::console_value::ui_return;
            }
            auto operator=( const option_shower & ) -> option_shower & = delete;
            auto operator=( option_shower && ) -> option_shower &      = delete;
            option_shower( option_node::main_key &_main_key )
              : main_key_{ _main_key }
            { }
            option_shower( const option_shower & ) = default;
            option_shower( option_shower && )      = default;
            ~option_shower()                       = default;
        };
        auto edit_()
        {
            auto ui{ cpp_utils::console_ui_ansi{} };
            std::print( " -> 准备用户界面.\n" );
            ui
              .add_back( std::format(
                "                    [ 配  置 ]\n\n\n"
                " (i) 所有选项默认禁用, 每 {} 自动应用.\n"
                "     相关信息可参阅文档.\n",
                default_thread_sleep_time ) )
              .add_back( " < 返回 ", quit, cpp_utils::console_value::text_foreground_green | cpp_utils::console_value::text_foreground_intensity )
              .add_back( " > 同步配置 ", [ this ]( cpp_utils::console_ui_ansi::func_args ) { return sync(); } )
              .add_back( " > 打开配置文件 ", [ this ]( cpp_utils::console_ui_ansi::func_args ) { return open_file(); } )
              .add_back( "\n[选项]\n" );
            for ( auto &key : options.main_keys ) {
                ui.add_back( std::format( " > {} ", key.showed_name ), option_shower{ key } );
            }
            ui.show();
        }
      public:
        auto operator()( cpp_utils::console_ui_ansi::func_args )
        {
            switch ( mod_data_ ) {
                case mod::load : load_( false ); break;
                case mod::edit : edit_(); break;
            }
            return cpp_utils::console_value::ui_return;
        }
        auto operator=( const config_op & ) -> config_op & = delete;
        auto operator=( config_op && ) -> config_op &      = delete;
        config_op( const mod _mod_data )
          : mod_data_{ _mod_data }
        { }
        config_op( const config_op & ) = default;
        config_op( config_op && )      = default;
        ~config_op()                   = default;
    };
    class rule_op final {
      public:
        enum class mod { crack, restore };
      private:
        const mod mod_data_;
        const rule_node &rules_;
      public:
        auto operator()( cpp_utils::console_ui_ansi::func_args )
        {
            std::print( "                 [ 破 解 / 恢 复 ]\n\n\n" );
            if ( rules_.empty() ) {
                std::print( " (i) 规则为空.\n\n" );
                wait( 3s );
                return cpp_utils::console_value::ui_return;
            }
            std::print( " -> 生成并执行操作系统命令.\n{}\n", ansi_string( WINDOW_WIDTH, '-' ) );
            switch ( mod_data_ ) {
                case mod::crack : {
                    if ( options[ "crack_restore" ][ "hijack_execs" ].is_enabled ) {
                        for ( const auto &exec : rules_.execs ) {
                            std::system(
                              std::format(
                                R"(reg.exe add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution options\{}" /f /t reg_sz /v debugger /d "nul")",
                                exec )
                                .c_str() );
                        }
                    }
                    if ( options[ "crack_restore" ][ "set_serv_startup_types" ].is_enabled ) {
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
                    break;
                }
                case mod::restore : {
                    if ( options[ "crack_restore" ][ "hijack_execs" ].is_enabled ) {
                        for ( const auto &exec : rules_.execs ) {
                            std::system(
                              std::format(
                                R"(reg.exe delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution options\{}" /f)",
                                exec )
                                .c_str() );
                        }
                    }
                    if ( options[ "crack_restore" ][ "set_serv_startup_types" ].is_enabled ) {
                        for ( const auto &serv : rules_.servs ) {
                            std::system( std::format( R"(sc.exe config "{}" start= auto)", serv ).c_str() );
                        }
                    }
                    for ( const auto &serv : rules_.servs ) {
                        std::system( std::format( R"(net.exe start "{}")", serv ).c_str() );
                    }
                    break;
                }
            }
            return cpp_utils::console_value::ui_return;
        }
        auto operator=( const rule_op & ) -> rule_op & = delete;
        auto operator=( rule_op && ) -> rule_op &      = delete;
        rule_op( const mod _mod_data, const rule_node &_rule )
          : mod_data_{ _mod_data }
          , rules_{ _rule }
        { }
        rule_op( const rule_op & ) = default;
        rule_op( rule_op && )      = default;
        ~rule_op()                 = default;
    };
}
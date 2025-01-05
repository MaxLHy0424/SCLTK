#pragma once
#include <climits>
#include <fstream>
#include "console_ui.hpp"
#include "info.hpp"
#define NOTHING_TO_DO \
    { }
namespace core {
    using namespace std::chrono_literals;
    using namespace std::string_view_literals;
    using size_type        = console_ui::size_type;
    using string_type      = console_ui::string_type;
    using string_view_type = console_ui::string_view_type;
    using wstring_type     = std::wstring;
    template < typename _type_ >
    using type_alloc = console_ui::type_alloc< _type_ >;
    inline constexpr auto config_file_name{ "config.ini"sv };
    inline constexpr auto default_thread_sleep_time{ 1s };
    struct option_node final {
        struct sub_key final {
            const string_view_type self_name, showed_name;
            bool is_enabled{};
            auto operator=( const sub_key & ) -> sub_key & = default;
            auto operator=( sub_key && ) -> sub_key &      = default;
            sub_key( const string_view_type &_self_name, const string_view_type &_showed_name )
              : self_name{ _self_name }
              , showed_name{ _showed_name }
            { }
            sub_key( const sub_key & ) = default;
            sub_key( sub_key && )      = default;
            ~sub_key()                 = default;
        };
        struct main_key final {
            const string_view_type self_name, showed_name;
            std::vector< sub_key > sub_keys;
            auto &operator[]( const string_view_type &_self_name )
            {
                for ( auto &key : sub_keys ) {
                    if ( key.self_name == _self_name ) {
                        return key;
                    }
                }
                throw std::runtime_error{ "not found sub option" };
            }
            auto operator=( const main_key & ) -> main_key & = default;
            auto operator=( main_key && ) -> main_key &      = default;
            main_key( const string_view_type &_self_name, const string_view_type &_showed_name, std::vector< sub_key > _sub_keys )
              : self_name{ _self_name }
              , showed_name{ _showed_name }
              , sub_keys{ std::move( _sub_keys ) }
            { }
            main_key( const main_key & ) = default;
            main_key( main_key && )      = default;
            ~main_key()                  = default;
        };
        std::vector< main_key > main_keys;
        auto &operator[]( const string_view_type &_self_name )
        {
            for ( auto &key : main_keys ) {
                if ( key.self_name == _self_name ) {
                    return key;
                }
            }
            throw std::runtime_error{ "not found sub option" };
            std::abort();
        }
        auto operator=( const option_node & ) -> option_node & = default;
        auto operator=( option_node && ) -> option_node &      = default;
        option_node( std::vector< main_key > _main_keys )
          : main_keys{ std::move( _main_keys ) }
        { }
        option_node( const option_node & ) = default;
        option_node( option_node && )      = default;
        ~option_node()                     = default;
    };
    inline option_node options{
      { { "rule_op", "破解/恢复", { { "hijack_execs", "劫持可执行文件" }, { "set_serv_startup_types", "设置服务启动类型" } } },
       { "window",
          "窗口显示",
          { { "topmost_show", "置顶显示" }, { "disable_close_ctrl", "禁用关闭控件" }, { "translucency", "半透明化" } } },
       { "other", "其他", { { "fix_os_env", "修复操作系统环境" } } } }
    };
    struct rule_node final {
        const string_view_type showed_name;
        std::deque< string_type > execs, servs;
        auto operator=( const rule_node & ) -> rule_node & = default;
        auto operator=( rule_node && ) -> rule_node &      = default;
        rule_node( const string_view_type &_showed_name, std::deque< string_type > _execs, std::deque< string_type > _servs )
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
    inline auto is_run_as_admin()
    {
        auto is_admin{ BOOL{} };
        auto admins_group{ PSID{} };
        auto nt_authority{ SID_IDENTIFIER_AUTHORITY{ SECURITY_NT_AUTHORITY } };
        if ( AllocateAndInitializeSid(
               &nt_authority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &admins_group )
             == true )
        {
            CheckTokenMembership( nullptr, admins_group, &is_admin );
            FreeSid( admins_group );
        }
        return is_admin;
    }
    inline auto relaunch_as_admin( console_ui::func_args )
    {
        auto file_path{ wstring_type( MAX_PATH, L'\0' ) };
        GetModuleFileNameW( nullptr, file_path.data(), MAX_PATH );
        ShellExecuteW( nullptr, L"runas", file_path.c_str(), nullptr, nullptr, SW_SHOWNORMAL );
        std::exit( 0 );
        return console_ui::value::ui_exit;
    }
    template < typename _chrono_type_ >
    inline auto perf_sleep( const _chrono_type_ &_time )
    {
        console_ui::perf_sleep( _time );
    }
    template < typename _chrono_type_ >
    inline auto wait( const _chrono_type_ &_t )
    {
        for ( auto i{ _t }; i > _chrono_type_{}; --i ) {
            std::print( " {} 后返回.\r", i );
            perf_sleep( _chrono_type_{ 1 } );
        }
    }
    inline auto quit( console_ui::func_args )
    {
        return console_ui::value::ui_exit;
    }
    inline auto info( console_ui::func_args )
    {
        auto visit_repo_webpage{ []( console_ui::func_args )
        {
            ShellExecuteA( nullptr, "open", INFO_REPO_URL, nullptr, nullptr, SW_SHOWNORMAL );
            return console_ui::value::ui_return;
        } };
        auto ui{ console_ui{} };
        std::print( " -> 准备用户界面.\n" );
        ui.add_back( "                    [ 信  息 ]\n\n" )
          .add_back( " < 返回 ", quit, console_ui::value::text_foreground_green | console_ui::value::text_foreground_intensity )
          .add_back( "\n[名称]\n\n " INFO_NAME "\n\n[版本]\n\n " INFO_VERSION "" )
          .add_back( "\n[仓库]\n" )
          .add_back( " " INFO_REPO_URL " ", visit_repo_webpage, console_ui::value::text_default | console_ui::value::text_common_lvb_underscore )
          .add_back( "\n[许可证]\n\n " INFO_LICENSE "\n\n (C) 2023 - present " INFO_DEVELOPER "." )
          .show();
        return console_ui::value::ui_return;
    }
    inline auto toolkit( console_ui::func_args )
    {
        std::print( " -> 准备用户界面.\n" );
        auto launch_cmd{ []( console_ui::func_args _args )
        {
            _args.parent_ui
              .set_console(
                WINDOW_TITLE " - 命令提示符", CODE_PAGE_CODE, 120, 30, false, false,
                options[ "window" ][ "disable_close_ctrl" ].is_enabled ? false : true,
                options[ "window" ][ "translucency" ].is_enabled ? 230 : 255 )
              .lock( false, false );
            SetConsoleScreenBufferSize( GetStdHandle( STD_OUTPUT_HANDLE ), { 128, SHRT_MAX - 1 } );
            system( "cmd.exe" );
            _args.parent_ui.set_console(
              WINDOW_TITLE, CODE_PAGE_CODE, WINDOW_WIDTH, WINDOW_HEIGHT, true, false,
              options[ "window" ][ "disable_close_ctrl" ].is_enabled ? false : true,
              options[ "window" ][ "translucency" ].is_enabled ? 230 : 255 );
            return console_ui::value::ui_return;
        } };
        class cmd_executor final {
          private:
            const string_type cmd_;
          public:
            auto operator()( console_ui::func_args )
            {
                std::print( " -> 执行操作系统命令.\n{}\n", string_type( WINDOW_WIDTH, '-' ) );
                system( cmd_.c_str() );
                return console_ui::value::ui_return;
            }
            auto operator=( const cmd_executor & ) -> cmd_executor & = default;
            auto operator=( cmd_executor && ) -> cmd_executor &      = default;
            cmd_executor( const string_view_type &_cmd )
              : cmd_{ _cmd }
            { }
            cmd_executor( const cmd_executor & ) = default;
            cmd_executor( cmd_executor && )      = default;
            ~cmd_executor()                      = default;
        };
        string_view_type common_ops[][ 2 ]{
          {"重启资源管理器",               R"(taskkill.exe /f /im explorer.exe && timeout /t 3 /nobreak && start C:\Windows\explorer.exe)"},
          {"恢复 USB 设备访问",            R"(reg.exe add "HKLM\SYSTEM\CurrentControlSet\Services\USBSTOR" /f /t reg_dword /v Start /d 3)"},
          {"恢复 Google Chrome 离线游戏",  R"(reg.exe delete "HKLM\SOFTWARE\Policies\Google\Chrome" /f /v AllowDinosaurEasterEgg)"        },
          {"恢复 Microsoft Edge 离线游戏", R"(reg.exe delete "HKLM\SOFTWARE\Policies\Microsoft\Edge" /f /v AllowSurfGame)"                }
        };
        auto ui{ console_ui{} };
        std::print( " -> 准备用户界面.\n" );
        ui.add_back( "                   [ 工 具 箱 ]\n\n" )
          .add_back( " < 返回 ", quit, console_ui::value::text_foreground_green | console_ui::value::text_foreground_intensity )
          .add_back( " > 命令提示符 ", launch_cmd )
          .add_back( "\n[常用操作]\n" );
        for ( auto &op : common_ops ) {
            ui.add_back( std::format( " > {} ", op[ 0 ] ), cmd_executor{ op[ 1 ] } );
        }
        ui.show();
        return console_ui::value::ui_return;
    }
    class multithread_task {
      protected:
        struct task_node_ final {
            std::jthread task_thread{};
            auto operator=( const task_node_ & ) -> task_node_ & = default;
            auto operator=( task_node_ && ) -> task_node_ &      = default;
            task_node_()                                         = default;
            task_node_( std::jthread _task_thread )
              : task_thread{ std::move( _task_thread ) }
            { }
            task_node_( const task_node_ & ) = default;
            task_node_( task_node_ && )      = default;
            ~task_node_()
            {
                if ( task_thread.joinable() ) {
                    std::print( " -> 终止线程 {}.\n", task_thread.get_id() );
                }
            }
        };
        std::vector< task_node_ > threads_{};
        auto operator=( const multithread_task & ) -> multithread_task & = default;
        auto operator=( multithread_task && ) -> multithread_task &      = default;
        multithread_task()                                               = default;
        multithread_task( const multithread_task & )                     = default;
        multithread_task( multithread_task && )                          = default;
        ~multithread_task()                                              = default;
    };
    class set_window final : private multithread_task {
      private:
        type_alloc< const bool & > is_topmost_, is_disable_close_ctrl_, is_translucency_;
        auto set_attrs_( std::stop_token &&_msg )
        {
            while ( !_msg.stop_requested() ) {
                SetLayeredWindowAttributes( GetConsoleWindow(), RGB( 0, 0, 0 ), is_translucency_ ? 230 : 255, LWA_ALPHA );
                EnableMenuItem(
                  GetSystemMenu( GetConsoleWindow(), FALSE ), SC_CLOSE,
                  is_disable_close_ctrl_ ? MF_BYCOMMAND | MF_DISABLED | MF_GRAYED : MF_BYCOMMAND | MF_ENABLED );
                perf_sleep( default_thread_sleep_time );
            }
        }
        auto topmost_show_( std::stop_token &&_msg )
        {
            const auto this_window{ GetConsoleWindow() };
            const auto foreground_id{ GetWindowThreadProcessId( this_window, nullptr ) };
            const auto current_id{ GetCurrentThreadId() };
            while ( !_msg.stop_requested() ) {
                if ( !is_topmost_ ) {
                    SetWindowPos( GetConsoleWindow(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
                    perf_sleep( default_thread_sleep_time );
                    continue;
                }
                AttachThreadInput( current_id, foreground_id, TRUE );
                ShowWindow( this_window, SW_SHOWNORMAL );
                SetForegroundWindow( this_window );
                AttachThreadInput( current_id, foreground_id, FALSE );
                SetWindowPos( this_window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
                perf_sleep( 100ms );
            }
            SetWindowPos( GetConsoleWindow(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
        }
      public:
        auto operator=( const set_window & ) -> set_window & = default;
        auto operator=( set_window && ) -> set_window &      = default;
        set_window( const bool &_is_topmost, const bool &_is_disable_close_ctrl, const bool &_is_translucency )
          : is_topmost_{ _is_topmost }
          , is_disable_close_ctrl_{ _is_disable_close_ctrl }
          , is_translucency_{ _is_translucency }
        {
            std::print( " -> 创建线程: 窗口属性设定.\n" );
            threads_.emplace_back( task_node_{
              std::jthread{ set_attrs_, this }
            } );
            std::print( " -> 创建线程: 置顶显示.\n" );
            threads_.emplace_back( task_node_{
              std::jthread{ topmost_show_, this }
            } );
        }
        set_window( const set_window & ) = default;
        set_window( set_window && )      = default;
        ~set_window()                    = default;
    };
    class fix_os_env final : private multithread_task {
      private:
        const bool &is_enabled_;
        auto exec_op_( std::stop_token &&_msg )
        {
            type_alloc< const string_view_type[] > hkcu_reg_dirs{
              R"(Software\Policies\Microsoft\Windows\System)", R"(Software\Microsoft\Windows\CurrentVersion\Policies\System)",
              R"(Software\Microsoft\Windows\CurrentVersion\Policies\Explorer)" },
              execs{ "mode.com", "chcp.com", "ntsd.exe",    "taskkill.exe", "sc.exe",      "net.exe",
                     "reg.exe",  "cmd.exe",  "taskmgr.exe", "perfmon.exe",  "regedit.exe", "mmc.exe" };
            while ( !_msg.stop_requested() ) {
                if ( !is_enabled_ ) {
                    perf_sleep( default_thread_sleep_time );
                    continue;
                }
                for ( const auto &reg_dir : hkcu_reg_dirs ) {
                    RegDeleteTreeA( HKEY_CURRENT_USER, reg_dir.data() );
                }
                for ( const auto &exec : execs ) {
                    RegDeleteTreeA(
                      HKEY_LOCAL_MACHINE,
                      std::format( R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\{})", exec ).c_str() );
                }
                perf_sleep( 1s );
            }
        }
      public:
        auto operator=( const fix_os_env & ) -> fix_os_env & = default;
        auto operator=( fix_os_env && ) -> fix_os_env &      = default;
        fix_os_env( const bool &_is_enabled )
          : is_enabled_{ _is_enabled }
        {
            std::print( " -> 创建线程: 修复操作系统环境.\n" );
            threads_.emplace_back( task_node_{
              std::jthread{ exec_op_, this }
            } );
        }
        fix_os_env( const fix_os_env & ) = default;
        fix_os_env( fix_os_env && )      = default;
        ~fix_os_env()                    = default;
    };
    class config_op final {
      public:
        enum class mod { load, edit };
      private:
        const mod mod_data_;
        static constexpr auto option_button_color{
          console_ui::value::text_foreground_red | console_ui::value::text_foreground_green };
        auto load_( const bool _is_reload )
        {
            auto config_file{
              std::ifstream{ config_file_name.data(), std::ios::in }
            };
            if ( !config_file.is_open() ) {
                config_file.close();
                return;
            }
            std::print( " -> 加载配置文件.\n" );
            if ( _is_reload ) {
                custom_rules.execs.clear();
                custom_rules.servs.clear();
            }
            enum class config_label { unknown, options, custom_rule_execs, custom_rule_servs };
            auto label{ config_label::unknown };
            auto line{ string_type{} };
            while ( std::getline( config_file, line ) ) {
                if ( line.empty() || line.front() == '#' ) {
                    continue;
                }
                if ( line.substr( 0, 2 ) == "[ " && line.substr( line.size() - 2, line.size() ) == " ]" ) {
                    if ( line == "[ options ]" ) {
                        label = config_label::options;
                        continue;
                    } else if ( line == "[ custom_rule_execs ]" ) {
                        label = config_label::custom_rule_execs;
                        continue;
                    } else if ( line == "[ custom_rule_servs ]" ) {
                        label = config_label::custom_rule_servs;
                        continue;
                    } else {
                        label = config_label::unknown;
                        continue;
                    }
                }
                switch ( label ) {
                    case config_label::unknown : break;
                    case config_label::options : {
                        if ( _is_reload ) {
                            continue;
                        }
                        for ( auto &main_key : options.main_keys ) {
                            for ( auto &sub_key : main_key.sub_keys ) {
                                if ( line == std::format( "{}::{} = true", main_key.self_name, sub_key.self_name ) ) {
                                    sub_key.is_enabled = true;
                                }
                                if ( line == std::format( "{}::{} = false", main_key.self_name, sub_key.self_name ) ) {
                                    sub_key.is_enabled = false;
                                }
                            }
                        }
                        break;
                    }
                    case config_label::custom_rule_execs : custom_rules.execs.emplace_back( std::move( line ) ); break;
                    case config_label::custom_rule_servs : custom_rules.servs.emplace_back( std::move( line ) ); break;
                }
            }
            config_file.close();
            return;
        }
        auto sync()
        {
            std::print( "                    [ 配  置 ]\n\n\n" );
            load_( true );
            std::print( " -> 保存更改.\n" );
            auto config_text{ string_type{} };
            config_text.append( "[ options ]\n" );
            for ( const auto &main_key : options.main_keys ) {
                for ( const auto &sub_key : main_key.sub_keys ) {
                    config_text.append( std::format(
                      "{}::{} = {}\n", main_key.self_name, sub_key.self_name, sub_key.is_enabled ? "true" : "false" ) );
                }
            }
            config_text.append( "[ custom_rule_execs ]\n" );
            for ( const auto &exec : custom_rules.execs ) {
                config_text.append( exec ).push_back( '\n' );
            }
            config_text.append( "[ custom_rule_servs ]\n" );
            for ( const auto &serv : custom_rules.servs ) {
                config_text.append( serv ).push_back( '\n' );
            }
            auto config_file{
              std::ofstream{ config_file_name.data(), std::ios::out | std::ios::trunc }
            };
            config_file << std::format(
              "# {}\n"
              "# [同步时间] {} (UTC)\n"
              "# [同步版本] {}\n"
              "# [字符编码] {} ({})\n",
              INFO_NAME, std::chrono::utc_clock::now(), INFO_VERSION, CODE_PAGE_NAME, CODE_PAGE_CODE )
                        << config_text << std::flush;
            std::print( "\n ({}) 同步配置{}.\n\n", config_file.fail() ? '!' : 'i', config_file.fail() ? "失败" : "成功" );
            wait( 3s );
            config_file.close();
            return console_ui::value::ui_return;
        }
        auto open_file()
        {
            if ( std::ifstream{ config_file_name.data(), std::ios::in }.is_open() ) {
                std::print( " -> 打开配置.\n" );
                ShellExecuteA( nullptr, "open", config_file_name.data(), nullptr, nullptr, SW_SHOWNORMAL );
                return console_ui::value::ui_return;
            }
            std::print(
              "                    [ 配  置 ]\n\n\n"
              " (i) 无法读取配置文件.\n\n" );
            wait( 3s );
            return console_ui::value::ui_return;
        }
        class option_setter {
          private:
            option_node::sub_key &sub_key_;
            const bool sub_key_value_;
          public:
            auto operator()( console_ui::func_args )
            {
                sub_key_.is_enabled = sub_key_value_;
                return console_ui::value::ui_return;
            }
            auto operator=( const option_setter & ) -> option_setter & = default;
            auto operator=( option_setter && ) -> option_setter &      = default;
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
            auto operator()( console_ui::func_args )
            {
                auto ui{ console_ui{} };
                std::print( " -> 准备用户界面.\n" );
                ui.add_back( "                    [ 配  置 ]\n\n" )
                  .add_back(
                    std::format( " < 折叠 {} ", main_key_.showed_name ), quit,
                    console_ui::value::text_foreground_green | console_ui::value::text_foreground_intensity );
                for ( auto &sub_key : main_key_.sub_keys ) {
                    ui.add_back( std::format( "\n[{}]\n", sub_key.showed_name ) )
                      .add_back( " > 启用 ", option_setter{ sub_key, true }, option_button_color )
                      .add_back( " > 禁用 ", option_setter{ sub_key, false }, option_button_color );
                }
                ui.show();
                return console_ui::value::ui_return;
            }
            auto operator=( const option_shower & ) -> option_shower & = default;
            auto operator=( option_shower && ) -> option_shower &      = default;
            option_shower( option_node::main_key &_main_key )
              : main_key_{ _main_key }
            { }
            option_shower( const option_shower & ) = default;
            option_shower( option_shower && )      = default;
            ~option_shower()                       = default;
        };
        auto edit_()
        {
            auto ui{ console_ui{} };
            std::print( " -> 准备用户界面.\n" );
            ui
              .add_back( std::format(
                "                    [ 配  置 ]\n\n\n"
                " (i) 所有选项默认禁用, 每 {} 自动应用.\n"
                "     相关信息可参阅文档.\n",
                default_thread_sleep_time ) )
              .add_back( " < 返回 ", quit, console_ui::value::text_foreground_green | console_ui::value::text_foreground_intensity )
              .add_back( " > 同步配置 ", [ this ]( console_ui::func_args ) { return sync(); } )
              .add_back( " > 打开配置文件 ", [ this ]( console_ui::func_args ) { return open_file(); } )
              .add_back( "\n[选项]\n" );
            for ( auto &key : options.main_keys ) {
                ui.add_back( std::format( " > {} ", key.showed_name ), option_shower{ key } );
            }
            ui.show();
        }
      public:
        auto operator()( console_ui::func_args )
        {
            switch ( mod_data_ ) {
                case mod::load : load_( false ); break;
                case mod::edit : edit_(); break;
            }
            return console_ui::value::ui_return;
        }
        auto operator=( const config_op & ) -> config_op & = default;
        auto operator=( config_op && ) -> config_op &      = default;
        config_op( const mod _mod_data )
          : mod_data_{ _mod_data }
        { }
        config_op( const config_op & ) = default;
        config_op( config_op && )      = default;
        ~config_op()                   = default;
    };
    class rule_op final {
      public:
        enum class mod { crack, recovery };
      private:
        const mod mod_data_;
        const rule_node &rules_;
      public:
        auto operator()( console_ui::func_args )
        {
            std::print( "                 [ 破 解 / 恢 复 ]\n\n\n" );
            if ( rules_.execs.empty() && rules_.servs.empty() ) {
                std::print( " (i) 规则为空.\n\n" );
                wait( 3s );
                return console_ui::value::ui_return;
            }
            std::print( " -> 生成并执行操作系统命令.\n{}\n", string_type( WINDOW_WIDTH, '-' ) );
            switch ( mod_data_ ) {
                case mod::crack : {
                    if ( options[ "rule_op" ][ "hijack_execs" ].is_enabled ) {
                        for ( const auto &exec : rules_.execs ) {
                            system(
                              std::format(
                                R"(reg.exe add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution options\{}" /f /t reg_sz /v debugger /d "nul")",
                                exec )
                                .c_str() );
                        }
                    }
                    if ( options[ "rule_op" ][ "set_serv_startup_types" ].is_enabled ) {
                        for ( const auto &serv : rules_.servs ) {
                            system( std::format( R"(sc.exe config "{}" start= disabled)", serv ).c_str() );
                        }
                    }
                    for ( const auto &exec : rules_.execs ) {
                        system( std::format( R"(taskkill.exe /f /im "{}")", exec ).c_str() );
                    }
                    for ( const auto &serv : rules_.servs ) {
                        system( std::format( R"(net.exe stop "{}" /y)", serv ).c_str() );
                    }
                    break;
                }
                case mod::recovery : {
                    if ( options[ "rule_op" ][ "hijack_execs" ].is_enabled ) {
                        for ( const auto &exec : rules_.execs ) {
                            system(
                              std::format(
                                R"(reg.exe delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution options\{}" /f)",
                                exec )
                                .c_str() );
                        }
                    }
                    if ( options[ "rule_op" ][ "set_serv_startup_types" ].is_enabled ) {
                        for ( const auto &serv : rules_.servs ) {
                            system( std::format( R"(sc.exe config "{}" start= auto)", serv ).c_str() );
                        }
                    }
                    for ( const auto &serv : rules_.servs ) {
                        system( std::format( R"(net.exe start "{}")", serv ).c_str() );
                    }
                    break;
                }
            }
            return console_ui::value::ui_return;
        }
        auto operator=( const rule_op & ) -> rule_op & = default;
        auto operator=( rule_op && ) -> rule_op &      = default;
        rule_op( const mod _mod_data, const rule_node &_rule )
          : mod_data_{ _mod_data }
          , rules_{ _rule }
        { }
        rule_op( const rule_op & ) = default;
        rule_op( rule_op && )      = default;
        ~rule_op()                 = default;
    };
}
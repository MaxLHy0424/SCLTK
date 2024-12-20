#pragma once
#ifdef _NEXT_
# include <limits.h>
# include <fstream>
#endif
#include "console_ui.hpp"
#include "info.hpp"
#ifndef _NEXT_
inline struct {
    bool front_show_window, translucent_window, window_ctrls;
} args_data{};
inline bool args_error{};
#endif
namespace core {
#ifdef _NEXT_
    using size_type    = console_ui::size_type;
    using string_type  = console_ui::string_type;
    using wstring_type = std::wstring;
    template < typename _type_ >
    using type_wrapper = console_ui::type_wrapper< _type_ >;
    struct option_item final {
        struct sub_item final {
            const string_type label_name, showed_name;
            const bool is_relaunch_to_apply;
            bool is_enabled;
            auto operator=( const sub_item & ) -> sub_item & = default;
            auto operator=( sub_item && ) -> sub_item &      = default;
            sub_item(
              string_type _label_name, string_type _showed_name, const bool _is_relaunch_to_apply )
              : label_name{ std::move( _label_name ) }
              , showed_name{ std::move( _showed_name ) }
              , is_relaunch_to_apply{ _is_relaunch_to_apply }
              , is_enabled{ false }
            { }
            sub_item(
              string_type _label_name,
              string_type _showed_name,
              const bool _is_relaunch_to_apply,
              const bool _default_value )
              : label_name{ std::move( _label_name ) }
              , showed_name{ std::move( _showed_name ) }
              , is_relaunch_to_apply{ _is_relaunch_to_apply }
              , is_enabled{ _default_value }
            { }
            sub_item( const sub_item & ) = default;
            sub_item( sub_item && )      = default;
            ~sub_item()                  = default;
        };
        const string_type label_name, showed_name;
        std::vector< sub_item > sub_options;
        auto &operator[]( size_type _index )
        {
            return sub_options.at( _index );
        }
        auto operator=( const option_item & ) -> option_item & = default;
        auto operator=( option_item && ) -> option_item &      = default;
        option_item(
          string_type _label_name, string_type _showed_name, std::vector< sub_item > _sub_options )
          : label_name{ std::move( _label_name ) }
          , showed_name{ std::move( _showed_name ) }
          , sub_options{ std::move( _sub_options ) }
        { }
        option_item( const option_item & ) = default;
        option_item( option_item && )      = default;
        ~option_item()                     = default;
    };
    struct rule_item final {
        const string_type showed_name;
        std::deque< string_type > pe_files, svc_items;
        auto operator=( const rule_item & ) -> rule_item & = default;
        auto operator=( rule_item && ) -> rule_item &      = default;
        rule_item(
          string_type _showed_name, std::deque< string_type > _pe_files, std::deque< string_type > _svc_items )
          : showed_name{ std::move( _showed_name ) }
          , pe_files{ std::move( _pe_files ) }
          , svc_items{ std::move( _svc_items ) }
        { }
        rule_item( const rule_item & ) = default;
        rule_item( rule_item && )      = default;
        ~rule_item()                   = default;
    };
    namespace data {
        inline const string_type config_file_name{ "config.ini" };
        inline type_wrapper< option_item[] > options{
          {"operation",
           "破解/恢复",         { { "hijack_reg", "注册表劫持", false },
              { "set_svc_startup_type", "设置服务启动类型", false } }},
          {"window",
           "窗口显示",          { { "force_show", "置顶显示", true },
              { "disable_close_ctrl", "禁用关闭控件", true },
              { "translucency", "半透明化", true } }                  },
          {"other",     "其他", { { "fix_runtime_env", "修复运行环境", true } }             }
        };
        inline rule_item custom_rules{ "自定义", {}, {} };
        inline type_wrapper< const rule_item[] > builtin_rules{
          {"极域电子教室",
           { "StudentMain.exe", "DispcapHelper.exe", "VRCwPlayer.exe", "InstHelpApp.exe",
              "InstHelpApp64.exe", "TDOvrSet.exe", "GATESRV.exe", "ProcHelper64.exe",
              "MasterHelper.exe" },
           {
              "STUDSRV",
              "TDNetFilter",
            }                                         },
          {"联想智能云教室",
           { "vncviewer.exe", "tvnserver32.exe", "WfbsPnpInstall.exe", "WFBSMon.exe",
              "WFBSMlogon.exe", "WFBSSvrLogShow.exe", "ResetIp.exe", "FuncForWIN64.exe",
              "CertMgr.exe", "Fireware.exe", "BCDBootCopy.exe", "refreship.exe",
              "lenovoLockScreen.exe", "PortControl64.exe", "DesktopCheck.exe",
              "DeploymentManager.exe", "DeploymentAgent.exe", "XYNTService.exe" },
           { "BSAgentSvr", "tvnserver", "WFBSMlogon" }},
          {"红蜘蛛多媒体网络教室",
           { "rscheck.exe", "checkrs.exe", "REDAgent.exe", "PerformanceCheck.exe", "edpaper.exe",
              "Adapter.exe", "repview.exe", "FormatPaper.exe" },
           { "appcheck2", "checkapp2" }               }
        };
    }
    inline auto is_run_as_admin()
    {
        BOOL is_admin{};
        PSID admins_group{};
        SID_IDENTIFIER_AUTHORITY nt_authority{ SECURITY_NT_AUTHORITY };
        if ( AllocateAndInitializeSid(
               &nt_authority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &admins_group )
             == true )
        {
            CheckTokenMembership( nullptr, admins_group, &is_admin );
            FreeSid( admins_group );
        }
        return is_admin;
    }
    inline auto force_show_window()
    {
        using namespace std::chrono_literals;
        const HWND this_window{ GetConsoleWindow() };
        const DWORD foreground_id{ GetWindowThreadProcessId( this_window, nullptr ) },
          current_id{ GetCurrentThreadId() };
        while ( true ) {
            AttachThreadInput( current_id, foreground_id, TRUE );
            ShowWindow( this_window, SW_SHOWNORMAL );
            SetForegroundWindow( this_window );
            AttachThreadInput( current_id, foreground_id, FALSE );
            SetWindowPos( this_window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
            std::this_thread::sleep_for( 100ms );
        }
    }
    inline auto fix_runtime_env()
    {
        using namespace std::chrono_literals;
        type_wrapper< const string_type[] > hkcu_reg_dirs{
          R"(Software\Policies\Microsoft\Windows\System)",
          R"(Software\Microsoft\Windows\CurrentVersion\Policies\System)",
          R"(Software\Microsoft\Windows\CurrentVersion\Policies\Explorer)" },
          pe_files{
            "mode.com", "chcp.com", "ntsd.exe",    "taskkill.exe", "sc.exe",      "net.exe",
            "reg.exe",  "cmd.exe",  "taskmgr.exe", "perfmon.exe",  "regedit.exe", "mmc.exe" };
        while ( true ) {
            for ( const auto &reg_dir : hkcu_reg_dirs ) {
                RegDeleteTreeA( HKEY_CURRENT_USER, reg_dir.c_str() );
            }
            for ( const auto &pe : pe_files ) {
                RegDeleteTreeA(
                  HKEY_LOCAL_MACHINE,
                  std::format(
                    R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\{})",
                    pe )
                    .c_str() );
            }
            std::this_thread::sleep_for( 1000ms );
        }
    }
    inline auto relaunch_as_admin( console_ui::func_args )
    {
        wstring_type file_path( MAX_PATH, L'\0' );
        GetModuleFileNameW( nullptr, file_path.data(), MAX_PATH );
        ShellExecuteW( nullptr, L"runas", file_path.data(), nullptr, nullptr, SW_SHOWNORMAL );
        return CONSOLE_UI_TERMINATE;
    }
    inline auto quit( console_ui::func_args )
    {
        return CONSOLE_UI_TERMINATE;
    }
    inline auto info( console_ui::func_args )
    {
        std::print( "-> 初始化用户界面.\n" );
        auto visit_repo_webpage{ []( console_ui::func_args )
        {
            ShellExecuteA( nullptr, "open", INFO_REPO_URL, nullptr, nullptr, SW_SHOWNORMAL );
            return CONSOLE_UI_REVERT;
        } };
        console_ui ui;
        ui.add_back( "                    [ 信  息 ]\n\n" )
          .add_back( " < 返回 ", quit, CONSOLE_TEXT_FOREGROUND_GREEN | CONSOLE_TEXT_FOREGROUND_INTENSITY )
          .add_back( "\n[名称]\n\n " INFO_NAME "\n\n[版本]\n\n " INFO_VERSION )
          .add_back( "\n[仓库]\n" )
          .add_back(
            " " INFO_REPO_URL " ",
            visit_repo_webpage,
            CONSOLE_TEXT_DEFAULT | CONSOLE_TEXT_COMMON_LVB_UNDERSCORE )
          .add_back( "\n[许可证]\n\n " INFO_LICENSE "\n\n (C) 2023 - present " INFO_DEVELOPER "." )
          .show();
        return CONSOLE_UI_REVERT;
    }
    inline auto toolkit( console_ui::func_args )
    {
        std::print( "-> 初始化用户界面.\n" );
        auto launch_cmd{ []( console_ui::func_args _args )
        {
            _args.parent_ui.lock( false, false );
            _args.parent_ui.set_console(
              CODE_PAGE,
              WINDOW_TITLE,
              120,
              30,
              false,
              false,
              data::options[ 1 ][ 1 ].is_enabled ? false : true,
              data::options[ 1 ][ 2 ].is_enabled ? 230 : 255 );
            SetConsoleScreenBufferSize( GetStdHandle( STD_OUTPUT_HANDLE ), { 125, SHRT_MAX - 1 } );
            system( "cmd.exe" );
            _args.parent_ui.set_console(
              CODE_PAGE,
              WINDOW_TITLE,
              WINDOW_WIDTH,
              WINDOW_HEIGHT,
              true,
              false,
              data::options[ 1 ][ 1 ].is_enabled ? false : true,
              data::options[ 1 ][ 2 ].is_enabled ? 230 : 255 );
            return CONSOLE_UI_REVERT;
        } };
        class cmd_executor final {
          private:
            const string_type cmd_;
          public:
            auto operator()( console_ui::func_args ) const
            {
                std::print( "-> 执行 Windows 命令.\n{}\n", string_type( WINDOW_WIDTH, '-' ) );
                system( cmd_.c_str() );
                return CONSOLE_UI_REVERT;
            }
            auto operator=( const cmd_executor & ) -> cmd_executor & = default;
            auto operator=( cmd_executor && ) -> cmd_executor &      = default;
            cmd_executor( string_type _cmd )
              : cmd_{ std::move( _cmd ) }
            { }
            cmd_executor( const cmd_executor & ) = default;
            cmd_executor( cmd_executor && )      = default;
            ~cmd_executor()                      = default;
        };
        type_wrapper< string_type[][ 2 ] > common_ops{
          {"重启资源管理器",
           R"(taskkill.exe /f /im explorer.exe && timeout /t 3 /nobreak && start C:\Windows\explorer.exe)"           },
          {"恢复 Google Chrome 离线游戏",
           R"(reg.exe delete "HKLM\SOFTWARE\Policies\Google\Chrome" /f /v AllowDinosaurEasterEgg)"                   },
          {"恢复 Microsoft Edge 离线游戏",
           R"(reg.exe delete "HKLM\SOFTWARE\Policies\Microsoft\Edge" /f /v AllowSurfGame)"                           },
          {"恢复 USB 设备访问",
           R"(reg.exe add "HKLM\SYSTEM\CurrentControlSet\Services\USBSTOR" /f /t reg_dword /v Start /d 3)"           },
          {"重置 Ctrl + Alt + Del 菜单",
           R"(reg.exe delete "HKCU\Software\Microsoft\Windows\CurrentVersion\Policies\System" /f /v DisableChangePassword)"
            R"( && reg.exe delete "HKCU\Software\Microsoft\Windows\CurrentVersion\Policies\System" /f /v DisableLockWorkstation)"
            R"( && reg.exe delete "HKCU\Software\Microsoft\Windows\CurrentVersion\Policies\System" /f /v DisableTaskMgr)"
            R"( && reg.exe delete "HKCU\Software\Microsoft\Windows\CurrentVersion\Policies\System" /f /v DisableSwitchUserOption)"
            R"( && reg.exe delete "HKCU\Software\Microsoft\Windows\CurrentVersion\Policies\Explorer" /f /v NoLogoff)"},
        };
        console_ui ui;
        ui.add_back( "                   [ 工 具 箱 ]\n\n" )
          .add_back( " < 返回 ", quit, CONSOLE_TEXT_FOREGROUND_GREEN | CONSOLE_TEXT_FOREGROUND_INTENSITY )
          .add_back( " > 命令提示符 ", launch_cmd )
          .add_back( "\n[常用操作]\n" );
        for ( const auto &op : common_ops ) {
            ui.add_back(
              std::format( " > {} ", std::move( op[ 0 ] ) ), cmd_executor{ std::move( op[ 1 ] ) } );
        }
        ui.show();
        return CONSOLE_UI_REVERT;
    }
    class config_op final {
      private:
        const char mode_;
        auto load_( const bool _is_reload ) const
        {
            std::ifstream config_file{ data::config_file_name, std::ios::in };
            if ( !config_file.is_open() ) {
                config_file.close();
                return;
            }
            std::print( "-> 加载配置文件.\n" );
            data::custom_rules.pe_files.clear();
            data::custom_rules.svc_items.clear();
            string_type line;
            enum class config_label {
                unknown,
                options,
                customized_rules_pe_files,
                customized_rules_svc_items
            };
            config_label label{ config_label::unknown };
            while ( std::getline( config_file, line ) ) {
                if ( line.empty() || line.front() == '#' ) {
                    continue;
                }
                if ( line == "[ options ]" ) {
                    label = config_label::options;
                    continue;
                } else if ( line == "[ customized_rules_pe_files ]" ) {
                    label = config_label::customized_rules_pe_files;
                    continue;
                } else if ( line == "[ customized_rules_svc_items ]" ) {
                    label = config_label::customized_rules_svc_items;
                    continue;
                } else if ( line.front() == '[' && line.back() == ']' ) {
                    label = config_label::unknown;
                    continue;
                }
                switch ( label ) {
                    case config_label::unknown : break;
                    case config_label::options : {
                        if ( _is_reload ) {
                            continue;
                        }
                        for ( auto &item_class : data::options ) {
                            for ( auto &item_option : item_class.sub_options ) {
                                if ( line
                                     == std::format(
                                       "{} :: {}", item_class.label_name, item_option.label_name ) )
                                {
                                    item_option.is_enabled = true;
                                }
                            }
                        }
                        break;
                    }
                    case config_label::customized_rules_pe_files :
                        data::custom_rules.pe_files.emplace_back( std::move( line ) );
                        break;
                    case config_label::customized_rules_svc_items :
                        data::custom_rules.svc_items.emplace_back( std::move( line ) );
                        break;
                }
            }
            config_file.close();
            return;
        }
        auto edit_() const
        {
            std::print( "-> 初始化用户界面.\n" );
            constexpr auto color_of_option_buttons{
              CONSOLE_TEXT_FOREGROUND_RED | CONSOLE_TEXT_FOREGROUND_GREEN };
            auto sync_config{ [ this ]( console_ui::func_args )
            {
                load_( true );
                std::print( "-> 保存更改.\n" );
                string_type config_text;
                config_text.append( "[ option ]\n" );
                for ( const auto &option : data::options ) {
                    for ( const auto &sub_option : option.sub_options ) {
                        if ( sub_option.is_enabled ) {
                            config_text.append( std::format(
                              "{} :: {}\n", option.label_name, sub_option.label_name ) );
                        }
                    }
                }
                config_text.append( "[ customized_rules_pe_files ]\n" );
                for ( const auto &pe : data::custom_rules.pe_files ) {
                    config_text.append( pe ).push_back( '\n' );
                }
                config_text.append( "[ customized_rules_svc_items ]\n" );
                for ( const auto &svc : data::custom_rules.svc_items ) {
                    config_text.append( svc ).push_back( '\n' );
                }
                std::ofstream config_file{ data::config_file_name, std::ios::out | std::ios::trunc };
                config_file.write( config_text.c_str(), config_text.size() );
                config_file.close();
                return CONSOLE_UI_TERMINATE;
            } };
            auto open_config_file{ []( console_ui::func_args )
            {
                if ( std::ifstream{ data::config_file_name, std::ios::in }.is_open() ) {
                    std::print( "-> 打开配置文件.\n" );
                    ShellExecuteA(
                      nullptr, "open", data::config_file_name.c_str(), nullptr, nullptr, SW_SHOWNORMAL );
                    return CONSOLE_UI_REVERT;
                }
                using namespace std::chrono_literals;
                std::print(
                  "                    [ 配  置 ]\n\n\n"
                  " (i) 无法读取配置文件.\n\n" );
                for ( unsigned short i{ 3 }; i > 0; --i ) {
                    std::print( " {}s 后返回.\r", i );
                    std::this_thread::sleep_for( 1000ms );
                }
                return CONSOLE_UI_REVERT;
            } };
            class option_setter {
              private:
                option_item::sub_item &sub_option_;
                const bool sub_option_value_;
              public:
                auto operator()( console_ui::func_args ) const
                {
                    sub_option_.is_enabled = sub_option_value_;
                    return CONSOLE_UI_REVERT;
                }
                auto operator=( const option_setter & ) -> option_setter & = delete;
                auto operator=( option_setter && ) -> option_setter &      = delete;
                option_setter( option_item::sub_item &_sub_option, const bool _sub_option_value )
                  : sub_option_{ _sub_option }
                  , sub_option_value_{ _sub_option_value }
                { }
                option_setter( const option_setter & ) = default;
                option_setter( option_setter && )      = default;
                ~option_setter()                       = default;
            };
            class option_shower final {
              private:
                option_item &option_;
              public:
                auto operator()( console_ui::func_args ) const
                {
                    console_ui ui;
                    ui.add_back( "                    [ 配  置 ]\n\n" )
                      .add_back(
                        std::format( " < 折叠 {}", option_.showed_name ),
                        quit,
                        CONSOLE_TEXT_FOREGROUND_GREEN | CONSOLE_TEXT_FOREGROUND_INTENSITY );
                    for ( auto &sub_option : option_.sub_options ) {
                        ui
                          .add_back( std::format(
                            "\n[({}生效) {}]\n",
                            sub_option.is_relaunch_to_apply ? "下次启动时" : "立即",
                            sub_option.showed_name ) )
                          .add_back(
                            " > 启用 ", option_setter{ sub_option, true }, color_of_option_buttons )
                          .add_back(
                            " > 禁用 ", option_setter{ sub_option, false }, color_of_option_buttons );
                    }
                    ui.show();
                    return CONSOLE_UI_REVERT;
                }
                auto operator=( const option_shower & ) -> option_shower & = delete;
                auto operator=( option_shower && ) -> option_shower &      = delete;
                option_shower( option_item &_option )
                  : option_{ _option }
                { }
                option_shower( const option_shower & ) = default;
                option_shower( option_shower && )      = default;
                ~option_shower()                       = default;
            };
            console_ui ui;
            ui.add_back(
                "                    [ 配  置 ]\n\n\n"
                " (i) 所有选项默认禁用.\n"
                "     相关信息可参阅文档.\n" )
              .add_back(
                " < 同步配置并返回 ",
                sync_config,
                CONSOLE_TEXT_FOREGROUND_GREEN | CONSOLE_TEXT_FOREGROUND_INTENSITY )
              .add_back( " > 打开配置文件 ", open_config_file )
              .add_back( "\n[选项分类]\n" );
            for ( auto &option : data::options ) {
                ui.add_back(
                  std::format( " > {}", option.showed_name ),
                  option_shower{ option },
                  color_of_option_buttons );
            }
            ui.show();
            return CONSOLE_UI_REVERT;
        }
      public:
        auto operator()( console_ui::func_args ) const
        {
            switch ( mode_ ) {
                case 'r' : load_( false ); break;
                case 'w' : edit_(); break;
            }
            return CONSOLE_UI_REVERT;
        }
        auto operator=( const config_op & ) -> config_op & = default;
        auto operator=( config_op && ) -> config_op &      = default;
        config_op( const char _mode )
          : mode_{ _mode }
        { }
        config_op( const config_op & ) = default;
        config_op( config_op && )      = default;
        ~config_op()                   = default;
    };
    class rule_op final {
      private:
        const char mode_;
        const rule_item &rules_;
      public:
        auto operator()( console_ui::func_args ) const
        {
            std::print( "                 [ 破 解 / 恢 复 ]\n\n\n" );
            if ( rules_.pe_files.empty() && rules_.svc_items.empty() ) {
                using namespace std::chrono_literals;
                std::print( " (i) 规则为空.\n\n" );
                for ( unsigned short i{ 3 }; i > 0; --i ) {
                    std::print( " {}s 后返回.\r", i );
                    std::this_thread::sleep_for( 1000ms );
                }
                return CONSOLE_UI_REVERT;
            }
            std::print( "-> 生成并执行 Windows 命令.\n{}\n", string_type( WINDOW_WIDTH, '-' ) );
            switch ( mode_ ) {
                case 'c' : {
                    if ( data::options[ 0 ][ 0 ].is_enabled ) {
                        for ( const auto &item : rules_.pe_files ) {
                            system(
                              std::format(
                                R"(reg.exe add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution options\{}" /f /t reg_sz /v debugger /d "nul")",
                                item )
                                .c_str() );
                        }
                    }
                    if ( data::options[ 0 ][ 1 ].is_enabled ) {
                        for ( const auto &svc : rules_.svc_items ) {
                            system( std::format( "sc.exe config {} start= disabled", svc ).c_str() );
                        }
                    }
                    for ( const auto &pe : rules_.pe_files ) {
                        system( std::format( R"(taskkill.exe /f /im "{}")", pe ).c_str() );
                    }
                    for ( const auto &svc : rules_.svc_items ) {
                        system( std::format( R"(net.exe stop "{}" /y)", svc ).c_str() );
                    }
                    break;
                }
                case 'r' : {
                    if ( data::options[ 0 ][ 0 ].is_enabled ) {
                        for ( const auto &pe : rules_.pe_files ) {
                            system(
                              std::format(
                                R"(reg.exe delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution options\{}" /f)",
                                pe )
                                .c_str() );
                        }
                    }
                    if ( data::options[ 0 ][ 1 ].is_enabled ) {
                        for ( const auto &svc : rules_.svc_items ) {
                            system( std::format( "sc.exe config {} start= auto", svc ).c_str() );
                        }
                    }
                    for ( const auto &svc : rules_.svc_items ) {
                        system( std::format( R"(net.exe start "{}")", svc ).c_str() );
                    }
                    break;
                }
            }
            return CONSOLE_UI_REVERT;
        }
        auto operator=( const rule_op & ) -> rule_op & = default;
        auto operator=( rule_op && ) -> rule_op &      = default;
        rule_op( const char _mode, const rule_item &_rule )
          : mode_{ _mode }
          , rules_{ _rule }
        { }
        rule_op( const rule_op & ) = default;
        rule_op( rule_op && )      = default;
        ~rule_op()                 = default;
    };
#else
    struct rule_item final {
        const std::vector< const char * > exe, svc;
    };
    inline struct {
        const rule_item mythware, lenovo;
    } rule{
      {{ "StudentMain.exe", "DispcapHelper.exe", "VRCwPlayer.exe", "InstHelpApp.exe",
          "InstHelpApp64.exe", "TDOvrSet.exe", "GATESRV.exe", "ProcHelper64.exe",
          "MasterHelper.exe" },
       {
          "STUDSRV",
          "TDNetFilter",
        }                                         },
      {{ "vncviewer.exe", "tvnserver32.exe", "WfbsPnpInstall.exe", "WFBSMon.exe", "WFBSMlogon.exe",
          "WFBSSvrLogShow.exe", "ResetIp.exe", "FuncForWIN64.exe", "CertMgr.exe", "Fireware.exe",
          "BCDBootCopy.exe", "refreship.exe", "lenovoLockScreen.exe", "PortControl64.exe",
          "DesktopCheck.exe", "DeploymentManager.exe", "DeploymentAgent.exe", "XYNTService.exe" },
       { "BSAgentSvr", "tvnserver", "WFBSMlogon" }}
    };
    inline auto is_run_as_admin()
    {
        BOOL is_admin{};
        PSID admins_group{};
        SID_IDENTIFIER_AUTHORITY nt_authority{ SECURITY_NT_AUTHORITY };
        if ( AllocateAndInitializeSid(
               &nt_authority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &admins_group ) )
        {
            CheckTokenMembership( nullptr, admins_group, &is_admin );
            FreeSid( admins_group );
        }
        return is_admin;
    }
    inline auto quit( console_ui::func_args )
    {
        return CONSOLE_UI_TERMINATE;
    }
    inline auto init()
    {
        system( "chcp.com 936 > nul" );
        SetConsoleTitleA( "CRCSN" );
        SetWindowLongPtrA(
          GetConsoleWindow(),
          GWL_STYLE,
          args_data.window_ctrls
            ? GetWindowLongPtrA( GetConsoleWindow(), GWL_STYLE ) | WS_SIZEBOX | WS_MAXIMIZEBOX | WS_MINIMIZEBOX
            : GetWindowLongPtrA( GetConsoleWindow(), GWL_STYLE ) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX
                & ~WS_MINIMIZEBOX );
        system( "mode.com con cols=50 lines=25" );
        SetLayeredWindowAttributes(
          GetConsoleWindow(), 0, args_data.translucent_window ? 230 : 255, LWA_ALPHA );
    }
    inline auto front_show_window()
    {
        using namespace std::chrono_literals;
        const HWND this_window{ GetConsoleWindow() };
        const DWORD foreground_id{ GetWindowThreadProcessId( this_window, nullptr ) },
          current_id{ GetCurrentThreadId() };
        while ( true ) {
            AttachThreadInput( current_id, foreground_id, TRUE );
            ShowWindow( this_window, SW_SHOWNORMAL );
            SetForegroundWindow( this_window );
            AttachThreadInput( current_id, foreground_id, FALSE );
            SetWindowPos( this_window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
            std::this_thread::sleep_for( 100ms );
        }
    }
    inline auto info( console_ui::func_args )
    {
        console_ui ui;
        ui.add_back( "                    [ 信  息 ]\n\n" )
          .add_back( " < 返回 ", quit, CONSOLE_TEXT_FOREGROUND_GREEN | CONSOLE_TEXT_FOREGROUND_INTENSITY )
          .add_back(
            "\n[名称]\n\n " INFO_NAME "\n\n[版本]\n\n " INFO_VERSION "\n\n[仓库]\n\n " INFO_REPO_URL
            "\n\n[许可证]\n\n " INFO_LICENSE "\n\n (C) 2023 - present " INFO_DEVELOPER "." )
          .show();
        return CONSOLE_UI_REVERT;
    }
    class rule_op final {
      private:
        const char mode_;
        const rule_item &rule_;
      public:
        auto operator()( console_ui::func_args ) const
        {
            std::puts( "                 [ 破 解 / 恢 复 ]\n\n" );
            if ( !is_run_as_admin() ) {
                using namespace std::chrono_literals;
                std::puts( "\n (i) 需要管理员权限.\n" );
                for ( unsigned short i{ 3 }; i > 0; --i ) {
                    std::printf( " %hus 后返回.\r", i );
                    std::this_thread::sleep_for( 1000ms );
                }
                return CONSOLE_UI_REVERT;
            }
            std::puts( "-> 生成命令." );
            std::string cmd;
            switch ( mode_ ) {
                case 'c' : {
                    for ( const auto &item : rule_.exe ) {
                        cmd
                          .append(
                            R"(reg.exe add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution options\)" )
                          .append( item )
                          .append( R"(" /f /t reg_sz /v debugger /d "nul" && taskkill.exe /f /im ")" )
                          .append( item )
                          .append( "\" & " );
                    }
                    for ( const auto &item : rule_.svc ) {
                        cmd.append( "net.exe stop \"" ).append( item ).append( "\" /y & " );
                    }
                    break;
                }
                case 'r' : {
                    for ( const auto &item : rule_.exe ) {
                        cmd
                          .append(
                            R"(reg.exe delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution options\)" )
                          .append( item )
                          .append( "\" /f & " );
                    }
                    for ( const auto &item : rule_.svc ) {
                        cmd.append( "net.exe start \"" ).append( item ).append( "\" & " );
                    }
                    break;
                }
            }
            std::printf( "-> 执行 Windows 命令.\n%s\n", std::string( 50, '-' ).c_str() );
            system( cmd.c_str() );
            std::printf( "%s\n-> 释放内存.", std::string( 50, '-' ).c_str() );
            return CONSOLE_UI_REVERT;
        }
        rule_op( const char _mode, const rule_item &_rule )
          : mode_{ _mode }
          , rule_{ _rule }
        { }
        ~rule_op() { }
    };
#endif
}
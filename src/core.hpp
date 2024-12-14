#pragma once
#ifdef _THE_NEXT_MAJOR_UPDATE_
# include <limits.h>
# include <fstream>
#endif
#include "console_ui.hpp"
#include "info.hpp"
#ifndef _THE_NEXT_MAJOR_UPDATE_
inline struct {
    bool front_show_window, translucent_window, window_ctrls;
} args_data{};
inline bool args_error{};
#endif
namespace core {
#ifdef _THE_NEXT_MAJOR_UPDATE_
    using c_str_type   = console_ui::c_str_type< console_ui::default_char_type >;
    using nullptr_type = console_ui::nullptr_type;
    struct config_data_node final {
        const c_str_type name;
        bool is_enabled;
        auto &operator=( const config_data_node & ) = delete;
        auto &operator=( config_data_node && )      = delete;
        config_data_node( nullptr_type, bool )      = delete;
        config_data_node( const c_str_type _name, bool _is_enabled )
          : name{ _name }
          , is_enabled{ _is_enabled }
        { }
        config_data_node( const config_data_node & ) = delete;
        config_data_node( config_data_node && )      = delete;
        ~config_data_node()                          = default;
    };
    struct rule_data_node final {
        std::deque< std::string > exe, svc;
        auto &operator=( const rule_data_node _src )
        {
            exe = _src.exe;
            svc = _src.svc;
            return *this;
        }
        auto &operator=( rule_data_node &_src )
        {
            exe = std::move( _src.exe );
            svc = std::move( _src.svc );
            return *this;
        }
        rule_data_node()
          : exe{}
          , svc{}
        { }
        rule_data_node( std::deque< std::string > _exe, std::deque< std::string > _svc )
          : exe{ std::move( _exe ) }
          , svc{ std::move( _svc ) }
        { }
        rule_data_node( const rule_data_node & ) = delete;
        rule_data_node( rule_data_node && )      = delete;
        ~rule_data_node()                        = default;
    };
    namespace data {
        inline const c_str_type config_file_name{ "config.ini" };
        inline object_type< config_data_node[] > config{
          {"enhanced_op",     false},
          {"enhanced_window", false},
          {"repair_env",      false}
        };
        inline struct {
            const rule_data_node mythware, lenovo;
            rule_data_node customized;
        } rule{
          { { "StudentMain.exe",
              "DispcapHelper.exe",
              "VRCwPlayer.exe",
              "InstHelpApp.exe",
              "InstHelpApp64.exe",
              "TDOvrSet.exe",
              "GATESRV.exe",
              "ProcHelper64.exe",
              "MasterHelper.exe" },
           {
              "STUDSRV",
              "TDNetFilter",
            } },
          { { "vncviewer.exe",
              "tvnserver32.exe",
              "WfbsPnpInstall.exe",
              "WFBSMon.exe",
              "WFBSMlogon.exe",
              "WFBSSvrLogShow.exe",
              "ResetIp.exe",
              "FuncForWIN64.exe",
              "CertMgr.exe",
              "Fireware.exe",
              "BCDBootCopy.exe",
              "refreship.exe",
              "lenovoLockScreen.exe",
              "PortControl64.exe",
              "DesktopCheck.exe",
              "DeploymentManager.exe",
              "DeploymentAgent.exe",
              "XYNTService.exe" },
           { "BSAgentSvr", "tvnserver", "WFBSMlogon" } },
          {}
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
    inline auto repair_env()
    {
        using namespace std::chrono_literals;
        object_type< const c_str_type[] > hkcu_reg_dir{
          R"(Software\Policies\Microsoft\Windows\System)",
          R"(Software\Microsoft\Windows\CurrentVersion\Policies\System)",
          R"(Software\Microsoft\Windows\CurrentVersion\Policies\Explorer)" },
          exe{
            "mode.com",
            "chcp.com",
            "reg.exe",
            "sc.exe",
            "taskkill.exe",
            "net.exe",
            "cmd.exe",
            "taskmgr.exe",
            "perfmon.exe",
            "regedit.exe",
            "mmc.exe" };
        while ( true ) {
            for ( const auto &item : hkcu_reg_dir ) {
                RegDeleteTreeA( HKEY_CURRENT_USER, item );
            }
            for ( const auto &item : exe ) {
                RegDeleteTreeA(
                  HKEY_LOCAL_MACHINE,
                  std::format(
                    R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\{})",
                    item )
                    .c_str() );
            }
            std::this_thread::sleep_for( 1000ms );
        }
    }
    inline auto relaunch_as_admin( console_ui::func_args )
    {
        object_type< char[ MAX_PATH ] > path{};
        GetModuleFileNameA( nullptr, path, MAX_PATH );
        ShellExecuteA( nullptr, "runas", path, nullptr, nullptr, SW_SHOWNORMAL );
        return CONSOLE_UI_TERMINATE;
    }
    inline auto quit( console_ui::func_args )
    {
        return CONSOLE_UI_TERMINATE;
    }
    inline auto info( console_ui::func_args )
    {
        std::print( ":: 初始化用户界面.\n" );
        auto view_repo_webpage{ []( console_ui::func_args )
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
            view_repo_webpage,
            CONSOLE_TEXT_FOREGROUND_BLUE | CONSOLE_TEXT_FOREGROUND_GREEN
              | CONSOLE_TEXT_COMMON_LVB_UNDERSCORE )
          .add_back( "\n[许可证]\n\n " INFO_LICENSE "\n\n (C) 2023 - present " INFO_DEVELOPER "." )
          .show();
        return CONSOLE_UI_REVERT;
    }
    inline auto toolkit( console_ui::func_args )
    {
        std::print( ":: 初始化用户界面.\n" );
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
              core::data::config[ 1 ].is_enabled == false ? true : false,
              core::data::config[ 1 ].is_enabled == true ? 230 : 255 );
            SetConsoleScreenBufferSize( GetStdHandle( STD_OUTPUT_HANDLE ), { 120, SHRT_MAX - 1 } );
            system( "cmd.exe" );
            _args.parent_ui.set_console(
              CODE_PAGE,
              WINDOW_TITLE,
              WINDOW_WIDTH,
              WINDOW_HEIGHT,
              true,
              false,
              core::data::config[ 1 ].is_enabled == false ? true : false,
              core::data::config[ 1 ].is_enabled == true ? 230 : 255 );
            return CONSOLE_UI_REVERT;
        } };
        class exec_cmd final {
          private:
            const c_str_type cmd_;
          public:
            auto operator()( console_ui::func_args )
            {
                std::print( ":: 执行命令.\n{}\n", std::string( WINDOW_WIDTH, '-' ) );
                system( cmd_ );
                return CONSOLE_UI_REVERT;
            }
            auto &operator=( const exec_cmd & ) = delete;
            auto &operator=( exec_cmd && )      = delete;
            exec_cmd( nullptr_type )            = delete;
            exec_cmd( const c_str_type _cmd )
              : cmd_{ _cmd }
            { }
            exec_cmd( const exec_cmd & ) = default;
            exec_cmd( exec_cmd && )      = default;
            ~exec_cmd()                  = default;
        };
        object_type< const c_str_type[] > cmd{
          R"(taskkill.exe /f /im explorer.exe && timeout /t 3 /nobreak && start C:\Windows\explorer.exe)",
          R"(reg.exe delete "HKLM\SOFTWARE\Policies\Google\Chrome" /f /v AllowDinosaurEasterEgg)",
          R"(reg.exe delete "HKLM\SOFTWARE\Policies\Microsoft\Edge" /f /v AllowSurfGame)",
          R"(reg.exe add "HKLM\SYSTEM\CurrentControlSet\Services\USBSTOR" /f /t reg_dword /v Start /d 3)" };
        console_ui ui;
        ui.add_back( "                   [ 工 具 箱 ]\n\n" )
          .add_back( " < 返回 ", quit, CONSOLE_TEXT_FOREGROUND_GREEN | CONSOLE_TEXT_FOREGROUND_INTENSITY )
          .add_back( " > 命令提示符 ", launch_cmd )
          .add_back( "\n[快捷操作]\n" )
          .add_back( " > 重启资源管理器 ", exec_cmd{ cmd[ 0 ] } )
          .add_back( " > 恢复 Google Chrome 离线游戏 ", exec_cmd{ cmd[ 1 ] } )
          .add_back( " > 恢复 Microsoft Edge 离线游戏 ", exec_cmd{ cmd[ 2 ] } )
          .add_back( " > 恢复 USB 设备访问 ", exec_cmd{ cmd[ 3 ] } )
          .show();
        return CONSOLE_UI_REVERT;
    }
    class config_op final {
      private:
        const char mode_;
        auto load_( bool _is_reload ) const
        {
            std::ifstream config_file{ data::config_file_name, std::ios::in };
            if ( !config_file.is_open() ) {
                config_file.close();
                return;
            }
            std::print( ":: 加载配置文件.\n" );
            data::rule.customized.exe.clear();
            data::rule.customized.svc.clear();
            std::string line;
            enum class config_tag { unknown, option, rule_exe, rule_svc };
            config_tag tag{ config_tag::unknown };
            while ( std::getline( config_file, line ) ) {
                if ( line.empty() || line.front() == '#' ) {
                    continue;
                }
                if ( line == "[option]" ) {
                    tag = config_tag::option;
                    continue;
                } else if ( line == "[rule_exe]" ) {
                    tag = config_tag::rule_exe;
                    continue;
                } else if ( line == "[rule_svc]" ) {
                    tag = config_tag::rule_svc;
                    continue;
                } else if ( line.front() == '[' && line.back() == ']' ) {
                    tag = config_tag::unknown;
                    continue;
                }
                switch ( tag ) {
                    case config_tag::unknown : break;
                    case config_tag::option : {
                        if ( _is_reload ) {
                            continue;
                        }
                        for ( auto &item : data::config ) {
                            if ( line == item.name ) {
                                item.is_enabled = true;
                            }
                        }
                        break;
                    }
                    case config_tag::rule_exe :
                        data::rule.customized.exe.emplace_back( std::move( line ) );
                        break;
                    case config_tag::rule_svc :
                        data::rule.customized.svc.emplace_back( std::move( line ) );
                        break;
                }
            }
            config_file.close();
            return;
        }
        auto edit_() const
        {
            std::print( ":: 初始化用户界面.\n" );
            auto sync{ [ this ]( console_ui::func_args )
            {
                load_( true );
                std::print( ":: 保存更改.\n" );
                std::string text;
                text.append( "[option]\n" );
                for ( const auto &item : data::config ) {
                    if ( item.is_enabled == true ) {
                        text.append( item.name ).push_back( '\n' );
                    }
                }
                text.append( "[rule_exe]\n" );
                for ( const auto &item : data::rule.customized.exe ) {
                    text.append( item ).push_back( '\n' );
                }
                text.append( "[rule_svc]\n" );
                for ( const auto &item : data::rule.customized.svc ) {
                    text.append( item ).push_back( '\n' );
                }
                std::ofstream config_file{ data::config_file_name, std::ios::out | std::ios::trunc };
                config_file.write( text.c_str(), text.size() );
                config_file.close();
                return CONSOLE_UI_TERMINATE;
            } };
            auto open_config_file{ []( console_ui::func_args )
            {
                if ( std::ifstream{ data::config_file_name, std::ios::in }.is_open() ) {
                    std::print( ":: 打开配置文件.\n" );
                    ShellExecuteA(
                      nullptr, "open", data::config_file_name, nullptr, nullptr, SW_SHOWNORMAL );
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
            constexpr auto option_button_color{
              CONSOLE_TEXT_FOREGROUND_RED | CONSOLE_TEXT_FOREGROUND_GREEN };
            console_ui ui;
            ui.add_back(
                "                    [ 配  置 ]\n\n\n"
                " (i) 相关信息可参阅文档.\n" )
              .add_back(
                " < 同步配置并返回 ", sync, CONSOLE_TEXT_FOREGROUND_GREEN | CONSOLE_TEXT_FOREGROUND_INTENSITY )
              .add_back( " > 打开配置文件 ", open_config_file )
              .add_back( "\n[增强操作]\n" )
              .add_back(
                " > 启用 ",
                []( console_ui::func_args )
            {
                data::config[ 0 ].is_enabled = true;
                return CONSOLE_UI_REVERT;
            },
                option_button_color )
              .add_back(
                " > 禁用 (默认) ",
                []( console_ui::func_args )
            {
                data::config[ 0 ].is_enabled = false;
                return CONSOLE_UI_REVERT;
            },
                option_button_color )
              .add_back( "\n[增强窗口 (下次启动时生效)]\n" )
              .add_back(
                " > 启用 ",
                []( console_ui::func_args )
            {
                data::config[ 1 ].is_enabled = true;
                return CONSOLE_UI_REVERT;
            },
                option_button_color )
              .add_back(
                " > 禁用 (默认) ",
                []( console_ui::func_args )
            {
                data::config[ 1 ].is_enabled = false;
                return CONSOLE_UI_REVERT;
            },
                option_button_color )
              .add_back( "\n[环境修复 (下次启动时生效)]\n" )
              .add_back(
                " > 启用 ",
                []( console_ui::func_args )
            {
                data::config[ 2 ].is_enabled = true;
                return CONSOLE_UI_REVERT;
            },
                option_button_color )
              .add_back(
                " > 禁用 (默认) ",
                []( console_ui::func_args )
            {
                data::config[ 2 ].is_enabled = false;
                return CONSOLE_UI_REVERT;
            },
                option_button_color )
              .show();
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
        auto &operator=( const config_op & ) = delete;
        auto &operator=( config_op && )      = delete;
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
        const rule_data_node &rule_data_;
      public:
        auto operator()( console_ui::func_args ) const
        {
            std::print( "                 [ 破 解 / 恢 复 ]\n\n\n" );
            if ( rule_data_.exe.empty() && rule_data_.svc.empty() ) {
                using namespace std::chrono_literals;
                std::print( " (i) 规则为空.\n\n" );
                for ( unsigned short i{ 3 }; i > 0; --i ) {
                    std::print( " {}s 后返回.\r", i );
                    std::this_thread::sleep_for( 1000ms );
                }
                return CONSOLE_UI_REVERT;
            }
            std::print( ":: 生成并执行命令.\n{}\n", std::string( WINDOW_WIDTH, '-' ) );
            switch ( mode_ ) {
                case 'c' : {
                    if ( data::config[ 0 ].is_enabled == true ) {
                        for ( const auto &item : rule_data_.exe ) {
                            system(
                              std::format(
                                R"(reg.exe add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution options\{}" /f /t reg_sz /v debugger /d "nul")",
                                item )
                                .c_str() );
                        }
                        for ( const auto &item : rule_data_.svc ) {
                            system( std::format( "sc.exe config {} start= disabled", item ).c_str() );
                        }
                    }
                    for ( const auto &item : rule_data_.exe ) {
                        system( std::format( R"(taskkill.exe /f /im "{}")", item ).c_str() );
                    }
                    for ( const auto &item : rule_data_.svc ) {
                        system( std::format( R"(net.exe stop "{}" /y)", item ).c_str() );
                    }
                    break;
                }
                case 'r' : {
                    if ( data::config[ 0 ].is_enabled == true ) {
                        for ( const auto &item : rule_data_.exe ) {
                            system(
                              std::format(
                                R"(reg.exe delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution options\{}" /f)",
                                item )
                                .c_str() );
                        }
                        for ( const auto &item : rule_data_.svc ) {
                            system( std::format( "sc.exe config {} start= auto", item ).c_str() );
                        }
                    }
                    for ( const auto &item : rule_data_.svc ) {
                        system( std::format( R"(net.exe start "{}")", item ).c_str() );
                    }
                    break;
                }
            }
            return CONSOLE_UI_REVERT;
        }
        auto &operator=( const rule_op & ) = delete;
        auto &operator=( rule_op && )      = delete;
        rule_op( const char _mode, const rule_data_node &_rule_data )
          : mode_{ _mode }
          , rule_data_{ _rule_data }
        { }
        rule_op( const rule_op & ) = default;
        rule_op( rule_op && )      = default;
        ~rule_op()                 = default;
    };
#else
    struct rule_data_node final {
        const std::vector< const char * > exe, svc;
    };
    inline struct {
        const rule_data_node mythware, lenovo;
    } rule{
      {{ "StudentMain.exe",
          "DispcapHelper.exe",
          "VRCwPlayer.exe",
          "InstHelpApp.exe",
          "InstHelpApp64.exe",
          "TDOvrSet.exe",
          "GATESRV.exe",
          "ProcHelper64.exe",
          "MasterHelper.exe" },
       {
          "STUDSRV",
          "TDNetFilter",
        }                                         },
      {{ "vncviewer.exe",
          "tvnserver32.exe",
          "WfbsPnpInstall.exe",
          "WFBSMon.exe",
          "WFBSMlogon.exe",
          "WFBSSvrLogShow.exe",
          "ResetIp.exe",
          "FuncForWIN64.exe",
          "CertMgr.exe",
          "Fireware.exe",
          "BCDBootCopy.exe",
          "refreship.exe",
          "lenovoLockScreen.exe",
          "PortControl64.exe",
          "DesktopCheck.exe",
          "DeploymentManager.exe",
          "DeploymentAgent.exe",
          "XYNTService.exe" },
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
        const rule_data_node &rule_;
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
            std::puts( ":: 生成命令." );
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
            std::printf( ":: 执行命令.\n%s\n", std::string( 50, '-' ).c_str() );
            system( cmd.c_str() );
            std::printf( "%s\n::释放内存.", std::string( 50, '-' ).c_str() );
            return CONSOLE_UI_REVERT;
        }
        rule_op( const char _mode, const rule_data_node &_rule )
          : mode_{ _mode }
          , rule_{ _rule }
        { }
        ~rule_op() { }
    };
#endif
}
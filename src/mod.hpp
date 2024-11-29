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
} config_data{};
inline bool config_error{};
#endif
namespace mod {
#ifdef _NEXT_
    struct config_data_node final {
        const char *const name;
        bool state;
    };
    struct rule_data_node final {
        std::vector< std::string > exe, svc;
    };
    namespace data {
        inline config_data_node config[]{
          {"enhanced_op",     false},
          {"enhanced_window", false},
          {"repair_mode",     false}
        };
        inline const char *const config_file_name{ "config.ini" };
        inline struct {
            const rule_data_node mythware, lenovo;
            rule_data_node customized;
        } rule{
          { { "StudentMain.exe", "DispcapHelper.exe", "VRCwPlayer.exe", "InstHelpApp.exe",
              "InstHelpApp64.exe", "TDOvrSet.exe", "GATESRV.exe", "ProcHelper64.exe",
              "MasterHelper.exe" },
           {
              "STUDSRV",
              "TDNetFilter",
            } },
          { { "vncviewer.exe", "tvnserver32.exe", "WfbsPnpInstall.exe", "WFBSMon.exe",
              "WFBSMlogon.exe", "WFBSSvrLogShow.exe", "ResetIp.exe", "FuncForWIN64.exe",
              "CertMgr.exe", "Fireware.exe", "BCDBootCopy.exe", "refreship.exe",
              "lenovoLockScreen.exe", "PortControl64.exe", "DesktopCheck.exe",
              "DeploymentManager.exe", "DeploymentAgent.exe", "XYNTService.exe" },
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
               &nt_authority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &admins_group ) )
        {
            CheckTokenMembership( nullptr, admins_group, &is_admin );
            FreeSid( admins_group );
        }
        return is_admin;
    }
    inline auto quit( console_ui::fn_args )
    {
        return FUNC_EXIT;
    }
    inline auto relaunch_as_admin( console_ui::fn_args )
    {
        char path[ MAX_PATH ]{};
        GetModuleFileNameA( nullptr, path, MAX_PATH );
        ShellExecuteA( nullptr, "runas", path, nullptr, nullptr, SW_SHOWNORMAL );
        return FUNC_EXIT;
    }
    inline auto info( console_ui::fn_args )
    {
        auto view_repo_webpage{ []( console_ui::fn_args )
        {
            ShellExecuteA( nullptr, "open", INFO_REPO_URL, nullptr, nullptr, SW_SHOWNORMAL );
            return FUNC_BACK;
        } };
        console_ui ui;
        ui.add( "                    [ 信  息 ]\n\n" )
          .add( " < 返回 ", quit, CONSOLE_TEXT_RED_WHITE )
          .add( "\n[名称]\n\n " INFO_NAME "\n\n[版本]\n\n " INFO_VERSION )
          .add( "\n[仓库]\n" )
          .add( " " INFO_REPO_URL, std::move( view_repo_webpage ) )
          .add( "\n[许可证]\n\n " INFO_LICENSE "\n\n (C) 2023 " INFO_DEVELOPER "." )
          .show();
        return FUNC_BACK;
    }
    inline auto toolkit( console_ui::fn_args )
    {
        auto launch_cmd{ []( console_ui::fn_args _args )
        {
            _args.ui->lock( false, false );
            _args.ui->set_console(
              936,
              "CRCSN",
              125,
              30,
              true,
              false,
              !data::config[ 1 ].state,
              ( data::config[ 1 ].state ) ? ( 230 ) : ( 255 ) );
            SetConsoleScreenBufferSize(
              GetStdHandle( STD_OUTPUT_HANDLE ), { WINDOW_WIDTH * 2, SHRT_MAX - 1 } );
            system( "cmd" );
            _args.ui->set_console(
              936,
              "CRCSN",
              WINDOW_WIDTH,
              WINDOW_HEIGHT,
              true,
              false,
              !data::config[ 1 ].state,
              ( data::config[ 1 ].state ) ? ( 230 ) : ( 255 ) );
            return FUNC_BACK;
        } };
        class exec_cmd {
          private:
            const char *const cmd_;
          public:
            auto operator=( const exec_cmd & ) = delete;
            auto operator()( console_ui::fn_args )
            {
                std::print( ":: 执行命令.\n{}\n", std::string( WINDOW_WIDTH, '-' ) );
                system( cmd_ );
                return FUNC_BACK;
            }
            explicit exec_cmd( const char *const _cmd )
              : cmd_{ _cmd }
            { }
            explicit exec_cmd( const exec_cmd &_obj )
              : cmd_{ _obj.cmd_ }
            { }
            ~exec_cmd() { }
        };
        const char *const cmds[]{
          R"(taskkill /f /im explorer.exe && timeout /t 3 /nobreak && start C:\Windows\explorer.exe)",
          R"(reg delete "HKLM\SOFTWARE\Policies\Google\Chrome" /f /v AllowDinosaurEasterEgg)",
          R"(reg delete "HKLM\SOFTWARE\Policies\Microsoft\Edge" /f /v AllowSurfGame)",
          R"(reg add "HKLM\SYSTEM\CurrentControlSet\Services\USBSTOR" /f /t reg_dword /v Start /d 3)" };
        console_ui ui;
        ui.add( "                   [ 工 具 箱 ]\n\n" )
          .add( " < 返回 ", quit, CONSOLE_TEXT_RED_WHITE )
          .add( " > 命令提示符 ", std::move( launch_cmd ) )
          .add( "\n[快捷操作]\n" )
          .add( " > 重启资源管理器 ", exec_cmd{ std::move( cmds[ 0 ] ) } )
          .add( " > 恢复 Google Chrome 离线游戏 ", exec_cmd{ std::move( cmds[ 1 ] ) } )
          .add( " > 恢复 Microsoft Edge 离线游戏 ", exec_cmd{ std::move( cmds[ 2 ] ) } )
          .add( " > 恢复 USB 设备访问 ", exec_cmd{ std::move( cmds[ 3 ] ) } )
          .show();
        return FUNC_BACK;
    }
    class config_op final {
      private:
        const char mode_;
        mutable bool is_reload_;
        auto load_() const
        {
            std::ifstream config_file{ data::config_file_name, std::ios::in };
            if ( !config_file.is_open() ) {
                config_file.close();
                return;
            }
            if ( is_reload_ ) {
                std::print( ":: 同步更改.\n" );
            } else {
                std::print( ":: 加载配置文件.\n" );
            }
            data::rule.customized.exe.clear();
            data::rule.customized.svc.clear();
            std::string line;
            enum {
                v_unknown  = -1,
                v_settings = 0,
                v_rule_exe = 1,
                v_rule_svc = 2,
            } config_item{ v_unknown };
            while ( std::getline( config_file, line ) ) {
                if ( ( line.empty() ) || ( line.front() == '#' ) ) {
                    continue;
                }
                if ( line == "[settings]" ) {
                    config_item = v_settings;
                    continue;
                } else if ( line == "[rule_exe]" ) {
                    config_item = v_rule_exe;
                    continue;
                } else if ( line == "[rule_svc]" ) {
                    config_item = v_rule_svc;
                    continue;
                } else if ( ( line.front() == '[' ) && ( line.back() == ']' ) ) {
                    config_item = v_unknown;
                    continue;
                }
                switch ( config_item ) {
                    case v_unknown : break;
                    case v_settings : {
                        if ( is_reload_ ) {
                            continue;
                        }
                        for ( std::size_t i{};
                              i < ( sizeof( data::config ) / sizeof( config_data_node ) );
                              ++i )
                        {
                            if ( line == data::config[ i ].name ) {
                                data::config[ i ].state = true;
                            }
                        }
                        break;
                    }
                    case v_rule_exe :
                        data::rule.customized.exe.emplace_back( std::move( line ) );
                        break;
                    case v_rule_svc :
                        data::rule.customized.svc.emplace_back( std::move( line ) );
                        break;
                }
            }
            config_file.close();
            return;
        }
        auto edit_() const
        {
            auto sync{ [ this ]( console_ui::fn_args )
            {
                is_reload_ = true;
                load_();
                std::print( ":: 保存更改.\n" );
                std::string text;
                text.append( "[settings]\n" );
                for ( std::size_t i{}; i < ( sizeof( data::config ) / sizeof( config_data_node ) ); ++i )
                {
                    if ( data::config[ i ].state ) {
                        text.append( data::config[ i ].name ).push_back( '\n' );
                    }
                }
                text.append( "[rule_exe]\n" );
                if ( !data::rule.customized.exe.empty() ) {
                    for ( const auto &item : data::rule.customized.exe ) {
                        text.append( item ).push_back( '\n' );
                    }
                }
                text.append( "[rule_svc]\n" );
                if ( !data::rule.customized.svc.empty() ) {
                    for ( const auto &item : data::rule.customized.svc ) {
                        text.append( item ).push_back( '\n' );
                    }
                }
                std::ofstream config_file{ data::config_file_name, std::ios::out | std::ios::trunc };
                config_file.write( text.c_str(), text.size() );
                config_file.close();
                return FUNC_EXIT;
            } };
            auto open_config_file{ []( console_ui::fn_args )
            {
                if ( std::ifstream{ data::config_file_name, std::ios::in }.is_open() ) {
                    std::print( ":: 打开配置文件.\n" );
                    ShellExecuteA(
                      nullptr, "open", data::config_file_name, nullptr, nullptr, SW_SHOWNORMAL );
                    return FUNC_BACK;
                }
                std::print(
                  "                    [ 配  置 ]\n\n\n"
                  " (i) 无法读取配置文件.\n\n" );
                for ( unsigned short i{ 3 }; i > 0; --i ) {
                    std::print( " {}s 后返回.\r", i );
                    Sleep( 1000 );
                }
                return FUNC_BACK;
            } };
            console_ui ui;
            ui.add( "                    [ 配  置 ]\n\n" )
              .add( " (i) 相关信息可参阅文档.\n" )
              .add( " < 同步配置并返回 ", std::move( sync ), CONSOLE_TEXT_RED_WHITE )
              .add( " > 打开配置文件 ", std::move( open_config_file ) )
              .add( "\n[增强操作]\n" )
              .add( " > 启用 ",
                    []( console_ui::fn_args )
            {
                data::config[ 0 ].state = true;
                return FUNC_BACK;
            } )
              .add( " > 禁用 (默认) ",
                    []( console_ui::fn_args )
            {
                data::config[ 0 ].state = false;
                return FUNC_BACK;
            } )
              .add( "\n[增强窗口 (下次启动时生效)]\n" )
              .add( " > 启用 ",
                    []( console_ui::fn_args )
            {
                data::config[ 1 ].state = true;
                return FUNC_BACK;
            } )
              .add( " > 禁用 (默认) ",
                    []( console_ui::fn_args )
            {
                data::config[ 1 ].state = false;
                return FUNC_BACK;
            } )
              .add( "\n[修复模式 (下次启动时生效)]\n" )
              .add( " > 启用 ",
                    []( console_ui::fn_args )
            {
                data::config[ 2 ].state = true;
                return FUNC_BACK;
            } )
              .add( " > 禁用 (默认) ",
                    []( console_ui::fn_args )
            {
                data::config[ 2 ].state = false;
                return FUNC_BACK;
            } )
              .show();
            return FUNC_BACK;
        }
      public:
        auto operator()( console_ui::fn_args ) const
        {
            switch ( mode_ ) {
                case 'r' : load_(); break;
                case 'w' : edit_(); break;
            }
            return FUNC_BACK;
        }
        explicit config_op( const char _mode )
          : mode_{ _mode }
          , is_reload_{}
        { }
        ~config_op() { }
    };
    class rule_op final {
      private:
        const char mode_;
        const rule_data_node &rule_data_;
      public:
        auto operator()( console_ui::fn_args ) const
        {
            std::print( "                 [ 破 解 / 恢 复 ]\n\n\n" );
            if ( ( rule_data_.exe.empty() ) && ( rule_data_.svc.empty() ) ) {
                std::print( " (i) 规则为空.\n\n" );
                for ( unsigned short i{ 3 }; i > 0; --i ) {
                    std::print( " {}s 后返回.\r", i );
                    Sleep( 1000 );
                }
                return FUNC_BACK;
            }
            std::print( ":: 生成并执行命令.\n{}\n", std::string( WINDOW_WIDTH, '-' ) );
            std::string cmd;
            switch ( mode_ ) {
                case 'c' : {
                    if ( data::config[ 0 ].state ) {
                        for ( const auto &item : rule_data_.exe ) {
                            cmd
                              .append(
                                R"(reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution options\)" )
                              .append( item )
                              .append( R"(" /f /t reg_sz /v debugger /d _)" );
                            system( cmd.c_str() );
                            cmd.clear();
                        }
                        for ( const auto &item : rule_data_.svc ) {
                            cmd.append( "sc config " ).append( item ).append( " start= disabled" );
                            system( cmd.c_str() );
                            cmd.clear();
                        }
                    }
                    for ( const auto &item : rule_data_.exe ) {
                        cmd.append( R"(taskkill /f /im ")" ).append( item ).append( R"(")" );
                        system( cmd.c_str() );
                        cmd.clear();
                    }
                    for ( const auto &item : rule_data_.svc ) {
                        cmd.append( R"(net stop ")" ).append( item ).append( R"(" /y)" );
                        system( cmd.c_str() );
                        cmd.clear();
                    }
                    break;
                }
                case 'r' : {
                    if ( data::config[ 0 ].state ) {
                        for ( const auto &item : rule_data_.exe ) {
                            cmd
                              .append(
                                R"(reg delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution options\)" )
                              .append( item )
                              .append( R"(" /f)" );
                            system( cmd.c_str() );
                            cmd.clear();
                        }
                        for ( const auto &item : rule_data_.svc ) {
                            cmd.append( "sc config " ).append( item ).append( " start= auto" );
                            system( cmd.c_str() );
                            cmd.clear();
                        }
                    }
                    for ( const auto &item : rule_data_.svc ) {
                        cmd.append( R"(net start ")" ).append( item ).append( R"(")" );
                        system( cmd.c_str() );
                        cmd.clear();
                    }
                    break;
                }
            }
            return FUNC_BACK;
        }
        explicit rule_op( const char _mode, const rule_data_node &_rule_data )
          : mode_{ _mode }
          , rule_data_{ _rule_data }
        { }
        ~rule_op() { }
    };
#else
    namespace rule_data {
        struct base final {
            std::vector< const char * > exe, svc;
        };
        inline const base mythware{
          { "StudentMain.exe", "DispcapHelper.exe", "VRCwPlayer.exe", "InstHelpApp.exe",
           "InstHelpApp64.exe", "TDOvrSet.exe", "GATESRV.exe", "ProcHelper64.exe",
           "MasterHelper.exe" },
          { "STUDSRV", "TDNetFilter", "TDFileFilter" }
        },
          lenovo{
            { "vncviewer.exe", "tvnserver32.exe", "WfbsPnpInstall.exe", "WFBSMon.exe",
              "WFBSMlogon.exe", "WFBSSvrLogShow.exe", "ResetIp.exe", "FuncForWIN64.exe",
              "CertMgr.exe", "Fireware.exe", "BCDBootCopy.exe", "refreship.exe",
              "lenovoLockScreen.exe", "PortControl64.exe", "DesktopCheck.exe",
              "DeploymentManager.exe", "DeploymentAgent.exe", "XYNTService.exe" },
            { "BSAgentSvr", "tvnserver", "WFBSMlogon" } };
    }
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
    inline auto quit( console_ui::fn_args )
    {
        return FUNC_EXIT;
    }
    inline auto init()
    {
        system( "chcp 936 > nul" );
        SetConsoleTitleA( "CRCSN" );
        SetWindowLongPtrA(
          GetConsoleWindow(),
          GWL_STYLE,
          ( config_data.window_ctrls )
            ? ( GetWindowLongPtrA( GetConsoleWindow(), GWL_STYLE ) | WS_SIZEBOX | WS_MAXIMIZEBOX
                | WS_MINIMIZEBOX )
            : ( GetWindowLongPtrA( GetConsoleWindow(), GWL_STYLE ) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX
                & ~WS_MINIMIZEBOX ) );
        system( "mode con cols=50 lines=25" );
        SetLayeredWindowAttributes(
          GetConsoleWindow(), 0, ( config_data.translucent_window ) ? ( 230 ) : ( 255 ), LWA_ALPHA );
    }
    inline auto front_show_window()
    {
        const HWND this_window{ GetConsoleWindow() };
        const DWORD foreground_id{ GetWindowThreadProcessId( this_window, nullptr ) },
          current_id{ GetCurrentThreadId() };
        while ( true ) {
            AttachThreadInput( current_id, foreground_id, TRUE );
            ShowWindow( this_window, SW_SHOWNORMAL );
            SetForegroundWindow( this_window );
            AttachThreadInput( current_id, foreground_id, FALSE );
            SetWindowPos( this_window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
            Sleep( 100 );
        }
    }
    inline auto info( console_ui::fn_args )
    {
        console_ui ui;
        ui.add( "                    [ 信  息 ]\n\n" )
          .add( " < 返回 ", quit, CONSOLE_TEXT_RED_WHITE )
          .add(
            "\n[名称]\n\n " INFO_NAME "\n\n[版本]\n\n " INFO_VERSION "\n\n[仓库]\n\n " INFO_REPO_URL
            "\n\n[许可证]\n\n " INFO_LICENSE "\n\n (C) 2023 " INFO_DEVELOPER "." )
          .show();
        return FUNC_BACK;
    }
    inline auto cmd( console_ui::fn_args _args )
    {
        _args.ui->lock( false, false );
        system( "cmd" );
        if ( !config_data.window_ctrls ) {
            init();
        }
        return FUNC_BACK;
    }
    class rule_op final {
      private:
        const char mode_;
        const rule_data::base &rule_;
      public:
        auto operator()( console_ui::fn_args ) const
        {
            puts( "                 [ 破 解 / 恢 复 ]\n\n" );
            if ( !is_run_as_admin() ) {
                puts( "\n (i) 需要管理员权限.\n" );
                for ( unsigned short i{ 3 }; i > 0; --i ) {
                    printf( " %hus 后返回.\r", i );
                    Sleep( 1000 );
                }
                return FUNC_BACK;
            }
            puts( ":: 生成命令." );
            std::string cmd;
            switch ( mode_ ) {
                case 'c' : {
                    for ( const auto &item : rule_.exe ) {
                        cmd
                          .append(
                            R"(reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution options\)" )
                          .append( item )
                          .append( R"(" /f /t reg_sz /v debugger /d ? & taskkill /f /im ")" )
                          .append( item )
                          .append( R"(" & )" );
                    }
                    for ( const auto &item : rule_.svc ) {
                        cmd.append( R"(net stop ")" ).append( item ).append( R"(" /y & )" );
                    }
                    break;
                }
                case 'r' : {
                    for ( const auto &item : rule_.exe ) {
                        cmd
                          .append(
                            R"(reg delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution options\)" )
                          .append( item )
                          .append( R"(" /f & )" );
                    }
                    for ( const auto &item : rule_.svc ) {
                        cmd.append( R"(net start ")" ).append( item ).append( R"(" & )" );
                    }
                    break;
                }
            }
            printf( ":: 执行命令.\n%s\n", std::string( 50, '-' ).c_str() );
            system( cmd.c_str() );
            printf( "%s\n::释放内存.", std::string( 50, '-' ).c_str() );
            return FUNC_BACK;
        }
        explicit rule_op( const char _mode, const rule_data::base &_rule )
          : mode_{ _mode }
          , rule_{ _rule }
        { }
        ~rule_op() { }
    };
#endif
}
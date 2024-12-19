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
    struct option_node final {
        struct item final {
            const string_type tag_name, showed_name;
            const bool is_relaunch_to_apply;
            bool is_enabled;
            auto &operator=( const item & ) = delete;
            auto &operator=( item && )      = delete;
            item( string_type _tag_name, string_type _showed_name, const bool _is_relaunch_to_apply )
              : tag_name{ std::move( _tag_name ) }
              , showed_name{ std::move( _showed_name ) }
              , is_relaunch_to_apply{ _is_relaunch_to_apply }
              , is_enabled{ false }
            { }
            item(
              string_type _tag_name,
              string_type _showed_name,
              const bool _is_relaunch_to_apply,
              const bool _default_value )
              : tag_name{ std::move( _tag_name ) }
              , showed_name{ std::move( _showed_name ) }
              , is_relaunch_to_apply{ _is_relaunch_to_apply }
              , is_enabled{ _default_value }
            { }
            item( const item & ) = default;
            item( item && )      = default;
            ~item()              = default;
        };
        const string_type tag_name, showed_name;
        std::vector< item > sub;
        auto &operator[]( size_type _index )
        {
            return sub.at( _index );
        }
        auto &operator=( const option_node & ) = delete;
        auto &operator=( option_node && )      = delete;
        option_node( string_type _tag_name, string_type _showed_name, std::vector< item > _options )
          : tag_name{ std::move( _tag_name ) }
          , showed_name{ std::move( _showed_name ) }
          , sub{ std::move( _options ) }
        { }
        option_node( const option_node & ) = default;
        option_node( option_node && )      = default;
        ~option_node()                     = default;
    };
    struct rule_node final {
        std::deque< string_type > exe, svc;
        auto &operator=( const rule_node &_src )
        {
            exe = _src.exe;
            svc = _src.svc;
            return *this;
        }
        auto &operator=( rule_node &&_src )
        {
            exe = std::move( _src.exe );
            svc = std::move( _src.svc );
            return *this;
        }
        rule_node()
          : exe{}
          , svc{}
        { }
        rule_node( std::deque< string_type > _exe, std::deque< string_type > _svc )
          : exe{ std::move( _exe ) }
          , svc{ std::move( _svc ) }
        { }
        rule_node( const rule_node & ) = default;
        rule_node( rule_node && )      = default;
        ~rule_node()                   = default;
    };
    namespace data {
        inline const string_type config_file_name{ "config.ini" };
        inline type_wrapper< option_node[] > option_class{
          {"operation",
           "破解/恢复",         { { "hijack_reg", "注册表劫持", false },
              { "set_svc_startup_type", "设置服务启动类型", false } }},
          {"window",
           "窗口显示",          { { "force_show", "置顶显示", true },
              { "disable_close_ctrl", "禁用关闭控件", true },
              { "translucency", "半透明化", true } }                  },
          {"other",     "其他", { { "repair_runtime_env", "运行环境修复", true } }          }
        };
        inline struct {
            rule_node customized;
            const rule_node mythware, lenovo;
        } rule{
          {},
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
           { "BSAgentSvr", "tvnserver", "WFBSMlogon" } }
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
    inline auto repair_runtime_env()
    {
        using namespace std::chrono_literals;
        type_wrapper< const string_type[] > hkcu_reg_dir{
          R"(Software\Policies\Microsoft\Windows\System)",
          R"(Software\Microsoft\Windows\CurrentVersion\Policies\System)",
          R"(Software\Microsoft\Windows\CurrentVersion\Policies\Explorer)" },
          exe{ "mode.com", "chcp.com",    "reg.exe",     "sc.exe",      "taskkill.exe", "net.exe",
               "cmd.exe",  "taskmgr.exe", "perfmon.exe", "regedit.exe", "mmc.exe" };
        while ( true ) {
            for ( const auto &item : hkcu_reg_dir ) {
                RegDeleteTreeA( HKEY_CURRENT_USER, item.c_str() );
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
        wstring_type path( MAX_PATH, L'\0' );
        GetModuleFileNameW( nullptr, path.data(), MAX_PATH );
        ShellExecuteW( nullptr, L"runas", path.data(), nullptr, nullptr, SW_SHOWNORMAL );
        return CONSOLE_UI_TERMINATE;
    }
    inline auto quit( console_ui::func_args )
    {
        return CONSOLE_UI_TERMINATE;
    }
    inline auto info( console_ui::func_args )
    {
        std::print( "-> 初始化用户界面.\n" );
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
              data::option_class[ 1 ][ 1 ].is_enabled ? false : true,
              data::option_class[ 1 ][ 2 ].is_enabled ? 230 : 255 );
            SetConsoleScreenBufferSize( GetStdHandle( STD_OUTPUT_HANDLE ), { 125, SHRT_MAX - 1 } );
            system( "cmd.exe" );
            _args.parent_ui.set_console(
              CODE_PAGE,
              WINDOW_TITLE,
              WINDOW_WIDTH,
              WINDOW_HEIGHT,
              true,
              false,
              data::option_class[ 1 ][ 1 ].is_enabled ? false : true,
              data::option_class[ 1 ][ 2 ].is_enabled ? 230 : 255 );
            return CONSOLE_UI_REVERT;
        } };
        class cmd_executor final {
          private:
            const string_type cmd_;
          public:
            auto operator()( console_ui::func_args )
            {
                std::print( "-> 执行 Windows 命令.\n{}\n", string_type( WINDOW_WIDTH, '-' ) );
                system( cmd_.c_str() );
                return CONSOLE_UI_REVERT;
            }
            auto &operator=( const cmd_executor & ) = delete;
            auto &operator=( cmd_executor && )      = delete;
            cmd_executor( string_type _cmd )
              : cmd_{ std::move( _cmd ) }
            { }
            cmd_executor( const cmd_executor & ) = default;
            cmd_executor( cmd_executor && )      = default;
            ~cmd_executor()                      = default;
        };
        type_wrapper< const string_type[][ 2 ] > quick_op{
          {"重启资源管理器",
           R"(taskkill.exe /f /im explorer.exe && timeout /t 3 /nobreak && start C:\Windows\explorer.exe)"},
          {"恢复 Google Chrome 离线游戏",
           R"(reg.exe delete "HKLM\SOFTWARE\Policies\Google\Chrome" /f /v AllowDinosaurEasterEgg)"        },
          {"恢复 Microsoft Edge 离线游戏",
           R"(reg.exe delete "HKLM\SOFTWARE\Policies\Microsoft\Edge" /f /v AllowSurfGame)"                },
          {"恢复 USB 设备访问",
           R"(reg.exe add "HKLM\SYSTEM\CurrentControlSet\Services\USBSTOR" /f /t reg_dword /v Start /d 3)"}
        };
        console_ui ui;
        ui.add_back( "                   [ 工 具 箱 ]\n\n" )
          .add_back( " < 返回 ", quit, CONSOLE_TEXT_FOREGROUND_GREEN | CONSOLE_TEXT_FOREGROUND_INTENSITY )
          .add_back( " > 命令提示符 ", launch_cmd )
          .add_back( "\n[快捷操作]\n" );
        for ( const auto &item : quick_op ) {
            ui.add_back( std::format( " > {} ", item[ 0 ] ), cmd_executor{ item[ 1 ] } );
        }
        ui.show();
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
            std::print( "-> 加载配置文件.\n" );
            data::rule.customized.exe.clear();
            data::rule.customized.svc.clear();
            string_type line;
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
                        for ( auto &item_class : data::option_class ) {
                            for ( auto &item_option : item_class.sub ) {
                                if ( line
                                     == std::format( "{}::{}", item_class.tag_name, item_option.tag_name ) )
                                {
                                    item_option.is_enabled = true;
                                }
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
            std::print( "-> 初始化用户界面.\n" );
            constexpr auto option_button_color{
              CONSOLE_TEXT_FOREGROUND_RED | CONSOLE_TEXT_FOREGROUND_GREEN };
            auto sync{ [ this ]( console_ui::func_args )
            {
                load_( true );
                std::print( "-> 保存更改.\n" );
                string_type text;
                text.append( "[option]\n" );
                for ( const auto &item_class : data::option_class ) {
                    for ( const auto &item_option : item_class.sub ) {
                        if ( item_option.is_enabled ) {
                            text.append(
                              std::format( "{}::{}\n", item_class.tag_name, item_option.tag_name ) );
                        }
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
                option_node::item &node_;
                bool value_;
              public:
                auto operator()( console_ui::func_args )
                {
                    node_.is_enabled = value_;
                    return CONSOLE_UI_REVERT;
                }
                auto &operator=( const option_setter & ) = delete;
                auto &operator=( option_setter && )      = delete;
                option_setter( option_node::item &_node, bool _value )
                  : node_{ _node }
                  , value_{ _value }
                { }
                option_setter( const option_setter & ) = default;
                option_setter( option_setter && )      = default;
                ~option_setter()                       = default;
            };
            class option_class_shower final {
              private:
                option_node &node_;
              public:
                auto operator()( console_ui::func_args )
                {
                    console_ui ui;
                    ui.add_back( "                    [ 配  置 ]\n\n" )
                      .add_back(
                        std::format( " < 折叠 {}", node_.showed_name ),
                        quit,
                        CONSOLE_TEXT_FOREGROUND_GREEN | CONSOLE_TEXT_FOREGROUND_INTENSITY );
                    for ( auto &item : node_.sub ) {
                        ui
                          .add_back( std::format(
                            "\n[({}生效) {}]\n",
                            item.is_relaunch_to_apply ? "下次启动时" : "立即",
                            item.showed_name ) )
                          .add_back( " > 启用 ", option_setter{ item, true }, option_button_color )
                          .add_back( " > 禁用 ", option_setter{ item, false }, option_button_color );
                    }
                    ui.show();
                    return CONSOLE_UI_REVERT;
                }
                auto &operator=( const option_class_shower & ) = delete;
                auto &operator=( option_class_shower && )      = delete;
                option_class_shower( option_node &_node )
                  : node_{ _node }
                { }
                option_class_shower( const option_class_shower & ) = default;
                option_class_shower( option_class_shower && )      = default;
                ~option_class_shower()                             = default;
            };
            console_ui ui;
            ui.add_back(
                "                    [ 配  置 ]\n\n\n"
                " (i) 所有选项默认禁用.\n"
                "     相关信息可参阅文档.\n" )
              .add_back(
                " < 同步配置并返回 ", sync, CONSOLE_TEXT_FOREGROUND_GREEN | CONSOLE_TEXT_FOREGROUND_INTENSITY )
              .add_back( " > 打开配置文件 ", open_config_file )
              .add_back( "\n[选项分类]\n" );
            for ( auto &item : data::option_class ) {
                ui.add_back(
                  std::format( " > {}", item.showed_name ),
                  option_class_shower{ item },
                  option_button_color );
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
        const rule_node &rule_data_;
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
            std::print( "-> 生成并执行 Windows 命令.\n{}\n", string_type( WINDOW_WIDTH, '-' ) );
            switch ( mode_ ) {
                case 'c' : {
                    if ( data::option_class[ 0 ][ 0 ].is_enabled ) {
                        for ( const auto &item : rule_data_.exe ) {
                            system(
                              std::format(
                                R"(reg.exe add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution options\{}" /f /t reg_sz /v debugger /d "nul")",
                                item )
                                .c_str() );
                        }
                    }
                    if ( data::option_class[ 0 ][ 1 ].is_enabled ) {
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
                    if ( data::option_class[ 0 ][ 0 ].is_enabled ) {
                        for ( const auto &item : rule_data_.exe ) {
                            system(
                              std::format(
                                R"(reg.exe delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution options\{}" /f)",
                                item )
                                .c_str() );
                        }
                    }
                    if ( data::option_class[ 0 ][ 1 ].is_enabled ) {
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
        rule_op( const char _mode, const rule_node &_rule_data )
          : mode_{ _mode }
          , rule_data_{ _rule_data }
        { }
        rule_op( const rule_op & ) = default;
        rule_op( rule_op && )      = default;
        ~rule_op()                 = default;
    };
#else
    struct rule_node final {
        const std::vector< const char * > exe, svc;
    };
    inline struct {
        const rule_node mythware, lenovo;
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
        const rule_node &rule_;
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
        rule_op( const char _mode, const rule_node &_rule )
          : mode_{ _mode }
          , rule_{ _rule }
        { }
        ~rule_op() { }
    };
#endif
}
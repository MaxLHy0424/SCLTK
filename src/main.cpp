#ifdef _WIN32
# include <thread>
# include "console_ui.hpp"
# include "core.hpp"
# ifdef _NEXT_
auto main() -> int
{
    console_ui ui;
    ui.set_console( CODE_PAGE, WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, true, false, true, 255 )
      .lock( true, true );
    std::print( ":: 检测运行权限.\n" );
    if ( !core::is_run_as_admin() ) {
        std::print( ":: 申请管理员权限.\n" );
        core::relaunch_as_admin( console_ui::func_args{} );
        return 1;
    }
    core::config_op{ 'r' }( console_ui::func_args{} );
    if ( core::data::config[ 1 ].is_enabled == true ) {
        std::thread{ []()
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
        } }
          .detach();
    }
    if ( core::data::config[ 2 ].is_enabled == true ) {
        std::thread{ []()
        {
            const char *const hkcu_reg_dir[]{
              R"(Software\Policies\Microsoft\Windows\System)",
              R"(Software\Microsoft\Windows\CurrentVersion\Policies\System)",
              R"(Software\Microsoft\Windows\CurrentVersion\Policies\Explorer)" };
            const char *const exe[]{
              "mode.com", "chcp.com",    "reg.exe",     "sc.exe",      "taskkill.exe", "net.exe",
              "cmd.exe",  "taskmgr.exe", "perfmon.exe", "regedit.exe", "mmc.exe" };
            std::string path;
            while ( true ) {
                for ( const auto &item : hkcu_reg_dir ) {
                    RegDeleteTreeA( HKEY_CURRENT_USER, item );
                }
                for ( const auto &item : exe ) {
                    path
                      .append(
                        R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\)" )
                      .append( item );
                    RegDeleteTreeA( HKEY_LOCAL_MACHINE, path.c_str() );
                    path.clear();
                }
                Sleep( 1000 );
            }
        } }
          .detach();
    }
    ui.add( "                    [ 主  页 ]\n\n" )
      .add( " < 退出 ", core::quit, CONSOLE_TEXT_RED_WHITE )
      .add( " < 重启 ", core::relaunch_as_admin, CONSOLE_TEXT_RED_WHITE )
      .add( " > 信息 ", core::info )
      .add( " > 配置 ", core::config_op{ 'w' } )
      .add( " > 工具箱 ", core::toolkit )
      .add( "\n[破解]\n" )
      .add( " > 极域电子教室 ", core::rule_op{ 'c', core::data::rule.mythware } )
      .add( " > 联想云教室 ", core::rule_op{ 'c', core::data::rule.lenovo } )
      .add( " > 自定义 ", core::rule_op{ 'c', core::data::rule.customized } )
      .add( "\n[恢复]\n" )
      .add( " > 极域电子教室 ", core::rule_op{ 'r', core::data::rule.mythware } )
      .add( " > 联想云教室 ", core::rule_op{ 'r', core::data::rule.lenovo } )
      .add( " > 自定义 ", core::rule_op{ 'r', core::data::rule.customized } )
      .set_console(
        CODE_PAGE,
        WINDOW_TITLE,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        true,
        false,
        ( core::data::config[ 1 ].is_enabled == false ) ? ( true ) : ( false ),
        ( core::data::config[ 1 ].is_enabled == true ) ? ( 230 ) : ( 255 ) )
      .show()
      .lock( false, false );
    if ( core::data::config[ 1 ].is_enabled == true ) {
        SetWindowLongPtrA(
          GetConsoleWindow(),
          GWL_STYLE,
          GetWindowLongPtrA( GetConsoleWindow(), GWL_STYLE ) | WS_SIZEBOX | WS_MAXIMIZEBOX
            | WS_MINIMIZEBOX );
        EnableMenuItem(
          GetSystemMenu( GetConsoleWindow(), FALSE ), SC_CLOSE, MF_BYCOMMAND | MF_ENABLED );
        SetLayeredWindowAttributes( GetConsoleWindow(), 0, 255, LWA_ALPHA );
        SetWindowPos( GetConsoleWindow(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
    }
    return 0;
}
# else
auto main( const int _argc, const char *const _argv[] ) -> int
{
    console_ui ui;
    if ( _argc == 1 ) {
        goto INIT;
    }
    for ( int i{ 1 }; i < _argc; ++i ) {
        std::string_view tmp{ _argv[ i ] };
        if ( ( tmp.size() > 2 ) && ( tmp.substr( 0, 2 ) == "-W" ) ) {
            for ( const auto &sub : tmp.substr( 2 ) ) {
                switch ( sub ) {
                    case 'f' : config_data.front_show_window = true; break;
                    case 't' : config_data.translucent_window = true; break;
                    case 'c' : config_data.window_ctrls = true; break;
                    default : config_error = true;
                }
            }
        } else {
            config_error = true;
            break;
        }
    }
INIT:
    core::init();
    if ( config_data.front_show_window ) {
        std::thread{ core::front_show_window }.detach();
    }
    ui.add( "                    [ 主  页 ]\n\n" );
    if ( config_error ) {
        ui.add( " (!) 参数错误.\n" );
    }
    ui.add( " < 退出 ", core::quit, CONSOLE_TEXT_RED_WHITE )
      .add( " > 信息 ", core::info )
      .add( " > 命令提示符 ", core::cmd )
      .add( "\n[破解]\n" )
      .add( " > 极域电子教室 ", core::rule_op{ 'c', core::rule.mythware } )
      .add( " > 联想云教室 ", core::rule_op{ 'c', core::rule.lenovo } )
      .add( "\n[恢复]\n" )
      .add( " > 极域电子教室 ", core::rule_op{ 'r', core::rule.mythware } )
      .add( " > 联想云教室 ", core::rule_op{ 'r', core::rule.lenovo } )
      .show()
      .lock( false, false );
    if ( config_data.translucent_window ) {
        SetLayeredWindowAttributes( GetConsoleWindow(), 0, 255, LWA_ALPHA );
    }
    if ( config_data.front_show_window ) {
        SetWindowPos( GetConsoleWindow(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
    }
    if ( !config_data.window_ctrls ) {
        SetWindowLongPtrA(
          GetConsoleWindow(),
          GWL_STYLE,
          GetWindowLongPtrA( GetConsoleWindow(), GWL_STYLE ) | WS_SIZEBOX | WS_MAXIMIZEBOX
            | WS_MINIMIZEBOX );
    }
    return 0;
}
# endif
#else
# error "The source code must be compiled on the Windows OS platform."
#endif
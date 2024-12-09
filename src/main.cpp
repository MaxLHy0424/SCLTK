#ifdef _WIN32
# include "console_ui.hpp"
# include "core.hpp"
# ifdef _THE_NEXT_MAJOR_UPDATE_
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
        std::thread{ core::force_show_window }.detach();
    }
    if ( core::data::config[ 2 ].is_enabled == true ) {
        std::thread{ core::repair_env }.detach();
    }
    std::print( ":: 初始化用户界面.\n" );
    ui.add_back( "                    [ 主  页 ]\n\n" )
      .add_back( " < 退出 ", core::quit, CONSOLE_TEXT_FOREGROUND_RED | CONSOLE_TEXT_FOREGROUND_INTENSITY )
      .add_back(
        " < 重启 ",
        core::relaunch_as_admin,
        CONSOLE_TEXT_FOREGROUND_GREEN | CONSOLE_TEXT_FOREGROUND_INTENSITY )
      .add_back( " > 信息 ", core::info )
      .add_back( " > 配置 ", core::config_op{ 'w' } )
      .add_back( " > 工具箱 ", core::toolkit )
      .add_back( "\n[破解]\n" )
      .add_back( " > 极域电子教室 ", core::rule_op{ 'c', core::data::rule.mythware } )
      .add_back( " > 联想云教室 ", core::rule_op{ 'c', core::data::rule.lenovo } )
      .add_back( " > 自定义 ", core::rule_op{ 'c', core::data::rule.customized } )
      .add_back( "\n[恢复]\n" )
      .add_back( " > 极域电子教室 ", core::rule_op{ 'r', core::data::rule.mythware } )
      .add_back( " > 联想云教室 ", core::rule_op{ 'r', core::data::rule.lenovo } )
      .add_back( " > 自定义 ", core::rule_op{ 'r', core::data::rule.customized } )
      .set_console(
        CODE_PAGE,
        WINDOW_TITLE,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        true,
        false,
        core::data::config[ 1 ].is_enabled == false ? true : false,
        core::data::config[ 1 ].is_enabled == true ? 230 : 255 )
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
        if ( tmp.size() > 2 && tmp.substr( 0, 2 ) == "-W" ) {
            for ( const auto &sub : tmp.substr( 2 ) ) {
                switch ( sub ) {
                    case 'f' : config_data.front_show_window = true; break;
                    case 't' : config_data.translucent_window = true; break;
                    case 'c' : config_data.window_ctrls = true; break;
                    default : config_error = true; break;
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
    ui.add_back( "                    [ 主  页 ]\n\n" );
    if ( config_error ) {
        ui.add_back( " (!) 参数错误.\n" );
    }
    ui.add_back( " < 退出 ", core::quit, CONSOLE_TEXT_FOREGROUND_RED | CONSOLE_TEXT_FOREGROUND_INTENSITY )
      .add_back( " > 信息 ", core::info )
      .add_back( "\n[破解]\n" )
      .add_back( " > 极域电子教室 ", core::rule_op{ 'c', core::rule.mythware } )
      .add_back( " > 联想云教室 ", core::rule_op{ 'c', core::rule.lenovo } )
      .add_back( "\n[恢复]\n" )
      .add_back( " > 极域电子教室 ", core::rule_op{ 'r', core::rule.mythware } )
      .add_back( " > 联想云教室 ", core::rule_op{ 'r', core::rule.lenovo } )
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
# error "must be compiled on the Windows OS platform."
#endif
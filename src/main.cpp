#include "console_ui.hpp"
#include "core.hpp"
#ifdef _NEXT_
auto main() -> int
{
    console_ui ui;
    ui.set_console( WINDOW_TITLE, CODE_PAGE, WINDOW_WIDTH, WINDOW_HEIGHT, true, false, true, 255 )
      .lock( true, true );
    std::print( "-> 检测运行权限.\n" );
    if ( !core::is_run_as_admin() ) {
        std::print( "-> 申请管理员权限.\n" );
        core::relaunch_as_admin( console_ui::func_args{ ui } );
        return EXIT_FAILURE;
    }
    core::config_op{ 'r' }( console_ui::func_args{ ui } );
    if ( core::data::options[ 1 ][ 0 ].is_enabled ) {
        std::print( "-> 启动置顶窗口线程.\n" );
        std::thread{ core::force_show_window }.detach();
    }
    if ( core::data::options[ 2 ][ 0 ].is_enabled ) {
        std::print( "-> 启动修复操作系统环境线程.\n" );
        std::thread{ core::fix_os_env }.detach();
    }
    std::print( "-> 初始化用户界面.\n" );
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
      .add_back(
        std::format( " > {} ", core::data::customized_rules.showed_name ),
        core::rule_op{ 'c', core::data::customized_rules } );
    for ( const auto &rule : core::data::builtin_rules ) {
        ui.add_back( std::format( " > {} ", rule.showed_name ), core::rule_op{ 'c', rule } );
    }
    ui.add_back( "\n[恢复]\n" )
      .add_back(
        std::format( " > {} ", core::data::customized_rules.showed_name ),
        core::rule_op{ 'r', core::data::customized_rules } );
    for ( const auto &rule : core::data::builtin_rules ) {
        ui.add_back( std::format( " > {} ", rule.showed_name ), core::rule_op{ 'r', rule } );
    }
    ui.set_console(
        WINDOW_TITLE,
        CODE_PAGE,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        true,
        false,
        core::data::options[ 1 ][ 1 ].is_enabled ? false : true,
        core::data::options[ 1 ][ 2 ].is_enabled ? 230 : 255 )
      .show()
      .lock( false, false );
    if ( core::data::options[ 1 ][ 0 ].is_enabled ) {
        SetWindowPos( GetConsoleWindow(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
    }
    if ( core::data::options[ 1 ][ 1 ].is_enabled ) {
        EnableMenuItem(
          GetSystemMenu( GetConsoleWindow(), FALSE ), SC_CLOSE, MF_BYCOMMAND | MF_ENABLED );
    }
    if ( core::data::options[ 1 ][ 2 ].is_enabled ) {
        SetLayeredWindowAttributes( GetConsoleWindow(), 0, 255, LWA_ALPHA );
    }
    SetWindowLongPtrA(
      GetConsoleWindow(),
      GWL_STYLE,
      GetWindowLongPtrA( GetConsoleWindow(), GWL_STYLE ) | WS_SIZEBOX | WS_MAXIMIZEBOX
        | WS_MINIMIZEBOX );
    return EXIT_SUCCESS;
}
#else
auto main( const int _argc, const char *const _argv[] ) -> int
{
    console_ui ui;
    if ( _argc == 1 ) {
        goto INIT;
    }
    for ( int i{ 1 }; i < _argc; ++i ) {
        std::string_view tmp{ _argv[ i ] };
        if ( tmp.size() > 2 && tmp.substr( 0, 2 ) == "-W" ) {
            for ( const auto &sub_item : tmp.substr( 2 ) ) {
                switch ( sub_item ) {
                    case 'f' : args_data.front_show_window = true; break;
                    case 't' : args_data.translucent_window = true; break;
                    case 'c' : args_data.window_ctrls = true; break;
                    default : args_error = true; break;
                }
            }
        } else {
            args_error = true;
            break;
        }
    }
INIT:
    core::init();
    if ( args_data.front_show_window ) {
        std::thread{ core::front_show_window }.detach();
    }
    ui.add_back( "                    [ 主  页 ]\n\n" );
    if ( args_error ) {
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
    if ( args_data.translucent_window ) {
        SetLayeredWindowAttributes( GetConsoleWindow(), 0, 255, LWA_ALPHA );
    }
    if ( args_data.front_show_window ) {
        SetWindowPos( GetConsoleWindow(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
    }
    if ( !args_data.window_ctrls ) {
        SetWindowLongPtrA(
          GetConsoleWindow(),
          GWL_STYLE,
          GetWindowLongPtrA( GetConsoleWindow(), GWL_STYLE ) | WS_SIZEBOX | WS_MAXIMIZEBOX
            | WS_MINIMIZEBOX );
    }
    return 0;
}
#endif
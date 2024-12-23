#include "console_ui.hpp"
#include "core.hpp"
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
        std::print( "-> 创建线程以置顶窗口.\n" );
        std::thread{ core::force_show_window }.detach();
    }
    if ( core::data::options[ 2 ][ 0 ].is_enabled ) {
        std::print( "-> 创建线程以修复操作系统环境.\n" );
        std::thread{ core::fix_os_env }.detach();
    }
    std::print( "-> 初始化用户界面.\n" );
    ui.add_back( "                    [ 主  页 ]\n\n" )
      .add_back( " < 退出 ", core::quit, CONSOLE_TEXT_FOREGROUND_RED | CONSOLE_TEXT_FOREGROUND_INTENSITY )
      .add_back(
        " < 重启 ", core::relaunch_as_admin,
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
        WINDOW_TITLE, CODE_PAGE, WINDOW_WIDTH, WINDOW_HEIGHT, true, false,
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
      GetConsoleWindow(), GWL_STYLE,
      GetWindowLongPtrA( GetConsoleWindow(), GWL_STYLE ) | WS_SIZEBOX | WS_MAXIMIZEBOX
        | WS_MINIMIZEBOX );
    return EXIT_SUCCESS;
}
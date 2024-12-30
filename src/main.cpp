#include "console_ui.hpp"
#include "core.hpp"
auto main() -> int
{
    console_ui ui;
    ui.set_console( WINDOW_TITLE, CODE_PAGE, WINDOW_WIDTH, WINDOW_HEIGHT, true, false, true, 255 )
      .lock( true, true );
    std::print( " -> 检测运行权限.\n" );
    if ( !core::is_run_as_admin() ) {
        std::print( " -> 申请管理员权限.\n" );
        core::relaunch_as_admin( console_ui::func_args{ ui } );
        return EXIT_FAILURE;
    }
    core::set_window set_window_thread{
      core::options[ 1 ][ 0 ].is_enabled, core::options[ 1 ][ 1 ].is_enabled,
      core::options[ 1 ][ 2 ].is_enabled };
    core::fix_os_env fix_os_env_thread{ core::options[ 2 ][ 0 ].is_enabled };
    core::config_op{ 'r' }( console_ui::func_args{ ui } );
    std::print( " -> 初始化用户界面.\n" );
    ui.add_back( "                    [ 主  页 ]\n\n" )
      .add_back( " < 退出 ", core::quit, TEXT_FOREGROUND_RED | TEXT_FOREGROUND_INTENSITY )
      .add_back( " < 重启 ", core::relaunch_as_admin, TEXT_FOREGROUND_GREEN | TEXT_FOREGROUND_INTENSITY )
      .add_back( " > 信息 ", core::info )
      .add_back( " > 配置 ", core::config_op{ 'w' } )
      .add_back( " > 工具箱 ", core::toolkit )
      .add_back( "\n[破解]\n" )
      .add_back(
        std::format( " > {} ", core::custom_rules.showed_name ),
        core::rule_op{ 'c', core::custom_rules } );
    for ( const auto &rule : core::builtin_rules ) {
        ui.add_back( std::format( " > {} ", rule.showed_name ), core::rule_op{ 'c', rule } );
    }
    ui.add_back( "\n[恢复]\n" )
      .add_back(
        std::format( " > {} ", core::custom_rules.showed_name ),
        core::rule_op{ 'r', core::custom_rules } );
    for ( const auto &rule : core::builtin_rules ) {
        ui.add_back( std::format( " > {} ", rule.showed_name ), core::rule_op{ 'r', rule } );
    }
    ui.set_console(
        WINDOW_TITLE, CODE_PAGE, WINDOW_WIDTH, WINDOW_HEIGHT, true, false,
        core::options[ 1 ][ 1 ].is_enabled ? false : true,
        core::options[ 1 ][ 2 ].is_enabled ? 230 : 255 )
      .show()
      .lock( true, true );
    return EXIT_SUCCESS;
}
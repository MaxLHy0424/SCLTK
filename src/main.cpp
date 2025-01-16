#include "core.hpp"
auto main() -> int
{
    auto ui{ cpp_utils::console_ui_ansi{} };
    ui.lock( true, true ).set_console( CONSOLE_TITLE, CODE_PAGE_ID, CONSOLE_WIDTH, CONSOLE_HEIGHT, true, false, true, 255 );
    std::print( " -> 检测运行权限.\n" );
    if ( !cpp_utils::is_run_as_admin() ) {
        std::print( " -> 申请管理员权限.\n" );
        cpp_utils::relaunch_as_admin();
        return EXIT_SUCCESS;
    }
    auto threads{ cpp_utils::thread_pool_ansi{} };
    threads.add_task( "置顶显示", core::topmost_show_window, core::options[ "window" ][ "topmost_show" ].value )
      .add_task(
        "窗口属性设定",
        core::set_console_attrs,
        core::options[ "window" ][ "disable_close_ctrl" ].value,
        core::options[ "window" ][ "translucency" ].value )
      .add_task( "修复操作系统环境", core::fix_os_env, core::options[ "other" ][ "fix_os_env" ].value );
    core::config_op{ core::config_op::mod::load }( cpp_utils::console_ui_ansi::func_args{ ui } );
    std::print( " -> 准备用户界面.\n" );
    ui.add_back( "                    [ 主  页 ]\n\n" )
      .add_back( " < 退出 ", core::quit, cpp_utils::console_value::text_foreground_red | cpp_utils::console_value::text_foreground_intensity )
      .add_back( " < 重启 ", core::relaunch, cpp_utils::console_value::text_foreground_green | cpp_utils::console_value::text_foreground_intensity )
      .add_back( " > 信息 ", core::info )
      .add_back( " > 配置 ", core::config_op{ core::config_op::mod::edit } )
      .add_back( " > 工具箱 ", core::toolkit )
      .add_back( "\n[破解]\n" )
      .add_back(
        std::format( " > {} ", core::custom_rules.showed_name ), core::rule_op{ core::rule_op::mod::crack, core::custom_rules } );
    for ( const auto &rule : core::builtin_rules ) {
        ui.add_back( std::format( " > {} ", rule.showed_name ), core::rule_op{ core::rule_op::mod::crack, rule } );
    }
    ui.add_back( "\n[恢复]\n" )
      .add_back(
        std::format( " > {} ", core::custom_rules.showed_name ), core::rule_op{ core::rule_op::mod::restore, core::custom_rules } );
    for ( const auto &rule : core::builtin_rules ) {
        ui.add_back( std::format( " > {} ", rule.showed_name ), core::rule_op{ core::rule_op::mod::restore, rule } );
    }
    ui.show().lock( true, true );
    return EXIT_SUCCESS;
}
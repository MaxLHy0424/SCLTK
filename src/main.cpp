#include "core.hpp"
auto main() -> int
{
    cpp_utils::console_ui_ansi ui{};
    ui.lock( true, true ).set_console( INFO_SHORT_NAME, CHARSET_ID, CONSOLE_WIDTH, CONSOLE_HEIGHT, true, false, true, 255 );
    std::print( " -> 检测运行权限.\n" );
    if ( !cpp_utils::is_run_as_admin() ) {
        std::print( " -> 申请管理员权限.\n" );
        cpp_utils::relaunch_as_admin();
    }
    core::config_op{ core::config_op::mod::load }( cpp_utils::console_ui_ansi::func_args{ ui } );
    cpp_utils::multithread_task_ansi threads{};
    threads.add( "置顶显示", core::topmost_show_window )
      .add( "窗口属性设定", core::set_console_attrs )
      .add( "修复操作系统环境", core::fix_os_env );
    std::print( " -> 准备用户界面.\n" );
    ui.add_back( "                    [ 主  页 ]\n\n" )
      .add_back( " < 退出 ", core::quit, cpp_utils::console_value::text_foreground_red | cpp_utils::console_value::text_foreground_intensity )
      .add_back(
        " < 重启 ", core::relaunch,
        cpp_utils::console_value::text_foreground_green | cpp_utils::console_value::text_foreground_intensity )
      .add_back( " > 信息 ", core::info )
      .add_back( " > 配置 ", core::config_op{ core::config_op::mod::edit } )
      .add_back( " > 工具箱 ", core::toolkit )
      .add_back( "\n[ 破解 ]\n" )
      .add_back(
        std::format( " > {} ", core::custom_rules.showed_name ), core::rule_op{ core::rule_op::mod::crack, core::custom_rules } );
    for ( const auto &rule : core::builtin_rules ) {
        ui.add_back( std::format( " > {} ", rule.showed_name ), core::rule_op{ core::rule_op::mod::crack, rule } );
    }
    ui.add_back( "\n[ 恢复 ]\n" )
      .add_back(
        std::format( " > {} ", core::custom_rules.showed_name ), core::rule_op{ core::rule_op::mod::restore, core::custom_rules } );
    for ( const auto &rule : core::builtin_rules ) {
        ui.add_back( std::format( " > {} ", rule.showed_name ), core::rule_op{ core::rule_op::mod::restore, rule } );
    }
    ui.show().lock( true, true );
    return EXIT_SUCCESS;
}
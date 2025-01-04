#include "console_ui.hpp"
#include "core.hpp"
auto main() -> int
{
    auto ui{ console_ui{} };
    ui.lock( true, true ).set_console( WINDOW_TITLE, CODE_PAGE_CODE, WINDOW_WIDTH, WINDOW_HEIGHT, true, false, true, 255 );
    std::print( " -> 检测运行权限.\n" );
    if ( !core::is_run_as_admin() ) {
        std::print( " -> 申请管理员权限.\n" );
        core::relaunch_as_admin( console_ui::func_args{ ui } );
        return EXIT_SUCCESS;
    }
    auto set_window_thread{
      core::set_window{ core::options[ 1 ][ 0 ].is_enabled, core::options[ 1 ][ 1 ].is_enabled, core::options[ 1 ][ 2 ].is_enabled }
    };
    auto fix_os_env_thread{ core::fix_os_env{ core::options[ 2 ][ 0 ].is_enabled } };
    core::config_op{ core::config_op::mod::load }( console_ui::func_args{ ui } );
    std::print( " -> 准备用户界面.\n" );
    ui.add_back( "                    [ 主  页 ]\n\n" )
      .add_back( " < 退出 ", core::quit, console_ui::value::text_foreground_red | console_ui::value::text_foreground_intensity )
      .add_back( " < 重启 ", core::relaunch_as_admin, console_ui::value::text_foreground_green | console_ui::value::text_foreground_intensity )
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
        std::format( " > {} ", core::custom_rules.showed_name ), core::rule_op{ core::rule_op::mod::recovery, core::custom_rules } );
    for ( const auto &rule : core::builtin_rules ) {
        ui.add_back( std::format( " > {} ", rule.showed_name ), core::rule_op{ core::rule_op::mod::recovery, rule } );
    }
    ui.show().lock( true, true );
    return EXIT_SUCCESS;
}
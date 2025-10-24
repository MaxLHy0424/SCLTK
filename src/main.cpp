#include "core.hpp"
auto main() -> int
{
    using namespace std::string_view_literals;
    core::con.ignore_exit_signal( true )
      .fix_size( true )
      .set_charset( core::charset_id )
      .set_title( INFO_SHORT_NAME )
      .set_size( core::console_width, core::console_height )
      .set_translucency( 255 )
      .enable_window_maximize_ctrl( false )
      .enable_window_minimize_ctrl( false )
      .enable_window_close_ctrl( false );
    cpp_utils::console_ui ui{ core::con };
    ui.set_constraints( true, true );
    std::print( " -> 准备就绪..." );
    core::load_config();
    core::create_threads();
    ui.reserve( 10 + core::builtin_rules.size() )
      .add_back( "                    [ 主  页 ]\n\n"sv )
      .add_back( " < 退出 "sv, core::quit, cpp_utils::console_text::foreground_red | cpp_utils::console_text::foreground_intensity )
      .add_back( " < 重启 "sv, core::relaunch, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
      .add_back( " > 关于 "sv, core::info )
      .add_back( " > 配置 "sv, core::config_ui )
      .add_back( " > 工具箱 "sv, core::toolkit )
      .add_back( ""sv )
      .add_back(
        core::make_executor_mode_ui_text(), core::flip_executor_mode,
        cpp_utils::console_text::foreground_red | cpp_utils::console_text::foreground_green )
      .add_back( " > 全部执行\n"sv, core::execute_all_rules )
      .add_back( " > 自定义 "sv, std::bind_back( core::execute_rules, std::cref( core::custom_rules ) ) );
    for ( const auto& rule : core::builtin_rules ) {
        ui.add_back( std::format( " > {} ", rule.shown_name ), std::bind_back( core::execute_rules, std::cref( rule ) ) );
    }
    ui.show();
    return EXIT_SUCCESS;
}
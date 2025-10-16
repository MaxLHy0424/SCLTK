#include "core.hpp"
auto main() -> int
{
    using namespace std::string_view_literals;
    cpp_utils::ignore_current_console_exit_signal( true );
    cpp_utils::fix_window_size( core::window_handle, true );
    cpp_utils::set_current_console_charset( core::charset_id );
    cpp_utils::set_current_console_title( INFO_SHORT_NAME );
    cpp_utils::set_console_size( core::window_handle, core::std_output_handle, core::console_width, core::console_height );
    cpp_utils::set_window_translucency( core::window_handle, 255 );
    cpp_utils::enable_window_maximize_ctrl( core::window_handle, false );
    cpp_utils::enable_window_minimize_ctrl( core::window_handle, false );
    cpp_utils::enable_window_close_ctrl( core::window_handle, false );
    cpp_utils::console_ui ui{ core::std_input_handle, core::std_output_handle };
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
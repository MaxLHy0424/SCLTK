#include "core.hpp"
auto main() -> int
{
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
    ui.set_limits( true, true );
    std::print( " -> 准备就绪..." );
    core::load_config();
    cpp_utils::thread_manager{}.add( core::set_console_attrs ).add( core::keep_window_top ).add( core::fix_os_env ).detach();
    ui.add_back( "                    [ 主  页 ]\n\n" )
      .add_back( " < 退出 ", core::quit, cpp_utils::console_text::foreground_red | cpp_utils::console_text::foreground_intensity )
      .add_back( " < 重启 ", core::relaunch, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
      .add_back( " > 关于 ", core::info )
      .add_back( " > 配置 ", core::config_ui )
      .add_back( " > 工具箱 ", core::toolkit )
      .add_back( "\n[ 破解 ]\n" )
      .add_back( std::format( " > {} ", core::custom_rules.shown_name ), core::crack{ core::custom_rules } );
    for ( const auto& rule : core::builtin_rules ) {
        ui.add_back( std::format( " > {} ", rule.shown_name ), core::crack{ rule } );
    }
    ui.add_back( "\n[ 恢复 ]\n" )
      .add_back( std::format( " > {} ", core::custom_rules.shown_name ), core::restore{ core::custom_rules } );
    for ( const auto& rule : core::builtin_rules ) {
        ui.add_back( std::format( " > {} ", rule.shown_name ), core::restore{ rule } );
    }
    ui.show().set_limits( true, true );
    std::print( " -> 正在清理..." );
    return EXIT_SUCCESS;
}
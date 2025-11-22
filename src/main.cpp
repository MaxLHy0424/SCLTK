#include "scltk.hpp"
auto main() -> int
{
    using namespace std::string_view_literals;
    scltk::con.ignore_exit_signal( true )
      .fix_size( true )
      .set_charset( scltk::charset_id )
      .set_title( L"" INFO_SHORT_NAME )
      .set_size( scltk::console_width, scltk::console_height, scltk::unsynced_mem_pool )
      .set_translucency( 255 )
      .enable_window_maximize_ctrl( false )
      .enable_window_minimize_ctrl( false )
      .enable_window_close_ctrl( false );
    cpp_utils::console_ui ui{ scltk::con, scltk::unsynced_mem_pool };
    ui.set_constraints( true, true );
    std::print( " -> 准备就绪..." );
    scltk::load_config();
    scltk::create_threads();
    ui.reserve( 8 + scltk::builtin_rules.size() )
      .add_back( "                    [ 主  页 ]\n\n"sv )
      .add_back( " < 退出\n"sv, scltk::quit, cpp_utils::console_text::foreground_red | cpp_utils::console_text::foreground_intensity )
      .add_back( " > 关于 "sv, scltk::info )
      .add_back( " > 配置 "sv, scltk::config_ui )
      .add_back( " > 工具箱\n"sv, scltk::toolkit )
      .add_back(
        scltk::make_executor_mode_ui_text(), scltk::flip_executor_mode,
        cpp_utils::console_text::foreground_red | cpp_utils::console_text::foreground_green )
      .add_back( " > 全部执行\n"sv, scltk::execute_all_rules )
      .add_back( " > 自定义 "sv, std::bind_back( scltk::execute_rules, std::cref( scltk::custom_rules ) ) );
    for ( const auto& rule : scltk::builtin_rules ) {
        ui.add_back( std::format( " > {} ", rule.shown_name ), std::bind_back( scltk::execute_rules, std::cref( rule ) ) );
    }
    ui.show();
}
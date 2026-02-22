#include "scltk.hpp"
auto main() -> int
{
    using namespace std::string_view_literals;
    scltk::con.set_charset( scltk::charset_id );
    std::print( " -> 准备就绪." );
    scltk::con.ignore_exit_signal( true )
      .set_title( L"" INFO_SHORT_NAME )
      .show_cursor( false )
      .fix_size( true )
      .lock_text( true )
      .set_size( scltk::console_width, scltk::console_height, scltk::unsynced_mem_pool )
      .enable_window_maximize_ctrl( false )
      .enable_window_minimize_ctrl( false )
      .enable_window_close_ctrl( false );
    scltk::elevate_privilege();
    scltk::load_config( false );
    scltk::create_parallel_tasks();
    cpp_utils::console_ui ui{ scltk::con, scltk::unsynced_mem_pool };
    ui.reserve( 9 + scltk::builtin_rules_t::size )
      .add_back( scltk::make_title_text< "[ 主  页 ]", 2 >.view() )
      .add_back( " < 退出 ", scltk::quit, cpp_utils::console_text::foreground_red | cpp_utils::console_text::foreground_intensity )
      .add_back( " < 重启\n", scltk::relaunch, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
      .add_back( " > 关于 ", scltk::info )
      .add_back( " > 配置 ", scltk::config_ui )
      .add_back( " > 工具箱\n", scltk::toolkit )
      .add_back(
        scltk::make_executor_mode_ui_text(), scltk::flip_executor_mode,
        cpp_utils::console_text::foreground_red | cpp_utils::console_text::foreground_green )
      .add_back( " > 全部执行\n", scltk::all_rules_t{} )
      .add_back( " > * 自定义 * ", scltk::rule_executor< scltk::custom_rule_executor_backend >{} );
    [ & ]< typename... Nodes >( const cpp_utils::type_list< Nodes... > )
    {
        ( ui.add_back(
            scltk::make_button_text< Nodes::display_name >.view(),
            scltk::rule_executor< scltk::builtin_rules_executor_backend< Nodes > >{} ),
          ... );
    }( scltk::builtin_rules_t{} );
    ui.show();
}
#include "scltk.hpp"
auto main() -> int
{
    using namespace std::string_view_literals;
    ( void ) cpp_utils::elevate_privilege();
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
    std::print( " -> 准备就绪." );
    scltk::load_config( false );
    scltk::create_threads();
    ui.reserve( 9 + scltk::builtin_rules_t::size )
      .add_back( scltk::make_title_text< "[ 主  页 ]", 2 >.view() )
      .add_back( " < 退出 "sv, scltk::quit, cpp_utils::console_text::foreground_red | cpp_utils::console_text::foreground_intensity )
      .add_back( " < 重启\n"sv, scltk::relaunch, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
      .add_back( " > 关于 "sv, scltk::info )
      .add_back( " > 配置 "sv, scltk::config_ui )
      .add_back( " > 工具箱\n"sv, scltk::toolkit )
      .add_back(
        scltk::make_executor_mode_ui_text(), scltk::flip_executor_mode,
        cpp_utils::console_text::foreground_red | cpp_utils::console_text::foreground_green )
      .add_back( " > 全部执行\n"sv, scltk::all_rules_t{} )
      .add_back( " > 自定义 "sv, scltk::rule_executor< scltk::custom_rule_executor_backend >{} );
    [ & ]< typename... Nodes >( const cpp_utils::type_list< Nodes... > )
    {
        ( ui.add_back(
            scltk::make_button_text< Nodes::display_name >.view(),
            scltk::rule_executor< scltk::builtin_rules_executor_backend< Nodes > >{} ),
          ... );
    }( scltk::builtin_rules_t{} );
    ui.show();
}
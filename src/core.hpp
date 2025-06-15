#pragma once
#include <array>
#include <fstream>
#include <tuple>
#include <variant>
#include "cpp_utils/const_string.hpp"
#include "cpp_utils/math.hpp"
#include "cpp_utils/multithread.hpp"
#include "cpp_utils/pointer.hpp"
#include "cpp_utils/windows_app_tools.hpp"
#include "cpp_utils/windows_console_ui.hpp"
#include "info.hpp"
namespace core
{
    using namespace std::chrono_literals;
    using namespace std::string_view_literals;
    using size_t = cpp_utils::size_t;
    inline constexpr SHORT console_width{ 50 };
    inline constexpr SHORT console_height{ 25 };
    inline constexpr UINT charset_id{ 936 };
    inline constexpr auto default_thread_sleep_time{ 1s };
    inline constexpr auto default_execution_sleep_time{ 50ms };
    inline constexpr auto config_file_name{ "config.ini" };
    inline constexpr auto func_back{ cpp_utils::console_ui::func_back };
    inline constexpr auto func_exit{ cpp_utils::console_ui::func_exit };
    inline const auto window_handle{ GetConsoleWindow() };
    inline const auto std_input_handle{ GetStdHandle( STD_INPUT_HANDLE ) };
    inline const auto std_output_handle{ GetStdHandle( STD_OUTPUT_HANDLE ) };
    inline const auto divider{ cpp_utils::make_repeated_const_string< console_width, '-' >() };
    static_assert( default_thread_sleep_time.count() != 0 );
    static_assert( default_execution_sleep_time.count() != 0 );
    using ui_func_args = cpp_utils::console_ui::func_args;
    inline auto quit( ui_func_args ) noexcept
    {
        return func_exit;
    }
    inline auto relaunch( ui_func_args ) noexcept
    {
        cpp_utils::relaunch_as_admin( EXIT_SUCCESS );
        return func_exit;
    }
    inline auto wait() noexcept
    {
        std::print( "\n\n" );
        for ( unsigned short i{ 3 }; i > 0; --i ) {
            std::print( " {}s 后返回.\r", i );
            std::this_thread::sleep_for( 1s );
        }
    }
    inline auto make_progress( const size_t now, const size_t total, const size_t digits_of_total ) noexcept
    {
        return std::format( "({}{}/{})", std::string( digits_of_total - cpp_utils::count_digits( now ), ' ' ), now, total );
    }
    struct option_set final
    {
        struct item final
        {
          private:
            std::atomic< bool > value_{ false };
          public:
            const char* self_name;
            const char* shown_name;
            auto set( const bool value ) noexcept
            {
                value_.store( value, std::memory_order_release );
            }
            auto get() const noexcept
            {
                return value_.load( std::memory_order_acquire );
            }
            operator bool() const noexcept
            {
                return get();
            }
            auto operator=( const item& ) -> item& = delete;
            auto operator=( item&& ) -> item&      = delete;
            item( const char* const self_name, const char* const shown_name )
              : self_name{ self_name }
              , shown_name{ shown_name }
            { }
            item( const item& src )
              : value_{ src }
              , self_name{ src.self_name }
              , shown_name{ src.shown_name }
            { }
            item( item& src )
              : value_{ src }
              , self_name{ src.self_name }
              , shown_name{ src.shown_name }
            { }
            ~item() = default;
        };
        struct category final
        {
            const char* self_name;
            const char* shown_name;
            std::vector< item > items;
            auto& operator[]( const std::string_view self_name )
            {
                for ( auto& item : items ) {
                    if ( self_name == item.self_name ) {
                        return item;
                    }
                }
                std::unreachable();
            }
            const auto& operator[]( const std::string_view self_name ) const
            {
                for ( const auto& item : items ) {
                    if ( self_name == item.self_name ) {
                        return item;
                    }
                }
                std::unreachable();
            }
            auto operator=( const category& ) -> category& = delete;
            auto operator=( category&& ) -> category&      = delete;
            category( const char* const self_name, const char* const shown_name, std::vector< item > items )
              : self_name{ self_name }
              , shown_name{ shown_name }
              , items{ std::move( items ) }
            { }
            category( const category& ) = default;
            category( category&& )      = default;
            ~category()                 = default;
        };
        std::vector< category > categories;
        auto& operator[]( const std::string_view self_name ) noexcept
        {
            for ( auto& category : categories ) {
                if ( self_name == category.self_name ) {
                    return category;
                }
            }
            std::unreachable();
        }
        const auto& operator[]( const std::string_view self_name ) const noexcept
        {
            for ( const auto& category : categories ) {
                if ( self_name == category.self_name ) {
                    return category;
                }
            }
            std::unreachable();
        }
        option_set( std::vector< category > categories )
          : categories{ std::move( categories ) }
        { }
    };
    inline option_set opt_set{
      { { { "crack_restore",
            "破解与恢复",
            { { "hijack_execs", "劫持可执行文件" }, { "set_serv_startup_types", "设置服务启动类型" } } },
          { "window",
            "窗口显示",
            { { "keep_window_top", "* 置顶窗口" }, { "minimalist_titlebar", "* 极简标题栏" }, { "translucent", "* 半透明" } } },
          { "misc", "杂项", { { "disable_x_option_hot_reload", "** 禁用标 * 选项热重载" } } } } } };
    inline const auto& is_disable_x_option_hot_reload{ opt_set[ "misc" ][ "disable_x_option_hot_reload" ] };
    struct rule_node final
    {
        using item_t      = std::string;
        using container_t = std::deque< item_t >;
        const char* shown_name;
        container_t execs;
        container_t servs;
        auto empty() const noexcept
        {
            return execs.empty() && servs.empty();
        }
        auto operator=( const rule_node& ) -> rule_node& = delete;
        auto operator=( rule_node&& ) -> rule_node&      = delete;
        rule_node( const char* const shown_name, container_t execs, container_t servs )
          : shown_name{ shown_name }
          , execs{ std::move( execs ) }
          , servs{ std::move( servs ) }
        { }
        rule_node( const rule_node& ) = default;
        rule_node( rule_node&& )      = default;
        ~rule_node()                  = default;
    };
    inline rule_node custom_rules{ "自定义", {}, {} };
    inline const std::array< rule_node, 3 > builtin_rules{
      { { "极域电子教室",
          { "StudentMain", "DispcapHelper", "VRCwPlayer", "InstHelpApp", "InstHelpApp64", "TDOvrSet", "GATESRV", "ProcHelper64",
            "MasterHelper" },
          { "TDNetFilter", "TDFileFilter", "STUDSRV" } },
       { "联想智能云教室",
          { "vncviewer", "tvnserver32", "WfbsPnpInstall", "WFBSMon", "WFBSMlogon", "WFBSSvrLogShow", "ResetIp", "FuncForWIN64",
            "CertMgr", "Fireware", "BCDBootCopy", "refreship", "lenovoLockScreen", "PortControl64", "DesktopCheck",
            "DeploymentManager", "DeploymentAgent", "XYNTService" },
          { "BSAgentSvr", "tvnserver", "WFBSMlogon" } },
       { "红蜘蛛多媒体网络教室",
          { "rscheck", "checkrs", "REDAgent", "PerformanceCheck", "edpaper", "Adapter", "repview", "FormatPaper" },
          { "appcheck2", "checkapp2" } } }
    };
    class config_node_impl
    {
      public:
        const cpp_utils::raw_pointer_wrapper< const char* > self_name;
        auto load( this auto&& self, const bool is_reload, std::string& line )
        {
            self.load_( is_reload, line );
        }
        auto sync( this auto&& self, std::ofstream& out )
        {
            self.sync_( out );
        }
        auto prepare_reload( this auto&& self )
        {
            using child_t = std::decay_t< decltype( self ) >;
            if constexpr ( requires( child_t obj ) { obj.prepare_reload_(); } ) {
                self.prepare_reload_();
            }
        }
        auto ui( this auto&& self, cpp_utils::console_ui& parent_ui )
        {
            using child_t = std::decay_t< decltype( self ) >;
            if constexpr ( requires( child_t obj ) { obj.ui_( parent_ui ); } ) {
                self.ui_( parent_ui );
            }
        }
        config_node_impl( const char* const self_name ) noexcept
          : self_name{ self_name }
        { }
        ~config_node_impl() noexcept = default;
    };
    class options final : public config_node_impl
    {
      private:
        friend config_node_impl;
        static constexpr auto format_string_{ "{}.{}: {}" };
        static auto make_swith_button_text_( const auto is_enable )
        {
            return std::format( " > {}用 ", is_enable ? "禁" : "启" );
        }
        auto load_( const bool is_reload, std::string& line )
        {
            if ( is_reload ) {
                return;
            }
            for ( auto& category : opt_set.categories ) {
                for ( auto& item : category.items ) {
                    if ( line == std::format( format_string_, category.self_name, item.self_name, true ) ) {
                        item.set( true );
                    } else if ( line == std::format( format_string_, category.self_name, item.self_name, false ) ) {
                        item.set( false );
                    }
                }
            }
        }
        auto sync_( std::ofstream& out )
        {
            for ( const auto& category : opt_set.categories ) {
                for ( const auto& item : category.items ) {
                    out << std::format( format_string_, category.self_name, item.self_name, item.get() ) << '\n';
                }
            }
        }
        auto ui_( cpp_utils::console_ui& ui )
        {
            constexpr auto option_ctrl_color{ cpp_utils::console_text::foreground_red | cpp_utils::console_text::foreground_green };
            using category_t = option_set::category;
            using item_t     = option_set::item;
            class option_setter final
            {
              private:
                item_t& item_;
              public:
                auto operator()( ui_func_args args )
                {
                    item_.set( !item_.get() );
                    args.parent_ui.edit_text( args.node_index, make_swith_button_text_( item_ ) );
                    return func_back;
                }
                option_setter( item_t& item ) noexcept
                  : item_{ item }
                { }
                ~option_setter() noexcept = default;
            };
            class option_ui final
            {
              private:
                category_t& category_;
              public:
                auto operator()( ui_func_args )
                {
                    cpp_utils::console_ui ui;
                    ui.add_back( "                    [ 配  置 ]\n\n" )
                      .add_back(
                        std::format( " < 折叠 {} ", category_.shown_name ), quit,
                        cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity );
                    for ( auto& item : category_.items ) {
                        ui.add_back( std::format( "\n[ {} ]\n", item.shown_name ) )
                          .add_back( make_swith_button_text_( item ), option_setter{ item }, option_ctrl_color );
                    }
                    ui.show();
                    return func_back;
                }
                option_ui( category_t& category ) noexcept
                  : category_{ category }
                { }
                ~option_ui() noexcept = default;
            };
            ui.add_back( std::format(
              "\n[ 选项 ]\n\n"
              " (i) 选项相关信息可参阅文档.\n"
              "     标 * 选项热重载每 {} 自动执行, 可禁用.\n"
              "     标 ** 选项无法热重载. 其余选项可实时热重载.\n",
              default_thread_sleep_time ) );
            for ( auto& category : opt_set.categories ) {
                ui.add_back( std::format( " > {} ", category.shown_name ), option_ui{ category }, option_ctrl_color );
            }
        }
      public:
        options() noexcept
          : config_node_impl{ "opt_set" }
        { }
        ~options() noexcept = default;
    };
    class custom_rules_execs final : public config_node_impl
    {
      private:
        friend config_node_impl;
        auto load_( const bool, std::string& line )
        {
            custom_rules.execs.emplace_back( std::move( line ) );
        }
        auto sync_( std::ofstream& out )
        {
            for ( const auto& exec : custom_rules.execs ) {
                out << exec << '\n';
            }
        }
        auto prepare_reload_() noexcept
        {
            custom_rules.execs.clear();
        }
      public:
        custom_rules_execs() noexcept
          : config_node_impl{ "custom_rules_execs" }
        { }
        ~custom_rules_execs() noexcept = default;
    };
    class custom_rules_servs final : public config_node_impl
    {
      private:
        friend config_node_impl;
        auto load_( const bool, std::string& line )
        {
            custom_rules.servs.emplace_back( std::move( line ) );
        }
        auto sync_( std::ofstream& out )
        {
            for ( const auto& serv : custom_rules.servs ) {
                out << serv << '\n';
            }
        }
        auto prepare_reload_() noexcept
        {
            custom_rules.servs.clear();
        }
      public:
        custom_rules_servs() noexcept
          : config_node_impl{ "custom_rules_servs" }
        { }
        ~custom_rules_servs() noexcept = default;
    };
    inline std::tuple< options, custom_rules_execs, custom_rules_servs > config_nodes{};
    inline consteval auto check_config_nodes_validity( auto ) -> std::false_type;
    template < typename... Ts >
        requires( std::is_base_of_v< config_node_impl, Ts > && ... )
    inline consteval auto check_config_nodes_validity( std::tuple< Ts... > ) -> std::true_type;
    static_assert( decltype( check_config_nodes_validity( config_nodes ) )::value == true );
    using unknown_config_node_t = void*;
    constexpr unknown_config_node_t unknown_config_node{ nullptr };
    template < typename... Ts >
    inline consteval auto get_config_node_variant_t( std::tuple< Ts... > )
      -> std::variant< unknown_config_node_t, std::add_pointer_t< Ts >... >;
    inline auto load_config( const bool is_reload = false )
    {
        std::ifstream config_file{ config_file_name, std::ios::in };
        if ( !config_file.good() ) {
            return;
        }
        std::apply( []( auto&&... config_node ) { ( config_node.prepare_reload(), ... ); }, config_nodes );
        std::string line;
        std::string_view line_view;
        decltype( get_config_node_variant_t( config_nodes ) ) current_config_node;
        while ( std::getline( config_file, line ) ) {
            line_view = line;
            if ( line_view.empty() ) {
                continue;
            }
            if ( line_view.front() == '#' ) {
                continue;
            }
            if ( line_view.size() >= sizeof( "[  ]" ) && line_view.substr( 0, sizeof( "[ " ) - 1 ) == "[ "
                 && line_view.substr( line_view.size() - sizeof( " ]" ) + 1, line_view.size() ) == " ]" )
            {
                line_view           = line_view.substr( sizeof( "[ " ) - 1, line_view.size() - sizeof( " ]" ) - 1 );
                current_config_node = unknown_config_node;
                std::apply( [ & ]( auto&&... config_node )
                {
                    ( [ & ]( auto&& current_node ) noexcept
                    {
                        if ( line_view == current_node.self_name.get() ) {
                            current_config_node = &current_node;
                        }
                    }( config_node ), ... );
                }, config_nodes );
                continue;
            }
            current_config_node.visit( [ & ]( const auto node_ptr )
            {
                if constexpr ( !std::is_same_v< std::decay_t< decltype( node_ptr ) >, unknown_config_node_t > ) {
                    node_ptr->load( is_reload, line );
                }
            } );
        }
    }
    inline auto config_ui( ui_func_args )
    {
        auto sync{ []( ui_func_args ) static
        {
            std::print(
              "                    [ 配  置 ]\n\n\n"
              " -> 正在同步配置...\n" );
            load_config( true );
            std::ofstream config_file_stream{ config_file_name, std::ios::out | std::ios::trunc };
            config_file_stream << "# " INFO_FULL_NAME "\n# " INFO_VERSION "\n";
            std::apply( [ & ]( auto&&... config_node )
            {
                ( ( config_file_stream << std::format( "[ {} ]\n", config_node.self_name.get() ),
                    config_node.sync( config_file_stream ) ),
                  ... );
            }, config_nodes );
            config_file_stream << std::flush;
            const auto is_good{ config_file_stream.good() };
            std::print( "\n ({}) 同步配置{}.", is_good ? 'i' : '!', is_good ? "成功" : "失败" );
            wait();
            return func_back;
        } };
        auto open_file{ []( ui_func_args ) static
        {
            if ( std::ifstream{ config_file_name, std::ios::in }.good() ) {
                std::print(
                  "                    [ 配  置 ]\n\n\n"
                  " -> 正在打开配置文件..." );
                ShellExecuteA( nullptr, "open", config_file_name, nullptr, nullptr, SW_SHOWNORMAL );
                return func_back;
            }
            std::print(
              "                    [ 配  置 ]\n\n\n"
              " (!) 无法打开配置文件." );
            wait();
            return func_back;
        } };
        cpp_utils::console_ui ui;
        ui.add_back( "                    [ 配  置 ]\n\n" )
          .add_back( " < 返回 ", quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
          .add_back( " > 同步配置 ", sync )
          .add_back( " > 打开配置文件 ", open_file );
        std::apply( [ & ]( auto&&... config_node ) { ( config_node.ui( ui ), ... ); }, config_nodes );
        ui.show();
        return func_back;
    }
    inline auto info( ui_func_args )
    {
        auto visit_repo_webpage{ []( ui_func_args ) static
        {
            ShellExecuteA( nullptr, "open", INFO_REPO_URL, nullptr, nullptr, SW_SHOWNORMAL );
            return func_back;
        } };
        cpp_utils::console_ui ui;
        ui.add_back( "                    [ 关  于 ]\n\n" )
          .add_back( " < 返回 ", quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
          .add_back(
            "\n[ 名称 ]\n\n " INFO_FULL_NAME " (缩写: " INFO_SHORT_NAME ")\n\n[ 版本 ]\n\n " INFO_VERSION
            "\n\n 构建时间: " INFO_BUILD_TIME "\n 编译工具: " INFO_COMPILER " " INFO_ARCH
            "\n\n[ 许可证& 版权 ]\n\n " INFO_LICENSE "\n " INFO_COPYRIGHT "\n\n[ 仓库 ]\n" )
          .add_back(
            " " INFO_REPO_URL " ", visit_repo_webpage,
            cpp_utils::console_text::default_attrs | cpp_utils::console_text::foreground_intensity
              | cpp_utils::console_text::lvb_underscore )
          .show();
        return func_back;
    }
    inline auto toolkit( ui_func_args )
    {
        auto launch_cmd{ []( ui_func_args args ) static noexcept
        {
            cpp_utils::set_current_console_title( INFO_SHORT_NAME " - 命令提示符" );
            cpp_utils::set_console_size( window_handle, std_output_handle, 120, 30 );
            cpp_utils::fix_window_size( window_handle, false );
            cpp_utils::enable_window_maximize_ctrl( window_handle, true );
            args.parent_ui.set_limits( false, false );
            SetConsoleScreenBufferSize( std_output_handle, COORD{ 127, SHRT_MAX - 1 } );
            std::system( "cmd.exe" );
            cpp_utils::set_current_console_charset( charset_id );
            cpp_utils::set_current_console_title( INFO_SHORT_NAME );
            cpp_utils::set_console_size( window_handle, std_output_handle, console_width, console_height );
            cpp_utils::fix_window_size( window_handle, true );
            cpp_utils::enable_window_maximize_ctrl( window_handle, false );
            return func_back;
        } };
        auto restore_several_disabled_os_components{ []( ui_func_args ) static
        {
            std::print(
              "                   [ 工 具 箱 ]\n\n\n"
              " -> 正在尝试恢复...\n\n{}\n\n",
              divider.data() );
            constexpr std::array reg_dirs{
              R"(Software\Policies\Microsoft\Windows\System)", R"(Software\Microsoft\Windows\CurrentVersion\Policies\System)",
              R"(Software\Microsoft\Windows\CurrentVersion\Policies\Explorer)", R"(Software\Policies\Microsoft\MMC\)" };
            constexpr std::array execs{
              "taskkill", "sc", "net", "reg", "cmd", "taskmgr", "perfmon", "regedit", "mmc", "dism", "sfc" };
            constexpr auto total_count{ reg_dirs.size() + execs.size() };
            constexpr auto digits_of_total{ cpp_utils::count_digits( total_count ) };
            size_t finished_count{ 0 };
            for ( const auto& reg_dir : reg_dirs ) {
                std::print(
                  "{} 删除注册表项 (0x{:x}).\n", make_progress( ++finished_count, total_count, digits_of_total ),
                  RegDeleteTreeA( HKEY_CURRENT_USER, reg_dir ) );
                std::this_thread::sleep_for( default_execution_sleep_time );
            }
            for ( const auto& exec : execs ) {
                std::print(
                  "{} 撤销劫持 {}.exe (0x{:x}).\n", make_progress( ++finished_count, total_count, digits_of_total ), exec,
                  RegDeleteTreeA(
                    HKEY_LOCAL_MACHINE,
                    std::format( R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\{}.exe)", exec )
                      .c_str() ) );
                std::this_thread::sleep_for( default_execution_sleep_time );
            }
            std::print( "\n{}\n\n (i) 操作已完成.", divider.data() );
            wait();
            return func_back;
        } };
        struct cmd_item final
        {
            const char* description;
            const char* command;
        };
        class cmd_executor final
        {
          private:
            const cmd_item& item_;
          public:
            auto operator()( ui_func_args )
            {
                auto execute{ [ this ]( ui_func_args )
                {
                    std::print(
                      "                   [ 工 具 箱 ]\n\n\n"
                      " -> 正在执行操作系统命令...\n{}\n",
                      divider.data() );
                    std::system( item_.command );
                    return func_exit;
                } };
                cpp_utils::console_ui ui;
                ui.add_back( "                   [ 工 具 箱 ]\n\n" )
                  .add_back( " (i) 是否继续执行?\n" )
                  .add_back( " > 是, 继续执行 ", execute, cpp_utils::console_text::foreground_red | cpp_utils::console_text::foreground_intensity )
                  .add_back( " > 否, 立即返回 ", quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
                  .show();
                return func_back;
            }
            cmd_executor( const cmd_item& item ) noexcept
              : item_{ item }
            { }
            cmd_executor( const cmd_executor& ) noexcept = default;
            cmd_executor( cmd_executor&& ) noexcept      = default;
            ~cmd_executor() noexcept                     = default;
        };
        constexpr std::array< cmd_item, 5 > common_cmds{
          { { "重启资源管理器", R"(taskkill.exe /f /im explorer.exe&& timeout.exe /t 3 /nobreak&& start C:\Windows\explorer.exe)" },
           { "重启至高级选项", "shutdown.exe /r /o /f /t 0" },
           { "恢复 USB 设备访问", R"(reg.exe add "HKLM\SYSTEM\CurrentControlSet\Services\USBSTOR" /f /t reg_dword /v Start /d 3)" },
           { "恢复 Google Chrome 离线游戏", R"(reg.exe delete "HKLM\SOFTWARE\Policies\Google\Chrome" /f /v AllowDinosaurEasterEgg)" },
           { "恢复 Microsoft Edge 离线游戏", R"(reg.exe delete "HKLM\SOFTWARE\Policies\Microsoft\Edge" /f /v AllowSurfGame)" } }
        };
        cpp_utils::console_ui ui;
        ui.add_back( "                   [ 工 具 箱 ]\n\n" )
          .add_back( " < 返回 ", quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
          .add_back( " > 命令提示符 ", launch_cmd )
          .add_back( " > 恢复部分被禁用的操作系统组件 ", restore_several_disabled_os_components )
          .add_back( "\n[ 常用操作 ]\n" );
        for ( const auto& common_cmd : common_cmds ) {
            ui.add_back( std::format( " > {} ", common_cmd.description ), cmd_executor{ common_cmd } );
        }
        ui.show();
        return func_back;
    }
    inline auto set_console_attrs()
    {
        const auto& window_options{ opt_set[ "window" ] };
        const auto& is_enable_minimalist_titlebar{ window_options[ "minimalist_titlebar" ] };
        const auto& is_translucent{ window_options[ "translucent" ] };
        if ( is_disable_x_option_hot_reload ) {
            cpp_utils::enable_window_menu( window_handle, !is_enable_minimalist_titlebar );
            cpp_utils::set_window_translucency( window_handle, is_translucent ? 230 : 255 );
            return;
        }
        while ( true ) {
            cpp_utils::enable_window_menu( window_handle, !is_enable_minimalist_titlebar );
            cpp_utils::set_window_translucency( window_handle, is_translucent ? 230 : 255 );
            std::this_thread::sleep_for( default_thread_sleep_time );
        }
    }
    inline auto keep_window_top()
    {
        const auto& is_keep_window_top{ opt_set[ "window" ][ "keep_window_top" ] };
        if ( is_disable_x_option_hot_reload && !is_keep_window_top ) {
            return;
        }
        constexpr auto sleep_time{ 100ms };
        const auto current_thread_id{ GetCurrentThreadId() };
        const auto current_window_thread_process_id{ GetWindowThreadProcessId( window_handle, nullptr ) };
        if ( is_disable_x_option_hot_reload ) {
            cpp_utils::loop_keep_window_top( window_handle, current_thread_id, current_window_thread_process_id, sleep_time );
            cpp_utils::cancel_top_window( window_handle );
            return;
        }
        while ( true ) {
            if ( !is_keep_window_top ) [[unlikely]] {
                cpp_utils::cancel_top_window( window_handle );
                std::this_thread::sleep_for( default_thread_sleep_time );
                continue;
            }
            cpp_utils::keep_window_top( window_handle, current_thread_id, current_window_thread_process_id );
            std::this_thread::sleep_for( sleep_time );
        }
        cpp_utils::cancel_top_window( window_handle );
    }
    using exec_const_ref_t = cpp_utils::add_const_lvalue_reference_t< rule_node::item_t >;
    using serv_const_ref_t = cpp_utils::add_const_lvalue_reference_t< rule_node::item_t >;
    inline const auto& option_crack_restore{ opt_set[ "crack_restore" ] };
    inline const auto& is_hijack_execs{ option_crack_restore[ "hijack_execs" ] };
    inline const auto& is_set_serv_startup_types{ option_crack_restore[ "set_serv_startup_types" ] };
    class crack final
    {
      private:
        const rule_node& rules_;
        static auto hijack_exec_( exec_const_ref_t exec ) noexcept
        {
            return cpp_utils::create_registry_key< charset_id >(
              HKEY_LOCAL_MACHINE,
              std::format( R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\{}.exe)", exec ).c_str(),
              "debugger", REG_SZ, reinterpret_cast< const BYTE* >( L"nul" ), sizeof( L"nul" ) );
        }
        static auto disable_serv_( serv_const_ref_t serv ) noexcept
        {
            return cpp_utils::set_service_status< charset_id >( serv.c_str(), SERVICE_DISABLED );
        }
        static auto kill_exec_( exec_const_ref_t exec ) noexcept
        {
            return cpp_utils::kill_process_by_name< charset_id >( std::format( "{}.exe", exec ).c_str() );
        }
        static auto stop_serv_( serv_const_ref_t serv ) noexcept
        {
            return cpp_utils::stop_service_with_dependencies< charset_id >( serv.c_str() );
        }
        auto engine_()
        {
            const auto& execs{ rules_.execs };
            const auto& servs{ rules_.servs };
            size_t finished_count{ 0 };
            const auto total_count{
              ( is_hijack_execs ? execs.size() * 2 : execs.size() )
              + ( is_set_serv_startup_types ? servs.size() * 2 : servs.size() ) };
            const auto digits_of_total{ cpp_utils::count_digits( total_count ) };
            if ( is_hijack_execs ) {
                for ( const auto& exec : execs ) {
                    std::print(
                      "{} 劫持文件 {}.exe (0x{:x}).\n", make_progress( ++finished_count, total_count, digits_of_total ), exec,
                      hijack_exec_( exec ) );
                    std::this_thread::sleep_for( default_execution_sleep_time );
                }
            }
            if ( is_set_serv_startup_types ) {
                for ( const auto& serv : servs ) {
                    std::print(
                      "{} 禁用服务 {} (0x{:x}).\n", make_progress( ++finished_count, total_count, digits_of_total ), serv,
                      disable_serv_( serv ) );
                    std::this_thread::sleep_for( default_execution_sleep_time );
                }
            }
            for ( const auto& exec : execs ) {
                std::print(
                  "{} 终止进程 {}.exe (0x{:x}).\n", make_progress( ++finished_count, total_count, digits_of_total ), exec,
                  kill_exec_( exec ) );
                std::this_thread::sleep_for( default_execution_sleep_time );
            }
            for ( const auto& serv : servs ) {
                std::print(
                  "{} 停止服务 {} (0x{:x}).\n", make_progress( ++finished_count, total_count, digits_of_total ), serv,
                  stop_serv_( serv ) );
                std::this_thread::sleep_for( default_execution_sleep_time );
            }
        }
      public:
        auto operator()( ui_func_args )
        {
            std::print( "                    [ 破  解 ]\n\n\n" );
            if ( rules_.empty() ) {
                std::print( " (i) 规则为空." );
                wait();
                return func_back;
            }
            std::print( " -> 正在执行...\n\n{}\n\n", divider.data() );
            engine_();
            std::print( "\n{}\n\n (i) 操作已完成.", divider.data() );
            wait();
            return func_back;
        }
        crack( const rule_node& rules ) noexcept
          : rules_{ rules }
        { }
        crack( const crack& )     = default;
        crack( crack&& ) noexcept = default;
        ~crack() noexcept         = default;
    };
    class restore final
    {
      private:
        const rule_node& rules_;
        static auto undo_hijack_exec_( exec_const_ref_t exec ) noexcept
        {
            return cpp_utils::delete_registry_tree< charset_id >(
              HKEY_LOCAL_MACHINE,
              std::format( R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\{}.exe)", exec ).c_str() );
        }
        static auto enable_serv_( serv_const_ref_t serv ) noexcept
        {
            return cpp_utils::set_service_status< charset_id >( serv.c_str(), SERVICE_AUTO_START );
        }
        static auto start_serv_( serv_const_ref_t serv ) noexcept
        {
            return cpp_utils::start_service_with_dependencies< charset_id >( serv.c_str() );
        }
        auto engine_()
        {
            const auto& execs{ rules_.execs };
            const auto& servs{ rules_.servs };
            size_t finished_count{ 0 };
            const auto total_count{
              ( is_hijack_execs ? execs.size() : 0ULL ) + ( is_set_serv_startup_types ? servs.size() * 2 : servs.size() ) };
            const auto digits_of_total{ cpp_utils::count_digits( total_count ) };
            if ( is_hijack_execs ) {
                for ( const auto& exec : execs ) {
                    std::print(
                      "{} 撤销劫持 {}.exe (0x{:x}).\n", make_progress( ++finished_count, total_count, digits_of_total ), exec,
                      undo_hijack_exec_( exec ) );
                    std::this_thread::sleep_for( default_execution_sleep_time );
                }
            }
            if ( is_set_serv_startup_types ) {
                for ( const auto& serv : servs ) {
                    std::print(
                      "{} 启用服务 {} (0x{:x}).\n", make_progress( ++finished_count, total_count, digits_of_total ), serv,
                      enable_serv_( serv ) );
                    std::this_thread::sleep_for( default_execution_sleep_time );
                }
            }
            for ( const auto& serv : servs ) {
                std::print(
                  "{} 启动服务 {} (0x{:x}).\n", make_progress( ++finished_count, total_count, digits_of_total ), serv,
                  start_serv_( serv ) );
                std::this_thread::sleep_for( default_execution_sleep_time );
            }
        }
      public:
        auto operator()( ui_func_args )
        {
            std::print( "                    [ 恢  复 ]\n\n\n" );
            if ( rules_.empty() ) {
                std::print( " (i) 规则为空." );
                wait();
                return func_back;
            }
            if ( !is_hijack_execs && rules_.servs.empty() ) {
                std::print( " (!) 当前配置下无可用恢复操作." );
                wait();
                return func_back;
            }
            std::print( " -> 正在执行...\n\n{}\n\n", divider.data() );
            engine_();
            std::print( "\n{}\n\n (i) 操作已完成.", divider.data() );
            wait();
            return func_back;
        }
        restore( const rule_node& rules ) noexcept
          : rules_{ rules }
        { }
        restore( const restore& )     = default;
        restore( restore&& ) noexcept = default;
        ~restore() noexcept           = default;
    };
}
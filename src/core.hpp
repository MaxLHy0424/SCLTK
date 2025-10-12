#pragma once
#include <cpp_utils/const_string.hpp>
#include <cpp_utils/math.hpp>
#include <cpp_utils/meta.hpp>
#include <cpp_utils/multithread.hpp>
#include <cpp_utils/windows_app_tools.hpp>
#include <cpp_utils/windows_console_ui.hpp>
#include <fstream>
#include <limits>
#include "info.hpp"
namespace core
{
    using namespace std::chrono_literals;
    using namespace std::string_view_literals;
    inline constexpr SHORT console_width{ 50 };
    inline constexpr SHORT console_height{ 25 };
    inline constexpr UINT charset_id{ 936 };
    inline constexpr auto default_thread_sleep_time{ 200ms };
    inline constexpr auto config_file_name{ "config.ini" };
    inline constexpr auto func_back{ cpp_utils::console_ui::func_back };
    inline constexpr auto func_exit{ cpp_utils::console_ui::func_exit };
    inline const auto window_handle{ GetConsoleWindow() };
    inline const auto std_input_handle{ GetStdHandle( STD_INPUT_HANDLE ) };
    inline const auto std_output_handle{ GetStdHandle( STD_OUTPUT_HANDLE ) };
    static_assert( default_thread_sleep_time.count() != 0 );
    using ui_func_args = cpp_utils::console_ui::func_args;
    inline auto quit() noexcept
    {
        return func_exit;
    }
    inline auto relaunch() noexcept
    {
        cpp_utils::relaunch_as_admin( EXIT_SUCCESS );
        return func_exit;
    }
    namespace details
    {
        inline auto press_any_key_to_return() noexcept
        {
            std::print( "\n\n 按任意键返回..." );
            cpp_utils::press_any_key_to_continue( std_input_handle );
        }
        inline constexpr auto is_whitespace( const char ch ) noexcept
        {
            switch ( ch ) {
                case '\t' :
                case '\v' :
                case '\f' :
                case ' ' : return true;
            }
            return false;
        }
    }
    struct rule_node final
    {
        using item_t      = const char*;
        using container_t = std::vector< item_t >;
        const char* shown_name{};
        container_t execs{};
        container_t servs{};
    };
    inline rule_node custom_rules;
    inline const std::array< rule_node, 4 > builtin_rules{
      { { "学生机房管理助手", { "jfglzs", "przs", "zmserv", "vprtt", "oporn" }, { "zmserv" } },
       { "极域电子教室",
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
    namespace details
    {
        struct initing_ui_only
        {
          protected:
            initing_ui_only()  = default;
            ~initing_ui_only() = default;
        };
        template < typename T >
        struct is_not_initing_ui_only final
        {
            static inline constexpr auto value{ !std::is_base_of_v< initing_ui_only, T > };
            is_not_initing_ui_only()  = delete;
            ~is_not_initing_ui_only() = delete;
        };
        template < typename T >
        inline constexpr auto is_not_initing_ui_only_v{ is_not_initing_ui_only< T >::value };
        class config_node_impl
        {
          public:
            const char* raw_name;
            auto load( this auto&& self, const std::string_view line )
            {
                using child_t = std::decay_t< decltype( self ) >;
                if constexpr ( is_not_initing_ui_only_v< child_t > ) {
                    self.load_( line );
                }
            }
            auto reload( this auto&& self, const std::string_view line )
            {
                using child_t = std::decay_t< decltype( self ) >;
                if constexpr ( is_not_initing_ui_only_v< child_t > && requires( child_t obj ) { obj.reload_( line ); } ) {
                    self.reload_( line );
                } else {
                    self.load( line );
                }
            }
            auto sync( this auto&& self, std::ofstream& out )
            {
                using child_t = std::decay_t< decltype( self ) >;
                if constexpr ( is_not_initing_ui_only_v< child_t > ) {
                    out << std::format( "[{}]\n", self.raw_name );
                    self.sync_( out );
                }
            }
            auto before_load( this auto&& self )
            {
                using child_t = std::decay_t< decltype( self ) >;
                if constexpr ( is_not_initing_ui_only_v< child_t > && requires( child_t obj ) { obj.before_load_(); } ) {
                    self.before_load_();
                }
            }
            auto after_load( this auto&& self )
            {
                using child_t = std::decay_t< decltype( self ) >;
                if constexpr ( is_not_initing_ui_only_v< child_t > && requires( child_t obj ) { obj.after_load_(); } ) {
                    self.after_load_();
                }
            }
            auto init_ui( this auto&& self, cpp_utils::console_ui& parent_ui )
            {
                using child_t = std::decay_t< decltype( self ) >;
                if constexpr ( requires( child_t obj ) { obj.init_ui_( parent_ui ); } ) {
                    self.init_ui_( parent_ui );
                }
            }
        };
        template < bool Atomic >
        class basic_options_config_node : public config_node_impl
        {
            friend config_node_impl;
          private:
            class item_ final
            {
              private:
                std::conditional_t< Atomic, std::atomic< bool >, bool > value_{ false };
              public:
                const char* description{ nullptr };
                auto get() const noexcept
                {
                    if constexpr ( Atomic ) {
                        return value_.load( std::memory_order_acquire );
                    } else {
                        return value_;
                    }
                }
                auto set( const bool value ) noexcept
                {
                    if constexpr ( Atomic ) {
                        value_.store( value, std::memory_order_release );
                    } else {
                        value_ = value;
                    }
                    return *this;
                }
                operator bool() const noexcept
                {
                    return get();
                }
                auto& operator=( const item_& src )
                {
                    description = src.description;
                    value_      = src.get();
                    return *this;
                }
                auto& operator=( item_&& src )
                {
                    description     = src.description;
                    value_          = src.get();
                    src.description = {};
                    src.value_      = {};
                    return *this;
                }
                item_() = default;
                item_( const char* const description )
                  : description{ description }
                { }
                item_( const item_& src )
                  : value_{ src.get() }
                  , description{ src.description }
                { }
                item_( item_&& src )
                  : value_{ src.get() }
                  , description{ src.description }
                {
                    src.description = {};
                    src.value_      = {};
                }
                ~item_() = default;
            };
            using key_t_   = std::string_view;
            using value_t_ = item_;
            using map_t_   = std::flat_map< key_t_, value_t_ >;
            const char* shown_name_;
            map_t_ options_;
            static constexpr auto str_of_the_enabled{ ": enabled"sv };
            static constexpr auto str_of_the_disabled{ ": disabled"sv };
            auto load_( const std::string_view line )
            {
                key_t_ key;
                bool value;
                if ( line.ends_with( str_of_the_enabled ) ) {
                    key   = line.substr( 0, line.size() - str_of_the_enabled.size() );
                    value = true;
                }
                if ( line.ends_with( str_of_the_disabled ) ) {
                    key   = line.substr( 0, line.size() - str_of_the_disabled.size() );
                    value = false;
                }
                if ( options_.contains( key ) ) {
                    options_.at( key ).set( value );
                }
            }
            static auto reload_( const std::string_view ) noexcept
            { }
            auto sync_( std::ofstream& out )
            {
                for ( const auto& [ key, item ] : options_ ) {
                    out << key << ( item == true ? str_of_the_enabled : str_of_the_disabled ) << '\n';
                }
            }
            static auto make_flip_button_text_( const value_t_& item )
            {
                return item == true ? " > 禁用 "sv : " > 启用 "sv;
            }
            static auto flip_item_value_( const ui_func_args args, value_t_& item )
            {
                args.parent_ui.edit_text( args.node_index, make_flip_button_text_( item.set( !item.get() ) ) );
                return func_back;
            }
            static auto make_option_editor_ui_( map_t_& options )
            {
                cpp_utils::console_ui ui;
                ui.reserve( 2 + options.size() * 2 )
                  .add_back( "                    [ 配  置 ]\n\n" )
                  .add_back(
                    " < 返回 ", quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity );
                for ( auto&& [ _, item ] : options ) {
                    ui.add_back( std::format( "\n[ {} ]\n", item.description ) )
                      .add_back(
                        make_flip_button_text_( item ), std::bind_back( flip_item_value_, std::ref( item ) ),
                        cpp_utils::console_text::foreground_red | cpp_utils::console_text::foreground_green );
                }
                ui.show();
                return func_back;
            }
            auto init_ui_( cpp_utils::console_ui& ui )
            {
                ui.add_back( std::format( " > {} ", shown_name_ ), std::bind_back( make_option_editor_ui_, std::ref( options_ ) ) );
            }
          public:
            const auto& operator[]( const key_t_ key ) const noexcept
            {
                return options_.at( key );
            }
            auto& operator[]( const key_t_ key ) noexcept
            {
                return options_.at( key );
            }
            auto operator=( const basic_options_config_node< Atomic >& ) -> basic_options_config_node< Atomic >& = delete;
            auto operator=( basic_options_config_node< Atomic >&& ) -> basic_options_config_node< Atomic >&      = delete;
            basic_options_config_node( const char* const raw_name, const char* const shown_name, map_t_ options )
              : config_node_impl{ raw_name }
              , shown_name_{ shown_name }
              , options_{ std::move( options ) }
            { }
            basic_options_config_node( const basic_options_config_node< Atomic >& ) = delete;
            basic_options_config_node( basic_options_config_node< Atomic >&& )      = delete;
            ~basic_options_config_node()                                            = default;
        };
    }
    class options_title_ui final
      : public details::config_node_impl
      , private details::initing_ui_only
    {
        friend details::config_node_impl;
      private:
        static auto init_ui_( cpp_utils::console_ui& ui )
        {
            ui.add_back( "\n[ 选项 ]\n" );
        }
      public:
        options_title_ui() noexcept  = default;
        ~options_title_ui() noexcept = default;
    };
    class crack_restore_config final : public details::basic_options_config_node< false >
    {
      public:
        crack_restore_config()
          : basic_options_config_node{
              "crack_restore",
              "破解与恢复",
              { { "hijack_execs", "劫持可执行文件" },
                { "set_serv_startup_types", "设置服务启动类型" },
                { "fast_mode", "快速模式" } }
        }
        { }
        ~crack_restore_config() = default;
    };
    class window_config final : public details::basic_options_config_node< true >
    {
      public:
        window_config()
          : basic_options_config_node{
              "window",
              "窗口显示",
              { { "force_show", "置顶窗口 (非实时)" },
                { "simple_titlebar", "极简标题栏 (非实时)" },
                { "translucent", "半透明 (非实时)" } }
        }
        { }
        ~window_config() = default;
    };
    class performance_config final : public details::basic_options_config_node< false >
    {
      public:
        performance_config()
          : basic_options_config_node{ "performance", "性能", { { "no_hot_reload", "禁用非实时热重载 (下次启动时生效)" } } }
        { }
        ~performance_config() = default;
    };
    class custom_rules_config final : public details::config_node_impl
    {
        friend details::config_node_impl;
      private:
        static inline constexpr auto flag_exec{ "exec:"sv };
        static inline constexpr auto flag_serv{ "serv:"sv };
        class simple_string final
        {
          private:
            static inline constexpr auto sso_size{ 16uz };
            std::variant< std::unique_ptr< char[] >, std::array< char, sso_size > > storage_{};
          public:
            auto c_str() const noexcept
            {
                return storage_.visit< const char* >( []< typename T >( const T& str )
                {
                    if constexpr ( std::is_same_v< T, std::unique_ptr< char[] > > ) {
                        return str.get();
                    } else if constexpr ( std::is_same_v< T, std::array< char, sso_size > > ) {
                        return str.data();
                    } else {
                        static_assert( false, "unkown type!" );
                    }
                } );
            }
            simple_string( std::string_view str )
            {
                if ( str.size() < sso_size ) {
                    std::array< char, sso_size > sso;
                    std::ranges::copy( str, sso.data() );
                    sso[ str.size() ] = '\0';
                    storage_          = sso;
                } else {
                    auto on_heap{ std::make_unique_for_overwrite< char[] >( str.size() + 1 ) };
                    std::ranges::copy( str, on_heap.get() );
                    on_heap[ str.size() ] = '\0';
                    storage_              = std::move( on_heap );
                }
            }
            simple_string( const simple_string& ) = delete;
            simple_string( simple_string&& )      = default;
            ~simple_string()                      = default;
        };
        std::vector< simple_string > execs{};
        std::vector< simple_string > servs{};
        static_assert( []( auto... strings ) consteval
        { return ( std::ranges::none_of( strings, details::is_whitespace ) && ... ); }( flag_exec, flag_serv ) );
        auto load_( const std::string_view line )
        {
            if ( line.size() > flag_exec.size() && line.starts_with( flag_exec ) ) {
                execs.emplace_back( std::ranges::find_if_not( line.substr( flag_exec.size() ), details::is_whitespace ) );
                return;
            }
            if ( line.size() > flag_serv.size() && line.starts_with( flag_serv ) ) {
                servs.emplace_back( std::ranges::find_if_not( line.substr( flag_serv.size() ), details::is_whitespace ) );
                return;
            }
        }
        auto sync_( std::ofstream& out )
        {
            for ( const auto& exec : execs ) {
                out << flag_exec << ' ' << exec.c_str() << '\n';
            }
            for ( const auto& serv : servs ) {
                out << flag_serv << ' ' << serv.c_str() << '\n';
            }
        }
        auto before_load_() noexcept
        {
            execs.clear();
            servs.clear();
            custom_rules.execs.clear();
            custom_rules.servs.clear();
        }
        auto after_load_()
        {
            constexpr auto to_cstr{ []( const simple_string& str ) static noexcept { return str.c_str(); } };
            custom_rules.execs.append_range( execs | std::views::transform( to_cstr ) );
            custom_rules.servs.append_range( servs | std::views::transform( to_cstr ) );
        }
        static auto show_help_info_()
        {
            cpp_utils::console_ui ui;
            ui.reserve( 3 )
              .add_back( "                    [ 配  置 ]\n\n" )
              .add_back( " < 返回 ", quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
              .add_back(
                "\n 自定义规则格式为 <flag>: <item>\n"
                " 其中, <flag> 可为 exec 或 serv,\n"
                " 分别表示以 .exe 为文件扩展名的可执行文件\n"
                " 和某个 Windows 服务的服务名称.\n"
                " <flag> 后的冒号与 <item> 之间可以有若干空白字符.\n"
                " <item> 的类型由 <flag> 决定.\n"
                " 如果 <item> 为空, 该项规则将会被忽略.\n"
                " 如果自定义规则不符合格式, 则会被忽略.\n"
                " 本配置项不对自定义规则的正确性进行检测,\n"
                " 在修改自定义规则时, 请仔细检查.\n\n"
                " 使用示例:\n\n"
                " [custom_rules]\n"
                " exec: abc_frontend\n"
                " exec: abc_backend\n"
                " serv: abc_connect\n"
                " serv: abc_proc_defender" )
              .show();
            return func_back;
        }
        static auto init_ui_( cpp_utils::console_ui& ui )
        {
            ui.add_back( "\n[ 自定义规则 ]\n" ).add_back( " > 查看帮助信息 ", show_help_info_ );
        }
      public:
        custom_rules_config() noexcept
          : config_node_impl{ "custom_rules" }
        { }
        ~custom_rules_config() noexcept = default;
    };
    namespace details
    {
        template < typename T >
        struct is_valid_config_node final
        {
          private:
            static inline constexpr auto is_valid_type{ std::is_final_v< T > && std::is_same_v< std::decay_t< T >, T > };
            static inline constexpr auto has_key_traits{
              std::is_base_of_v< config_node_impl, T > && std::is_default_constructible_v< T > };
          public:
            static inline constexpr auto value{ is_valid_type && has_key_traits };
            is_valid_config_node()  = delete;
            ~is_valid_config_node() = delete;
        };
    }
    using config_node_types
      = cpp_utils::type_list< options_title_ui, crack_restore_config, window_config, performance_config, custom_rules_config >;
    static_assert( config_node_types::all_of< details::is_valid_config_node > );
    static_assert( config_node_types::unique::size == config_node_types::size );
    inline config_node_types::apply< std::tuple > config_nodes{};
    namespace details
    {
        inline auto get_config_node_raw_name_by_tag( std::string_view str ) noexcept
        {
            str = str.substr( 1, str.size() - 2 );
            const auto head{ std::ranges::find_if_not( str, is_whitespace ) };
            const auto tail{ std::ranges::find_if_not( str | std::views::reverse, is_whitespace ).base() };
            if ( head >= tail ) {
                return std::string_view{};
            }
            return std::string_view{ head, tail };
        }
    }
    inline auto load_config( const bool is_reload = false )
    {
        std::ifstream config_file{ config_file_name, std::ios::in };
        if ( !config_file.good() ) {
            return;
        }
        std::apply( []< typename... Ts >( Ts&... config_node ) static { ( config_node.before_load(), ... ); }, config_nodes );
        std::string line;
        using active_config_node_types = config_node_types::filter< details::is_not_initing_ui_only >;
        using config_node_recorder
          = active_config_node_types::transform< std::add_pointer >::add_front< std::monostate >::apply< std::variant >;
        config_node_recorder current_config_node;
        while ( std::getline( config_file, line ) ) {
            const auto parsed_begin{ std::ranges::find_if_not( line, details::is_whitespace ) };
            const auto parsed_end{ std::ranges::find_if_not( line | std::views::reverse, details::is_whitespace ).base() };
            if ( parsed_begin >= parsed_end ) {
                continue;
            }
            const std::string_view parsed_line{ parsed_begin, parsed_end };
            if ( parsed_line.front() == '#' ) {
                continue;
            }
            if ( parsed_line.front() == '[' && parsed_line.back() == ']' && parsed_line.size() > "[]"sv.size() ) {
                current_config_node = std::monostate{};
                std::apply( [ & ]( auto&... config_node ) noexcept
                {
                    const auto current_raw_name{ details::get_config_node_raw_name_by_tag( parsed_line ) };
                    bool is_found{ false };
                    ( [ & ]< typename T >( T& current_node ) noexcept
                    {
                        if constexpr ( active_config_node_types::contains< T > ) {
                            if ( is_found ) {
                                return;
                            }
                            if ( current_raw_name == current_node.raw_name ) {
                                current_config_node = std::addressof( current_node );
                                is_found            = true;
                            }
                        }
                    }( config_node ), ... );
                }, config_nodes );
                continue;
            }
            if ( is_reload ) {
                current_config_node.visit( [ & ]< typename T >( const T node_ptr )
                {
                    if constexpr ( !std::is_same_v< T, std::monostate > ) {
                        node_ptr->reload( parsed_line );
                    }
                } );
            } else {
                current_config_node.visit( [ & ]< typename T >( const T node_ptr )
                {
                    if constexpr ( !std::is_same_v< T, std::monostate > ) {
                        node_ptr->load( parsed_line );
                    }
                } );
            }
        }
        std::apply( []< typename... Ts >( Ts&... config_node ) static { ( config_node.after_load(), ... ); }, config_nodes );
    }
    namespace details
    {
        inline auto show_config_parsing_rules()
        {
            cpp_utils::console_ui ui;
            ui.reserve( 3 )
              .add_back( "                    [ 配  置 ]\n\n" )
              .add_back( " < 返回 ", quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
              .add_back(
                "\n 配置以行作为单位解析.\n\n"
                " 以 # 开头的行是注释, 不进行解析\n\n"
                " 各个配置项在配置文件中由不同标签区分,\n"
                " 标签的格式为 [<标签名>],\n"
                " <标签名> 与中括号间可以有若干空格.\n\n"
                " 如果匹配不到配置项,\n"
                " 则当前读取的标签到下一标签之间的内容都将被忽略.\n\n"
                " 解析时会忽略每行前导和末尾的空白字符.\n"
                " 如果当前行不是标签, 则该行将由上一个标签处理." )
              .show();
            return func_back;
        }
        inline auto sync_config()
        {
            std::print(
              "                    [ 配  置 ]\n\n\n"
              " -> 正在同步配置...\n" );
            load_config( true );
            std::ofstream config_file_stream{ config_file_name, std::ios::out | std::ios::trunc };
            config_file_stream << "# " INFO_FULL_NAME "\n# " INFO_VERSION "\n";
            std::apply( [ & ]( auto&... config_node ) { ( config_node.sync( config_file_stream ), ... ); }, config_nodes );
            config_file_stream.flush();
            std::print( "\n (i) 同步配置{}.", config_file_stream.good() ? "成功" : "失败" );
            press_any_key_to_return();
            return func_back;
        }
        inline auto open_config_file()
        {
            std::print(
              "                    [ 配  置 ]\n\n\n"
              " -> 正在尝试打开配置文件...\n\n" );
            std::print(
              " (i) 打开配置文件{}.",
              std::bit_cast< INT_PTR >( ShellExecuteA( nullptr, "open", config_file_name, nullptr, nullptr, SW_SHOWNORMAL ) ) > 32
                ? "成功"
                : "失败" );
            press_any_key_to_return();
            return func_back;
        }
    }
    inline auto config_ui()
    {
        cpp_utils::console_ui ui;
        ui.reserve( 5 + config_node_types::size + config_node_types::size / 2 )
          .add_back( "                    [ 配  置 ]\n\n" )
          .add_back( " < 返回 ", quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
          .add_back( " > 查看解析规则 ", details::show_config_parsing_rules )
          .add_back( " > 同步配置 ", details::sync_config )
          .add_back( " > 打开配置文件 ", details::open_config_file );
        std::apply( [ & ]( auto&... config_node ) { ( config_node.init_ui( ui ), ... ); }, config_nodes );
        ui.show();
        return func_back;
    }
    inline auto info()
    {
        cpp_utils::console_ui ui;
        ui.reserve( 3 )
          .add_back( "                    [ 关  于 ]\n\n" )
          .add_back( " < 返回 ", quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
          .add_back(
            "\n[ 名称 ]\n\n " INFO_FULL_NAME " (" INFO_SHORT_NAME ")\n\n[ 版本 ]\n\n " INFO_VERSION
            "\n\n 构建时间: " INFO_BUILD_TIME "\n 编译工具: " INFO_COMPILER " " INFO_ARCH
            "\n\n[ 许可证与版权 ]\n\n " INFO_LICENSE "\n " INFO_COPYRIGHT "\n\n[ 仓库 ]\n\n " INFO_REPO_URL )
          .show();
        return func_back;
    }
    namespace details
    {
        inline auto launch_cmd( const ui_func_args args )
        {
            STARTUPINFOA startup{};
            PROCESS_INFORMATION proc;
            char name[]{ "cmd.exe" };
            startup.cb = sizeof( startup );
            if ( CreateProcessA( nullptr, name, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &startup, &proc ) ) {
                cpp_utils::set_current_console_title( INFO_SHORT_NAME " - 命令提示符" );
                cpp_utils::set_console_size( window_handle, std_output_handle, 120, 30 );
                cpp_utils::fix_window_size( window_handle, false );
                cpp_utils::enable_window_maximize_ctrl( window_handle, true );
                args.parent_ui.set_constraints( false, false );
                SetConsoleScreenBufferSize( std_output_handle, { 120, std::numeric_limits< SHORT >::max() - 1 } );
                WaitForSingleObject( proc.hProcess, INFINITE );
                CloseHandle( proc.hProcess );
                CloseHandle( proc.hThread );
                cpp_utils::set_current_console_charset( charset_id );
                cpp_utils::set_current_console_title( INFO_SHORT_NAME );
                cpp_utils::set_console_size( window_handle, std_output_handle, console_width, console_height );
                cpp_utils::fix_window_size( window_handle, true );
                cpp_utils::enable_window_maximize_ctrl( window_handle, false );
            }
            return func_back;
        }
        inline auto restore_os_components()
        {
            std::print(
              "                   [ 工 具 箱 ]\n\n\n"
              " -> 正在尝试恢复...\n\n" );
            constexpr std::array reg_dirs{
              R"(Software\Policies\Microsoft\Windows\System)", R"(Software\Microsoft\Windows\CurrentVersion\Policies\System)",
              R"(Software\Microsoft\Windows\CurrentVersion\Policies\Explorer)", R"(Software\Policies\Microsoft\MMC)" };
            constexpr std::array execs{
              "taskkill", "ntsd", "sc", "net", "reg", "cmd", "taskmgr", "perfmon", "regedit", "mmc", "dism", "sfc" };
            for ( const auto reg_dir : reg_dirs ) {
                RegDeleteTreeA( HKEY_CURRENT_USER, reg_dir );
            }
            for ( const auto exec : execs ) {
                RegDeleteTreeA(
                  cpp_utils::registry::hkey_local_machine,
                  std::format( R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\{}.exe)", exec ).c_str() );
            }
            std::print( " (i) 操作已完成." );
            details::press_any_key_to_return();
            return func_back;
        }
        struct cmd_item final
        {
            const char* description;
            const char* command;
        };
        inline auto execute_cmd( const cmd_item& item )
        {
            constexpr auto output{ cpp_utils::concat_const_string(
              cpp_utils::const_string{
                "                   [ 工 具 箱 ]\n\n\n"
                " -> 正在执行操作系统命令...\n\n" },
              cpp_utils::make_repeated_const_string< '-', console_width >(), cpp_utils::const_string{ "\n\n" } ) };
            std::print( "{}", output.c_str() );
            std::system( item.command );
            return func_exit;
        }
        inline auto make_cmd_executor_ui( const cmd_item& item )
        {
            cpp_utils::console_ui ui;
            ui.reserve( 3 )
              .add_back(
                "                   [ 工 具 箱 ]\n\n\n"
                " (i) 是否继续执行?\n" )
              .add_back(
                " > 是, 继续 ", std::bind_back( execute_cmd, std::cref( item ) ),
                cpp_utils::console_text::foreground_red | cpp_utils::console_text::foreground_intensity )
              .add_back( " > 否, 返回 ", quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
              .show();
            return func_back;
        }
    }
    inline auto toolkit()
    {
        constexpr std::array< details::cmd_item, 5 > common_cmds{
          { { "重启资源管理器", R"(taskkill.exe /f /im explorer.exe && timeout.exe /t 3 /nobreak && start explorer.exe)" },
           { "重启至高级选项", "shutdown.exe /r /o /f /t 0" },
           { "恢复 USB 设备访问", R"(reg.exe add "HKLM\SYSTEM\CurrentControlSet\Services\USBSTOR" /f /t reg_dword /v Start /d 3)" },
           { "重置 Google Chrome 管理策略", R"(reg.exe delete "HKLM\SOFTWARE\Policies\Google\Chrome" /f)" },
           { "重置 Microsoft Edge 管理策略", R"(reg.exe delete "HKLM\SOFTWARE\Policies\Microsoft\Edge" /f)" } }
        };
        cpp_utils::console_ui ui;
        ui.reserve( 5 + common_cmds.size() )
          .add_back( "                   [ 工 具 箱 ]\n\n" )
          .add_back( " < 返回 ", quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
          .add_back( " > 命令提示符 ", details::launch_cmd )
          .add_back( " > 恢复操作系统组件 ", details::restore_os_components )
          .add_back( "\n[ 常用命令 ]\n" );
        for ( const auto& common_cmd : common_cmds ) {
            ui.add_back(
              std::format( " > {} ", common_cmd.description ),
              std::bind_back( details::make_cmd_executor_ui, std::cref( common_cmd ) ) );
        }
        ui.show();
        return func_back;
    }
    namespace details
    {
        inline auto set_console_attrs() noexcept
        {
            const auto& window_options{ std::get< window_config >( config_nodes ) };
            const auto& is_enable_simple_titlebar{ window_options[ "simple_titlebar" ] };
            const auto& is_translucent{ window_options[ "translucent" ] };
            const auto& is_no_hot_reload{ std::get< performance_config >( config_nodes )[ "no_hot_reload" ] };
            if ( is_no_hot_reload ) {
                cpp_utils::enable_window_menu( window_handle, !is_enable_simple_titlebar );
                cpp_utils::set_window_translucency( window_handle, is_translucent ? 230 : 255 );
                return;
            }
            while ( true ) {
                cpp_utils::enable_window_menu( window_handle, !is_enable_simple_titlebar );
                cpp_utils::set_window_translucency( window_handle, is_translucent ? 230 : 255 );
                std::this_thread::sleep_for( default_thread_sleep_time );
            }
        }
        inline auto force_show() noexcept
        {
            const auto& is_no_hot_reload{ std::get< performance_config >( config_nodes )[ "no_hot_reload" ] };
            const auto& is_force_show{ std::get< window_config >( config_nodes )[ "force_show" ] };
            if ( is_no_hot_reload && !is_force_show ) {
                return;
            }
            constexpr auto sleep_time{ 50ms };
            const auto current_thread_id{ GetCurrentThreadId() };
            const auto current_window_thread_process_id{ GetWindowThreadProcessId( window_handle, nullptr ) };
            if ( is_no_hot_reload ) {
                cpp_utils::force_show_window_forever(
                  window_handle, current_thread_id, current_window_thread_process_id, sleep_time );
                return;
            }
            while ( true ) {
                if ( !is_force_show ) {
                    cpp_utils::cancel_force_show_window( window_handle );
                    std::this_thread::sleep_for( default_thread_sleep_time );
                    continue;
                }
                cpp_utils::force_show_window( window_handle, current_thread_id, current_window_thread_process_id );
                std::this_thread::sleep_for( sleep_time );
            }
        }
        inline constexpr std::array threads_func{ set_console_attrs, force_show };
    }
    inline auto create_threads() noexcept
    {
        for ( const auto func : details::threads_func ) {
            std::thread{ func }.detach();
        }
    }
    namespace details
    {
        enum class rule_executing : bool
        {
            crack,
            restore
        };
        inline auto executor_mode{ rule_executing::crack };
        inline auto hijack_exec( const char* const exec ) noexcept
        {
            cpp_utils::create_registry_key< charset_id >(
              cpp_utils::registry::hkey_local_machine,
              std::format( R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\{}.exe)", exec ).c_str(),
              "Debugger", cpp_utils::registry::string_type, reinterpret_cast< const BYTE* >( L"nul" ), sizeof( L"nul" ) );
        }
        inline auto disable_serv( const char* const serv ) noexcept
        {
            cpp_utils::set_service_status< charset_id >( serv, cpp_utils::service::disabled_start );
        }
        inline auto kill_exec( const char* const exec ) noexcept
        {
            cpp_utils::kill_process_by_name< charset_id >( std::format( "{}.exe", exec ).c_str() );
        }
        inline auto stop_serv( const char* const serv ) noexcept
        {
            cpp_utils::stop_service_with_dependencies< charset_id >( serv );
        }
        inline auto undo_hijack_exec( const char* const exec ) noexcept
        {
            cpp_utils::delete_registry_tree< charset_id >(
              cpp_utils::registry::hkey_local_machine,
              std::format( R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\{}.exe)", exec ).c_str() );
        }
        inline auto enable_serv( const char* const serv ) noexcept
        {
            cpp_utils::set_service_status< charset_id >( serv, cpp_utils::service::auto_start );
        }
        inline auto start_serv( const char* const serv ) noexcept
        {
            cpp_utils::start_service_with_dependencies< charset_id >( serv );
        }
        inline auto get_executing_count( const rule_node& rules ) noexcept
        {
            const auto& options{ std::get< crack_restore_config >( config_nodes ) };
            const auto& execs{ rules.execs };
            const auto& servs{ rules.servs };
            switch ( details::executor_mode ) {
                case details::rule_executing::crack :
                    return execs.size() * ( options[ "hijack_execs" ] ? 2 : 1 )
                         + servs.size() * ( options[ "set_serv_startup_types" ] ? 2 : 1 );
                case details::rule_executing::restore :
                    return ( options[ "hijack_execs" ] ? execs.size() : 0 )
                         + servs.size() * ( options[ "set_serv_startup_types" ] ? 2 : 1 );
                default : std::unreachable();
            }
        }
        inline auto default_crack( const rule_node& rules )
        {
            const auto& execs{ rules.execs };
            const auto& servs{ rules.servs };
            const auto& options{ std::get< crack_restore_config >( config_nodes ) };
            const auto can_hijack_execs{ options[ "hijack_execs" ].get() };
            const auto can_set_serv_startup_types{ options[ "set_serv_startup_types" ].get() };
            const auto total_op_count{ 2u + can_hijack_execs + can_set_serv_startup_types };
            auto finished_count{ 0u };
            if ( can_hijack_execs ) {
                std::print( " ({}/{}) 劫持文件.\n", ++finished_count, total_op_count );
                for ( const auto exec : execs ) {
                    hijack_exec( exec );
                }
            }
            if ( can_set_serv_startup_types ) {
                std::print( " ({}/{}) 禁用服务.\n", ++finished_count, total_op_count );
                for ( const auto serv : servs ) {
                    disable_serv( serv );
                }
            }
            std::print( " ({}/{}) 终止进程.\n", ++finished_count, total_op_count );
            for ( const auto exec : execs ) {
                kill_exec( exec );
            }
            std::print( " ({}/{}) 停止服务.\n\n", ++finished_count, total_op_count );
            for ( const auto serv : servs ) {
                stop_serv( serv );
            }
        }
        inline auto fast_crack( const rule_node& rules )
        {
            const auto& options{ std::get< crack_restore_config >( config_nodes ) };
            const auto execs{ rules.execs };
            const auto servs{ rules.servs };
            const auto nproc{ std::ranges::max( std::thread::hardware_concurrency() / 3, 2u ) };
            std::array tasks{
              options[ "hijack_execs" ]
                ? cpp_utils::create_parallel_task( nproc, execs.begin(), execs.end(), hijack_exec )
                : std::vector< std::thread >{},
              options[ "set_serv_startup_types" ]
                ? cpp_utils::create_parallel_task( nproc, servs.begin(), servs.end(), disable_serv )
                : std::vector< std::thread >{},
              cpp_utils::create_parallel_task( nproc, execs.begin(), execs.end(), kill_exec ),
              cpp_utils::create_parallel_task( nproc, servs.begin(), servs.end(), stop_serv ) };
            for ( auto& task : tasks ) {
                for ( auto& thread : task ) {
                    if ( thread.joinable() ) {
                        thread.join();
                    }
                }
            }
        }
        inline auto default_restore( const rule_node& rules )
        {
            const auto& execs{ rules.execs };
            const auto& servs{ rules.servs };
            const auto& options{ std::get< crack_restore_config >( config_nodes ) };
            const auto can_hijack_execs{ options[ "hijack_execs" ].get() };
            const auto can_set_serv_startup_types{ options[ "set_serv_startup_types" ].get() };
            const auto total_op_count{ 1u + can_hijack_execs + can_set_serv_startup_types };
            auto finished_count{ 0u };
            if ( can_hijack_execs ) {
                std::print( " ({}/{}) 撤销劫持.\n", ++finished_count, total_op_count );
                for ( const auto exec : execs ) {
                    undo_hijack_exec( exec );
                }
            }
            if ( can_set_serv_startup_types ) {
                std::print( " ({}/{}) 启用服务.\n", ++finished_count, total_op_count );
                for ( const auto serv : servs ) {
                    enable_serv( serv );
                }
            }
            std::print( " ({}/{}) 启动服务.\n\n", ++finished_count, total_op_count );
            for ( const auto serv : servs ) {
                start_serv( serv );
            }
        }
        inline auto fast_restore( const rule_node& rules )
        {
            const auto& options{ std::get< crack_restore_config >( config_nodes ) };
            const auto& execs{ rules.execs };
            const auto& servs{ rules.servs };
            const auto nproc{ std::ranges::max( std::thread::hardware_concurrency() / 3, 2u ) };
            std::array tasks{
              options[ "hijack_execs" ]
                ? cpp_utils::create_parallel_task( nproc, execs.begin(), execs.end(), undo_hijack_exec )
                : std::vector< std::thread >{},
              options[ "set_serv_startup_types" ]
                ? cpp_utils::create_parallel_task( nproc, servs.begin(), servs.end(), enable_serv )
                : std::vector< std::thread >{} };
            for ( auto& task : tasks ) {
                for ( auto& thread : task ) {
                    if ( thread.joinable() ) {
                        thread.join();
                    }
                }
            }
            for ( auto& thread : cpp_utils::create_parallel_task( nproc, servs.begin(), servs.end(), start_serv ) ) {
                if ( thread.joinable() ) {
                    thread.join();
                }
            }
        }
    }
    inline auto execute_rules( const rule_node& rules )
    {
        switch ( details::executor_mode ) {
            case details::rule_executing::crack : std::print( "                    [ 破  解 ]\n\n\n" ); break;
            case details::rule_executing::restore : std::print( "                    [ 恢  复 ]\n\n\n" ); break;
            default : std::unreachable();
        }
        if ( details::get_executing_count( rules ) == 0 ) {
            std::print( " (i) 无可用操作." );
            details::press_any_key_to_return();
            return func_back;
        }
        std::print( " -> 正在执行...\n\n" );
        const auto is_enable_fast_mode{ std::get< crack_restore_config >( config_nodes )[ "fast_mode" ].get() };
        void ( *f )( const rule_node& );
        switch ( details::executor_mode ) {
            case details::rule_executing::crack :
                f = ( is_enable_fast_mode ? details::fast_crack : details::default_crack );
                break;
            case details::rule_executing::restore :
                f = ( is_enable_fast_mode ? details::fast_restore : details::default_restore );
                break;
            default : std::unreachable();
        }
        f( rules );
        std::print( " (i) 操作已完成." );
        details::press_any_key_to_return();
        return func_back;
    }
    inline auto make_executor_mode_ui_text() noexcept
    {
        switch ( details::executor_mode ) {
            case details::rule_executing::crack : return "[ 破解 (点击切换) ]\n"sv;
            case details::rule_executing::restore : return "[ 恢复 (点击切换) ]\n"sv;
            default : std::unreachable();
        }
    }
    inline auto flip_executor_mode( const ui_func_args args )
    {
        switch ( details::executor_mode ) {
            case details::rule_executing::crack : details::executor_mode = details::rule_executing::restore; break;
            case details::rule_executing::restore : details::executor_mode = details::rule_executing::crack; break;
            default : std::unreachable();
        }
        args.parent_ui.edit_text( args.node_index, make_executor_mode_ui_text() );
        return func_back;
    }
    inline auto execute_all_rules()
    {
        std::print( " -> 正在准备数据...\n" );
        rule_node total;
        auto execs_size{ 0uz };
        auto servs_size{ 0uz };
        execs_size += custom_rules.execs.size();
        servs_size += custom_rules.servs.size();
        for ( const auto& builtin_rule : builtin_rules ) {
            execs_size += builtin_rule.execs.size();
            servs_size += builtin_rule.servs.size();
        }
        total.execs.reserve( execs_size );
        total.servs.reserve( servs_size );
        total.execs.append_range( custom_rules.execs );
        total.servs.append_range( custom_rules.servs );
        for ( const auto& builtin_rule : builtin_rules ) {
            total.execs.append_range( builtin_rule.execs );
            total.servs.append_range( builtin_rule.servs );
        }
        cpp_utils::clear_console( std_output_handle );
        return execute_rules( total );
    }
}
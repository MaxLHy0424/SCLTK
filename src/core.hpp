#pragma once
#include <cpp_utils/const_string.hpp>
#include <cpp_utils/math.hpp>
#include <cpp_utils/meta.hpp>
#include <cpp_utils/windows_app_tools.hpp>
#include <cpp_utils/windows_console_ui.hpp>
#include <experimental/scope>
#include <filesystem>
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
    inline const cpp_utils::console con;
    inline const auto unsynced_mem_pool{ [] static noexcept
    {
        static std::pmr::unsynchronized_pool_resource pool{
          std::pmr::pool_options{ .max_blocks_per_chunk{ 1024 }, .largest_required_pool_block{ 2048 } },
          std::pmr::new_delete_resource()
        };
        std::pmr::set_default_resource( &pool );
        return &pool;
    }() };
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
            con.press_any_key_to_continue();
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
        inline constexpr auto is_whitespace_w( const wchar_t ch ) noexcept
        {
            switch ( ch ) {
                case L'\t' :
                case L'\v' :
                case L'\f' :
                case L' ' : return true;
            }
            return false;
        }
    }
    struct rule_node final
    {
        using item_t      = const wchar_t*;
        using container_t = std::pmr::vector< item_t >;
        const char* shown_name{};
        container_t execs{};
        container_t servs{};
    };
    inline rule_node custom_rules;
    inline const std::array< rule_node, 4 > builtin_rules{
      { { "学生机房管理助手",
          { L"yz", L"abcut", L"jfglzs", L"jfglzsn", L"prozs", L"przs", L"uninstal1", L"sct", L"zmserv", L"zmsrv" },
          { L"zmserv" } },
       { "极域电子教室",
          { L"StudentMain", L"DispcapHelper", L"VRCwPlayer", L"InstHelpApp", L"InstHelpApp64", L"TDOvrSet", L"GATESRV",
            L"ProcHelper64", L"MasterHelper" },
          { L"STUDSRV" } },
       { "联想智能云教室",
          { L"vncviewer", L"tvnserver32", L"WfbsPnpInstall", L"WFBSMon", L"WFBSMlogon", L"WFBSSvrLogShow", L"ResetIp",
            L"FuncForWIN64", L"CertMgr", L"Fireware", L"BCDBootCopy", L"refreship", L"lenovoLockScreen", L"PortControl64",
            L"DesktopCheck", L"DeploymentManager", L"DeploymentAgent", L"XYNTService" },
          { L"BSAgentSvr", L"tvnserver", L"WFBSMlogon" } },
       { "红蜘蛛多媒体网络教室",
          { L"rscheck", L"checkrs", L"REDAgent", L"PerformanceCheck", L"edpaper", L"Adapter", L"repview", L"FormatPaper" },
          { L"appcheck2", L"checkapp2" } } }
    };
    namespace details
    {
        struct initing_ui_only
        {
          protected:
            initing_ui_only() noexcept  = default;
            ~initing_ui_only() noexcept = default;
        };
        template < typename T >
        struct is_not_initing_ui_only final
        {
            static inline constexpr auto value{ !std::is_base_of_v< initing_ui_only, T > };
            is_not_initing_ui_only() noexcept  = delete;
            ~is_not_initing_ui_only() noexcept = delete;
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
                item_() noexcept = default;
                item_( const char* const description ) noexcept
                  : description{ description }
                { }
                item_( const item_& src ) noexcept
                  : value_{ src.get() }
                  , description{ src.description }
                { }
                item_( item_&& src ) noexcept
                  : value_{ src.get() }
                  , description{ src.description }
                {
                    src.description = {};
                    src.value_      = {};
                }
                ~item_() noexcept = default;
            };
            using key_t_    = std::string_view;
            using mapped_t_ = item_;
            using map_t_
              = std::flat_map< key_t_, mapped_t_, std::less< key_t_ >, std::pmr::vector< key_t_ >, std::pmr::vector< mapped_t_ > >;
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
                } else if ( line.ends_with( str_of_the_disabled ) ) {
                    key   = line.substr( 0, line.size() - str_of_the_disabled.size() );
                    value = false;
                } else {
                    return;
                }
                if ( auto it{ options_.find( key ) }; it != options_.end() ) {
                    it->second.set( value );
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
            static auto make_flip_button_text_( const mapped_t_& item )
            {
                return item == true ? " > 禁用 "sv : " > 启用 "sv;
            }
            static auto flip_item_value_( const ui_func_args args, mapped_t_& item )
            {
                args.parent_ui.set_text( args.node_index, make_flip_button_text_( item.set( !item.get() ) ) );
                return func_back;
            }
            static auto make_option_editor_ui_( map_t_& options )
            {
                cpp_utils::console_ui ui{ con, unsynced_mem_pool };
                ui.reserve( 2 + options.size() * 2 )
                  .add_back( "                    [ 配  置 ]\n\n"sv )
                  .add_back(
                    " < 返回 "sv, quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity );
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
            basic_options_config_node( const basic_options_config_node< Atomic >& )     = delete;
            basic_options_config_node( basic_options_config_node< Atomic >&& ) noexcept = delete;
            ~basic_options_config_node() noexcept                                       = default;
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
            ui.add_back( "\n[ 选项 ]\n"sv );
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
              { { "hijack_execs", "劫持可执行文件" }, { "set_serv_startup_types", "设置服务启动类型" } }
        }
        { }
        ~crack_restore_config() noexcept = default;
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
        ~window_config() noexcept = default;
    };
    class performance_config final : public details::basic_options_config_node< false >
    {
      public:
        performance_config()
          : basic_options_config_node{ "performance", "性能", { { "no_hot_reload", "禁用非实时热重载 (下次启动时生效)" } } }
        { }
        ~performance_config() noexcept = default;
    };
    class custom_rules_config final : public details::config_node_impl
    {
        friend details::config_node_impl;
      private:
        static inline constexpr auto flag_exec{ L"exec:"sv };
        static inline constexpr auto flag_serv{ L"serv:"sv };
        std::pmr::vector< std::pmr::wstring > execs{};
        std::pmr::vector< std::pmr::wstring > servs{};
        static_assert( []( auto... strings ) consteval
        { return ( std::ranges::none_of( strings, details::is_whitespace_w ) && ... ); }( flag_exec, flag_serv ) );
        auto load_( const std::string_view line )
        {
            const auto w_line{ cpp_utils::to_wstring( line, charset_id ) };
            const std::wstring_view w_line_v{ w_line };
            if ( w_line_v.size() > flag_exec.size() && w_line_v.starts_with( flag_exec ) ) {
                execs.emplace_back( std::ranges::find_if_not( w_line_v.substr( flag_exec.size() ), details::is_whitespace_w ) );
                return;
            }
            if ( w_line_v.size() > flag_serv.size() && w_line_v.starts_with( flag_serv ) ) {
                servs.emplace_back( std::ranges::find_if_not( w_line_v.substr( flag_serv.size() ), details::is_whitespace_w ) );
                return;
            }
        }
        auto sync_( std::ofstream& out )
        {
            const auto flag_exec_ansi{ cpp_utils::to_string( flag_exec, charset_id, unsynced_mem_pool ) };
            const auto flag_serv_ansi{ cpp_utils::to_string( flag_serv, charset_id, unsynced_mem_pool ) };
            for ( const auto& exec : execs ) {
                out << flag_exec_ansi << ' ' << cpp_utils::to_string( exec, charset_id, unsynced_mem_pool ) << '\n';
            }
            for ( const auto& serv : servs ) {
                out << flag_serv_ansi << ' ' << cpp_utils::to_string( serv, charset_id, unsynced_mem_pool ) << '\n';
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
            constexpr auto to_cstr{ []( const std::pmr::wstring& str ) static noexcept { return str.c_str(); } };
            custom_rules.execs.append_range( execs | std::views::transform( to_cstr ) );
            custom_rules.servs.append_range( servs | std::views::transform( to_cstr ) );
        }
        static auto show_help_info_()
        {
            cpp_utils::console_ui ui{ con, unsynced_mem_pool };
            ui.reserve( 3 )
              .add_back( "                    [ 配  置 ]\n\n"sv )
              .add_back( " < 返回 "sv, quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
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
                " serv: abc_proc_defender"sv )
              .show();
            return func_back;
        }
        static auto init_ui_( cpp_utils::console_ui& ui )
        {
            ui.add_back( "\n[ 自定义规则 ]\n"sv ).add_back( " > 查看帮助信息 "sv, show_help_info_ );
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
            is_valid_config_node()           = delete;
            ~is_valid_config_node() noexcept = delete;
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
        std::pmr::string line;
        using parsable_config_node_types = config_node_types::filter< details::is_not_initing_ui_only >;
        using config_node_recorder
          = parsable_config_node_types::transform< std::add_pointer >::add_front< std::monostate >::apply< std::variant >;
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
                        if constexpr ( parsable_config_node_types::contains< T > ) {
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
            cpp_utils::console_ui ui{ con, unsynced_mem_pool };
            ui.reserve( 3 )
              .add_back( "                    [ 配  置 ]\n\n"sv )
              .add_back( " < 返回 "sv, quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
              .add_back(
                "\n 配置以行作为单位解析.\n\n"
                " 以 # 开头的行是注释, 不进行解析\n\n"
                " 各个配置项在配置文件中由不同标签区分,\n"
                " 标签的格式为 [<标签名>],\n"
                " <标签名> 与中括号间可以有若干空格.\n\n"
                " 如果匹配不到配置项,\n"
                " 则当前读取的标签到下一标签之间的内容都将被忽略.\n\n"
                " 解析时会忽略每行前导和末尾的空白字符.\n"
                " 如果当前行不是标签, 则该行将由上一个标签处理."sv )
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
        cpp_utils::console_ui ui{ con, unsynced_mem_pool };
        ui.reserve( 5 + config_node_types::size + config_node_types::size / 2 )
          .add_back( "                    [ 配  置 ]\n\n"sv )
          .add_back( " < 返回 \n"sv, quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
          .add_back( " > 查看解析规则 "sv, details::show_config_parsing_rules )
          .add_back( " > 同步配置 "sv, details::sync_config )
          .add_back( " > 打开配置文件 "sv, details::open_config_file );
        std::apply( [ & ]( auto&... config_node ) { ( config_node.init_ui( ui ), ... ); }, config_nodes );
        ui.show();
        return func_back;
    }
    inline auto info()
    {
        cpp_utils::console_ui ui{ con, unsynced_mem_pool };
        ui.reserve( 3 )
          .add_back( "                    [ 关  于 ]\n\n"sv )
          .add_back( " < 返回 "sv, quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
          .add_back(
            "\n[ 名称 ]\n\n " INFO_FULL_NAME " (" INFO_SHORT_NAME ")\n\n[ 版本 ]\n\n " INFO_VERSION
            "\n\n 构建时间: " INFO_BUILD_TIME "\n 编译工具: " INFO_COMPILER " " INFO_ARCH
            "\n\n[ 许可证与版权 ]\n\n " INFO_LICENSE "\n " INFO_COPYRIGHT "\n\n[ 仓库 ]\n\n " INFO_REPO_URL ""sv )
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
                con.set_title( INFO_SHORT_NAME " - 命令提示符" );
                con.set_size( 120, 30, unsynced_mem_pool );
                con.fix_size( false );
                con.enable_window_maximize_ctrl( true );
                args.parent_ui.set_constraints( false, false );
                SetConsoleScreenBufferSize( con.std_output_handle, { 120, std::numeric_limits< SHORT >::max() - 1 } );
                WaitForSingleObject( proc.hProcess, INFINITE );
                CloseHandle( proc.hProcess );
                CloseHandle( proc.hThread );
                con.set_charset( charset_id );
                con.set_title( INFO_SHORT_NAME );
                con.set_size( console_width, console_height, unsynced_mem_pool );
                con.fix_size( true );
                con.enable_window_maximize_ctrl( false );
            }
            return func_back;
        }
        inline auto restore_os_components() noexcept
        {
            std::print( " -> 正在尝试恢复...\n" );
            constexpr std::array reg_dirs{
              R"(Software\Policies\Microsoft\Windows\System)", R"(Software\Microsoft\Windows\CurrentVersion\Policies\System)",
              R"(Software\Microsoft\Windows\CurrentVersion\Policies\Explorer)", R"(Software\Policies\Microsoft\MMC)" };
            constexpr std::array execs{
              "tasklist", "taskkill", "ntsd",    "sc",          "net",         "reg",       "cmd",
              "taskmgr",  "perfmon",  "regedit", "mmc",         "dism",        "sfc",       "sethc",
              "sidebar",  "shvlzm",   "winmine", "bckgzm",      "Chess",       "chkrzm",    "FreeCell",
              "Hearts",   "Magnify",  "Mahjong", "Minesweeper", "PurblePlace", "Solitaire", "SpiderSolitaire" };
            for ( const auto reg_dir : reg_dirs ) {
                RegDeleteTreeA( HKEY_CURRENT_USER, reg_dir );
            }
            for ( const auto exec : execs ) {
                RegDeleteTreeA(
                  HKEY_LOCAL_MACHINE,
                  std::format( R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\{}.exe)", exec ).c_str() );
            }
        }
        inline auto reset_hosts() noexcept
        {
            constexpr auto default_content{
              "# Copyright (c) 1993-2009 Microsoft Corp.\n"
              "#\n"
              "# This is a sample HOSTS file used by Microsoft TCP/IP for Windows.\n"
              "#\n"
              "# This file contains the mappings of IP addresses to host names. Each\n"
              "# entry should be kept on an individual line. The IP address should\n"
              "# be placed in the first column followed by the corresponding host name.\n"
              "# The IP address and the host name should be separated by at least one\n"
              "# space.\n"
              "#\n"
              "# Additionally, comments (such as these) may be inserted on individual\n"
              "# lines or following the machine name denoted by a '#' symbol.\n"
              "#\n"
              "# For example:\n"
              "#\n"
              "#      102.54.94.97     rhino.acme.com          # source server\n"
              "#       38.25.63.10     x.acme.com              # x client host\n"
              "\n"
              "# localhost name resolution is handled within DNS itself.\n"
              "# 127.0.0.1       localhost\n"
              "# ::1             localhost\n" };
            constexpr auto has_error{ []( const std::error_code& ec ) static
            {
                if ( ec ) {
                    std::print( " (!) 重置失败. (错误 {})", ec.value() );
                    return true;
                }
                return false;
            } };
            std::print( " -> 检查文件是否存在...\n" );
            auto hosts_path{ [] static
            {
                std::array< wchar_t, MAX_PATH > result;
                GetWindowsDirectoryW( result.data(), MAX_PATH );
                std::ranges::copy( LR"(\System32\drivers\etc\hosts)", std::ranges::find( result, L'\0' ) );
                return std::filesystem::path{ result.data() };
            }() };
            std::error_code ec;
            auto is_exists_hosts_file{ std::filesystem::exists( hosts_path, ec ) };
            if ( has_error( ec ) || !is_exists_hosts_file ) {
                return;
            }
            std::print( " -> 获取原文件权限...\n" );
            const auto original_status{ std::filesystem::status( hosts_path, ec ) };
            if ( has_error( ec ) ) {
                return;
            }
            const auto original_perms{ original_status.permissions() };
            std::print( " -> 获取权限...\n" );
            std::filesystem::permissions( hosts_path, std::filesystem::perms::all, std::filesystem::perm_options::add, ec );
            if ( has_error( ec ) ) {
                return;
            }
            std::print( " -> 写入原始内容...\n" );
            std::ofstream file{ hosts_path, std::ios::out | std::ios::trunc };
            file << default_content << std::flush;
            if ( !file.good() ) {
                std::print( " (!) 重置失败, 无法写入.\n" );
            }
            std::print( " -> 恢复文件权限...\n" );
            std::filesystem::permissions( hosts_path, original_perms, std::filesystem::perm_options::replace, ec );
            has_error( ec );
            std::print( " -> 刷新 DNS 缓存...\n" );
            const auto dnsapi{ LoadLibraryA( "dnsapi.dll" ) };
            if ( dnsapi != nullptr ) {
                using dns_flush_resolver_cache_func = BOOL( WINAPI* )();
                const auto dns_flush_resolver_cache{
                  std::bit_cast< dns_flush_resolver_cache_func >( GetProcAddress( dnsapi, "DnsFlushResolverCache" ) ) };
                if ( dns_flush_resolver_cache != nullptr ) {
                    BOOL result{ dns_flush_resolver_cache() };
                    if ( !result ) {
                        std::print( " (!) 刷新 DNS 失败.\n" );
                    }
                }
                FreeLibrary( dnsapi );
            }
        }
        inline auto CALLBACK enum_window_proc( HWND window, LPARAM param )
        {
            if ( window == nullptr ) {
                return TRUE;
            }
            const auto title_length{ GetWindowTextLengthW( window ) };
            if ( title_length == 0 ) {
                return TRUE;
            }
            auto& found_window{ *std::bit_cast< std::pmr::vector< HWND >* >( param ) };
            std::pmr::wstring title_buffer{ static_cast< std::size_t >( title_length ), L'\0', unsynced_mem_pool };
            GetWindowTextW( window, title_buffer.data(), title_length + 1 );
            if ( title_buffer == L"bianhao"sv ) {
                found_window.emplace_back( window );
                return TRUE;
            }
            return TRUE;
        }
        inline auto kill_jfglzs_daemon() noexcept
        {
            std::print( " -> 正在查找窗口...\n" );
            std::pmr::vector< HWND > found_windows{ unsynced_mem_pool };
            found_windows.reserve( 1 );
            if ( EnumWindows( enum_window_proc, std::bit_cast< LPARAM >( &found_windows ) ) ) {
                for ( const auto found_window : found_windows ) {
                    if ( found_window != nullptr ) {
                        DWORD process_id;
                        GetWindowThreadProcessId( found_window, &process_id );
                        if ( GetLastError() != ERROR_SUCCESS ) {
                            continue;
                        }
                        const auto proc{ OpenProcess( PROCESS_TERMINATE, FALSE, process_id ) };
                        if ( proc == nullptr ) {
                            continue;
                        }
                        if ( !TerminateProcess( proc, 0 ) ) {
                            CloseHandle( proc );
                            continue;
                        }
                        CloseHandle( proc );
                    }
                }
            }
        }
        struct tool_item final
        {
            const char* description;
            void ( *function )() noexcept;
        };
        inline auto execute_tools( void ( *function )() noexcept ) noexcept
        {
            std::print( "                   [ 工 具 箱 ]\n\n\n" );
            function();
            std::print( "\n (i) 操作已完成." );
            details::press_any_key_to_return();
            return func_back;
        }
        struct cmd_item final
        {
            const char* description;
            const char* command;
        };
        inline auto execute_cmd( const char* const command ) noexcept
        {
            constexpr cpp_utils::const_string end_line{ "\n" };
            constexpr auto dividing_line{ cpp_utils::make_repeated_const_string< '-', console_width >() };
            constexpr auto before{ cpp_utils::concat_const_string(
              cpp_utils::const_string{
                "                   [ 工 具 箱 ]\n\n\n"
                " -> 正在执行操作系统命令...\n\n" },
              dividing_line, end_line ) };
            constexpr auto after{
              cpp_utils::concat_const_string( end_line, dividing_line, cpp_utils::const_string{ "\n\n (i) 操作已完成." } ) };
            std::print( "{}", before.c_str() );
            std::system( command );
            std::print( "{}", after.c_str() );
            details::press_any_key_to_return();
            return func_back;
        }
    }
    inline auto toolkit()
    {
        constexpr std::array< details::tool_item, 3 > tools{
          { { "恢复操作系统组件", details::restore_os_components },
           { "重置 Hosts", details::reset_hosts },
           { "终止 \"学生机房管理助手\" 守护进程", details::kill_jfglzs_daemon } }
        };
        constexpr std::array< details::cmd_item, 5 > cmds{
          { { "重启资源管理器", R"(taskkill.exe /f /im explorer.exe && timeout.exe /t 3 /nobreak && start explorer.exe)" },
           { "解除极域电子教室网络限制与文件访问限制", "sc.exe stop TDNetFilter && sc.exe stop TDFileFilter" },
           { "恢复 USB 设备访问", R"(reg.exe add "HKLM\SYSTEM\CurrentControlSet\Services\USBSTOR" /f /t reg_dword /v Start /d 3)" },
           { "重置 Google Chrome 管理策略", R"(reg.exe delete "HKLM\SOFTWARE\Policies\Google\Chrome" /f)" },
           { "重置 Microsoft Edge 管理策略", R"(reg.exe delete "HKLM\SOFTWARE\Policies\Microsoft\Edge" /f)" } }
        };
        cpp_utils::console_ui ui{ con, unsynced_mem_pool };
        ui.reserve( 5 + tools.size() + cmds.size() )
          .add_back( "                   [ 工 具 箱 ]\n\n"sv )
          .add_back( " < 返回 "sv, quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
          .add_back( "\n[ 高级工具 ]\n"sv )
          .add_back( " > 启动命令提示符 "sv, details::launch_cmd );
        for ( const auto [ description, function ] : tools ) {
            ui.add_back( std::format( " > {} ", description ), std::bind_back( details::execute_tools, function ) );
        }
        ui.add_back( "\n[ 快捷命令 ]\n"sv );
        for ( const auto [ description, command ] : cmds ) {
            ui.add_back( std::format( " > {} ", description ), std::bind_back( details::execute_cmd, command ) );
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
                con.enable_context_menu( !is_enable_simple_titlebar );
                con.set_translucency( is_translucent ? 230 : 255 );
                return;
            }
            while ( true ) {
                con.enable_context_menu( !is_enable_simple_titlebar );
                con.set_translucency( is_translucent ? 230 : 255 );
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
            if ( is_no_hot_reload ) {
                con.force_show_forever( sleep_time );
                return;
            }
            while ( true ) {
                if ( !is_force_show ) {
                    con.cancel_force_show();
                    std::this_thread::sleep_for( default_thread_sleep_time );
                    continue;
                }
                con.force_show();
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
        inline auto hijack_exec( const std::pmr::wstring& exec ) noexcept
        {
            constexpr const wchar_t data[]{ L"nul" };
            cpp_utils::create_registry_key(
              cpp_utils::registry_flag::hkey_local_machine,
              std::format( LR"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\{}.exe)", exec ),
              L"Debugger", cpp_utils::registry_flag::string_type, std::bit_cast< const BYTE* >( +data ), sizeof( data ) );
        }
        inline auto disable_serv( const std::pmr::wstring& serv ) noexcept
        {
            cpp_utils::set_service_status( serv, cpp_utils::service_flag::disabled_start );
        }
        inline auto kill_exec( const std::pmr::wstring& exec ) noexcept
        {
            cpp_utils::terminate_process_by_name( std::format( L"{}.exe", exec ) );
        }
        inline auto stop_serv( const std::pmr::wstring& serv ) noexcept
        {
            cpp_utils::stop_service_with_dependencies( serv, unsynced_mem_pool );
        }
        inline auto undo_hijack_exec( const std::pmr::wstring& exec ) noexcept
        {
            cpp_utils::delete_registry_tree(
              cpp_utils::registry_flag::hkey_local_machine,
              std::format( LR"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\{}.exe)", exec ) );
        }
        inline auto enable_serv( const std::pmr::wstring& serv ) noexcept
        {
            cpp_utils::set_service_status( serv, cpp_utils::service_flag::auto_start );
        }
        inline auto start_serv( const std::pmr::wstring& serv ) noexcept
        {
            cpp_utils::start_service_with_dependencies( serv, unsynced_mem_pool );
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
        inline auto crack( const rule_node& rules )
        {
            const auto execs{ rules.execs };
            const auto servs{ rules.servs };
            const auto& options{ std::get< crack_restore_config >( config_nodes ) };
            const auto can_hijack_execs{ options[ "hijack_execs" ].get() };
            const auto can_set_serv_startup_types{ options[ "set_serv_startup_types" ].get() };
            if ( can_hijack_execs ) {
                std::print( " - 劫持文件.\n" );
                for ( const auto& exec : execs ) {
                    hijack_exec( exec );
                }
            }
            if ( can_set_serv_startup_types ) {
                std::print( " - 禁用服务.\n" );
                for ( const auto& serv : servs ) {
                    disable_serv( serv );
                }
            }
            std::print( " - 停止服务.\n" );
            for ( const auto& serv : servs ) {
                stop_serv( serv );
            }
            std::print( " - 终止进程.\n" );
            for ( const auto& exec : execs ) {
                kill_exec( exec );
            }
        }
        inline auto restore( const rule_node& rules )
        {
            const auto& execs{ rules.execs };
            const auto& servs{ rules.servs };
            const auto& options{ std::get< crack_restore_config >( config_nodes ) };
            const auto can_hijack_execs{ options[ "hijack_execs" ].get() };
            const auto can_set_serv_startup_types{ options[ "set_serv_startup_types" ].get() };
            if ( can_hijack_execs ) {
                std::print( " - 撤销劫持.\n" );
                for ( const auto& exec : execs ) {
                    undo_hijack_exec( exec );
                }
            }
            if ( can_set_serv_startup_types ) {
                std::print( " - 启用服务.\n" );
                for ( const auto& serv : servs ) {
                    enable_serv( serv );
                }
            }
            std::print( " - 启动服务.\n" );
            for ( const auto& serv : servs ) {
                start_serv( serv );
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
        switch ( details::executor_mode ) {
            case details::rule_executing::crack : details::crack( rules ); break;
            case details::rule_executing::restore : details::restore( rules ); break;
            default : std::unreachable();
        }
        std::print( "\n (i) 操作已完成." );
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
        args.parent_ui.set_text( args.node_index, make_executor_mode_ui_text() );
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
        con.clear( unsynced_mem_pool );
        return execute_rules( total );
    }
}
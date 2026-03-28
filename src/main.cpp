#define WINVER       0x0601
#define _WIN32_WINNT 0x0601
#define NOCOMM
#include <cpp_utils/const_string.hpp>
#include <cpp_utils/meta.hpp>
#include <cpp_utils/windows_app_tools.hpp>
#include <cpp_utils/windows_console_ui.hpp>
#include <iphlpapi.h>
#include <filesystem>
#include <fstream>
#include "info.hpp"
namespace scltk
{
    using namespace std::chrono_literals;
    using namespace std::string_view_literals;
    using namespace cpp_utils::const_string_literals;
    constexpr SHORT console_width{ 50 };
    constexpr SHORT console_height{ 25 };
    constexpr UINT charset_id{ 936 };
    constexpr const auto& config_file_name{ L"SCLTK.conf" };
    constexpr auto func_back{ cpp_utils::console_ui::func_back };
    constexpr auto func_exit{ cpp_utils::console_ui::func_exit };
    const cpp_utils::console con;
    const auto unsynced_mem_pool{ [] static noexcept
    {
        static std::pmr::unsynchronized_pool_resource pool{
          std::pmr::pool_options{ .max_blocks_per_chunk{ 1024 }, .largest_required_pool_block{ 4096 } },
          std::pmr::new_delete_resource()
        };
        std::pmr::set_default_resource( &pool );
        return &pool;
    }() };
    using ui_func_args_t = cpp_utils::console_ui::func_args;
    auto quit() noexcept
    {
        return func_exit;
    }
    auto relaunch() noexcept
    {
        cpp_utils::clone_self();
        return func_exit;
    }
    template < cpp_utils::const_string Title, std::size_t NewLineCount >
    constexpr auto make_middle_text{ cpp_utils::concat_const_string(
      cpp_utils::make_repeated_const_string< ' ', ( static_cast< std::size_t >( console_width ) - Title.size() + 1 ) / 2 >(),
      Title, cpp_utils::make_repeated_const_string< '\n', NewLineCount >() ) };
    template < cpp_utils::const_string Text >
    constexpr auto make_item_text{ cpp_utils::concat_const_string( " > "_cs, Text, " "_cs ) };
    namespace details_
    {
        auto press_any_key_to_return() noexcept
        {
            std::print( "\n\n 请按任意键返回." );
            con.press_any_key_to_continue();
        }
        template < cpp_utils::character CharT >
        constexpr auto is_whitespace( const CharT ch ) noexcept
        {
            switch ( ch ) {
                case static_cast< CharT >( '\t' ) :
                case static_cast< CharT >( '\v' ) :
                case static_cast< CharT >( '\f' ) :
                case static_cast< CharT >( ' ' ) : return true;
            }
            return false;
        }
        template < cpp_utils::const_wstring... Items >
        using make_const_wstring_list_t = cpp_utils::type_list< cpp_utils::value_identity< Items >... >;
        constexpr auto empty_lambda{ [] static noexcept { } };
        auto terminate_jfglzs_daemon() noexcept
        {
            constexpr auto close_handle{ []( const HANDLE handle ) static noexcept { CloseHandle( handle ); } };
            using handle_wrapper_t = const std::unique_ptr< std::remove_pointer_t< HANDLE >, decltype( close_handle ) >;
            handle_wrapper_t process_snapshot{ CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 ) };
            if ( process_snapshot.get() == INVALID_HANDLE_VALUE ) [[unlikely]] {
                return;
            }
            constexpr auto is_lower_case{ []( const wchar_t ch ) static noexcept { return ch >= L'a' && ch <= L'z'; } };
            constexpr auto needle{ L"Program Files"sv };
            const std::boyer_moore_horspool_searcher searcher{ needle.begin(), needle.end() };
            PROCESSENTRY32W process_entry{};
            process_entry.dwSize = sizeof( process_entry );
            if ( Process32FirstW( process_snapshot.get(), &process_entry ) ) [[likely]] {
                do {
                    std::wstring_view name{ process_entry.szExeFile };
                    if ( name.size() != L"xxxxx.exe"sv.size() ) [[likely]] {
                        continue;
                    }
                    name.remove_suffix( L".exe"sv.size() );
                    if ( std::ranges::all_of( name, is_lower_case ) ) {
                        handle_wrapper_t process_handle{ OpenProcess(
                          PROCESS_TERMINATE | PROCESS_QUERY_LIMITED_INFORMATION, FALSE, process_entry.th32ProcessID ) };
                        if ( process_handle == nullptr ) [[unlikely]] {
                            continue;
                        }
                        DWORD size{ MAX_PATH };
                        std::array< wchar_t, MAX_PATH > buffer{};
                        QueryFullProcessImageNameW( process_handle.get(), 0, buffer.data(), &size );
                        if ( std::search( buffer.begin(), buffer.end(), searcher ) != buffer.end() ) {
                            TerminateProcess( process_handle.get(), 1 );
                        }
                    }
                } while ( Process32NextW( process_snapshot.get(), &process_entry ) );
            }
        }
    }
    template < cpp_utils::const_string DisplayName, cpp_utils::same_as_type_list Procs, cpp_utils::same_as_type_list Servs,
               std::invocable auto CrackHelper = details_::empty_lambda, std::invocable auto RestoreHelper = details_::empty_lambda >
    struct compile_time_rule_node final
    {
        static constexpr auto display_name{ DisplayName };
        static constexpr auto crack_helper{ CrackHelper };
        static constexpr auto restore_helper{ RestoreHelper };
        using procs = Procs;
        using servs = Servs;
    };
    struct runtime_rule_node final
    {
        using item_t = std::pmr::vector< std::pmr::wstring >;
        item_t procs{ unsynced_mem_pool };
        item_t servs{ unsynced_mem_pool };
        item_t crack_helpers{ unsynced_mem_pool };
        item_t restore_helpers{ unsynced_mem_pool };
    };
    runtime_rule_node custom_rules;
    namespace details_
    {
        template < cpp_utils::const_string RawName >
        struct config_node_raw_name
        {
            static constexpr auto raw_name{ RawName };
        };
        class config_node_interface;
        template < typename T >
        struct is_parsable_config_node final
        {
            static constexpr auto value{ ( std::is_base_of_v< config_node_interface, T > && requires { T::raw_name; } ) };
        };
        template < typename T >
        constexpr auto is_parsable_config_node_v{ is_parsable_config_node< T >::value };
        class config_node_interface
        {
          public:
            auto load( this auto&& self, const std::string_view line )
            {
                using child_t = std::decay_t< decltype( self ) >;
                if constexpr ( is_parsable_config_node_v< child_t > ) {
                    self.load_( line );
                }
            }
            auto reload( this auto&& self, const std::string_view line )
            {
                using child_t = std::decay_t< decltype( self ) >;
                if constexpr ( is_parsable_config_node_v< child_t > && requires( child_t obj ) { obj.reload_( line ); } ) {
                    self.reload_( line );
                } else {
                    self.load( line );
                }
            }
            auto sync( this auto&& self, std::ofstream& out )
            {
                using child_t = std::decay_t< decltype( self ) >;
                if constexpr ( is_parsable_config_node_v< child_t > ) {
                    out << cpp_utils::value_identity_v< cpp_utils::concat_const_string( "["_cs, child_t::raw_name, "]\n"_cs ) >.view();
                    self.sync_( out );
                }
            }
            auto before_load( this auto&& self )
            {
                using child_t = std::decay_t< decltype( self ) >;
                if constexpr ( is_parsable_config_node_v< child_t > && requires( child_t obj ) { obj.before_load_(); } ) {
                    self.before_load_();
                }
            }
            auto after_load( this auto&& self )
            {
                using child_t = std::decay_t< decltype( self ) >;
                if constexpr ( is_parsable_config_node_v< child_t > && requires( child_t obj ) { obj.after_load_(); } ) {
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
            consteval auto request_ui_count( this auto&& self ) noexcept
            {
                using child_t = std::decay_t< decltype( self ) >;
                if constexpr ( requires( child_t obj ) {
                                   { child_t::ui_count_ } -> std::convertible_to< std::size_t >;
                               } )
                {
                    return cpp_utils::value_identity< static_cast< std::size_t >( child_t::ui_count_ ) >{};
                } else {
                    return cpp_utils::value_identity< 0uz >{};
                }
            }
        };
        template < cpp_utils::const_string RawName, cpp_utils::const_string DisplayName >
        struct option_info final
        {
            static constexpr auto raw_name{ RawName };
            static constexpr auto display_name{ DisplayName };
        };
        template < typename >
        struct is_option_info final : std::false_type
        { };
        template < cpp_utils::const_string RawName, cpp_utils::const_string DisplayName >
        struct is_option_info< option_info< RawName, DisplayName > > final : std::true_type
        { };
        template < typename... Options >
            requires( is_option_info< Options >::value && ... )
        struct options_info_table final
        {
            using base_t      = cpp_utils::type_list< Options... >;
            using raw_names_t = cpp_utils::type_list< cpp_utils::value_identity< Options::raw_name >... >;
            static consteval auto is_valid() noexcept
            {
                return raw_names_t::unique::size == raw_names_t::size
                    && []< cpp_utils::const_string... Items >(
                         const cpp_utils::type_list< cpp_utils::value_identity< Items >... > ) static constexpr noexcept
                {
                    return ( std::ranges::all_of( Items.view(), []( const char ch ) static constexpr noexcept {
                        return !is_whitespace< char >( ch ) && ch != '\r' && ch != '\n';
                    } ) && ... );
                }( raw_names_t{} );
            }
            template < cpp_utils::const_string RawName >
            static consteval auto contains() noexcept
            {
                return raw_names_t::template contains< cpp_utils::value_identity< RawName > >;
            }
            template < cpp_utils::const_string RawName >
            static consteval auto index_of() noexcept
            {
                return raw_names_t::template find_first< cpp_utils::value_identity< RawName > >;
            }
        };
        template < typename >
        struct is_valid_options_info_table final : std::false_type
        { };
        template < typename... Options >
        struct is_valid_options_info_table< options_info_table< Options... > > final
          : std::conditional_t< options_info_table< Options... >::is_valid(), std::true_type, std::false_type >
        { };
        template < cpp_utils::const_string RawName, cpp_utils::const_string DisplayName, bool Atomic, typename OptionsInfoTable >
            requires( is_valid_options_info_table< OptionsInfoTable >::value == true )
        class options_config_node final
          : public config_node_raw_name< RawName >
          , public config_node_interface
        {
            friend config_node_interface;
          private:
            using info_table_base_t_ = typename OptionsInfoTable::base_t;
            using value_t_           = std::conditional_t< Atomic, std::atomic_flag, bool >;
            std::array< value_t_, info_table_base_t_::size > data_{};
            static constexpr auto str_enabled_{ ": enabled"sv };
            static constexpr auto str_disabled_{ ": disabled"sv };
            static auto get_value_( const value_t_& value ) noexcept
            {
                if constexpr ( std::is_same_v< value_t_, std::atomic_flag > ) {
                    return value.test( std::memory_order_acquire );
                } else {
                    return value;
                }
            }
            static auto set_value_( value_t_& obj, const bool val ) noexcept
            {
                if constexpr ( std::is_same_v< value_t_, std::atomic_flag > ) {
                    switch ( val ) {
                        case false : obj.clear( std::memory_order_release ); break;
                        case true : ( void ) obj.test_and_set( std::memory_order_release ); break;
                    }
                } else {
                    obj = val;
                }
            }
            static auto set_value_then_notify_all_( value_t_& obj, const bool val ) noexcept
            {
                set_value_( obj, val );
                if constexpr ( std::is_same_v< value_t_, std::atomic_flag > ) {
                    obj.notify_all();
                }
            }
            auto load_( std::string_view line ) noexcept
            {
                bool value;
                if ( line.size() > str_enabled_.size() && line.ends_with( str_enabled_ ) ) [[likely]] {
                    line.remove_suffix( str_enabled_.size() );
                    value = true;
                } else if ( line.size() > str_disabled_.size() && line.ends_with( str_disabled_ ) ) [[likely]] {
                    line.remove_suffix( str_disabled_.size() );
                    value = false;
                } else {
                    return;
                }
                [ & ]< std::size_t... Is >( const std::index_sequence< Is... > ) noexcept
                {
                    (
                      [ & ]< std::size_t I > noexcept
                    {
                        if ( info_table_base_t_::template at< I >::raw_name.view() == line ) {
                            set_value_( std::get< I >( data_ ), value );
                            return true;
                        }
                        return false;
                    }.template operator()< Is >()
                      || ... );
                }( std::make_index_sequence< info_table_base_t_::size >{} );
            }
            static auto reload_( const std::string_view ) noexcept
            { }
            auto sync_( std::ofstream& out )
            {
                [ & ]< std::size_t... Is >( const std::index_sequence< Is... > )
                {
                    ( ( out << info_table_base_t_::template at< Is >::raw_name.view()
                            << ( get_value_( std::get< Is >( data_ ) ) == true ? str_enabled_ : str_disabled_ ) << '\n' ),
                      ... );
                }( std::make_index_sequence< info_table_base_t_::size >{} );
            }
            static auto make_flip_button_text_( const bool current_value ) noexcept
            {
                return current_value == true ? " > 禁用 "sv : " > 启用 "sv;
            }
            static auto flip_item_value_( const ui_func_args_t args, value_t_& value ) noexcept
            {
                const auto value_to_set{ !get_value_( value ) };
                set_value_then_notify_all_( value, value_to_set );
                args.parent_ui.set_text( args.node_index, make_flip_button_text_( value_to_set ) );
                return func_back;
            }
            static auto make_option_editor_ui_( std::array< value_t_, info_table_base_t_::size >& data_ )
            {
                cpp_utils::console_ui ui{ con, unsynced_mem_pool };
                ui.reserve( 2 + data_.size() * 2 )
                  .add_back( make_middle_text< "[ 配  置 ]", 2 >.view() )
                  .add_back(
                    " < 返回 ", quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity );
                [ & ]< std::size_t... Is >( const std::index_sequence< Is... > )
                {
                    ( ui.add_back(
                          cpp_utils::value_identity_v< cpp_utils::concat_const_string(
                            "\n[ "_cs, info_table_base_t_::template at< Is >::display_name, " ]\n"_cs ) >.view() )
                        .add_back(
                          make_flip_button_text_( get_value_( std::get< Is >( data_ ) ) ),
                          std::bind_back( flip_item_value_, std::ref( std::get< Is >( data_ ) ) ),
                          cpp_utils::console_text::foreground_red | cpp_utils::console_text::foreground_green ),
                      ... );
                }( std::make_index_sequence< info_table_base_t_::size >{} );
                ui.show();
                return func_back;
            }
            auto init_ui_( cpp_utils::console_ui& ui )
            {
                ui.add_back( make_item_text< DisplayName >.view(), std::bind_back( make_option_editor_ui_, std::ref( data_ ) ) );
            }
            static constexpr auto ui_count_{ 1uz };
          public:
            template < cpp_utils::const_string OptionRawName >
                requires( OptionsInfoTable::template contains< OptionRawName >() )
            constexpr auto&& at( this auto&& self ) noexcept
            {
                return std::get< OptionsInfoTable::template index_of< OptionRawName >() >( self.data_ );
            }
            auto operator=( const options_config_node& ) -> options_config_node&     = delete;
            auto operator=( options_config_node&& ) noexcept -> options_config_node& = delete;
            options_config_node() noexcept                                           = default;
            options_config_node( const options_config_node& )                        = delete;
            options_config_node( options_config_node&& ) noexcept                    = delete;
            ~options_config_node() noexcept                                          = default;
        };
    }
    class options_title_ui final : public details_::config_node_interface
    {
        friend details_::config_node_interface;
      private:
        static auto init_ui_( cpp_utils::console_ui& ui )
        {
            ui.add_back( "\n[ 选项 ]\n" );
        }
        static constexpr auto ui_count_{ 1uz };
      public:
        options_title_ui() noexcept  = default;
        ~options_title_ui() noexcept = default;
    };
    using crack_restore_config = details_::options_config_node<
      "crack_restore", "破解与恢复", false,
      details_::options_info_table<
        details_::option_info< "hijack_procs", "劫持进程" >,
        details_::option_info< "set_servs_start_type", "设置服务启动类型" > > >;
    using window_config = details_::options_config_node<
      "window", "窗口显示", true,
      details_::options_info_table<
        details_::option_info< "force_show", "置顶窗口 (异步)" >,
        details_::option_info< "simple_titlebar", "极简标题栏 (异步)" >,
        details_::option_info< "translucent", "半透明 (异步)" > > >;
    using perf_config = details_::options_config_node<
      "perf", "性能", false,
      details_::options_info_table< details_::option_info< "no_async_hot_reload", "禁用异步热重载 (下次启动时生效)" > > >;
    class custom_rules_config final
      : public details_::config_node_interface
      , public details_::config_node_raw_name< "custom_rules" >
    {
        friend details_::config_node_interface;
      private:
        static constexpr auto flag_proc_{ L"proc:"sv };
        static constexpr auto flag_serv_{ L"serv:"sv };
        static constexpr auto flag_crack_helper_{ L"crack_helper:"sv };
        static constexpr auto flag_restore_helper_{ L"restore_helper:"sv };
        static_assert( []( auto... strings ) static consteval noexcept {
            return ( std::ranges::none_of( strings, details_::is_whitespace< wchar_t > ) && ... );
        }( flag_proc_, flag_serv_, flag_crack_helper_, flag_restore_helper_ ) );
        static auto load_( const std::string_view unconverted_line )
        {
            const auto converted_line{ cpp_utils::to_wstring( unconverted_line, CP_UTF8, unsynced_mem_pool ) };
            const std::wstring_view line{ converted_line };
            if ( line.size() > flag_proc_.size() && line.starts_with( flag_proc_ ) ) [[likely]] {
                custom_rules.procs.emplace_back(
                  std::ranges::find_if_not( line.substr( flag_proc_.size() ), details_::is_whitespace< wchar_t > ) );
                return;
            }
            if ( line.size() > flag_serv_.size() && line.starts_with( flag_serv_ ) ) [[likely]] {
                custom_rules.servs.emplace_back(
                  std::ranges::find_if_not( line.substr( flag_serv_.size() ), details_::is_whitespace< wchar_t > ) );
                return;
            }
            if ( line.size() > flag_crack_helper_.size() && line.starts_with( flag_crack_helper_ ) ) [[likely]] {
                custom_rules.crack_helpers.emplace_back(
                  std::ranges::find_if_not( line.substr( flag_crack_helper_.size() ), details_::is_whitespace< wchar_t > ) );
                return;
            }
            if ( line.size() > flag_restore_helper_.size() && line.starts_with( flag_restore_helper_ ) ) [[likely]] {
                custom_rules.restore_helpers.emplace_back(
                  std::ranges::find_if_not( line.substr( flag_restore_helper_.size() ), details_::is_whitespace< wchar_t > ) );
                return;
            }
        }
        static auto sync_( std::ofstream& out )
        {
            const auto flag_proc{ cpp_utils::to_string( flag_proc_, CP_UTF8, unsynced_mem_pool ) };
            const auto flag_serv{ cpp_utils::to_string( flag_serv_, CP_UTF8, unsynced_mem_pool ) };
            const auto flag_crack_helper{ cpp_utils::to_string( flag_crack_helper_, CP_UTF8, unsynced_mem_pool ) };
            const auto flag_restore_helper{ cpp_utils::to_string( flag_restore_helper_, CP_UTF8, unsynced_mem_pool ) };
            for ( const auto& proc : custom_rules.procs ) {
                out << flag_proc << ' ' << cpp_utils::to_string( proc, CP_UTF8, unsynced_mem_pool ) << '\n';
            }
            for ( const auto& serv : custom_rules.servs ) {
                out << flag_serv << ' ' << cpp_utils::to_string( serv, CP_UTF8, unsynced_mem_pool ) << '\n';
            }
            for ( const auto& crack_helper : custom_rules.crack_helpers ) {
                out << flag_crack_helper << ' ' << cpp_utils::to_string( crack_helper, CP_UTF8, unsynced_mem_pool ) << '\n';
            }
            for ( const auto& restore_helper : custom_rules.restore_helpers ) {
                out << flag_restore_helper << ' ' << cpp_utils::to_string( restore_helper, CP_UTF8, unsynced_mem_pool ) << '\n';
            }
        }
        static auto before_load_() noexcept
        {
            custom_rules.procs.clear();
            custom_rules.servs.clear();
            custom_rules.crack_helpers.clear();
            custom_rules.restore_helpers.clear();
        }
        static auto show_help_info_()
        {
            cpp_utils::console_ui ui{ con, unsynced_mem_pool };
            ui.reserve( 3 )
              .add_back( make_middle_text< "[ 配  置 ]", 2 >.view() )
              .add_back( " < 返回 ", quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
              .add_back(
                "\n 自定义规则格式为 <flag>: <item>\n"
                " <flag> 后的冒号与 <item> 之间可以有若干空白字符.\n"
                " 不符合格式的规则将会被忽略.\n"
                " <item> 的类型由 <flag> 决定.\n"
                " 其中, <flag> 有如下选项:\n"
                " proc - Windows 进程名称.\n"
                " serv - Windows 服务的服务名称.\n"
                " crack_helper - 破解时执行的程序的命令行.\n"
                " restore_helper - 恢复时执行的程序的命令行.\n\n"
                " 使用示例:\n"
                " [custom_rules]\n"
                " proc: abc_frontend.exe\n"
                " proc: abc_backend.com\n"
                " serv: abc_connect\n"
                " serv: abc_proc_defender\n"
                " crack_helper: \"abc helper.exe\" crack\n"
                " restore_helper: \"abc helper.exe\" restore" )
              .show();
            return func_back;
        }
        static auto init_ui_( cpp_utils::console_ui& ui )
        {
            ui.add_back( "\n[ 自定义规则 ]\n" ).add_back( " > 查看帮助信息 ", show_help_info_ );
        }
        static constexpr auto ui_count_{ 2uz };
      public:
        custom_rules_config() noexcept  = default;
        ~custom_rules_config() noexcept = default;
    };
    namespace details_
    {
        template < typename T >
        struct is_valid_config_node final
        {
            static constexpr auto is_valid_type{ std::is_same_v< std::decay_t< T >, T > };
            static constexpr auto has_traits{
              std::is_base_of_v< config_node_interface, T > && std::is_default_constructible_v< T > };
            static constexpr auto value{ is_valid_type && has_traits };
            is_valid_config_node()           = delete;
            ~is_valid_config_node() noexcept = delete;
        };
    }
    using config_nodes_t
      = cpp_utils::type_list< options_title_ui, crack_restore_config, window_config, perf_config, custom_rules_config >;
    static_assert( config_nodes_t::all_of< details_::is_valid_config_node > );
    static_assert( config_nodes_t::unique::size == config_nodes_t::size );
    config_nodes_t::apply< std::tuple > config_nodes{};
    namespace details_
    {
        auto get_config_node_raw_name_by_tag( std::string_view str ) noexcept
        {
            str.remove_prefix( 1 );
            str.remove_suffix( 1 );
            const auto head{ std::ranges::find_if_not( str, is_whitespace< char > ) };
            const auto tail{ std::ranges::find_if_not( str | std::views::reverse, is_whitespace< char > ).base() };
            if ( head >= tail ) [[unlikely]] {
                return std::string_view{};
            }
            return std::string_view{ head, tail };
        }
    }
    auto load_config( const bool is_reload )
    {
        std::ifstream config_file{ config_file_name, std::ios::in };
        if ( !config_file.good() ) [[unlikely]] {
            return;
        }
        std::apply( []< typename... Ts >( Ts&... config_node ) static { ( config_node.before_load(), ... ); }, config_nodes );
        std::pmr::string line;
        using parsable_config_nodes_t = config_nodes_t::filter< details_::is_parsable_config_node >;
        using config_node_recorder_t
          = parsable_config_nodes_t::transform< std::add_pointer >::add_front< std::monostate >::apply< std::variant >;
        config_node_recorder_t current_config_node;
        while ( std::getline( config_file, line ) ) {
            const auto parsed_begin{ std::ranges::find_if_not( line, details_::is_whitespace< char > ) };
            const auto parsed_end{ std::ranges::find_if_not( line | std::views::reverse, details_::is_whitespace< char > ).base() };
            if ( parsed_begin >= parsed_end ) [[unlikely]] {
                continue;
            }
            const std::string_view parsed_line{ parsed_begin, parsed_end };
            if ( parsed_line.front() == '#' ) {
                continue;
            }
            if ( parsed_line.front() == '[' && parsed_line.back() == ']' && parsed_line.size() > "[]"sv.size() ) [[likely]] {
                current_config_node = std::monostate{};
                std::apply( [ & ]( auto&... config_node ) noexcept
                {
                    const auto current_raw_name{ details_::get_config_node_raw_name_by_tag( parsed_line ) };
                    ( [ & ]< typename T >( T& current_node ) noexcept
                    {
                        if constexpr ( parsable_config_nodes_t::contains< T > ) {
                            if ( T::raw_name.view() == current_raw_name ) {
                                current_config_node = std::addressof( current_node );
                                return true;
                            }
                        }
                        return false;
                    }( config_node ) || ... );
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
    namespace details_
    {
        auto show_config_parsing_rules()
        {
            cpp_utils::console_ui ui{ con, unsynced_mem_pool };
            ui.reserve( 3 )
              .add_back( make_middle_text< "[ 配  置 ]", 2 >.view() )
              .add_back( " < 返回 ", quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
              .add_back(
                "\n 配置以行作为单位解析.\n\n"
                " 以 # 开头的行是注释, 不进行解析.\n\n"
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
        auto sync_config()
        {
            std::print(
              cpp_utils::value_identity_v< cpp_utils::concat_const_string(
                make_middle_text< "[ 配  置 ]", 3 >, " -> 同步配置.\n\n"_cs ) >.view() );
            load_config( true );
            std::ofstream config_file_stream{ config_file_name, std::ios::out | std::ios::trunc };
            constexpr auto header{
              u8"# " INFO_FULL_NAME "\n# " INFO_GIT_TAG " (" INFO_GIT_BRANCH " " INFO_GIT_HASH ")\n# 本文件编码为 UTF-8。\n" };
            constexpr auto header_size{ std::char_traits< char8_t >::length( header ) * sizeof( char8_t ) };
            config_file_stream.write( reinterpret_cast< const char* >( header ), header_size );
            std::apply( [ & ]( auto&... config_node ) { ( config_node.sync( config_file_stream ), ... ); }, config_nodes );
            config_file_stream.flush();
            std::print( " (i) 同步配置{}.", config_file_stream.good() ? "成功" : "失败" );
            press_any_key_to_return();
            return func_back;
        }
        auto open_config_file()
        {
            std::print(
              cpp_utils::value_identity_v< cpp_utils::concat_const_string(
                make_middle_text< "[ 配  置 ]", 3 >,  " -> 尝试打开配置文件.\n\n"_cs ) >.view() );
            bool success{ false };
            if ( std::filesystem::exists( config_file_name ) ) {
                auto cmd{
                  cpp_utils::concat_const_string( L"notepad.exe "_cs, cpp_utils::const_wstring{ config_file_name } ).data() };
                STARTUPINFOW startup_info{};
                PROCESS_INFORMATION proc_info;
                startup_info.cb = sizeof( startup_info );
                if ( CreateProcessW( nullptr, cmd.data(), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &startup_info, &proc_info ) )
                  [[likely]]
                {
                    CloseHandle( proc_info.hProcess );
                    CloseHandle( proc_info.hThread );
                    success = true;
                }
            }
            std::print( " (i) 打开配置文件{}.", success ? "成功" : "失败" );
            press_any_key_to_return();
            return func_back;
        }
    }
    auto config_ui()
    {
        std::apply( []( auto&... nodes ) static
        {
            cpp_utils::console_ui ui{ con, unsynced_mem_pool };
            ui.reserve( 5 + ( decltype( nodes.request_ui_count() )::value + ... ) )
              .add_back( make_middle_text< "[ 配  置 ]", 2 >.view() )
              .add_back( " < 返回\n", quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
              .add_back( " > 查看解析规则 ", details_::show_config_parsing_rules )
              .add_back( " > 同步配置 ", details_::sync_config )
              .add_back( " > 打开配置文件 ", details_::open_config_file );
            ( nodes.init_ui( ui ), ... );
            ui.show();
        }, config_nodes );
        return func_back;
    }
    auto info()
    {
        cpp_utils::console_ui ui{ con, unsynced_mem_pool };
        ui.reserve( 3 )
          .add_back( make_middle_text< "[ 关  于 ]", 2 >.view() )
          .add_back( " < 返回 ", quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
          .add_back(
            "\n[ 软件名 ]\n\n " INFO_FULL_NAME " (" INFO_SHORT_NAME ")\n\n[ 软件版本 ]\n\n Tag: " INFO_GIT_TAG
            "\n Branch: " INFO_GIT_BRANCH "\n Commit: " INFO_GIT_HASH "\n\n[ 许可证与版权 ]\n\n " INFO_LICENSE
            "\n\n " INFO_COPYRIGHT "\n\n[ 开源仓库 ]\n\n " INFO_REPO_URL )
          .show();
        return func_back;
    }
    namespace details_
    {
        template < cpp_utils::const_string Description, void ( *Func )() noexcept >
        struct func_item final
        {
            static constexpr auto description{ Description };
            static auto execute() noexcept
            {
                std::print( make_middle_text< "[ 工 具 箱 ]", 3 >.view() );
                Func();
                std::print( "\n (i) 操作已完成." );
                press_any_key_to_return();
                return func_back;
            }
        };
        auto launch_cmd()
        {
            STARTUPINFOW startup_info{};
            PROCESS_INFORMATION proc_info;
            wchar_t cmd[]{ L"cmd.exe" };
            startup_info.cb = sizeof( startup_info );
            if ( CreateProcessW( nullptr, cmd, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &startup_info, &proc_info ) )
              [[likely]]
            {
                con.set_title( L"" INFO_SHORT_NAME " - 命令提示符" )
                  .set_size( 120, 30, unsynced_mem_pool )
                  .fix_size( false )
                  .enable_window_maximize_ctrl( true )
                  .show_cursor( true )
                  .lock_text( false );
                SetConsoleScreenBufferSize( con.std_output_handle, { 120, std::numeric_limits< SHORT >::max() - 1 } );
                WaitForSingleObject( proc_info.hProcess, INFINITE );
                CloseHandle( proc_info.hProcess );
                CloseHandle( proc_info.hThread );
                con.set_charset( charset_id )
                  .set_title( L"" INFO_SHORT_NAME )
                  .set_size( console_width, console_height, unsynced_mem_pool )
                  .fix_size( true )
                  .enable_window_maximize_ctrl( false );
            }
            return func_back;
        }
        auto restore_os_components() noexcept
        {
            using execs_t = make_const_wstring_list_t<
              L"tasklist", L"taskkill", L"ntsd", L"sc", L"net", L"reg", L"cmd", L"taskmgr", L"perfmon", L"regedit", L"mmc",
              L"dism", L"sfc", L"netsh", L"sethc", L"sidebar", L"shvlzm", L"winmine", L"bckgzm", L"Chess", L"chkrzm", L"route",
              L"FreeCell", L"Hearts", L"Magnify", L"Mahjong", L"Minesweeper", L"PurblePlace", L"Solitaire", L"SpiderSolitaire" >;
            constexpr auto ifeo_regs{
              []< cpp_utils::const_wstring... Items >(
                const cpp_utils::type_list< cpp_utils::value_identity< Items >... > ) static consteval noexcept
            {
                return std::array< std::wstring_view, sizeof...( Items ) * 2 >{
                    cpp_utils::value_identity_v< cpp_utils::concat_const_string(
                      LR"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\)"_cs, Items, L".exe"_cs ) >.view()...,
                    cpp_utils::value_identity_v< cpp_utils::concat_const_string(
                      LR"(SOFTWARE\WOW6432Node\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\)"_cs, Items, L".exe"_cs ) >.view()... };
            }( execs_t{} ) };
            constexpr std::array policy_key_regs{
              LR"(Software\Policies\Microsoft\Windows\System)"sv,
              LR"(Software\Policies\Microsoft\MMC)"sv,
              LR"(Software\Microsoft\Windows\CurrentVersion\Policies\System)"sv,
              LR"(Software\Microsoft\Windows\CurrentVersion\Policies\Explorer)"sv,
            };
            constexpr std::pair< std::wstring_view, std::wstring_view > policy_value_regs[]{
              {LR"(SOFTWARE\Policies\Microsoft\Windows NT\SystemRestore)"sv, L"DisableSR"sv   },
              {LR"(Control Panel\Desktop)"sv,                                L"AutoEndTasks"sv}
            };
            constexpr std::pair< std::wstring_view, std::wstring_view > need_enabled_regs[]{
              {LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Advanced)"sv,                       L"ShowTaskViewButton"sv},
              {LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Advanced\Folder\Hidden\SHOWALL)"sv, L"CheckedValue"sv      }
            };
            constexpr DWORD need_enabled_reg_value{ 1 };
            std::print( " -> 撤销映像劫持.\n" );
            for ( const auto& ifeo_reg : ifeo_regs ) {
                ( void ) cpp_utils::delete_registry_tree_without_redirect( HKEY_LOCAL_MACHINE, ifeo_reg );
            }
            std::print( " -> 撤销功能禁用.\n" );
            for ( const auto& policy_reg : policy_key_regs ) {
                ( void ) cpp_utils::delete_registry_tree_without_redirect( HKEY_CURRENT_USER, policy_reg );
            }
            for ( const auto& [ key, value ] : policy_value_regs ) {
                ( void ) cpp_utils::delete_registry_key_without_redirect( HKEY_LOCAL_MACHINE, key, value );
            }
            for ( const auto& [ key, value ] : need_enabled_regs ) {
                ( void ) cpp_utils::create_registry_key_without_redirect(
                  HKEY_LOCAL_MACHINE, key, value, cpp_utils::registry_flag::dword_type,
                  reinterpret_cast< const BYTE* >( &need_enabled_reg_value ), sizeof( need_enabled_reg_value ) );
            }
        }
        auto remove_malicious_route_rules() noexcept
        {
            std::print( " -> 移除恶意路由规则.\n" );
            constexpr auto normal_route{ []( const DWORD ip ) static noexcept
            {
                const auto first_byte{ static_cast< BYTE >( ( ip >> 24 ) & 0xFF ) };
                return ( first_byte == 127 ) || ( first_byte == 224 ) || ( ip == 0xFFFFFFFF ) || ( ip == 0x00000000 );
            } };
            constexpr auto private_ip{ []( const DWORD ip ) static noexcept
            {
                const auto first_byte{ static_cast< BYTE >( ( ip >> 24 ) & 0xFF ) };
                const auto second_byte{ static_cast< BYTE >( ( ip >> 16 ) & 0xFF ) };
                if ( first_byte == 10 ) {
                    return true;
                }
                if ( first_byte == 192 && second_byte == 168 ) {
                    return true;
                }
                if ( first_byte == 169 && second_byte == 254 ) {
                    return true;
                }
                if ( first_byte == 172 ) {
                    return ( second_byte >= 16 && second_byte <= 31 );
                }
                return false;
            } };
            constexpr auto deleter{ []( PMIB_IPFORWARDTABLE ptr ) static noexcept { std::free( ptr ); } };
            std::unique_ptr< std::remove_pointer_t< PMIB_IPFORWARDTABLE >, decltype( deleter ) > route_table{ nullptr };
            DWORD route_table_size{ 0 };
            if ( GetIpForwardTable( nullptr, &route_table_size, 0 ) == ERROR_INSUFFICIENT_BUFFER ) {
                route_table.reset( static_cast< PMIB_IPFORWARDTABLE >( std::malloc( route_table_size ) ) );
            }
            if ( route_table == nullptr || GetIpForwardTable( route_table.get(), &route_table_size, 0 ) != NO_ERROR ) {
                return;
            }
            std::pmr::vector< DWORD > malicious_indices{ unsynced_mem_pool };
            malicious_indices.reserve( route_table->dwNumEntries );
            for ( DWORD i{ 0 }; i < route_table->dwNumEntries; ++i ) {
                const auto& ip{ route_table->table[ i ] };
                if ( ip.dwForwardMask == 0xFFFFFFFF && !normal_route( ip.dwForwardDest ) && !private_ip( ip.dwForwardDest ) ) {
                    malicious_indices.emplace_back( i );
                }
            }
            if ( malicious_indices.empty() ) {
                return;
            }
            std::ranges::sort( malicious_indices, std::ranges::greater{} );
            for ( const auto& idx : malicious_indices ) {
                if ( idx >= route_table->dwNumEntries ) {
                    continue;
                }
                DeleteIpForwardEntry( &route_table->table[ idx ] );
            }
        }
        auto reset_firewall_rules() noexcept
        {
            std::print( " -> 重置防火墙规则.\n" );
            STARTUPINFOW startup_info{};
            PROCESS_INFORMATION proc_info{};
            SECURITY_ATTRIBUTES sec_attrib{ sizeof( sec_attrib ), nullptr, TRUE };
            startup_info.cb = sizeof( startup_info );
            const auto nul_file_handle{
              CreateFileW( L"NUL", GENERIC_WRITE, FILE_SHARE_WRITE, &sec_attrib, OPEN_EXISTING, 0, nullptr ) };
            if ( nul_file_handle == INVALID_HANDLE_VALUE ) [[unlikely]] {
                return;
            }
            startup_info.dwFlags    = STARTF_USESTDHANDLES;
            startup_info.hStdInput  = GetStdHandle( STD_INPUT_HANDLE );
            startup_info.hStdOutput = nul_file_handle;
            startup_info.hStdError  = nul_file_handle;
            wchar_t cmd[]{ L"netsh.exe advfirewall reset" };
            const auto has_created_process_successfully{ CreateProcessW(
              nullptr, cmd, nullptr, nullptr, TRUE, CREATE_NO_WINDOW | CREATE_UNICODE_ENVIRONMENT, nullptr, nullptr,
              &startup_info, &proc_info ) };
            CloseHandle( nul_file_handle );
            if ( has_created_process_successfully ) [[likely]] {
                WaitForSingleObject( proc_info.hProcess, INFINITE );
                CloseHandle( proc_info.hProcess );
                CloseHandle( proc_info.hThread );
            }
        }
        auto reset_hosts() noexcept
        {
            std::print( " -> 重置 Hosts.\n" );
            constexpr const auto& default_content{
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
            constexpr const auto& reset_hosts_error_message{ "\n (!) 重置 Hosts 失败.\n\n" };
#ifndef _WIN64
            struct wow64_filesystem_guard final
            {
                PVOID value{ nullptr };
                auto operator=( const wow64_filesystem_guard& ) noexcept -> wow64_filesystem_guard& = delete;
                wow64_filesystem_guard() noexcept
                {
                    Wow64DisableWow64FsRedirection( &value );
                }
                wow64_filesystem_guard( const wow64_filesystem_guard& ) noexcept = delete;
                ~wow64_filesystem_guard() noexcept
                {
                    Wow64RevertWow64FsRedirection( value );
                }
            } filesystem_redirect_guard;
#endif
            const auto hosts_path{ [] static
            {
                std::array< wchar_t, MAX_PATH > result;
                GetWindowsDirectoryW( result.data(), MAX_PATH );
                std::ranges::copy( LR"(\System32\drivers\etc\hosts)", std::ranges::find( result, L'\0' ) );
                return std::filesystem::path{ result.data() };
            }() };
            std::error_code ec;
            const auto original_perms{ std::filesystem::status( hosts_path, ec ).permissions() };
            if ( ec ) [[unlikely]] {
                std::print( reset_hosts_error_message );
                return;
            }
            std::filesystem::permissions( hosts_path, std::filesystem::perms::all, std::filesystem::perm_options::replace, ec );
            std::filesystem::remove( hosts_path, ec );
            std::ofstream file{ hosts_path, std::ios::out | std::ios::trunc };
            file.write( default_content, sizeof( default_content ) - sizeof( '\0' ) ).flush();
            if ( !file.good() ) [[unlikely]] {
                std::print( reset_hosts_error_message );
            }
            std::filesystem::permissions( hosts_path, original_perms, std::filesystem::perm_options::replace, ec );
        }
        auto flush_dns() noexcept
        {
            constexpr const auto& flush_dns_error_message{ "\n (!) 刷新 DNS 失败.\n\n" };
            std::print( " -> 刷新 DNS 缓存.\n" );
            const auto dnsapi{ LoadLibraryW( L"dnsapi.dll" ) };
            if ( dnsapi == nullptr ) [[unlikely]] {
                std::print( flush_dns_error_message );
                return;
            }
            const auto dns_flush_resolver_cache{
              std::bit_cast< BOOL( WINAPI* )() noexcept >( GetProcAddress( dnsapi, "DnsFlushResolverCache" ) ) };
            if ( dns_flush_resolver_cache == nullptr ) [[unlikely]] {
                std::print( flush_dns_error_message );
                return;
            }
            if ( !dns_flush_resolver_cache() ) [[unlikely]] {
                std::print( flush_dns_error_message );
            }
            FreeLibrary( dnsapi );
        }
        auto reset_partial_network_settings() noexcept
        {
            remove_malicious_route_rules();
            reset_firewall_rules();
            reset_hosts();
            flush_dns();
        }
        auto reset_jfglzs_config() noexcept
        {
            std::print( " -> 删除密码.\n" );
            ( void ) cpp_utils::delete_registry_key_without_redirect( HKEY_CURRENT_USER, L"Software"sv, L"n"sv );
            std::print( " -> 删除配置.\n" );
            ( void ) cpp_utils::delete_registry_tree_without_redirect( HKEY_CURRENT_USER, LR"(Software\jfglzs)"sv );
            std::print( " -> 删除自启动项.\n" );
            constexpr std::array autorun_items{ L"jfglzs"sv, L"jfglzsn"sv, L"jfglzsp"sv, L"prozs"sv, L"przs"sv };
            constexpr std::array notification_items{ L"StartupTNotijfglzsn"sv, L"StartupTNotiprozs"sv };
            for ( const auto& autorun_item : autorun_items ) {
                ( void ) cpp_utils::delete_registry_key_without_redirect(
                  HKEY_LOCAL_MACHINE, LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\Run)", autorun_item );
                ( void ) cpp_utils::delete_registry_key_without_redirect(
                  HKEY_LOCAL_MACHINE, LR"(SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Run)", autorun_item );
            }
            for ( const auto& notification_item : notification_items ) {
                ( void ) cpp_utils::delete_registry_key_without_redirect(
                  HKEY_CURRENT_USER, LR"(Software\Microsoft\Windows\CurrentVersion\RunNotification)"sv, notification_item );
            }
        }
        auto relaunch_explorer() noexcept
        {
            std::print( " -> 终止进程.\n" );
            ( void ) cpp_utils::terminate_process_by_name( L"explorer.exe"sv );
            std::print( " -> 启动进程.\n" );
            ShellExecuteW( nullptr, L"open", L"explorer.exe", nullptr, nullptr, SW_HIDE );
        }
        auto restore_usb_device_access() noexcept
        {
            std::print( " -> 写入注册表.\n" );
            constexpr DWORD start_type{ 3 };
            ( void ) cpp_utils::create_registry_key_without_redirect(
              HKEY_LOCAL_MACHINE, LR"(SYSTEM\CurrentControlSet\Services\USBSTOR)"sv, L"Start"sv,
              cpp_utils::registry_flag::dword_type, reinterpret_cast< const BYTE* >( &start_type ), sizeof( start_type ) );
        }
        auto reset_common_web_browsers_policy() noexcept
        {
            std::print( " -> 删除注册表.\n" );
            constexpr std::array regs{
              LR"(SOFTWARE\Policies\Google\Chrome)"sv,   LR"(SOFTWARE\WOW6432Node\Policies\Google\Chrome)"sv,
              LR"(SOFTWARE\Policies\Microsoft\Edge)"sv,  LR"(SOFTWARE\WOW6432Node\Policies\Microsoft\Edge)"sv,
              LR"(SOFTWARE\Policies\Mozilla\Firefox)"sv, LR"(SOFTWARE\WOW6432Node\Policies\Mozilla\Firefox)"sv };
            for ( const auto& reg : regs ) {
                ( void ) cpp_utils::delete_registry_tree_without_redirect( HKEY_LOCAL_MACHINE, reg );
            }
        }
    }
    auto toolkit()
    {
        using funcs_t = cpp_utils::type_list<
          details_::func_item< "重启资源管理器", details_::relaunch_explorer >,
          details_::func_item< "恢复操作系统组件", details_::restore_os_components >,
          details_::func_item< "恢复 USB 存储设备访问", details_::restore_usb_device_access >,
          details_::func_item< "重置部分网络设置", details_::reset_partial_network_settings >,
          details_::func_item< "重置 \"机房管理助手\" 配置", details_::reset_jfglzs_config >,
          details_::func_item< "重置 Chrome & Edge & Firefox 管理策略", details_::reset_common_web_browsers_policy > >;
        cpp_utils::console_ui ui{ con, unsynced_mem_pool };
        ui.reserve( 4 + funcs_t::size )
          .add_back( make_middle_text< "[ 工 具 箱 ]", 2 >.view() )
          .add_back( " < 返回 ", quit, cpp_utils::console_text::foreground_green | cpp_utils::console_text::foreground_intensity )
          .add_back(
            "\n[ 快捷工具 ]\n\n"
            " (i) 请破解电子教室软件后再使用此处功能.\n" )
          .add_back( " > 启动命令提示符\n", details_::launch_cmd );
        [ & ]< typename... Items >( const cpp_utils::type_list< Items... > )
        { ( ui.add_back( make_item_text< Items::description >.view(), Items::execute ), ... ); }( funcs_t{} );
        ui.show();
        return func_back;
    }
    namespace details_
    {
        auto enable_translucent() noexcept
        {
            constexpr const auto& no_async_hot_reload{ std::get< perf_config >( config_nodes ).at< "no_async_hot_reload" >() };
            constexpr const auto& enabled{ std::get< window_config >( config_nodes ).at< "translucent" >() };
            if ( no_async_hot_reload ) {
                con.set_translucency( enabled.test( std::memory_order_acquire ) ? 217 : 255 );
            }
            for ( ;; ) {
                enabled.wait( false, std::memory_order_acquire );
                con.set_translucency( 217 );
                enabled.wait( true, std::memory_order_acquire );
                con.set_translucency( 255 );
            }
        }
        auto enable_simple_titlebar() noexcept
        {
            constexpr const auto& no_async_hot_reload{ std::get< perf_config >( config_nodes ).at< "no_async_hot_reload" >() };
            constexpr const auto& enabled{ std::get< window_config >( config_nodes ).at< "simple_titlebar" >() };
            if ( no_async_hot_reload ) {
                con.enable_context_menu( !enabled.test( std::memory_order_acquire ) );
            }
            for ( ;; ) {
                enabled.wait( false, std::memory_order_acquire );
                con.enable_context_menu( false );
                enabled.wait( true, std::memory_order_acquire );
                con.enable_context_menu( true );
            }
        }
        auto force_show() noexcept
        {
            constexpr const auto& no_async_hot_reload{ std::get< perf_config >( config_nodes ).at< "no_async_hot_reload" >() };
            constexpr const auto& enabled{ std::get< window_config >( config_nodes ).at< "force_show" >() };
            if ( no_async_hot_reload && !enabled.test( std::memory_order_acquire ) ) {
                return;
            }
            constexpr auto sleep_duration{ 50ms };
            if ( no_async_hot_reload ) {
                con.force_show_forever( sleep_duration );
            }
            constexpr auto condition_checker{ [] static noexcept
            {
                if ( enabled.test( std::memory_order_acquire ) == false ) {
                    con.cancel_force_show();
                    enabled.wait( false, std::memory_order_acquire );
                }
                return false;
            } };
            con.force_show_until( sleep_duration, condition_checker );
        }
    }
    auto create_parallel_tasks() noexcept
    {
        constexpr std::array parallel_tasks{ details_::enable_translucent, details_::enable_simple_titlebar, details_::force_show };
        for ( const auto& parallel_task : parallel_tasks ) {
            std::thread{ parallel_task }.detach();
        }
    }
    namespace details_
    {
        enum class rule_executor_mode : bool
        {
            crack,
            restore
        };
        auto current_rule_executor_mode{ rule_executor_mode::crack };
        constexpr std::array mythware_servs{ L"STUDSRV"sv, L"TDKeybd"sv, L"TDNetFilter"sv, L"TDFileFilter"sv };
    }
    using builtin_rules_t = cpp_utils::type_list<
      compile_time_rule_node<
        "机房管理助手",
        details_::make_const_wstring_list_t<
          L"yz.exe", L"jfglzs.exe", L"jfglzsn.exe", L"jfglzsp.exe", L"przs.exe", L"zmserv.exe", L"zmsrv.exe" >,
        details_::make_const_wstring_list_t< L"zmserv" >, details_::terminate_jfglzs_daemon >,
      compile_time_rule_node<
        "极域电子教室",
        details_::make_const_wstring_list_t<
          L"StudentMain.exe", L"DispcapHelper.exe", L"VRCwPlayer.exe", L"InstHelpApp.exe", L"InstHelpApp64.exe",
          L"TDOvrSet.exe", L"GATESRV.exe", L"ProcHelper64.exe", L"MasterHelper.exe" >,
        details_::make_const_wstring_list_t<>,
        [] static noexcept
    {
        constexpr const auto& use_set_serv_startup_types{
          std::get< crack_restore_config >( config_nodes ).at< "set_servs_start_type" >() };
        if ( use_set_serv_startup_types ) {
            for ( const auto& serv : details_::mythware_servs ) {
                ( void ) cpp_utils::set_service_start_type( serv, cpp_utils::service_flag::disabled_start );
            }
        }
        for ( const auto& serv : details_::mythware_servs ) {
            ( void ) cpp_utils::stop_service_with_dependencies( serv );
        }
    },
        [] static noexcept
    {
        constexpr const auto& use_set_serv_startup_types{
          std::get< crack_restore_config >( config_nodes ).at< "set_servs_start_type" >() };
        if ( use_set_serv_startup_types ) {
            for ( const auto& serv : details_::mythware_servs ) {
                ( void ) cpp_utils::set_service_start_type( serv, cpp_utils::service_flag::auto_start );
            }
        }
        for ( const auto& serv : details_::mythware_servs ) {
            ( void ) cpp_utils::start_service_with_dependencies( serv );
        }
    } >,
      compile_time_rule_node<
        "联想智能云教室",
        details_::make_const_wstring_list_t<
          L"WfbsPnpInstall.exe", L"WFBSMon.exe", L"WFBSMlogon.exe", L"WFBSSvrLogShow.exe", L"ResetIp.exe", L"FuncForWIN64.exe",
          L"Fireware.exe", L"BCDBootCopy.exe", L"refreship.exe", L"WFDeskShow.exe", L"lenovoLockScreen.exe",
          L"PortControl64.exe", L"DesktopCheck.exe", L"DeploymentManager.exe", L"DeploymentAgent.exe", L"XYNTService.exe" >,
        details_::make_const_wstring_list_t< L"BSAgentSvr", L"tvnserver", L"WFBSMlogon" > >,
      compile_time_rule_node<
        "红蜘蛛多媒体网络教室",
        details_::make_const_wstring_list_t<
          L"rscheck.exe", L"checkrs.exe", L"REDAgent.exe", L"PerformanceCheck.exe", L"edpaper.exe", L"Adapter.exe",
          L"repview.exe", L"FormatPaper.exe" >,
        details_::make_const_wstring_list_t< L"appcheck2", L"checkapp2" > >,
      compile_time_rule_node< "伽卡他卡电子教室", details_::make_const_wstring_list_t< L"Student.exe", L"Smonitor.exe" >,
                              details_::make_const_wstring_list_t< L"Smsvc" > >,
      compile_time_rule_node<
        "凌波网络教室", details_::make_const_wstring_list_t< L"sbkup.exe", L"wsf.exe", L"NCStu.exe", L"NCCmn.dll" >,
        details_::make_const_wstring_list_t< L"Windows Application and Components Data Backup Support Service" > >,
      compile_time_rule_node<
        "Veyon",
        details_::make_const_wstring_list_t<
          L"veyon-worker.exe", L"veyon-configurator.exe", L"veyon-server.exe", L"veyon-cli.exe", L"veyon-wcli.exe",
          L"veyon-service.exe" >,
        details_::make_const_wstring_list_t< L"VeyonService" > > >;
    template < typename... Backends >
        requires requires {
            requires cpp_utils::as_concept< ( sizeof...( Backends ) != 0 ) >;
            { ( Backends::run_enable_servs || ... ) } -> std::convertible_to< bool >;
            ( Backends::enable_servs(), ... );
            { ( Backends::run_disable_servs || ... ) } -> std::convertible_to< bool >;
            ( Backends::disable_servs(), ... );
            { ( Backends::run_start_servs || ... ) } -> std::convertible_to< bool >;
            ( Backends::start_servs(), ... );
            { ( Backends::run_stop_servs || ... ) } -> std::convertible_to< bool >;
            ( Backends::stop_servs(), ... );
            { ( Backends::run_hijack_procs || ... ) } -> std::convertible_to< bool >;
            ( Backends::hijack_procs(), ... );
            { ( Backends::run_undo_hijack_procs || ... ) } -> std::convertible_to< bool >;
            ( Backends::undo_hijack_procs(), ... );
            { ( Backends::run_terminate_procs || ... ) } -> std::convertible_to< bool >;
            ( Backends::terminate_procs(), ... );
            { ( Backends::run_crack_helper || ... ) } -> std::convertible_to< bool >;
            ( Backends::crack_helper(), ... );
            { ( Backends::run_restore_helper || ... ) } -> std::convertible_to< bool >;
            ( Backends::restore_helper(), ... );
        }
    struct rule_executor final
    {
        static auto crack()
        {
            constexpr const auto& options{ std::get< crack_restore_config >( config_nodes ) };
            constexpr const auto& use_hijack_procs{ options.at< "hijack_procs" >() };
            constexpr const auto& use_set_serv_startup_types{ options.at< "set_servs_start_type" >() };
            if constexpr ( ( Backends::run_hijack_procs || ... ) ) {
                if ( use_hijack_procs ) {
                    std::print( " -> 劫持进程.\n" );
                    (
                      []< typename Backend >() static
                    {
                        if constexpr ( Backend::run_hijack_procs ) {
                            Backend::hijack_procs();
                        }
                    }.template operator()< Backends >(),
                      ... );
                }
            }
            if constexpr ( ( Backends::run_disable_servs || ... ) ) {
                if ( use_set_serv_startup_types ) {
                    std::print( " -> 禁用服务.\n" );
                    (
                      []< typename Backend >() static
                    {
                        if constexpr ( Backend::run_disable_servs ) {
                            Backend::disable_servs();
                        }
                    }.template operator()< Backends >(),
                      ... );
                }
            }
            if constexpr ( ( Backends::run_stop_servs || ... ) ) {
                std::print( " -> 停止服务.\n" );
                (
                  []< typename Backend >() static
                {
                    if constexpr ( Backend::run_stop_servs ) {
                        Backend::stop_servs();
                    }
                }.template operator()< Backends >(),
                  ... );
            }
            if constexpr ( ( Backends::run_terminate_procs || ... ) ) {
                std::print( " -> 终止进程.\n" );
                (
                  []< typename Backend >() static
                {
                    if constexpr ( Backend::run_terminate_procs ) {
                        Backend::terminate_procs();
                    }
                }.template operator()< Backends >(),
                  ... );
            }
            if constexpr ( ( Backends::run_crack_helper || ... ) ) {
                std::print( " -> 执行扩展操作.\n" );
                (
                  []< typename Backend >() static
                {
                    if constexpr ( Backend::run_crack_helper ) {
                        Backend::crack_helper();
                    }
                }.template operator()< Backends >(),
                  ... );
            }
        }
        static auto restore()
        {
            constexpr const auto& options{ std::get< crack_restore_config >( config_nodes ) };
            constexpr const auto& use_hijack_procs{ options.at< "hijack_procs" >() };
            constexpr const auto& use_set_serv_startup_types{ options.at< "set_servs_start_type" >() };
            if constexpr ( ( Backends::run_undo_hijack_procs || ... ) ) {
                if ( use_hijack_procs ) {
                    std::print( " -> 撤销劫持.\n" );
                    (
                      []< typename Backend >() static
                    {
                        if constexpr ( Backend::run_undo_hijack_procs ) {
                            Backend::undo_hijack_procs();
                        }
                    }.template operator()< Backends >(),
                      ... );
                }
            }
            if constexpr ( ( Backends::run_enable_servs || ... ) ) {
                if ( use_set_serv_startup_types ) {
                    std::print( " -> 启用服务.\n" );
                    (
                      []< typename Backend >() static
                    {
                        if constexpr ( Backend::run_enable_servs ) {
                            Backend::enable_servs();
                        }
                    }.template operator()< Backends >(),
                      ... );
                }
            }
            if constexpr ( ( Backends::run_start_servs || ... ) ) {
                std::print( " -> 启动服务.\n" );
                (
                  []< typename Backend >() static
                {
                    if constexpr ( Backend::run_start_servs ) {
                        Backend::start_servs();
                    }
                }.template operator()< Backends >(),
                  ... );
            }
            if constexpr ( ( Backends::run_restore_helper || ... ) ) {
                std::print( " -> 执行扩展操作.\n" );
                (
                  []< typename Backend >() static
                {
                    if constexpr ( Backend::run_restore_helper ) {
                        Backend::restore_helper();
                    }
                }.template operator()< Backends >(),
                  ... );
            }
        }
        static auto operator()()
        {
            switch ( details_::current_rule_executor_mode ) {
                case details_::rule_executor_mode::crack : std::print( make_middle_text< "[ 破  解 ]", 3 >.view() ); break;
                case details_::rule_executor_mode::restore : std::print( make_middle_text< "[ 恢  复 ]", 3 >.view() ); break;
            }
            switch ( details_::current_rule_executor_mode ) {
                case details_::rule_executor_mode::crack : crack(); break;
                case details_::rule_executor_mode::restore : restore(); break;
            }
            std::print( "\n (i) 操作已完成." );
            details_::press_any_key_to_return();
            return func_back;
        }
    };
    template < typename BuiltinRuleNode >
    struct builtin_rules_executor_backend final
    {
        using empty_lambda_t = decltype( details_::empty_lambda );
        static constexpr auto procs{
          []< cpp_utils::const_wstring... Procs >(
            const cpp_utils::type_list< cpp_utils::value_identity< Procs >... > ) static consteval noexcept
        { return std::array< std::wstring_view, sizeof...( Procs ) >{ Procs.view()... }; }( typename BuiltinRuleNode::procs{} ) };
        static constexpr auto servs{
          []< cpp_utils::const_wstring... Servs >(
            const cpp_utils::type_list< cpp_utils::value_identity< Servs >... > ) static consteval noexcept
        { return std::array< std::wstring_view, sizeof...( Servs ) >{ Servs.view()... }; }( typename BuiltinRuleNode::servs{} ) };
        static constexpr auto ifeo_regs{
          []< cpp_utils::const_wstring... Procs >(
            const cpp_utils::type_list< cpp_utils::value_identity< Procs >... > ) static consteval noexcept
        {
            return std::array< std::wstring_view, sizeof...( Procs ) * 2 >{
                cpp_utils::value_identity_v< cpp_utils::concat_const_string(
                  LR"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\)"_cs, Procs ) >.view()...,
                cpp_utils::value_identity_v< cpp_utils::concat_const_string(
                  LR"(SOFTWARE\WOW6432Node\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\)"_cs, Procs ) >.view()... };
        }( typename BuiltinRuleNode::procs{} ) };
        static constexpr auto run_enable_servs{ !servs.empty() };
        static auto enable_servs() noexcept
        {
            for ( const auto& serv : servs ) {
                ( void ) cpp_utils::set_service_start_type( serv, cpp_utils::service_flag::auto_start );
            }
        }
        static constexpr auto run_disable_servs{ !servs.empty() };
        static auto disable_servs() noexcept
        {
            for ( const auto& serv : servs ) {
                ( void ) cpp_utils::set_service_start_type( serv, cpp_utils::service_flag::disabled_start );
            }
        }
        static constexpr auto run_start_servs{ !servs.empty() };
        static auto start_servs() noexcept
        {
            for ( const auto& serv : servs ) {
                ( void ) cpp_utils::start_service_with_dependencies( serv, unsynced_mem_pool );
            }
        }
        static constexpr auto run_stop_servs{ !servs.empty() };
        static auto stop_servs() noexcept
        {
            for ( const auto& serv : servs ) {
                ( void ) cpp_utils::stop_service_with_dependencies( serv, unsynced_mem_pool );
            }
        }
        static constexpr auto run_hijack_procs{ !ifeo_regs.empty() };
        static auto hijack_procs() noexcept
        {
            constexpr const auto& value{ L"nul" };
            for ( const auto& reg : ifeo_regs ) {
                ( void ) cpp_utils::create_registry_key_without_redirect(
                  HKEY_LOCAL_MACHINE, reg, L"Debugger", cpp_utils::registry_flag::string_type,
                  reinterpret_cast< const BYTE* >( +value ), sizeof( value ) );
            }
        }
        static constexpr auto run_undo_hijack_procs{ !ifeo_regs.empty() };
        static auto undo_hijack_procs() noexcept
        {
            for ( const auto& reg : ifeo_regs ) {
                ( void ) cpp_utils::delete_registry_tree_without_redirect( HKEY_LOCAL_MACHINE, reg );
            }
        }
        static constexpr auto run_terminate_procs{ !procs.empty() };
        static auto terminate_procs() noexcept
        {
            ( void ) cpp_utils::terminate_process_by_names( procs );
        }
        static constexpr auto run_crack_helper{ !std::is_same_v< decltype( BuiltinRuleNode::crack_helper ), empty_lambda_t > };
        static auto crack_helper()
        {
            BuiltinRuleNode::crack_helper();
        }
        static constexpr auto run_restore_helper{ !std::is_same_v< decltype( BuiltinRuleNode::restore_helper ), empty_lambda_t > };
        static auto restore_helper()
        {
            BuiltinRuleNode::restore_helper();
        }
    };
    struct custom_rule_executor_backend final
    {
        static constexpr auto run_enable_servs{ true };
        static auto enable_servs() noexcept
        {
            for ( const auto& serv : custom_rules.servs ) {
                ( void ) cpp_utils::set_service_start_type( serv, cpp_utils::service_flag::auto_start );
            }
        }
        static constexpr auto run_disable_servs{ true };
        static auto disable_servs() noexcept
        {
            for ( const auto& serv : custom_rules.servs ) {
                ( void ) cpp_utils::set_service_start_type( serv, cpp_utils::service_flag::disabled_start );
            }
        }
        static constexpr auto run_start_servs{ true };
        static auto start_servs() noexcept
        {
            for ( const auto& serv : custom_rules.servs ) {
                ( void ) cpp_utils::start_service_with_dependencies( serv, unsynced_mem_pool );
            }
        }
        static constexpr auto run_stop_servs{ true };
        static auto stop_servs() noexcept
        {
            for ( const auto& serv : custom_rules.servs ) {
                ( void ) cpp_utils::stop_service_with_dependencies( serv, unsynced_mem_pool );
            }
        }
        static constexpr auto run_hijack_procs{ true };
        static auto hijack_procs()
        {
            constexpr const auto& value{ L"nul" };
            for ( const auto& proc : custom_rules.procs ) {
                ( void ) cpp_utils::create_registry_key_without_redirect(
                  HKEY_LOCAL_MACHINE,
                  std::format( LR"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\{})", proc ),
                  L"Debugger", cpp_utils::registry_flag::string_type, reinterpret_cast< const BYTE* >( +value ), sizeof( value ) );
                ( void ) cpp_utils::create_registry_key_without_redirect(
                  HKEY_LOCAL_MACHINE,
                  std::format( LR"(SOFTWARE\WOW6432Node\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\{})", proc ),
                  L"Debugger", cpp_utils::registry_flag::string_type, reinterpret_cast< const BYTE* >( +value ), sizeof( value ) );
            }
        }
        static constexpr auto run_undo_hijack_procs{ true };
        static auto undo_hijack_procs()
        {
            for ( const auto& proc : custom_rules.procs ) {
                ( void ) cpp_utils::delete_registry_tree_without_redirect(
                  HKEY_LOCAL_MACHINE,
                  std::format( LR"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\{})", proc ) );
                ( void ) cpp_utils::delete_registry_tree_without_redirect(
                  HKEY_LOCAL_MACHINE,
                  std::format(
                    LR"(SOFTWARE\WOW6432Node\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\{})", proc ) );
            }
        }
        static constexpr auto run_terminate_procs{ true };
        static auto terminate_procs() noexcept
        {
            ( void ) cpp_utils::terminate_process_by_names( custom_rules.procs );
        }
        static auto execute_helpers_( const std::pmr::vector< std::pmr::wstring >& helpers ) noexcept
        {
            for ( const auto& helper : helpers ) {
                std::pmr::wstring cmd{ helper, unsynced_mem_pool };
                STARTUPINFOW startup_info{};
                PROCESS_INFORMATION proc_info{};
                startup_info.cb = sizeof( startup_info );
                if ( CreateProcessW( nullptr, cmd.data(), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &startup_info, &proc_info ) )
                  [[likely]]
                {
                    WaitForSingleObject( proc_info.hProcess, INFINITE );
                    CloseHandle( proc_info.hProcess );
                    CloseHandle( proc_info.hThread );
                }
            }
        }
        static constexpr auto run_crack_helper{ true };
        static auto crack_helper() noexcept
        {
            execute_helpers_( custom_rules.crack_helpers );
        }
        static constexpr auto run_restore_helper{ true };
        static auto restore_helper() noexcept
        {
            execute_helpers_( custom_rules.restore_helpers );
        }
    };
    using all_rules_t
      = builtin_rules_t::apply_each< builtin_rules_executor_backend >::add_front< custom_rule_executor_backend >::apply< rule_executor >;
    auto make_executor_mode_ui_text() noexcept
    {
        switch ( details_::current_rule_executor_mode ) {
            case details_::rule_executor_mode::crack : return "[ 破解 (点击切换) ]\n"sv;
            case details_::rule_executor_mode::restore : return "[ 恢复 (点击切换) ]\n"sv;
        }
    }
    auto flip_executor_mode( const ui_func_args_t args ) noexcept
    {
        switch ( details_::current_rule_executor_mode ) {
            case details_::rule_executor_mode::crack :
                details_::current_rule_executor_mode = details_::rule_executor_mode::restore;
                break;
            case details_::rule_executor_mode::restore :
                details_::current_rule_executor_mode = details_::rule_executor_mode::crack;
                break;
        }
        args.parent_ui.set_text( args.node_index, make_executor_mode_ui_text() );
        return func_back;
    }
}
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
    ( void ) cpp_utils::set_privilege( GetCurrentProcess(), L"" SE_DEBUG_NAME, true );
    scltk::load_config( false );
    scltk::create_parallel_tasks();
    cpp_utils::console_ui ui{ scltk::con, scltk::unsynced_mem_pool };
    ui.reserve( 9 + scltk::builtin_rules_t::size )
      .add_back( scltk::make_middle_text< "[ 主  页 ]", 2 >.view() )
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
            scltk::make_item_text< Nodes::display_name >.view(),
            scltk::rule_executor< scltk::builtin_rules_executor_backend< Nodes > >{} ),
          ... );
    }( scltk::builtin_rules_t{} );
    ui.show();
}
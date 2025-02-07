#pragma once
#if defined( _WIN32 ) || defined( _WIN64 )
# include <windows.h>
#endif
#include <chrono>
#include <concepts>
#include <deque>
#include <functional>
#include <print>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>
#include <vector>
namespace cpp_utils {
    using namespace std::chrono_literals;
    using namespace std::string_literals;
    using io_buffer             = std::FILE;
    using size_type             = std::size_t;
    using nullptr_type          = std::nullptr_t;
    using ansi_char             = char;
    using ansi_std_string       = std::string;
    using ansi_std_string_view  = std::string_view;
    using wide_char             = wchar_t;
    using wide_std_string       = std::wstring;
    using wide_std_string_view  = std::wstring_view;
    using utf8_char             = char8_t;
    using utf8_std_string       = std::u8string;
    using utf8_std_string_view  = std::u8string_view;
    using utf16_char            = char16_t;
    using utf16_std_string      = std::u16string;
    using utf16_std_string_view = std::u16string_view;
    using utf32_char            = char32_t;
    using utf32_std_string      = std::u32string;
    using utf32_std_string_view = std::u32string_view;
    template < typename _type_ >
    using type_alloc = _type_;
    template < typename _char_type >
    using std_string = std::basic_string< _char_type >;
    template < typename _char_type >
    using std_string_view = std::basic_string_view< _char_type >;
    template < typename _type_ >
    concept char_type
      = std::is_same_v< std::decay_t< _type_ >, ansi_char > || std::is_same_v< std::decay_t< _type_ >, wide_char >
     || std::is_same_v< std::decay_t< _type_ >, utf8_char > || std::is_same_v< std::decay_t< _type_ >, utf16_char >
     || std::is_same_v< std::decay_t< _type_ >, utf32_char >;
    template < typename _type_a_, typename _type_b_ >
    concept convertible_char_type = char_type< _type_a_ > && char_type< _type_b_ > && sizeof( _type_a_ ) == sizeof( _type_b_ );
    template < typename _type_ >
    concept pointer_type = std::is_pointer_v< _type_ >;
    template < typename _type_ >
    concept callable_object
      = !std::is_same_v< std::decay_t< _type_ >, std::thread > && !std::is_same_v< std::decay_t< _type_ >, std::jthread >;
    template < char_type _target_, char_type _source_ >
        requires convertible_char_type< _target_, _source_ >
    inline auto string_convert( const std_string< _source_ > &_str )
    {
        const auto str_it{ reinterpret_cast< const _target_ * >( _str.c_str() ) };
        return std_string< _target_ >{ str_it, str_it + _str.size() };
    }
    template < char_type _target_, char_type _source_ >
        requires convertible_char_type< _target_, _source_ >
    inline auto string_convert( const std_string_view< _source_ > _str )
    {
        const auto str_it{ reinterpret_cast< const _target_ * >( _str.data() ) };
        return std_string< _target_ >{ str_it, str_it + _str.size() };
    }
    template < char_type _target_, char_type _source_ >
        requires convertible_char_type< _target_, _source_ >
    inline auto string_convert( const _source_ *const _str )
    {
        return std_string< _target_ >{ reinterpret_cast< const _target_ * >( _str ) };
    }
    template < char_type _target_, char_type _source_ >
        requires convertible_char_type< _target_, _source_ >
    inline auto string_view_convert( const std_string< _source_ > &_str )
    {
        const auto str_it{ reinterpret_cast< const _target_ * >( _str.c_str() ) };
        return std_string_view< _target_ >{ str_it, str_it + _str.size() };
    }
    template < char_type _target_, char_type _source_ >
        requires convertible_char_type< _target_, _source_ >
    inline auto string_view_convert( const std_string_view< _source_ > _str )
    {
        const auto str_it{ reinterpret_cast< const _target_ * >( _str.data() ) };
        return std_string_view< _target_ >{ str_it, str_it + _str.size() };
    }
    template < char_type _target_, char_type _source_ >
        requires convertible_char_type< _target_, _source_ >
    inline auto string_view_convert( const _source_ *const _str )
    {
        return std_string_view< _target_ >{ reinterpret_cast< const _target_ * >( _str ) };
    }
    template < pointer_type _type_ >
    inline auto ptr_to_ansi_string( const _type_ _ptr )
    {
        return _ptr == nullptr ? "nullptr"s : std::format( "0x{:x}", reinterpret_cast< std::uintptr_t >( _ptr ) );
    }
    template < pointer_type _type_ >
    inline auto ptr_to_utf8_string( const _type_ _ptr )
    {
        return string_convert< utf8_char >(
          _ptr == nullptr ? "nullptr"s : std::format( "0x{:x}", reinterpret_cast< std::uintptr_t >( _ptr ) ) );
    }
    template < typename... _args_ >
    inline auto utf8_format( const utf8_std_string_view _fmt, _args_ &&..._args )
    {
        const auto convert_arg{ []< typename _type_ >( _type_ &&_arg ) static -> decltype( auto )
        {
            if constexpr ( std::is_same_v< std::decay_t< _type_ >, utf8_std_string > ) {
                return reinterpret_cast< ansi_std_string * >( &_arg );
            } else if constexpr ( std::is_same_v< std::decay_t< _type_ >, utf8_std_string_view > ) {
                return reinterpret_cast< ansi_std_string_view * >( &_arg );
            } else if constexpr ( std::is_same_v< std::decay_t< _type_ >, utf8_char * > ) {
                return std::make_unique< ansi_std_string_view >( reinterpret_cast< ansi_char * >( &_arg ) );
            } else if constexpr ( std::is_same_v< std::decay_t< _type_ >, const utf8_char * > ) {
                return std::make_unique< ansi_std_string_view >( reinterpret_cast< const ansi_char * >( &_arg ) );
            } else if constexpr (
              std::is_pointer_v< std::decay_t< _type_ > >
              && !( std::is_same_v< std::decay_t< _type_ >, ansi_char * >
                    || std::is_same_v< std::decay_t< _type_ >, const ansi_char * > ) )
            {
                return std::make_unique< const ansi_std_string >( ptr_to_ansi_string( std::forward< _type_ >( _arg ) ) );
            } else {
                return &_arg;
            }
        } };
        return string_convert< utf8_char >( std::vformat(
          string_view_convert< ansi_char >( _fmt ), std::make_format_args( *convert_arg( std::forward< _args_ >( _args ) )... ) ) );
    }
    template < typename... _args_ >
    inline auto &utf8_format_to( utf8_std_string &_str, const utf8_std_string_view _fmt, _args_ &&..._args )
    {
        auto tmp{ utf8_format( _fmt, std::forward< _args_ >( _args )... ) };
        _str.swap( tmp );
        return _str;
    }
    template < typename... _args_ >
    inline auto utf8_print( const utf8_std_string_view _fmt, _args_ &&..._args )
    {
        std::print( "{}", string_view_convert< ansi_char >( utf8_format( _fmt, std::forward< _args_ >( _args )... ) ) );
    }
    template < typename... _args_ >
    inline auto utf8_print( io_buffer *const _stream, const utf8_std_string_view _fmt, _args_ &&..._args )
    {
        std::print( _stream, "{}", string_view_convert< ansi_char >( utf8_format( _fmt, std::forward< _args_ >( _args )... ) ) );
    }
    template < typename... _args_ >
    inline auto utf8_println( const utf8_std_string_view _fmt, _args_ &&..._args )
    {
        std::println( "{}", string_view_convert< ansi_char >( utf8_format( _fmt, std::forward< _args_ >( _args )... ) ) );
    }
    template < typename... _args_ >
    inline auto utf8_println( io_buffer *const _stream, const utf8_std_string_view _fmt, _args_ &&..._args )
    {
        std::println( _stream, "{}", string_view_convert< ansi_char >( utf8_format( _fmt, std::forward< _args_ >( _args )... ) ) );
    }
    template < char_type _type_, size_type _capacity_ >
        requires( std::is_same_v< _type_, std::decay_t< _type_ > > )
    struct constant_string final {
      private:
        _type_ data_[ _capacity_ ]{};
      public:
        auto c_str() const
        {
            return reinterpret_cast< const _type_ * >( data_ );
        }
        auto compare( const _type_ *const _src ) const
        {
            if ( _src == nullptr ) {
                return false;
            }
            size_type src_size{ 0 };
            while ( _src[ src_size ] != '\0' ) {
                ++src_size;
            }
            if ( src_size + 1 != _capacity_ ) {
                return false;
            }
            for ( size_type i{ 0 }; i < _capacity_; ++i ) {
                if ( data_[ i ] != _src[ i ] ) {
                    return false;
                }
            }
            return true;
        }
        template < size_type _src_capacity_ >
        auto compare( const _type_ ( &_src )[ _src_capacity_ ] ) const
        {
            if ( _src_capacity_ != _capacity_ ) {
                return false;
            }
            for ( size_type i{ 0 }; i < _capacity_; ++i ) {
                if ( data_[ i ] != _src[ i ] ) {
                    return false;
                }
            }
            return true;
        }
        template < size_type _src_capacity_ >
        auto compare( const constant_string< _type_, _src_capacity_ > &_src ) const
        {
            if ( _src_capacity_ != _capacity_ ) {
                return false;
            }
            for ( size_type i{ 0 }; i < _capacity_; ++i ) {
                if ( data_[ i ] != _src.data_[ i ] ) {
                    return false;
                }
            }
            return true;
        }
        auto operator==( const _type_ *const _src ) const
        {
            return compare( _src );
        }
        template < size_type _src_capacity_ >
        auto operator==( const _type_ ( &_src )[ _src_capacity_ ] ) const
        {
            return compare( _src );
        }
        template < size_type _src_capacity_ >
        auto operator==( const constant_string< _type_, _src_capacity_ > &_src ) const
        {
            return compare( _src );
        }
        const auto &operator[]( size_type _index ) const
        {
            return data_[ _index ];
        }
        auto operator=( const constant_string< _type_, _capacity_ > & ) -> constant_string< _type_, _capacity_ > & = delete;
        auto operator=( constant_string< _type_, _capacity_ > && ) -> constant_string< _type_, _capacity_ > &      = delete;
        constexpr constant_string( const _type_ ( &_str )[ _capacity_ ] )
        {
            std::copy( _str, _str + _capacity_, data_ );
        }
        constexpr constant_string( const constant_string< _type_, _capacity_ > & ) = default;
        constexpr constant_string( constant_string< _type_, _capacity_ > && )      = delete;
        constexpr ~constant_string()                                               = default;
    };
    template < size_type _capacity_ >
    using constant_ansi_string = constant_string< ansi_char, _capacity_ >;
    template < size_type _capacity_ >
    using constant_wide_string = constant_string< wide_char, _capacity_ >;
    template < size_type _capacity_ >
    using constant_utf8_string = constant_string< utf8_char, _capacity_ >;
    template < size_type _capacity_ >
    using constant_utf16_string = constant_string< utf16_char, _capacity_ >;
    template < size_type _capacity_ >
    using constant_utf32_string = constant_string< utf32_char, _capacity_ >;
    template < typename _chrono_type_ >
    inline auto perf_sleep( const _chrono_type_ _time )
    {
        std::this_thread::yield();
        std::this_thread::sleep_for( _time );
    }
    template < typename _type_ >
        requires std::is_same_v< _type_, ansi_char > || std::is_same_v< _type_, utf8_char >
    class multithread_task final {
      private:
        struct node_ final {
            std::jthread task_thread;
            auto operator=( const node_ & ) -> node_ & = delete;
            auto operator=( node_ && ) -> node_ &      = default;
            template < callable_object _callee_, typename... _args_ >
            node_( _callee_ &&_func, _args_ &&..._args )
              : task_thread{ std::forward< _callee_ >( _func ), std::forward< _args_ >( _args )... }
            { }
            node_( const node_ & ) = delete;
            node_( node_ && )      = default;
            ~node_()
            {
                if ( task_thread.joinable() ) {
                    if constexpr ( std::is_same_v< _type_, ansi_char > ) {
                        std::print( " -> 终止线程 {}.\n", task_thread.get_id() );
                    } else if constexpr ( std::is_same_v< _type_, utf8_char > ) {
                        utf8_print( u8" -> 终止线程 {}.\n", task_thread.get_id() );
                    }
                }
            }
        };
        std::vector< node_ > tasks_{};
      public:
        template < callable_object _callee_, typename... _args_ >
        auto &add( const std_string_view< _type_ > _comment, _callee_ &&_func, _args_ &&..._args )
        {
            if constexpr ( std::is_same_v< _type_, ansi_char > ) {
                std::print( " -> 创建线程: {}.\n", _comment );
            } else if constexpr ( std::is_same_v< _type_, utf8_char > ) {
                utf8_print( u8" -> 创建线程: {}.\n", _comment );
            }
            tasks_.emplace_back( std::forward< _callee_ >( _func ), std::forward< _args_ >( _args )... );
            return *this;
        }
        auto &join( const size_type _index )
        {
            auto &task{ tasks_.at( _index ).task_thread };
            if ( task.joinable() ) {
                task.join();
            }
            return *this;
        }
        auto &detach( const size_type _index )
        {
            auto &task{ tasks_.at( _index ).task_thread };
            if ( task.joinable() ) {
                task.detach();
            }
            return *this;
        }
        auto operator=( const multithread_task< _type_ > & ) -> multithread_task< _type_ > & = delete;
        auto operator=( multithread_task< _type_ > && ) -> multithread_task< _type_ > &      = default;
        multithread_task()                                                                   = default;
        multithread_task( const multithread_task< _type_ > & )                               = delete;
        multithread_task( multithread_task< _type_ > && )                                    = default;
        ~multithread_task()                                                                  = default;
    };
    using multithread_task_ansi = multithread_task< ansi_char >;
    using multithread_task_utf8 = multithread_task< utf8_char >;
    class multithread_task_nolog final {
      private:
        struct node_ final {
            std::jthread task_thread;
            auto operator=( const node_ & ) -> node_ & = delete;
            auto operator=( node_ && ) -> node_ &      = default;
            template < callable_object _callee_, typename... _args_ >
            node_( _callee_ &&_func, _args_ &&..._args )
              : task_thread{ std::forward< _callee_ >( _func ), std::forward< _args_ >( _args )... }
            { }
            node_( const node_ & ) = delete;
            node_( node_ && )      = default;
            ~node_()               = default;
        };
        std::vector< node_ > tasks_{};
      public:
        template < callable_object _callee_, typename... _args_ >
        auto &add( _callee_ &&_func, _args_ &&..._args )
        {
            tasks_.emplace_back( std::forward< _callee_ >( _func ), std::forward< _args_ >( _args )... );
            return *this;
        }
        auto &join( const size_type _index )
        {
            auto &task{ tasks_.at( _index ).task_thread };
            if ( task.joinable() ) {
                task.join();
            }
            return *this;
        }
        auto &detach( const size_type _index )
        {
            auto &task{ tasks_.at( _index ).task_thread };
            if ( task.joinable() ) {
                task.detach();
            }
            return *this;
        }
        auto operator=( const multithread_task_nolog & ) -> multithread_task_nolog & = delete;
        auto operator=( multithread_task_nolog && ) -> multithread_task_nolog &      = default;
        multithread_task_nolog()                                                     = default;
        multithread_task_nolog( const multithread_task_nolog & )                     = delete;
        multithread_task_nolog( multithread_task_nolog && )                          = default;
        ~multithread_task_nolog()                                                    = default;
    };
#if defined( _WIN32 ) || defined( _WIN64 )
    inline auto ignore_console_exit_sinal( const bool _is_ignore )
    {
        return SetConsoleCtrlHandler( nullptr, static_cast< BOOL >( _is_ignore ) );
    }
    inline auto is_run_as_admin()
    {
        BOOL is_admin;
        PSID admins_group;
        SID_IDENTIFIER_AUTHORITY nt_authority{ SECURITY_NT_AUTHORITY };
        if ( AllocateAndInitializeSid(
               &nt_authority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &admins_group )
             == TRUE )
        {
            CheckTokenMembership( nullptr, admins_group, &is_admin );
            FreeSid( admins_group );
        }
        return is_admin ? true : false;
    }
    inline auto relaunch()
    {
        wide_char file_path[ MAX_PATH ]{};
        GetModuleFileNameW( nullptr, file_path, MAX_PATH );
        ShellExecuteW( nullptr, L"open", file_path, nullptr, nullptr, SW_SHOWNORMAL );
        std::exit( 0 );
    }
    inline auto relaunch_as_admin()
    {
        wide_char file_path[ MAX_PATH ]{};
        GetModuleFileNameW( nullptr, file_path, MAX_PATH );
        ShellExecuteW( nullptr, L"runas", file_path, nullptr, nullptr, SW_SHOWNORMAL );
        std::exit( 0 );
    }
    namespace console_value {
        inline constexpr DWORD mouse_button_left{ FROM_LEFT_1ST_BUTTON_PRESSED };
        inline constexpr DWORD mouse_button_middle{ FROM_LEFT_2ND_BUTTON_PRESSED };
        inline constexpr DWORD mouse_button_right{ RIGHTMOST_BUTTON_PRESSED };
        inline constexpr DWORD mouse_click{ 0x0000 };
        inline constexpr DWORD mouse_click_double{ DOUBLE_CLICK };
        inline constexpr DWORD mouse_move{ MOUSE_MOVED };
        inline constexpr DWORD mouse_wheel_height{ MOUSE_HWHEELED };
        inline constexpr DWORD mouse_wheel{ MOUSE_WHEELED };
        inline constexpr DWORD key_right_alt_press{ RIGHT_ALT_PRESSED };
        inline constexpr DWORD key_left_alt_press{ LEFT_ALT_PRESSED };
        inline constexpr DWORD key_right_ctrl_press{ RIGHT_CTRL_PRESSED };
        inline constexpr DWORD key_left_ctrl_press{ LEFT_CTRL_PRESSED };
        inline constexpr DWORD key_shift_press{ SHIFT_PRESSED };
        inline constexpr DWORD key_num_lock_on{ NUMLOCK_ON };
        inline constexpr DWORD key_scroll_lock_on{ SCROLLLOCK_ON };
        inline constexpr DWORD key_caps_lock_on{ CAPSLOCK_ON };
        inline constexpr DWORD key_enhanced_key{ ENHANCED_KEY };
        inline constexpr WORD text_default{ FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE };
        inline constexpr WORD text_foreground_red{ FOREGROUND_RED };
        inline constexpr WORD text_foreground_green{ FOREGROUND_GREEN };
        inline constexpr WORD text_foreground_blue{ FOREGROUND_BLUE };
        inline constexpr WORD text_foreground_intensity{ FOREGROUND_INTENSITY };
        inline constexpr WORD text_background_red{ BACKGROUND_RED };
        inline constexpr WORD text_background_green{ BACKGROUND_GREEN };
        inline constexpr WORD text_background_blue{ BACKGROUND_BLUE };
        inline constexpr WORD text_background_intensity{ BACKGROUND_INTENSITY };
        inline constexpr WORD text_lvb_leading_byte{ COMMON_LVB_LEADING_BYTE };
        inline constexpr WORD text_lvb_trailing_byte{ COMMON_LVB_TRAILING_BYTE };
        inline constexpr WORD text_lvb_grid_horizontal{ COMMON_LVB_GRID_HORIZONTAL };
        inline constexpr WORD text_lvb_grid_lvertical{ COMMON_LVB_GRID_LVERTICAL };
        inline constexpr WORD text_lvb_grid_rvertical{ COMMON_LVB_GRID_RVERTICAL };
        inline constexpr WORD text_lvb_reverse_video{ COMMON_LVB_REVERSE_VIDEO };
        inline constexpr WORD text_lvb_underscore{ COMMON_LVB_UNDERSCORE };
        inline constexpr WORD text_lvb_sbcsdbcs{ COMMON_LVB_SBCSDBCS };
    };
    template < typename _type_ >
        requires std::is_same_v< _type_, ansi_char > || std::is_same_v< _type_, utf8_char >
    class console_ui final {
      public:
        inline static constexpr auto back{ false };
        inline static constexpr auto quit{ true };
        struct func_args final {
            console_ui< _type_ > &parent_ui;
            const DWORD button_state;
            const DWORD ctrl_key_state;
            const DWORD event_flag;
            auto operator=( const func_args & ) -> func_args & = default;
            auto operator=( func_args && ) -> func_args &      = default;
            func_args(
              console_ui< _type_ > &_parent_ui,
              const MOUSE_EVENT_RECORD _mouse_event = MOUSE_EVENT_RECORD{ {}, console_value::mouse_button_left, {}, {} } )
              : parent_ui{ _parent_ui }
              , button_state{ _mouse_event.dwButtonState }
              , ctrl_key_state{ _mouse_event.dwControlKeyState }
              , event_flag{ _mouse_event.dwEventFlags }
            { }
            func_args( const func_args & ) = default;
            func_args( func_args && )      = default;
            ~func_args()                   = default;
        };
        using callback_type = std::function< bool( func_args ) >;
      private:
        enum class console_attrs_ { normal, lock_text, lock_all };
        struct line_node_ final {
            std_string< _type_ > text{};
            callback_type func{};
            WORD default_attrs{};
            WORD intensity_attrs{};
            WORD last_attrs{};
            COORD position{};
            auto set_attrs( const WORD _attrs )
            {
                SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), _attrs );
                last_attrs = _attrs;
            }
            auto operator==( const COORD _mouse_position ) const
            {
                return position.Y == _mouse_position.Y && position.X <= _mouse_position.X
                    && _mouse_position.X < ( position.X + static_cast< SHORT >( text.size() ) );
            }
            auto operator!=( const COORD _mouse_position ) const
            {
                return !operator==( _mouse_position );
            }
            auto operator=( const line_node_ & ) -> line_node_ & = default;
            auto operator=( line_node_ && ) -> line_node_ &      = default;
            line_node_()
              : default_attrs{ console_value::text_default }
              , intensity_attrs{ console_value::text_foreground_green | console_value::text_foreground_blue }
              , last_attrs{ console_value::text_default }
            { }
            line_node_( const std_string_view< _type_ > _text, callback_type &_func, const WORD _default_attrs, const WORD _intensity_attrs )
              : text{ _text }
              , func{ std::move( _func ) }
              , default_attrs{ _default_attrs }
              , intensity_attrs{ _intensity_attrs }
              , last_attrs{ _default_attrs }
            { }
            line_node_( const line_node_ & ) = default;
            line_node_( line_node_ && )      = default;
            ~line_node_()                    = default;
        };
        std::deque< line_node_ > lines_{};
        SHORT console_width_{};
        SHORT console_height_{};
        static auto show_cursor_( const bool _is_show )
        {
            CONSOLE_CURSOR_INFO cursor_data;
            GetConsoleCursorInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &cursor_data );
            cursor_data.bVisible = static_cast< WINBOOL >( _is_show );
            SetConsoleCursorInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &cursor_data );
        }
        static auto edit_console_attrs_( const console_attrs_ _mod )
        {
            DWORD attrs;
            GetConsoleMode( GetStdHandle( STD_INPUT_HANDLE ), &attrs );
            switch ( _mod ) {
                case console_attrs_::normal :
                    attrs |= ENABLE_QUICK_EDIT_MODE;
                    attrs |= ENABLE_INSERT_MODE;
                    attrs |= ENABLE_MOUSE_INPUT;
                    break;
                case console_attrs_::lock_text :
                    attrs &= ~ENABLE_QUICK_EDIT_MODE;
                    attrs &= ~ENABLE_INSERT_MODE;
                    attrs |= ENABLE_MOUSE_INPUT;
                    break;
                case console_attrs_::lock_all :
                    attrs &= ~ENABLE_QUICK_EDIT_MODE;
                    attrs &= ~ENABLE_INSERT_MODE;
                    attrs &= ~ENABLE_MOUSE_INPUT;
                    break;
            }
            SetConsoleMode( GetStdHandle( STD_INPUT_HANDLE ), attrs );
        }
        static auto get_cursor_()
        {
            CONSOLE_SCREEN_BUFFER_INFO console_data;
            GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &console_data );
            return console_data.dwCursorPosition;
        }
        static auto set_cursor_( const COORD _cursor_position )
        {
            SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), _cursor_position );
        }
        static auto wait_mouse_event_( const bool _is_mouse_move = true )
        {
            INPUT_RECORD record;
            DWORD reg;
            while ( true ) {
                perf_sleep( 10ms );
                ReadConsoleInputW( GetStdHandle( STD_INPUT_HANDLE ), &record, 1, &reg );
                if ( record.EventType == MOUSE_EVENT
                     && ( _is_mouse_move || record.Event.MouseEvent.dwEventFlags != console_value::mouse_move ) )
                {
                    return record.Event.MouseEvent;
                }
            }
        }
        auto get_console_size_()
        {
            CONSOLE_SCREEN_BUFFER_INFO console_data;
            GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &console_data );
            console_height_ = console_data.dwSize.Y;
            console_width_  = console_data.dwSize.X;
        }
        auto cls_()
        {
            get_console_size_();
            set_cursor_( { 0, 0 } );
            std::print(
              "{}",
              ansi_std_string( static_cast< size_type >( console_width_ ) * static_cast< size_type >( console_height_ ), ' ' ) );
            set_cursor_( { 0, 0 } );
        }
        static auto write_( const std_string_view< _type_ > _text, const bool _is_endl = false )
        {
            if constexpr ( std::is_same_v< _type_, ansi_char > ) {
                std::print( "{}{}", _text, _is_endl ? '\n' : '\0' );
            } else if constexpr ( std::is_same_v< _type_, utf8_char > ) {
                std::print( "{}{}", *reinterpret_cast< const ansi_std_string_view * >( &_text ), _is_endl ? '\n' : '\0' );
            }
        }
        static auto rewrite_( const COORD _cursor_position, const std_string_view< _type_ > _text )
        {
            set_cursor_( COORD{ 0, _cursor_position.Y } );
            if constexpr ( std::is_same_v< _type_, ansi_char > ) {
                write_( ansi_std_string( _cursor_position.X, ' ' ) );
            } else if constexpr ( std::is_same_v< _type_, utf8_char > ) {
                write_( utf8_std_string( _cursor_position.X, ' ' ) );
            }
            set_cursor_( COORD{ 0, _cursor_position.Y } );
            write_( _text );
            set_cursor_( COORD{ 0, _cursor_position.Y } );
        }
        auto init_pos_()
        {
            cls_();
            for ( auto &line : lines_ ) {
                line.position = get_cursor_();
                line.set_attrs( line.default_attrs );
                write_( line.text, true );
            }
        }
        auto refresh_( const COORD _hang_position )
        {
            for ( auto &line : lines_ ) {
                if ( line == _hang_position && line.last_attrs != line.intensity_attrs ) {
                    line.set_attrs( line.intensity_attrs );
                    rewrite_( line.position, line.text );
                }
                if ( line != _hang_position && line.last_attrs != line.default_attrs ) {
                    line.set_attrs( line.default_attrs );
                    rewrite_( line.position, line.text );
                }
            }
        }
        auto call_func_( const MOUSE_EVENT_RECORD &_mouse_event )
        {
            auto is_exit{ back };
            for ( auto &line : lines_ ) {
                if ( line != _mouse_event.dwMousePosition ) {
                    continue;
                }
                if ( line.func == nullptr ) {
                    break;
                }
                cls_();
                line.set_attrs( line.default_attrs );
                show_cursor_( false );
                edit_console_attrs_( console_attrs_::lock_all );
                is_exit = line.func( func_args{ *this, _mouse_event } );
                show_cursor_( false );
                edit_console_attrs_( console_attrs_::lock_text );
                init_pos_();
                break;
            }
            return is_exit;
        }
      public:
        auto empty() const
        {
            return lines_.empty();
        }
        auto size() const
        {
            return lines_.size();
        }
        auto max_size() const
        {
            return lines_.max_size();
        }
        auto &resize( const size_type _size )
        {
            lines_.resize( _size );
            return *this;
        }
        auto &optimize_storage()
        {
            for ( auto &line : lines_ ) {
                line.text.shrink_to_fit();
            }
            lines_.shrink_to_fit();
            return *this;
        }
        auto &swap( console_ui< _type_ > &_src )
        {
            lines_.swap( _src.lines_ );
            return *this;
        }
        auto &add_front(
          const std_string_view< _type_ > _text, callback_type _func = nullptr,
          const WORD _intensity_attrs = console_value::text_foreground_green | console_value::text_foreground_blue,
          const WORD _default_attrs   = console_value::text_default )
        {
            lines_.emplace_front( _text, _func, _default_attrs, _func != nullptr ? _intensity_attrs : _default_attrs );
            return *this;
        }
        auto &add_back(
          const std_string_view< _type_ > _text, callback_type _func = nullptr,
          const WORD _intensity_attrs = console_value::text_foreground_blue | console_value::text_foreground_green,
          const WORD _default_attrs   = console_value::text_default )
        {
            lines_.emplace_back( _text, _func, _default_attrs, _func != nullptr ? _intensity_attrs : _default_attrs );
            return *this;
        }
        auto &insert(
          const size_type _index, const std_string_view< _type_ > _text, callback_type _func = nullptr,
          const WORD _intensity_attrs = console_value::text_foreground_green | console_value::text_foreground_blue,
          const WORD _default_attrs   = console_value::text_default )
        {
            lines_.emplace(
              lines_.cbegin() + _index, _text, _func, _default_attrs, _func != nullptr ? _intensity_attrs : _default_attrs );
            return *this;
        }
        auto &edit(
          const size_type _index, const std_string_view< _type_ > _text, callback_type _func = nullptr,
          const WORD _intensity_attrs = console_value::text_foreground_green | console_value::text_foreground_blue,
          const WORD _default_attrs   = console_value::text_default )
        {
            lines_.at( _index ) = line_node_{ _text, _func, _default_attrs, _func != nullptr ? _intensity_attrs : _default_attrs };
            return *this;
        }
        auto &remove_front()
        {
            lines_.pop_front();
            return *this;
        }
        auto &remove_back()
        {
            lines_.pop_back();
            return *this;
        }
        auto &remove( const size_type _begin, const size_type _end )
        {
            lines_.erase( lines_.cbegin() + _begin, lines_.cbegin() + _end );
            return *this;
        }
        auto &clear()
        {
            lines_.clear();
            return *this;
        }
        auto &show()
        {
            show_cursor_( false );
            edit_console_attrs_( console_attrs_::lock_text );
            init_pos_();
            MOUSE_EVENT_RECORD mouse_event;
            auto func_return_value{ back };
            while ( func_return_value == back ) {
                mouse_event = wait_mouse_event_();
                switch ( mouse_event.dwEventFlags ) {
                    case console_value::mouse_move : refresh_( mouse_event.dwMousePosition ); break;
                    case console_value::mouse_click : {
                        if ( mouse_event.dwButtonState != false ) {
                            func_return_value = call_func_( mouse_event );
                        }
                        break;
                    }
                }
                perf_sleep( 10ms );
            }
            cls_();
            return *this;
        }
        auto &set_console(
          const std_string_view< _type_ > _title, const UINT _charset, const SHORT _width, const SHORT _height,
          const bool _is_fix_size, const bool _is_enable_minimize_ctrl, const bool is_enable_close_ctrl,
          const BYTE _translucency_value )
        {
            SetConsoleOutputCP( _charset );
            SetConsoleCP( _charset );
            if constexpr ( std::is_same_v< _type_, ansi_char > ) {
                SetConsoleTitleA( _title.data() );
            } else if constexpr ( std::is_same_v< _type_, utf8_char > ) {
                SetConsoleTitleA( string_view_convert< ansi_char >( _title ).c_str() );
            }
            std::system( std::format( R"(C:\Windows\System32\mode.com con cols={} lines={})", _width, _height ).c_str() );
            SetWindowLongPtrW(
              GetConsoleWindow(), GWL_STYLE,
              _is_fix_size
                ? GetWindowLongPtrW( GetConsoleWindow(), GWL_STYLE ) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX
                : GetWindowLongPtrW( GetConsoleWindow(), GWL_STYLE ) | WS_SIZEBOX | WS_MAXIMIZEBOX );
            SetWindowLongPtrW(
              GetConsoleWindow(), GWL_STYLE,
              _is_enable_minimize_ctrl
                ? GetWindowLongPtrW( GetConsoleWindow(), GWL_STYLE ) | WS_MINIMIZEBOX
                : GetWindowLongPtrW( GetConsoleWindow(), GWL_STYLE ) & ~WS_MINIMIZEBOX );
            EnableMenuItem(
              GetSystemMenu( GetConsoleWindow(), FALSE ), SC_CLOSE,
              is_enable_close_ctrl ? MF_BYCOMMAND | MF_ENABLED : MF_BYCOMMAND | MF_DISABLED | MF_GRAYED );
            SetLayeredWindowAttributes( GetConsoleWindow(), RGB( 0, 0, 0 ), _translucency_value, LWA_ALPHA );
            return *this;
        }
        auto &lock( const bool _is_hide_cursor, const bool _is_lock_text )
        {
            show_cursor_( !_is_hide_cursor );
            edit_console_attrs_( _is_lock_text ? console_attrs_::lock_all : console_attrs_::normal );
            return *this;
        }
        auto operator=( const console_ui< _type_ > & ) -> console_ui< _type_ > & = default;
        auto operator=( console_ui< _type_ > && ) -> console_ui< _type_ > &      = default;
        console_ui()                                                             = default;
        console_ui( const console_ui< _type_ > & )                               = default;
        console_ui( console_ui< _type_ > && )                                    = default;
        ~console_ui()                                                            = default;
    };
    using console_ui_ansi = console_ui< ansi_char >;
    using console_ui_utf8 = console_ui< utf8_char >;
#endif
}
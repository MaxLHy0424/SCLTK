#pragma once
#if __cplusplus >= 202302L
# include <windows.h>
# ifdef _THE_NEXT_MAJOR_UPDATE_
#  include <print>
# else
#  include <cstdio>
# endif
# include <chrono>
# include <functional>
# include <queue>
# include <string>
# include <thread>
# include <type_traits>
# define CONSOLE_MOUSE_BUTTON_LEFT               FROM_LEFT_1ST_BUTTON_PRESSED
# define CONSOLE_MOUSE_BUTTON_MIDDLE             FROM_LEFT_2ND_BUTTON_PRESSED
# define CONSOLE_MOUSE_BUTTON_RIGHT              RIGHTMOST_BUTTON_PRESSED
# define CONSOLE_MOUSE_CLICK                     0x0000
# define CONSOLE_MOUSE_CLICK_DOUBLE              DOUBLE_CLICK
# define CONSOLE_MOUSE_MOVE                      MOUSE_MOVED
# define CONSOLE_MOUSE_WHEEL_HEIGHT              MOUSE_HWHEELED
# define CONSOLE_MOUSE_WHEEL                     MOUSE_WHEELED
# define CONSOLE_KEY_RIGHT_ALT_PRESS             RIGHT_ALT_PRESSED
# define CONSOLE_KEY_LEFT_ALT_PRESS              LEFT_ALT_PRESSED
# define CONSOLE_KEY_RIGHT_CTRL_PRESS            RIGHT_CTRL_PRESSED
# define CONSOLE_KEY_LEFT_CTRL_PRESS             LEFT_CTRL_PRESSED
# define CONSOLE_KEY_SHIFT_PRESS                 SHIFT_PRESSED
# define CONSOLE_KEY_NUM_LOCK_ON                 NUMLOCK_ON
# define CONSOLE_KEY_SCROLL_LOCK_ON              SCROLLLOCK_ON
# define CONSOLE_KEY_CAPS_LOCK_ON                CAPSLOCK_ON
# define CONSOLE_KEY_ENHANCED_KEY                ENHANCED_KEY
# define CONSOLE_TEXT_DEFAULT                    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
# define CONSOLE_TEXT_FOREGROUND_RED             FOREGROUND_RED
# define CONSOLE_TEXT_FOREGROUND_GREEN           FOREGROUND_GREEN
# define CONSOLE_TEXT_FOREGROUND_BLUE            FOREGROUND_BLUE
# define CONSOLE_TEXT_FOREGROUND_INTENSITY       FOREGROUND_INTENSITY
# define CONSOLE_TEXT_BACKGROUND_RED             BACKGROUND_RED
# define CONSOLE_TEXT_BACKGROUND_GREEN           BACKGROUND_GREEN
# define CONSOLE_TEXT_BACKGROUND_BLUE            BACKGROUND_BLUE
# define CONSOLE_TEXT_BACKGROUND_INTENSITY       BACKGROUND_INTENSITY
# define CONSOLE_TEXT_COMMON_LVB_LEADING_BYTE    COMMON_LVB_LEADING_BYTE
# define CONSOLE_TEXT_COMMON_LVB_TRAILING_BYTE   COMMON_LVB_TRAILING_BYTE
# define CONSOLE_TEXT_COMMON_LVB_GRID_HORIZONTAL COMMON_LVB_GRID_HORIZONTAL
# define CONSOLE_TEXT_COMMON_LVB_GRID_LVERTICAL  COMMON_LVB_GRID_LVERTICAL
# define CONSOLE_TEXT_COMMON_LVB_GRID_RVERTICAL  COMMON_LVB_GRID_RVERTICAL
# define CONSOLE_TEXT_COMMON_LVB_REVERSE_VIDEO   COMMON_LVB_REVERSE_VIDEO
# define CONSOLE_TEXT_COMMON_LVB_UNDERSCORE      COMMON_LVB_UNDERSCORE
# define CONSOLE_TEXT_COMMON_LVB_SBCSDBCS        COMMON_LVB_SBCSDBCS
# define CONSOLE_UI_REVERT                       false
# define CONSOLE_UI_TERMINATE                    true
class console_ui final {
  public:
    struct func_args final {
        const DWORD button_state, ctrl_key_state, event_flag;
        console_ui &parent_ui;
        auto &operator=( const func_args & ) = delete;
        auto &operator=( func_args && )      = delete;
        func_args(
          console_ui &_parent_ui,
          const MOUSE_EVENT_RECORD _mouse_event = { {}, CONSOLE_MOUSE_BUTTON_LEFT, {}, {} } )
          : button_state{ _mouse_event.dwButtonState }
          , ctrl_key_state{ _mouse_event.dwControlKeyState }
          , event_flag{ _mouse_event.dwEventFlags }
          , parent_ui{ _parent_ui }
        { }
        func_args( const func_args & ) = default;
        func_args( func_args && )      = default;
        ~func_args()                   = default;
    };
    template < typename _type_ >
    using object_type = _type_;
    template < typename _char_type_ >
    using c_str_type         = const _char_type_ *;
    using default_char_type  = char;
    using size_type          = decltype( sizeof( void * ) );
    using nullptr_type       = decltype( nullptr );
    using func_callback_type = std::function< bool( func_args ) >;
  private:
    enum class console_attrs_ { normal, lock_text, lock_all };
    struct ui_item_ final {
        c_str_type< default_char_type > text;
        short default_attrs, highlight_attrs, last_attrs;
        COORD position;
        func_callback_type func;
        auto set_attrs( const short _attrs )
        {
            SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), _attrs );
            last_attrs = _attrs;
        }
        auto operator==( const COORD &_mouse_position ) const
        {
            return position.Y == _mouse_position.Y && position.X <= _mouse_position.X
                && _mouse_position.X < ( position.X + static_cast< short >( strlen( text ) ) );
        }
        auto operator!=( const COORD &_mouse_position ) const
        {
            return !operator==( _mouse_position );
        }
        auto &operator=( const ui_item_ &_src )
        {
            text            = _src.text;
            default_attrs   = _src.default_attrs;
            highlight_attrs = _src.highlight_attrs;
            last_attrs      = _src.last_attrs;
            position        = _src.position;
            func            = _src.func;
            return *this;
        }
        auto &operator=( ui_item_ &&_src )
        {
            text            = std::move( _src.text );
            default_attrs   = std::move( _src.default_attrs );
            highlight_attrs = std::move( _src.highlight_attrs );
            last_attrs      = std::move( _src.last_attrs );
            position        = std::move( _src.position );
            func            = std::move( _src.func );
            return *this;
        }
        ui_item_()
          : text{}
          , default_attrs{ CONSOLE_TEXT_DEFAULT }
          , highlight_attrs{ CONSOLE_TEXT_FOREGROUND_GREEN | CONSOLE_TEXT_FOREGROUND_BLUE }
          , last_attrs{ CONSOLE_TEXT_DEFAULT }
          , position{}
          , func{}
        { }
        ui_item_( nullptr_type, const short, const short, const func_callback_type ) = delete;
        ui_item_(
          const c_str_type< default_char_type > _text,
          const short _default_attrs,
          const short _highlight_attrs,
          const func_callback_type _func )
          : text{ _text }
          , default_attrs{ _default_attrs }
          , highlight_attrs{ _highlight_attrs }
          , last_attrs{ CONSOLE_TEXT_DEFAULT }
          , position{}
          , func{ std::move( _func ) }
        { }
        ui_item_( const ui_item_ & ) = default;
        ui_item_( ui_item_ && )      = default;
        ~ui_item_()                  = default;
    };
    std::deque< ui_item_ > item_;
    short width_, height_;
    auto show_cursor_( const bool _is_show )
    {
        CONSOLE_CURSOR_INFO cursor;
        GetConsoleCursorInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &cursor );
        cursor.bVisible = _is_show;
        SetConsoleCursorInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &cursor );
    }
    auto edit_console_attrs_( const console_attrs_ _mode )
    {
        DWORD attrs;
        GetConsoleMode( GetStdHandle( STD_INPUT_HANDLE ), &attrs );
        switch ( _mode ) {
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
    auto get_cursor_()
    {
        CONSOLE_SCREEN_BUFFER_INFO console;
        GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &console );
        return console.dwCursorPosition;
    }
    auto set_cursor_( const COORD &_position )
    {
        SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), _position );
    }
    auto wait_mouse_event_( const bool _move = true )
    {
        using namespace std::chrono_literals;
        INPUT_RECORD record;
        DWORD reg;
        while ( true ) {
            std::this_thread::sleep_for( 10ms );
            ReadConsoleInputA( GetStdHandle( STD_INPUT_HANDLE ), &record, 1, &reg );
            if ( record.EventType == MOUSE_EVENT
                 && _move | ( record.Event.MouseEvent.dwEventFlags != CONSOLE_MOUSE_MOVE ) )
            {
                return record.Event.MouseEvent;
            }
        }
    }
    auto get_console_size_()
    {
        CONSOLE_SCREEN_BUFFER_INFO console;
        GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &console );
        height_ = console.dwSize.Y;
        width_  = console.dwSize.X;
    }
    auto cls_()
    {
        get_console_size_();
        set_cursor_( { 0, 0 } );
# ifdef _THE_NEXT_MAJOR_UPDATE_
        std::print( "{}", std::string( width_ * height_, ' ' ) );
# else
        std::printf( std::string( width_ * height_, ' ' ).c_str() );
# endif
        set_cursor_( { 0, 0 } );
    }
    auto write_( nullptr_type, const bool = false ) = delete;
    auto write_( const c_str_type< default_char_type > _text, const bool _is_endl = false )
    {
# ifdef _THE_NEXT_MAJOR_UPDATE_
        std::print( "{}{}", _text, _is_endl ? '\n' : '\0' );
# else
        std::printf( "%s%c", _text, _is_endl ? '\n' : '\0' );
# endif
    }
    auto rewrite_( const COORD &_position, nullptr_type ) = delete;
    auto rewrite_( const COORD &_position, const c_str_type< default_char_type > _text )
    {
        set_cursor_( { 0, _position.Y } );
        write_( std::string( _position.X, ' ' ).c_str() );
        set_cursor_( { 0, _position.Y } );
        write_( _text );
        set_cursor_( { 0, _position.Y } );
    }
    auto init_pos_()
    {
        cls_();
        for ( auto &line : item_ ) {
            if ( line.text == nullptr ) {
                continue;
            }
            line.position = get_cursor_();
            line.set_attrs( line.default_attrs );
            write_( line.text, true );
        }
    }
    auto refresh_( const COORD &_hang_position )
    {
        for ( auto &line : item_ ) {
            if ( line == _hang_position && line.last_attrs != line.highlight_attrs ) {
                line.set_attrs( line.highlight_attrs );
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
        bool is_exit{};
        for ( auto &line : item_ ) {
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
        return item_.empty();
    }
    auto size() const
    {
        return item_.size();
    }
    auto max_size() const
    {
        return item_.max_size();
    }
    auto &resize( const size_type _size )
    {
        item_.resize( _size );
        return *this;
    }
    auto &shrink_to_fit()
    {
        item_.shrink_to_fit();
        return *this;
    }
    auto &swap( console_ui &_src )
    {
        item_.swap( _src.item_ );
        return *this;
    }
    auto &add_front(
      const nullptr_type,
      const func_callback_type = nullptr,
      const short              = CONSOLE_TEXT_FOREGROUND_GREEN | CONSOLE_TEXT_FOREGROUND_BLUE,
      const short              = CONSOLE_TEXT_DEFAULT )
      = delete;
    auto &add_front(
      const c_str_type< default_char_type > _text,
      const func_callback_type _func = nullptr,
      const short _highlight_attrs   = CONSOLE_TEXT_FOREGROUND_GREEN | CONSOLE_TEXT_FOREGROUND_BLUE,
      const short _default_attrs     = CONSOLE_TEXT_DEFAULT )
    {
        item_.emplace_front( ui_item_{
          _text,
          _default_attrs,
          _func == nullptr ? _default_attrs : _highlight_attrs,
          std::move( _func ) } );
        return *this;
    }
    auto &add_back(
      const nullptr_type,
      const func_callback_type = nullptr,
      const short              = CONSOLE_TEXT_FOREGROUND_GREEN | CONSOLE_TEXT_FOREGROUND_BLUE,
      const short              = CONSOLE_TEXT_DEFAULT )
      = delete;
    auto &add_back(
      const c_str_type< default_char_type > _text,
      const func_callback_type _func = nullptr,
      const short _highlight_attrs   = CONSOLE_TEXT_FOREGROUND_BLUE | CONSOLE_TEXT_FOREGROUND_GREEN,
      const short _default_attrs     = CONSOLE_TEXT_DEFAULT )
    {
        item_.emplace_back( ui_item_{
          _text,
          _default_attrs,
          _func == nullptr ? _default_attrs : _highlight_attrs,
          std::move( _func ) } );
        return *this;
    }
    auto &insert(
      const size_type,
      const nullptr_type,
      const func_callback_type = nullptr,
      const short              = CONSOLE_TEXT_FOREGROUND_GREEN | CONSOLE_TEXT_FOREGROUND_BLUE,
      const short              = CONSOLE_TEXT_DEFAULT )
      = delete;
    auto &insert(
      const size_type _index,
      const c_str_type< default_char_type > _text,
      const func_callback_type _func = nullptr,
      const short _highlight_attrs   = CONSOLE_TEXT_FOREGROUND_GREEN | CONSOLE_TEXT_FOREGROUND_BLUE,
      const short _default_attrs     = CONSOLE_TEXT_DEFAULT )
    {
        item_.emplace(
          item_.cbegin() + _index,
          ui_item_{
            _text,
            _default_attrs,
            _func == nullptr ? _default_attrs : _highlight_attrs,
            std::move( _func ) } );
        return *this;
    }
    auto &edit(
      const size_type,
      const nullptr_type,
      const func_callback_type = nullptr,
      const short              = CONSOLE_TEXT_FOREGROUND_GREEN | CONSOLE_TEXT_FOREGROUND_BLUE,
      const short              = CONSOLE_TEXT_DEFAULT )
      = delete;
    auto &edit(
      const size_type _index,
      const c_str_type< default_char_type > _text,
      const func_callback_type _func = nullptr,
      const short _highlight_attrs   = CONSOLE_TEXT_FOREGROUND_GREEN | CONSOLE_TEXT_FOREGROUND_BLUE,
      const short _default_attrs     = CONSOLE_TEXT_DEFAULT )
    {
        item_.at( _index ) = ui_item_{
          _text,
          _default_attrs,
          _func == nullptr ? _default_attrs : _highlight_attrs,
          std::move( _func ) };
        return *this;
    }
    auto &remove_front()
    {
        item_.pop_front();
        return *this;
    }
    auto &remove_back()
    {
        item_.pop_back();
        return *this;
    }
    auto &remove( const size_type _begin, const size_type _end )
    {
        item_.erase( item_.cbegin() + _begin, item_.cbegin() + _end );
        return *this;
    }
    auto &clear()
    {
        item_.clear();
        return *this;
    }
    auto &show()
    {
        using namespace std::chrono_literals;
        show_cursor_( false );
        edit_console_attrs_( console_attrs_::lock_text );
        MOUSE_EVENT_RECORD mouse_event;
        init_pos_();
        auto func_return_value{ CONSOLE_UI_REVERT };
        while ( func_return_value == CONSOLE_UI_REVERT ) {
            mouse_event = wait_mouse_event_();
            switch ( mouse_event.dwEventFlags ) {
                case CONSOLE_MOUSE_MOVE : refresh_( mouse_event.dwMousePosition ); break;
                case CONSOLE_MOUSE_CLICK : {
                    if ( mouse_event.dwButtonState != false ) {
                        func_return_value = call_func_( mouse_event );
                    }
                    break;
                }
            }
            std::this_thread::sleep_for( 10ms );
        }
        cls_();
        return *this;
    }
    auto &set_console(
      const UINT, const nullptr_type, const SHORT, const SHORT, const bool, const bool, const bool, const BYTE )
      = delete;
    auto &set_console(
      const UINT _code_page,
      const CHAR *const _title,
      const SHORT _width,
      const SHORT _height,
      const bool _fix_size,
      const bool _minimize_ctrl,
      const bool _close_window_ctrl,
      const BYTE _transparency )
    {
        SetConsoleOutputCP( _code_page );
        SetConsoleCP( _code_page );
        SetConsoleTitleA( _title );
# ifdef _THE_NEXT_MAJOR_UPDATE_
        system( std::format( "mode.com con cols={} lines={}", _width, _height ).c_str() );
# else
        using namespace std::string_literals;
        system(
          "mode.com con cols="s.append( std::to_string( _width ) )
            .append( " lines=" )
            .append( std::to_string( _height ) )
            .c_str() );
# endif
        SetWindowLongPtrA(
          GetConsoleWindow(),
          GWL_STYLE,
          _fix_size
            ? GetWindowLongPtrA( GetConsoleWindow(), GWL_STYLE ) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX
            : GetWindowLongPtrA( GetConsoleWindow(), GWL_STYLE ) | WS_SIZEBOX | WS_MAXIMIZEBOX );
        SetWindowLongPtrA(
          GetConsoleWindow(),
          GWL_STYLE,
          _minimize_ctrl
            ? GetWindowLongPtrA( GetConsoleWindow(), GWL_STYLE ) | WS_MINIMIZEBOX
            : GetWindowLongPtrA( GetConsoleWindow(), GWL_STYLE ) & ~WS_MINIMIZEBOX );
        EnableMenuItem(
          GetSystemMenu( GetConsoleWindow(), FALSE ),
          SC_CLOSE,
          _close_window_ctrl ? MF_BYCOMMAND | MF_ENABLED : MF_BYCOMMAND | MF_DISABLED | MF_GRAYED );
        SetLayeredWindowAttributes( GetConsoleWindow(), 0, _transparency, LWA_ALPHA );
        return *this;
    }
    auto &lock( const bool _is_hide_cursor, const bool _is_lock_text )
    {
        show_cursor_( !_is_hide_cursor );
        edit_console_attrs_( _is_lock_text ? console_attrs_::lock_all : console_attrs_::normal );
        return *this;
    }
    auto &operator=( const console_ui &_src )
    {
        item_ = _src.item_;
        return *this;
    }
    auto &operator=( console_ui &&_src )
    {
        item_ = std::move( _src.item_ );
        return *this;
    }
    console_ui()
      : item_{}
      , width_{}
      , height_{}
    { }
    console_ui( const console_ui & ) = default;
    console_ui( console_ui && )      = default;
    ~console_ui()                    = default;
};
#else
# error "must be compiled on c++23 / gnu++23 or later C++ standards."
#endif
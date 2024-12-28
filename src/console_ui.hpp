#pragma once
#include <windows.h>
#include <chrono>
#include <functional>
#include <print>
#include <queue>
#include <string>
#include <thread>
#define MOUSE_BUTTON_LEFT               FROM_LEFT_1ST_BUTTON_PRESSED
#define MOUSE_BUTTON_MIDDLE             FROM_LEFT_2ND_BUTTON_PRESSED
#define MOUSE_BUTTON_RIGHT              RIGHTMOST_BUTTON_PRESSED
#define MOUSE_CLICK                     0x0000
#define MOUSE_CLICK_DOUBLE              DOUBLE_CLICK
#define MOUSE_MOVE                      MOUSE_MOVED
#define MOUSE_WHEEL_HEIGHT              MOUSE_HWHEELED
#define MOUSE_WHEEL                     MOUSE_WHEELED
#define KEY_RIGHT_ALT_PRESS             RIGHT_ALT_PRESSED
#define KEY_LEFT_ALT_PRESS              LEFT_ALT_PRESSED
#define KEY_RIGHT_CTRL_PRESS            RIGHT_CTRL_PRESSED
#define KEY_LEFT_CTRL_PRESS             LEFT_CTRL_PRESSED
#define KEY_SHIFT_PRESS                 SHIFT_PRESSED
#define KEY_NUM_LOCK_ON                 NUMLOCK_ON
#define KEY_SCROLL_LOCK_ON              SCROLLLOCK_ON
#define KEY_CAPS_LOCK_ON                CAPSLOCK_ON
#define KEY_ENHANCED_KEY                ENHANCED_KEY
#define TEXT_DEFAULT                    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
#define TEXT_FOREGROUND_RED             FOREGROUND_RED
#define TEXT_FOREGROUND_GREEN           FOREGROUND_GREEN
#define TEXT_FOREGROUND_BLUE            FOREGROUND_BLUE
#define TEXT_FOREGROUND_INTENSITY       FOREGROUND_INTENSITY
#define TEXT_BACKGROUND_RED             BACKGROUND_RED
#define TEXT_BACKGROUND_GREEN           BACKGROUND_GREEN
#define TEXT_BACKGROUND_BLUE            BACKGROUND_BLUE
#define TEXT_BACKGROUND_INTENSITY       BACKGROUND_INTENSITY
#define TEXT_COMMON_LVB_LEADING_BYTE    COMMON_LVB_LEADING_BYTE
#define TEXT_COMMON_LVB_TRAILING_BYTE   COMMON_LVB_TRAILING_BYTE
#define TEXT_COMMON_LVB_GRID_HORIZONTAL COMMON_LVB_GRID_HORIZONTAL
#define TEXT_COMMON_LVB_GRID_LVERTICAL  COMMON_LVB_GRID_LVERTICAL
#define TEXT_COMMON_LVB_GRID_RVERTICAL  COMMON_LVB_GRID_RVERTICAL
#define TEXT_COMMON_LVB_REVERSE_VIDEO   COMMON_LVB_REVERSE_VIDEO
#define TEXT_COMMON_LVB_UNDERSCORE      COMMON_LVB_UNDERSCORE
#define TEXT_COMMON_LVB_SBCSDBCS        COMMON_LVB_SBCSDBCS
#define UI_RETURN                       false
#define UI_EXIT                         true
class console_ui final {
  public:
    struct func_args final {
        console_ui &parent_ui;
        const DWORD button_state, ctrl_key_state, event_flag;
        auto operator=( const func_args & ) -> func_args & = default;
        auto operator=( func_args && ) -> func_args &      = default;
        func_args(
          console_ui &_parent_ui, const MOUSE_EVENT_RECORD _mouse_event = { {}, MOUSE_BUTTON_LEFT, {}, {} } )
          : parent_ui{ _parent_ui }
          , button_state{ _mouse_event.dwButtonState }
          , ctrl_key_state{ _mouse_event.dwControlKeyState }
          , event_flag{ _mouse_event.dwEventFlags }
        { }
        func_args( const func_args & ) = default;
        func_args( func_args && )      = default;
        ~func_args()                   = default;
    };
    using size_type     = decltype( sizeof( void * ) );
    using string_type   = std::string;
    using callback_type = std::function< bool( func_args ) >;
    template < typename _type_ >
    using type_wrapper = _type_;
  private:
    enum class console_attrs_ { normal, lock_text, lock_all };
    struct line_item_ final {
        string_type text{};
        callback_type func{};
        WORD default_attrs, intensity_attrs, last_attrs;
        COORD position{};
        auto set_attrs( const WORD _attrs )
        {
            SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), _attrs );
            last_attrs = _attrs;
        }
        auto operator==( const COORD &_mouse_position ) const
        {
            return position.Y == _mouse_position.Y && position.X <= _mouse_position.X
                && _mouse_position.X < ( position.X + static_cast< SHORT >( text.size() ) );
        }
        auto operator!=( const COORD &_mouse_position ) const
        {
            return !operator==( _mouse_position );
        }
        auto operator=( const line_item_ & ) -> line_item_ & = default;
        auto operator=( line_item_ && ) -> line_item_ &      = default;
        line_item_()
          : default_attrs{ TEXT_DEFAULT }
          , intensity_attrs{ TEXT_FOREGROUND_GREEN | TEXT_FOREGROUND_BLUE }
          , last_attrs{ TEXT_DEFAULT }
        { }
        line_item_(
          string_type _text, callback_type _func, const WORD _default_attrs, const WORD _intensity_attrs )
          : text{ std::move( _text ) }
          , func{ std::move( _func ) }
          , default_attrs{ _default_attrs }
          , intensity_attrs{ _intensity_attrs }
          , last_attrs{ TEXT_DEFAULT }
        { }
        line_item_( const line_item_ & ) = default;
        line_item_( line_item_ && )      = default;
        ~line_item_()                    = default;
    };
    std::deque< line_item_ > lines_{};
    SHORT width_{}, height_{};
    auto show_cursor_( const bool _is_show )
    {
        CONSOLE_CURSOR_INFO cursor_data;
        GetConsoleCursorInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &cursor_data );
        cursor_data.bVisible = _is_show;
        SetConsoleCursorInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &cursor_data );
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
        CONSOLE_SCREEN_BUFFER_INFO console_data;
        GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &console_data );
        return console_data.dwCursorPosition;
    }
    auto set_cursor_( const COORD &_cursor_position )
    {
        SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), _cursor_position );
    }
    auto wait_mouse_event_( const bool _is_mouse_move = true )
    {
        using namespace std::chrono_literals;
        INPUT_RECORD record;
        DWORD reg;
        while ( true ) {
            std::this_thread::sleep_for( 10ms );
            ReadConsoleInputA( GetStdHandle( STD_INPUT_HANDLE ), &record, 1, &reg );
            if ( record.EventType == MOUSE_EVENT
                 && _is_mouse_move | ( record.Event.MouseEvent.dwEventFlags != MOUSE_MOVE ) )
            {
                return record.Event.MouseEvent;
            }
        }
    }
    auto get_console_size_()
    {
        CONSOLE_SCREEN_BUFFER_INFO console_data;
        GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &console_data );
        height_ = console_data.dwSize.Y;
        width_  = console_data.dwSize.X;
    }
    auto cls_()
    {
        get_console_size_();
        set_cursor_( { 0, 0 } );
        std::print( "{}", string_type( width_ * height_, ' ' ) );
        set_cursor_( { 0, 0 } );
    }
    auto write_( const string_type &_text, const bool _is_endl = false )
    {
        std::print( "{}{}", _text, _is_endl ? '\n' : '\0' );
    }
    auto rewrite_( const COORD &_cursor_position, const string_type &_text )
    {
        set_cursor_( { 0, _cursor_position.Y } );
        write_( string_type( _cursor_position.X, ' ' ) );
        set_cursor_( { 0, _cursor_position.Y } );
        write_( _text );
        set_cursor_( { 0, _cursor_position.Y } );
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
    auto refresh_( const COORD &_hang_position )
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
        bool is_exit{};
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
        lines_.shrink_to_fit();
        return *this;
    }
    auto &optimize_text()
    {
        for ( auto &line : lines_ ) {
            line.text.shrink_to_fit();
        }
        return *this;
    }
    auto &swap( console_ui &_src )
    {
        lines_.swap( _src.lines_ );
        return *this;
    }
    auto &add_front(
      string_type _text, callback_type _func = nullptr,
      const WORD _intensity_attrs = TEXT_FOREGROUND_GREEN | TEXT_FOREGROUND_BLUE,
      const WORD _default_attrs   = TEXT_DEFAULT )
    {
        auto is_func{ _func == nullptr ? false : true };
        lines_.emplace_front( line_item_{
          std::move( _text ),
          std::move( _func ),
          _default_attrs,
          is_func ? _intensity_attrs : _default_attrs,
        } );
        return *this;
    }
    auto &add_back(
      string_type _text, callback_type _func = nullptr,
      const WORD _intensity_attrs = TEXT_FOREGROUND_BLUE | TEXT_FOREGROUND_GREEN,
      const WORD _default_attrs   = TEXT_DEFAULT )
    {
        auto is_func{ _func == nullptr ? false : true };
        lines_.emplace_back( line_item_{
          std::move( _text ),
          std::move( _func ),
          _default_attrs,
          is_func ? _intensity_attrs : _default_attrs,
        } );
        return *this;
    }
    auto &insert(
      const size_type _index, string_type _text, callback_type _func = nullptr,
      const WORD _intensity_attrs = TEXT_FOREGROUND_GREEN | TEXT_FOREGROUND_BLUE,
      const WORD _default_attrs   = TEXT_DEFAULT )
    {
        auto is_func{ _func == nullptr ? false : true };
        lines_.emplace(
          lines_.cbegin() + _index,
          line_item_{
            std::move( _text ), std::move( _func ), _default_attrs,
            is_func ? _intensity_attrs : _default_attrs } );
        return *this;
    }
    auto &edit(
      const size_type _index, string_type _text, callback_type _func = nullptr,
      const WORD _intensity_attrs = TEXT_FOREGROUND_GREEN | TEXT_FOREGROUND_BLUE,
      const WORD _default_attrs   = TEXT_DEFAULT )
    {
        auto is_func{ _func == nullptr ? false : true };
        lines_.at( _index ) = line_item_{
          std::move( _text ), std::move( _func ), _default_attrs, is_func ? _intensity_attrs : _default_attrs };
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
        using namespace std::chrono_literals;
        show_cursor_( false );
        edit_console_attrs_( console_attrs_::lock_text );
        MOUSE_EVENT_RECORD mouse_event;
        init_pos_();
        auto func_return_value{ UI_RETURN };
        while ( func_return_value == UI_RETURN ) {
            mouse_event = wait_mouse_event_();
            switch ( mouse_event.dwEventFlags ) {
                case MOUSE_MOVE : refresh_( mouse_event.dwMousePosition ); break;
                case MOUSE_CLICK : {
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
      const string_type &_title, const UINT _code_page, const SHORT _width, const SHORT _height,
      const bool _is_fix_size, const bool _is_enable_minimize_ctrl, const bool is_enable_close_window_ctrl,
      const BYTE _translucency )
    {
        SetConsoleOutputCP( _code_page );
        SetConsoleCP( _code_page );
        SetConsoleTitleA( _title.c_str() );
        system( std::format( "mode.com con cols={} lines={}", _width, _height ).c_str() );
        SetWindowLongPtrA(
          GetConsoleWindow(), GWL_STYLE,
          _is_fix_size
            ? GetWindowLongPtrA( GetConsoleWindow(), GWL_STYLE ) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX
            : GetWindowLongPtrA( GetConsoleWindow(), GWL_STYLE ) | WS_SIZEBOX | WS_MAXIMIZEBOX );
        SetWindowLongPtrA(
          GetConsoleWindow(), GWL_STYLE,
          _is_enable_minimize_ctrl
            ? GetWindowLongPtrA( GetConsoleWindow(), GWL_STYLE ) | WS_MINIMIZEBOX
            : GetWindowLongPtrA( GetConsoleWindow(), GWL_STYLE ) & ~WS_MINIMIZEBOX );
        EnableMenuItem(
          GetSystemMenu( GetConsoleWindow(), FALSE ), SC_CLOSE,
          is_enable_close_window_ctrl ? MF_BYCOMMAND | MF_ENABLED : MF_BYCOMMAND | MF_DISABLED | MF_GRAYED );
        SetLayeredWindowAttributes( GetConsoleWindow(), RGB( 0, 0, 0 ), _translucency, LWA_ALPHA );
        return *this;
    }
    auto &lock( const bool _is_hide_cursor, const bool _is_lock_text )
    {
        show_cursor_( !_is_hide_cursor );
        edit_console_attrs_( _is_lock_text ? console_attrs_::lock_all : console_attrs_::normal );
        return *this;
    }
    auto operator=( const console_ui & ) -> console_ui & = default;
    auto operator=( console_ui && ) -> console_ui &      = default;
    console_ui()                                         = default;
    console_ui( const console_ui & )                     = default;
    console_ui( console_ui && )                          = default;
    ~console_ui()                                        = default;
};
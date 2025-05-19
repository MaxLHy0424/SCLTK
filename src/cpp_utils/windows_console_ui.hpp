#pragma once
#include <chrono>
#include <deque>
#include <functional>
#include <print>
#include <ranges>
#include <string>
#include <thread>
#include <utility>
#include "type_tools.hpp"
#include "windows_definations.hpp"
namespace cpp_utils {
#if defined( _WIN32 ) || defined( _WIN64 )
    class console_ui final {
      public:
        using func_return_type = bool;
        inline static constexpr func_return_type back{ false };
        inline static constexpr func_return_type exit{ true };
        struct func_args final {
            console_ui &parent_ui;
            const size_type node_index;
            const DWORD button_state;
            const DWORD ctrl_state;
            const DWORD event_flag;
            auto operator=( const func_args & ) noexcept -> func_args & = default;
            auto operator=( func_args && ) noexcept -> func_args &      = default;
            func_args(
              console_ui &_parent_ui, const size_type _node_index,
              const MOUSE_EVENT_RECORD _event = MOUSE_EVENT_RECORD{ {}, mouse::button_left, {}, {} } ) noexcept
              : parent_ui{ _parent_ui }
              , node_index{ _node_index }
              , button_state{ _event.dwButtonState }
              , ctrl_state{ _event.dwControlKeyState }
              , event_flag{ _event.dwEventFlags }
            { }
            func_args( const func_args & ) noexcept = default;
            func_args( func_args && ) noexcept      = default;
            ~func_args() noexcept                   = default;
        };
        using callback_type = std::function< func_return_type( func_args ) >;
      private:
        inline static HANDLE std_input_handle_;
        inline static HANDLE std_output_handle_;
        enum class console_attrs_ : char { normal, lock_text, lock_all };
        struct line_node_ final {
            std::string text{};
            callback_type func{};
            WORD default_attrs{ console_text::default_attrs };
            WORD intensity_attrs{ console_text::foreground_green | console_text::foreground_blue };
            WORD last_attrs{ console_text::default_attrs };
            COORD position{};
            auto set_attrs( const WORD _attrs ) noexcept
            {
                SetConsoleTextAttribute( std_output_handle_, _attrs );
                last_attrs = _attrs;
            }
            auto operator==( const COORD _position ) const noexcept
            {
                return position.Y == _position.Y && position.X <= _position.X
                    && _position.X < position.X + static_cast< SHORT >( text.size() );
            }
            auto operator!=( const COORD _position ) const noexcept
            {
                return !( *this == _position );
            }
            auto operator=( const line_node_ & ) noexcept -> line_node_ & = default;
            auto operator=( line_node_ && ) noexcept -> line_node_ &      = default;
            line_node_() noexcept                                         = default;
            line_node_( const std::string_view _text, callback_type &_func, const WORD _default_attrs, const WORD _intensity_attrs ) noexcept
              : text{ _text }
              , func{ std::move( _func ) }
              , default_attrs{ _default_attrs }
              , intensity_attrs{ _intensity_attrs }
              , last_attrs{ _default_attrs }
            { }
            line_node_( const line_node_ & )     = default;
            line_node_( line_node_ && ) noexcept = default;
            ~line_node_() noexcept               = default;
        };
        std::deque< line_node_ > lines_{};
        static auto show_cursor_( const WINBOOL _is_show ) noexcept
        {
            CONSOLE_CURSOR_INFO cursor_data;
            GetConsoleCursorInfo( std_output_handle_, &cursor_data );
            cursor_data.bVisible = _is_show;
            SetConsoleCursorInfo( std_output_handle_, &cursor_data );
        }
        static auto edit_console_attrs_( const console_attrs_ _attrs ) noexcept
        {
            DWORD attrs;
            GetConsoleMode( std_input_handle_, &attrs );
            switch ( _attrs ) {
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
                default : std::unreachable();
            }
            SetConsoleMode( std_input_handle_, attrs );
        }
        static auto get_cursor_() noexcept
        {
            CONSOLE_SCREEN_BUFFER_INFO console_data;
            GetConsoleScreenBufferInfo( std_output_handle_, &console_data );
            return console_data.dwCursorPosition;
        }
        static auto set_cursor_( const COORD _cursor_position ) noexcept
        {
            SetConsoleCursorPosition( std_output_handle_, _cursor_position );
        }
        static auto get_event_( const bool _is_move = true ) noexcept
        {
            using namespace std::chrono_literals;
            INPUT_RECORD record;
            DWORD reg;
            while ( true ) {
                std::this_thread::sleep_for( 50ms );
                ReadConsoleInputW( std_input_handle_, &record, 1, &reg );
                if ( record.EventType == MOUSE_EVENT && ( _is_move || record.Event.MouseEvent.dwEventFlags != mouse::move ) ) {
                    return record.Event.MouseEvent;
                }
            }
        }
        static auto get_console_size_() noexcept
        {
            CONSOLE_SCREEN_BUFFER_INFO console_data;
            GetConsoleScreenBufferInfo( std_output_handle_, &console_data );
            return console_data.dwSize;
        }
        static auto cls_()
        {
            const auto [ width, height ]{ get_console_size_() };
            set_cursor_( COORD{ 0, 0 } );
            std::print( "{}", std::string( static_cast< unsigned int >( width ) * static_cast< unsigned int >( height ), ' ' ) );
            set_cursor_( COORD{ 0, 0 } );
        }
        static auto write_( const std::string &_text, const bool _is_endl = false )
        {
            std::print( "{}", _text );
            if ( _is_endl ) {
                std::print( "\n" );
            }
        }
        static auto rewrite_( const COORD _cursor_position, const std::string &_text )
        {
            set_cursor_( COORD{ 0, _cursor_position.Y } );
            write_( std::string( _cursor_position.X, ' ' ) );
            set_cursor_( COORD{ 0, _cursor_position.Y } );
            write_( _text );
            set_cursor_( COORD{ 0, _cursor_position.Y } );
        }
        auto init_pos_()
        {
            cls_();
            const auto tail{ &lines_.back() };
            for ( auto &line : lines_ ) {
                line.position = get_cursor_();
                line.set_attrs( line.default_attrs );
                write_( line.text, &line != tail );
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
        auto invoke_func_( const MOUSE_EVENT_RECORD &_event )
        {
            auto is_exit{ back };
            auto size{ lines_.size() };
            for ( const auto idx : std::ranges::iota_view{ decltype( size ){ 0 }, size } ) {
                auto &line{ lines_[ idx ] };
                if ( line != _event.dwMousePosition ) {
                    continue;
                }
                if ( line.func == nullptr ) {
                    break;
                }
                cls_();
                line.set_attrs( line.default_attrs );
                show_cursor_( FALSE );
                edit_console_attrs_( console_attrs_::lock_all );
                is_exit = line.func( func_args{ *this, idx, _event } );
                show_cursor_( FALSE );
                edit_console_attrs_( console_attrs_::lock_text );
                init_pos_();
                break;
            }
            return is_exit;
        }
      public:
        auto empty() const noexcept
        {
            return lines_.empty();
        }
        auto size() const noexcept
        {
            return lines_.size();
        }
        auto max_size() const noexcept
        {
            return lines_.max_size();
        }
        auto &resize( const size_type _size )
        {
            lines_.resize( _size );
            return *this;
        }
        auto &optimize_storage() noexcept
        {
            constexpr auto default_capacity{ std::string{}.capacity() };
            for ( auto &line : lines_ ) {
                auto &text{ line.text };
                if ( text.capacity() > text.size() && text.capacity() != default_capacity ) {
                    text.shrink_to_fit();
                }
            }
            lines_.shrink_to_fit();
            return *this;
        }
        auto &swap( console_ui &_src ) noexcept
        {
            lines_.swap( _src.lines_ );
            return *this;
        }
        auto &add_front(
          const std::string_view _text, callback_type _func = nullptr,
          const WORD _intensity_attrs = console_text::foreground_green | console_text::foreground_blue,
          const WORD _default_attrs   = console_text::default_attrs )
        {
            lines_.emplace_front( _text, _func, _default_attrs, _func != nullptr ? _intensity_attrs : _default_attrs );
            return *this;
        }
        auto &add_back(
          const std::string_view _text, callback_type _func = nullptr,
          const WORD _intensity_attrs = console_text::foreground_blue | console_text::foreground_green,
          const WORD _default_attrs   = console_text::default_attrs )
        {
            lines_.emplace_back( _text, _func, _default_attrs, _func != nullptr ? _intensity_attrs : _default_attrs );
            return *this;
        }
        auto &insert(
          const size_type _index, const std::string_view _text, callback_type _func = nullptr,
          const WORD _intensity_attrs = console_text::foreground_green | console_text::foreground_blue,
          const WORD _default_attrs   = console_text::default_attrs )
        {
            lines_.emplace(
              lines_.cbegin() + _index, _text, _func, _default_attrs, _func != nullptr ? _intensity_attrs : _default_attrs );
            return *this;
        }
        auto &edit_text( const size_type _index, const std::string_view _text )
        {
            lines_.at( _index ).text = _text;
            return *this;
        }
        auto &edit_func( const size_type _index, callback_type _func )
        {
            lines_.at( _index ).func = std::move( _func );
            return *this;
        }
        auto &edit_intensity_attrs( const size_type _index, const WORD _intensity_attrs )
        {
            lines_.at( _index ).intensity_attrs = _intensity_attrs;
            return *this;
        }
        auto &edit_default_attrs( const size_type _index, const WORD _default_attrs )
        {
            lines_.at( _index ).default_attrs = _default_attrs;
            return *this;
        }
        auto &remove_front() noexcept
        {
            lines_.pop_front();
            return *this;
        }
        auto &remove_back() noexcept
        {
            lines_.pop_back();
            return *this;
        }
        auto &remove( const size_type _begin, const size_type _end )
        {
            lines_.erase( lines_.cbegin() + _begin, lines_.cbegin() + _end );
            return *this;
        }
        auto &clear() noexcept
        {
            lines_.clear();
            return *this;
        }
        auto &show()
        {
            if ( empty() ) {
                return *this;
            }
            using namespace std::chrono_literals;
            show_cursor_( FALSE );
            edit_console_attrs_( console_attrs_::lock_text );
            init_pos_();
            MOUSE_EVENT_RECORD event;
            auto func_return_value{ back };
            while ( func_return_value == back ) {
                event = get_event_();
                switch ( event.dwEventFlags ) {
                    case mouse::move : refresh_( event.dwMousePosition ); break;
                    case mouse::click : {
                        if ( event.dwButtonState != false ) {
                            func_return_value = invoke_func_( event );
                        }
                        break;
                    }
                }
            }
            cls_();
            return *this;
        }
        auto &set_limits( const bool _is_hide_cursor, const bool _is_lock_text ) noexcept
        {
            show_cursor_( static_cast< WINBOOL >( !_is_hide_cursor ) );
            edit_console_attrs_( _is_lock_text ? console_attrs_::lock_all : console_attrs_::normal );
            return *this;
        }
        auto operator=( const console_ui & ) noexcept -> console_ui & = default;
        auto operator=( console_ui && ) noexcept -> console_ui &      = default;
        console_ui() noexcept
        {
            if ( std_input_handle_ == nullptr ) [[unlikely]] {
                std_input_handle_ = GetStdHandle( console_handle_flag::std_input );
            }
            if ( std_output_handle_ == nullptr ) [[unlikely]] {
                std_output_handle_ = GetStdHandle( console_handle_flag::std_output );
            }
        }
        console_ui( const HANDLE _std_input_handle, const HANDLE _std_output_handle ) noexcept
        {
            std_input_handle_  = _std_input_handle;
            std_output_handle_ = _std_output_handle;
        }
        console_ui( const console_ui & ) noexcept = default;
        console_ui( console_ui && ) noexcept      = default;
        ~console_ui() noexcept                    = default;
    };
#else
# error "must be compiled on the Windows OS"
#endif
}
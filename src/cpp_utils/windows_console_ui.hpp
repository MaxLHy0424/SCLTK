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
namespace cpp_utils
{
#if defined( _WIN32 ) || defined( _WIN64 )
    class console_ui final
    {
      public:
        using func_return_t = bool;
        static inline constexpr func_return_t func_back{ false };
        static inline constexpr func_return_t func_exit{ true };
        struct func_args final
        {
            console_ui& parent_ui;
            const size_t node_index;
            const DWORD button_state;
            const DWORD ctrl_state;
            const DWORD event_flag;
            auto operator=( const func_args& ) noexcept -> func_args& = default;
            auto operator=( func_args&& ) noexcept -> func_args&      = default;
            func_args(
              console_ui& parent_ui, const size_t node_index,
              const MOUSE_EVENT_RECORD event = MOUSE_EVENT_RECORD{ {}, mouse::button_left, {}, {} } ) noexcept
              : parent_ui{ parent_ui }
              , node_index{ node_index }
              , button_state{ event.dwButtonState }
              , ctrl_state{ event.dwControlKeyState }
              , event_flag{ event.dwEventFlags }
            { }
            func_args( const func_args& ) noexcept = default;
            func_args( func_args&& ) noexcept      = default;
            ~func_args() noexcept                  = default;
        };
        using callback_t = std::variant< std::function< func_return_t() >, std::function< func_return_t( func_args ) > >;
      private:
        static inline HANDLE std_input_handle_;
        static inline HANDLE std_output_handle_;
        enum class console_attrs_selection_ : char
        {
            normal,
            lock_text,
            lock_all
        };
        struct line_node_ final
        {
            std::string text{};
            callback_t func{};
            WORD default_attrs{ console_text::default_attrs };
            WORD intensity_attrs{ console_text::foreground_green | console_text::foreground_blue };
            WORD last_attrs{ console_text::default_attrs };
            COORD position{};
            auto set_attrs( const WORD current_attrs ) noexcept
            {
                SetConsoleTextAttribute( std_output_handle_, current_attrs );
                last_attrs = current_attrs;
            }
            auto operator==( const COORD current_position ) const noexcept
            {
                return position.Y == current_position.Y && position.X <= current_position.X
                    && current_position.X < position.X + static_cast< SHORT >( text.size() );
            }
            auto operator!=( const COORD current_position ) const noexcept
            {
                return !( *this == current_position );
            }
            auto operator=( const line_node_& ) noexcept -> line_node_& = default;
            auto operator=( line_node_&& ) noexcept -> line_node_&      = default;
            line_node_() noexcept                                       = default;
            line_node_( const std::string_view text, callback_t& func, const WORD default_attrs, const WORD intensity_attrs ) noexcept
              : text{ text }
              , func{ std::move( func ) }
              , default_attrs{ default_attrs }
              , intensity_attrs{ intensity_attrs }
              , last_attrs{ default_attrs }
            { }
            line_node_( const line_node_& )     = default;
            line_node_( line_node_&& ) noexcept = default;
            ~line_node_() noexcept              = default;
        };
        std::deque< line_node_ > lines_{};
        static auto show_cursor_( const WINBOOL is_show ) noexcept
        {
            CONSOLE_CURSOR_INFO cursor_data;
            GetConsoleCursorInfo( std_output_handle_, &cursor_data );
            cursor_data.bVisible = is_show;
            SetConsoleCursorInfo( std_output_handle_, &cursor_data );
        }
        static auto edit_console_attrs_( const console_attrs_selection_ attrs_selection ) noexcept
        {
            DWORD attrs;
            GetConsoleMode( std_input_handle_, &attrs );
            switch ( attrs_selection ) {
                case console_attrs_selection_::normal :
                    attrs |= ENABLE_QUICK_EDIT_MODE;
                    attrs |= ENABLE_INSERT_MODE;
                    attrs |= ENABLE_MOUSE_INPUT;
                    break;
                case console_attrs_selection_::lock_text :
                    attrs &= ~ENABLE_QUICK_EDIT_MODE;
                    attrs &= ~ENABLE_INSERT_MODE;
                    attrs |= ENABLE_MOUSE_INPUT;
                    break;
                case console_attrs_selection_::lock_all :
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
        static auto set_cursor_( const COORD cursor_position ) noexcept
        {
            SetConsoleCursorPosition( std_output_handle_, cursor_position );
        }
        static auto get_event_( const bool is_move = true ) noexcept
        {
            using namespace std::chrono_literals;
            INPUT_RECORD record;
            DWORD reg;
            while ( true ) {
                std::this_thread::sleep_for( 50ms );
                ReadConsoleInputW( std_input_handle_, &record, 1, &reg );
                if ( record.EventType == MOUSE_EVENT && ( is_move || record.Event.MouseEvent.dwEventFlags != mouse::move ) ) {
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
        static auto write_( const std::string& text, const bool is_endl = false )
        {
            std::print( "{}", text );
            if ( is_endl ) {
                std::print( "\n" );
            }
        }
        static auto rewrite_( const COORD cursor_position, const std::string& text )
        {
            set_cursor_( COORD{ 0, cursor_position.Y } );
            write_( std::string( cursor_position.X, ' ' ) );
            set_cursor_( COORD{ 0, cursor_position.Y } );
            write_( text );
            set_cursor_( COORD{ 0, cursor_position.Y } );
        }
        auto init_pos_()
        {
            cls_();
            const auto tail{ &lines_.back() };
            for ( auto& line : lines_ ) {
                line.position = get_cursor_();
                line.set_attrs( line.default_attrs );
                write_( line.text, &line != tail );
            }
        }
        auto refresh_( const COORD hang_position )
        {
            for ( auto& line : lines_ ) {
                if ( line == hang_position && line.last_attrs != line.intensity_attrs ) {
                    line.set_attrs( line.intensity_attrs );
                    rewrite_( line.position, line.text );
                }
                if ( line != hang_position && line.last_attrs != line.default_attrs ) {
                    line.set_attrs( line.default_attrs );
                    rewrite_( line.position, line.text );
                }
            }
        }
        auto invoke_func_( const MOUSE_EVENT_RECORD& current_event )
        {
            auto is_exit{ func_back };
            auto size{ lines_.size() };
            for ( const auto i : std::ranges::iota_view{ decltype( size ){ 0 }, size } ) {
                auto& line{ lines_[ i ] };
                if ( line != current_event.dwMousePosition ) {
                    continue;
                }
                bool is_text{ false };
                line.func.visit( [ & ]( auto&& func )
                {
                    if ( func == nullptr ) {
                        is_text = true;
                    }
                } );
                if ( is_text == true ) {
                    break;
                }
                cls_();
                line.set_attrs( line.default_attrs );
                show_cursor_( FALSE );
                edit_console_attrs_( console_attrs_selection_::lock_all );
                line.func.visit( [ & ]( auto&& func )
                {
                    using func_t = std::decay_t< decltype( func ) >;
                    if constexpr ( std::is_same_v< func_t, std::function< func_return_t() > > ) {
                        is_exit = func();
                    } else if constexpr ( std::is_same_v< func_t, std::function< func_return_t( func_args ) > > ) {
                        is_exit = func( func_args{ *this, i, current_event } );
                    } else {
                        static_assert( false, "unknown callback!" );
                    }
                } );
                show_cursor_( FALSE );
                edit_console_attrs_( console_attrs_selection_::lock_text );
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
        auto& resize( const size_t size )
        {
            lines_.resize( size );
            return *this;
        }
        auto& optimize_storage() noexcept
        {
            constexpr auto default_capacity{ std::string{}.capacity() };
            for ( auto& line : lines_ ) {
                auto& text{ line.text };
                if ( text.capacity() > text.size() && text.capacity() != default_capacity ) {
                    text.shrink_to_fit();
                }
            }
            lines_.shrink_to_fit();
            return *this;
        }
        auto& swap( console_ui& src ) noexcept
        {
            lines_.swap( src.lines_ );
            return *this;
        }
        auto& add_front(
          const std::string_view text, callback_t func = {},
          const WORD intensity_attrs = console_text::foreground_green | console_text::foreground_blue,
          const WORD default_attrs   = console_text::default_attrs )
        {
            bool is_func{ false };
            func.visit( [ & ]( auto&& func ) { is_func = ( func != nullptr ); } );
            lines_.emplace_front( text, func, default_attrs, is_func ? intensity_attrs : default_attrs );
            return *this;
        }
        auto& add_back(
          const std::string_view text, callback_t func = {},
          const WORD intensity_attrs = console_text::foreground_blue | console_text::foreground_green,
          const WORD default_attrs   = console_text::default_attrs )
        {
            bool is_func{ false };
            func.visit( [ & ]( auto&& func ) { is_func = ( func != nullptr ); } );
            lines_.emplace_back( text, func, default_attrs, is_func ? intensity_attrs : default_attrs );
            return *this;
        }
        auto& insert(
          const size_t index, const std::string_view text, callback_t func = {},
          const WORD intensity_attrs = console_text::foreground_green | console_text::foreground_blue,
          const WORD default_attrs   = console_text::default_attrs )
        {
            bool is_func{ false };
            func.visit( [ & ]( auto&& func ) { is_func = ( func != nullptr ); } );
            lines_.emplace( lines_.cbegin() + index, text, func, default_attrs, is_func ? intensity_attrs : default_attrs );
            return *this;
        }
        auto& edit_text( const size_t index, const std::string_view text )
        {
            if constexpr ( is_debug_build ) {
                lines_.at( index ).text = text;
            } else {
                lines_[ index ].text = text;
            }
            return *this;
        }
        auto& edit_func( const size_t index, callback_t func )
        {
            if constexpr ( is_debug_build ) {
                lines_.at( index ).func = std::move( func );
            } else {
                lines_[ index ].func = std::move( func );
            }
            return *this;
        }
        auto& edit_intensity_attrs( const size_t index, const WORD intensity_attrs )
        {
            if constexpr ( is_debug_build ) {
                lines_.at( index ).intensity_attrs = intensity_attrs;
            } else {
                lines_[ index ].intensity_attrs = intensity_attrs;
            }
            return *this;
        }
        auto& edit_default_attrs( const size_t index, const WORD default_attrs )
        {
            if constexpr ( is_debug_build ) {
                lines_.at( index ).default_attrs = default_attrs;
            } else {
                lines_[ index ].default_attrs = default_attrs;
            }
            return *this;
        }
        auto& remove_front() noexcept
        {
            lines_.pop_front();
            return *this;
        }
        auto& remove_back() noexcept
        {
            lines_.pop_back();
            return *this;
        }
        auto& remove( const size_t begin, const size_t length )
        {
            lines_.erase( lines_.cbegin() + begin, lines_.cbegin() + begin + length );
            return *this;
        }
        auto& clear() noexcept
        {
            lines_.clear();
            return *this;
        }
        auto& show()
        {
            if ( empty() ) {
                return *this;
            }
            using namespace std::chrono_literals;
            show_cursor_( FALSE );
            edit_console_attrs_( console_attrs_selection_::lock_text );
            init_pos_();
            MOUSE_EVENT_RECORD event;
            auto func_return_value{ func_back };
            while ( func_return_value == func_back ) {
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
        auto& set_limits( const bool is_hide_cursor, const bool is_lock_text ) noexcept
        {
            show_cursor_( static_cast< WINBOOL >( !is_hide_cursor ) );
            edit_console_attrs_( is_lock_text ? console_attrs_selection_::lock_all : console_attrs_selection_::normal );
            return *this;
        }
        auto operator=( const console_ui& ) noexcept -> console_ui& = default;
        auto operator=( console_ui&& ) noexcept -> console_ui&      = default;
        console_ui() noexcept
        {
            if ( std_input_handle_ == nullptr ) [[unlikely]] {
                std_input_handle_ = GetStdHandle( console_handle_flag::std_input );
            }
            if ( std_output_handle_ == nullptr ) [[unlikely]] {
                std_output_handle_ = GetStdHandle( console_handle_flag::std_output );
            }
        }
        console_ui( const HANDLE std_input_handle, const HANDLE std_output_handle ) noexcept
        {
            std_input_handle_  = std_input_handle;
            std_output_handle_ = std_output_handle;
        }
        console_ui( const console_ui& ) noexcept = default;
        console_ui( console_ui&& ) noexcept      = default;
        ~console_ui() noexcept                   = default;
    };
#else
# error "must be compiled on the windows os"
#endif
}
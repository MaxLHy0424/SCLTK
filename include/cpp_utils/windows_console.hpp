#pragma once
#include <functional>
#include <memory_resource>
#include <print>
#include <string>
#include <utility>
#include "compiler.hpp"
#include "windows_impl.hpp"
namespace cpp_utils
{
#if defined( _WIN32 ) || defined( _WIN64 )
    class console final
    {
      public:
        HWND window_handle{ GetConsoleWindow() };
        HANDLE std_input_handle{ GetStdHandle( STD_INPUT_HANDLE ) };
        HANDLE std_output_handle{ GetStdHandle( STD_OUTPUT_HANDLE ) };
        HANDLE std_error_handle{ GetStdHandle( STD_ERROR_HANDLE ) };
        [[nodiscard]] auto get_state() noexcept
        {
            WINDOWPLACEMENT wp;
            wp.length = sizeof( WINDOWPLACEMENT );
            GetWindowPlacement( window_handle, &wp );
            return wp.showCmd;
        }
        auto&& set_state( this auto&& self, const UINT state ) noexcept
        {
            ShowWindow( self.window_handle, state );
            return self;
        }
        auto&& forced_show( this auto&& self ) noexcept
        {
            const auto thread_id{ GetCurrentThreadId() };
            const auto window_thread_proc_id{ GetWindowThreadProcessId( self.window_handle, nullptr ) };
            AttachThreadInput( thread_id, window_thread_proc_id, TRUE );
            SetWindowPos( self.window_handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
            SetForegroundWindow( self.window_handle );
            AttachThreadInput( thread_id, window_thread_proc_id, FALSE );
            return self;
        }
        template < typename ChronoRep, typename ChronoPeriod >
        [[noreturn]] auto
          forced_show_forever( this auto&& self, const std::chrono::duration< ChronoRep, ChronoPeriod > sleep_duration ) noexcept
        {
            const auto thread_id{ GetCurrentThreadId() };
            const auto window_thread_proc_id{ GetWindowThreadProcessId( self.window_handle, nullptr ) };
            for ( ;; ) {
                AttachThreadInput( thread_id, window_thread_proc_id, TRUE );
                SetWindowPos( self.window_handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
                SetForegroundWindow( self.window_handle );
                AttachThreadInput( thread_id, window_thread_proc_id, FALSE );
                std::this_thread::sleep_for( sleep_duration );
            }
        }
        template < typename ChronoRep, typename ChronoPeriod, std::invocable F >
        auto&& forced_show_until(
          this auto&& self, const std::chrono::duration< ChronoRep, ChronoPeriod > sleep_duration, F&& condition_checker ) noexcept
        {
            const auto thread_id{ GetCurrentThreadId() };
            const auto window_thread_proc_id{ GetWindowThreadProcessId( self.window_handle, nullptr ) };
            while ( !std::forward< F >( condition_checker )() ) {
                AttachThreadInput( thread_id, window_thread_proc_id, TRUE );
                SetWindowPos( self.window_handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
                SetForegroundWindow( self.window_handle );
                AttachThreadInput( thread_id, window_thread_proc_id, FALSE );
                std::this_thread::sleep_for( sleep_duration );
            }
            return self;
        }
        auto&& cancel_forced_show( this auto&& self ) noexcept
        {
            SetWindowPos( self.window_handle, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
            return self;
        }
        auto&& fix_size( this auto&& self, const bool is_enable ) noexcept
        {
            SetWindowLongPtrW(
              self.window_handle, GWL_STYLE,
              is_enable
                ? GetWindowLongPtrW( self.window_handle, GWL_STYLE ) & ~WS_SIZEBOX
                : GetWindowLongPtrW( self.window_handle, GWL_STYLE ) | WS_SIZEBOX );
            return self;
        }
        auto&& enable_context_menu( this auto&& self, const bool is_enable ) noexcept
        {
            SetWindowLongPtrW(
              self.window_handle, GWL_STYLE,
              is_enable
                ? GetWindowLongPtrW( self.window_handle, GWL_STYLE ) | WS_SYSMENU
                : GetWindowLongPtrW( self.window_handle, GWL_STYLE ) & ~WS_SYSMENU );
            return self;
        }
        auto&& enable_window_minimize_ctrl( this auto&& self, const bool is_enable ) noexcept
        {
            SetWindowLongPtrW(
              self.window_handle, GWL_STYLE,
              is_enable
                ? GetWindowLongPtrW( self.window_handle, GWL_STYLE ) | WS_MINIMIZEBOX
                : GetWindowLongPtrW( self.window_handle, GWL_STYLE ) & ~WS_MINIMIZEBOX );
            return self;
        }
        auto&& enable_window_maximize_ctrl( this auto&& self, const bool is_enable ) noexcept
        {
            SetWindowLongPtrW(
              self.window_handle, GWL_STYLE,
              is_enable
                ? GetWindowLongPtrW( self.window_handle, GWL_STYLE ) | WS_MAXIMIZEBOX
                : GetWindowLongPtrW( self.window_handle, GWL_STYLE ) & ~WS_MAXIMIZEBOX );
            return self;
        }
        auto&& enable_window_close_ctrl( this auto&& self, const bool is_enable ) noexcept
        {
            EnableMenuItem(
              GetSystemMenu( self.window_handle, FALSE ), SC_CLOSE,
              is_enable ? MF_BYCOMMAND | MF_ENABLED : MF_BYCOMMAND | MF_DISABLED | MF_GRAYED );
            return self;
        }
        auto&& press_any_key_to_continue( this auto&& self ) noexcept
        {
            DWORD mode;
            if ( !GetConsoleMode( self.std_input_handle, &mode ) ) [[unlikely]] {
                return self;
            }
            SetConsoleMode( self.std_input_handle, ENABLE_EXTENDED_FLAGS | ( mode & ~ENABLE_QUICK_EDIT_MODE ) );
            FlushConsoleInputBuffer( self.std_input_handle );
            INPUT_RECORD record;
            DWORD events;
            do {
                ReadConsoleInputW( self.std_input_handle, &record, 1, &events );
            } while ( record.EventType != KEY_EVENT || !record.Event.KeyEvent.bKeyDown );
            SetConsoleMode( self.std_input_handle, mode );
            return self;
        }
        auto&& ignore_exit_signal( this auto&& self, const bool is_ignore ) noexcept
        {
            SetConsoleCtrlHandler( nullptr, static_cast< WINBOOL >( is_ignore ) );
            return self;
        }
        auto&& enable_virtual_terminal_processing( this auto&& self, const bool is_enable ) noexcept
        {
            DWORD mode;
            GetConsoleMode( self.std_output_handle, &mode );
            is_enable ? mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING : mode &= ~ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode( self.std_output_handle, mode );
            return self;
        }
        auto&& clear( this auto&& self, std::pmr::memory_resource* const resource = std::pmr::get_default_resource() )
        {
            CONSOLE_SCREEN_BUFFER_INFO info;
            GetConsoleScreenBufferInfo( self.std_output_handle, &info );
            constexpr COORD top_left{ 0, 0 };
            const auto area{ info.dwSize.X * info.dwSize.Y };
            DWORD written;
            SetConsoleCursorPosition( self.std_output_handle, top_left );
            std::print( "{}", std::pmr::string( static_cast< std::size_t >( area ), ' ', resource ) );
            FillConsoleOutputAttribute( self.std_output_handle, info.wAttributes, area, top_left, &written );
            SetConsoleCursorPosition( self.std_output_handle, top_left );
            return self;
        }
        [[nodiscard]] auto get_size() const noexcept
        {
            CONSOLE_SCREEN_BUFFER_INFO info;
            GetConsoleScreenBufferInfo( std_output_handle, &info );
            return info.dwSize;
        }
        auto&& set_size(
          this auto&& self, const SHORT width, const SHORT height,
          std::pmr::memory_resource* const resource = std::pmr::get_default_resource() )
        {
            SMALL_RECT wrt{ 0, 0, static_cast< SHORT >( width - 1 ), static_cast< SHORT >( height - 1 ) };
            ShowWindow( self.window_handle, SW_SHOWNORMAL );
            SetConsoleScreenBufferSize( self.std_output_handle, { width, height } );
            SetConsoleWindowInfo( self.std_output_handle, TRUE, &wrt );
            SetConsoleScreenBufferSize( self.std_output_handle, { width, height } );
            SetConsoleWindowInfo( self.std_output_handle, TRUE, &wrt );
            self.clear( resource );
            return self;
        }
        auto&& set_title( this auto&& self, const char* const title ) noexcept
        {
            SetConsoleTitleA( title );
            return self;
        }
        auto&& set_title( this auto&& self, const wchar_t* const title ) noexcept
        {
            SetConsoleTitleW( title );
            return self;
        }
        auto&& set_charset( this auto&& self, const UINT charset_id ) noexcept
        {
            SetConsoleOutputCP( charset_id );
            SetConsoleCP( charset_id );
            return self;
        }
        auto&& set_translucency( this auto&& self, const BYTE value ) noexcept
        {
            SetLayeredWindowAttributes( self.window_handle, RGB( 0, 0, 0 ), value, LWA_ALPHA );
            return self;
        }
        auto&& show_cursor( this auto&& self, const bool is_shown ) noexcept
        {
            CONSOLE_CURSOR_INFO cursor_data;
            GetConsoleCursorInfo( self.std_output_handle, &cursor_data );
            cursor_data.bVisible = static_cast< WINBOOL >( is_shown );
            SetConsoleCursorInfo( self.std_output_handle, &cursor_data );
            return self;
        }
        auto&& lock_text( this auto&& self, const bool is_locked ) noexcept
        {
            DWORD attrs;
            if ( !GetConsoleMode( self.std_input_handle, &attrs ) ) [[unlikely]] {
                return self;
            }
            switch ( is_locked ) {
                case false :
                    attrs |= ENABLE_QUICK_EDIT_MODE;
                    attrs |= ENABLE_INSERT_MODE;
                    break;
                case true :
                    attrs &= ~ENABLE_QUICK_EDIT_MODE;
                    attrs &= ~ENABLE_INSERT_MODE;
                    break;
            }
            attrs |= ENABLE_MOUSE_INPUT;
            attrs |= ENABLE_LINE_INPUT;
            SetConsoleMode( self.std_input_handle, attrs );
            return self;
        }
    };
    class console_ui final
    {
      public:
        enum class func_action : bool
        {
            back,
            exit
        };
        static inline constexpr auto func_back{ func_action::back };
        static inline constexpr auto func_exit{ func_action::exit };
        struct func_args final
        {
            console_ui& parent_ui;
            std::size_t node_index;
            DWORD button_state;
            DWORD ctrl_state;
            DWORD event_flag;
            auto operator=( const func_args& ) noexcept -> func_args& = default;
            auto operator=( func_args&& ) noexcept -> func_args&      = default;
            func_args(
              console_ui& parent_ui_ref, const std::size_t node_index_in_parent_ui,
              const MOUSE_EVENT_RECORD event = { {}, mouse::button_left, {}, {} } ) noexcept
              : parent_ui{ parent_ui_ref }
              , node_index{ node_index_in_parent_ui }
              , button_state{ event.dwButtonState }
              , ctrl_state{ event.dwControlKeyState }
              , event_flag{ event.dwEventFlags }
            { }
            func_args( const func_args& ) noexcept = default;
            func_args( func_args&& ) noexcept      = default;
            ~func_args() noexcept                  = default;
        };
        using basic_text_type = std::variant< std::string_view, std::pmr::string, std::string >;
        struct text_type final : public basic_text_type
        {
            using basic_text_type::variant;
            template < std::size_t N >
            consteval text_type( const char ( &string_literal )[ N ] ) noexcept
              : basic_text_type{ ( std::string_view{ string_literal, N - 1 } ) }
            { }
        };
        using function_type
          = std::variant< std::copyable_function< func_action() >, std::copyable_function< func_action( func_args ) > >;
      private:
        struct line_node_ final
        {
            text_type text{};
            function_type func{};
            WORD default_attrs{ console_text::foreground_white };
            WORD intensity_attrs{ console_text::foreground_green | console_text::foreground_blue };
            WORD last_attrs{ console_text::foreground_white };
            COORD position{};
            auto print_text() const
            {
                text.visit( []( const auto& line_text ) static
                {
                    std::print( "{}", line_text );
                } );
            }
            auto operator==( const COORD current_position ) const noexcept
            {
                const auto text_size{ text.visit< std::size_t >( []( const auto& txt ) static noexcept
                {
                    return txt.size();
                } ) };
                return position.Y == current_position.Y && position.X <= current_position.X
                    && current_position.X < position.X + static_cast< SHORT >( text_size );
            }
            auto operator=( const line_node_& ) -> line_node_&     = delete;
            auto operator=( line_node_&& ) noexcept -> line_node_& = default;
            line_node_() noexcept                                  = default;
            line_node_( text_type& text_ref, function_type& func_ref, const WORD default_text_attrs, const WORD intensity_text_attrs ) noexcept
              : text{ std::move( text_ref ) }
              , func{ std::move( func_ref ) }
              , default_attrs{ default_text_attrs }
              , intensity_attrs{ intensity_text_attrs }
              , last_attrs{ default_text_attrs }
            { }
            line_node_( const line_node_& )     = default;
            line_node_( line_node_&& ) noexcept = default;
            ~line_node_() noexcept              = default;
        };
        std::pmr::vector< line_node_ > lines_;
        const console& con_;
        auto set_line_attrs_( line_node_& self, const WORD current_attrs ) noexcept
        {
            SetConsoleTextAttribute( con_.std_output_handle, current_attrs );
            self.last_attrs = current_attrs;
        }
        auto get_cursor_() noexcept
        {
            CONSOLE_SCREEN_BUFFER_INFO console_data;
            GetConsoleScreenBufferInfo( con_.std_output_handle, &console_data );
            return console_data.dwCursorPosition;
        }
        auto try_get_event_( INPUT_RECORD& record )
        {
            if ( WaitForSingleObject( con_.std_input_handle, INFINITE ) == WAIT_OBJECT_0 ) {
                DWORD reg;
                if ( ReadConsoleInputW( con_.std_input_handle, &record, 1, &reg ) && reg != 0 ) {
                    return true;
                }
            }
            return false;
        }
        auto rewrite_( const COORD cursor_position, const line_node_& line )
        {
            const auto [ console_width, console_height ]{ cursor_position };
            SetConsoleCursorPosition( con_.std_output_handle, { 0, console_height } );
            std::print(
              "{}", std::pmr::string( static_cast< std::size_t >( console_width ), ' ', lines_.get_allocator().resource() ) );
            SetConsoleCursorPosition( con_.std_output_handle, { 0, console_height } );
            line.print_text();
            SetConsoleCursorPosition( con_.std_output_handle, { 0, console_height } );
        }
        auto init_pos_()
        {
            con_.clear( lines_.get_allocator().resource() );
            for ( const auto back_ptr{ &lines_.back() }; auto& line : lines_ ) {
                line.position = get_cursor_();
                set_line_attrs_( line, line.default_attrs );
                line.print_text();
                if ( &line != back_ptr ) {
                    std::print( "\n" );
                }
            }
        }
        auto refresh_( const COORD hang_position )
        {
            for ( auto& line : lines_ ) {
                if ( line == hang_position && line.last_attrs != line.intensity_attrs ) {
                    set_line_attrs_( line, line.intensity_attrs );
                    rewrite_( line.position, line );
                }
                if ( line != hang_position && line.last_attrs != line.default_attrs ) {
                    set_line_attrs_( line, line.default_attrs );
                    rewrite_( line.position, line );
                }
            }
        }
        static auto is_empty_function_( const function_type& func ) noexcept
        {
            return func.visit< bool >( []( const auto& various_func ) static noexcept
            {
                return various_func == nullptr;
            } );
        }
        auto invoke_func_( const MOUSE_EVENT_RECORD current_event )
        {
            line_node_* target{ nullptr };
            for ( auto& line : lines_ ) {
                if ( line == current_event.dwMousePosition ) {
                    target = &line;
                }
            }
            if ( target == nullptr ) [[unlikely]] {
                return func_back;
            }
            if ( is_empty_function_( target->func ) ) {
                return func_back;
            }
            con_.clear( lines_.get_allocator().resource() );
            set_line_attrs_( *target, target->default_attrs );
            con_.show_cursor( false );
            con_.lock_text( true );
            const auto value{ target->func.visit< func_action >( [ & ]< typename F >( F& func )
            {
                if constexpr ( std::is_same_v< F, std::copyable_function< func_action() > > ) {
                    return func();
                } else if constexpr ( std::is_same_v< F, std::copyable_function< func_action( func_args ) > > ) {
                    return func( func_args{ *this, static_cast< std::size_t >( target - lines_.begin().base() ), current_event } );
                } else {
                    static_assert( false, "unknown callback!" );
                }
            } ) };
            con_.show_cursor( false );
            con_.lock_text( true );
            init_pos_();
            return value;
        }
      public:
        [[nodiscard]] auto empty() const noexcept
        {
            return lines_.empty();
        }
        [[nodiscard]] auto size() const noexcept
        {
            return lines_.size();
        }
        [[nodiscard]] constexpr auto max_size() const noexcept
        {
            return lines_.max_size();
        }
        auto& reserve( const std::size_t size )
        {
            lines_.reserve( size );
            return *this;
        }
        auto& shrink_to_fit() noexcept
        {
            for ( auto& line : lines_ ) {
                line.text.visit( []< typename T >( T& line_text ) static
                {
                    if constexpr ( requires( T& t ) { t.shrink_to_fit(); } ) {
                        if ( line_text.capacity() > line_text.size() ) {
                            line_text.shrink_to_fit();
                        }
                    }
                } );
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
          text_type text, function_type func = {},
          const WORD intensity_attrs = console_text::foreground_green | console_text::foreground_blue,
          const WORD default_attrs   = console_text::foreground_white )
        {
            lines_.emplace(
              lines_.cbegin(), text, func, default_attrs, is_empty_function_( func ) ? default_attrs : intensity_attrs );
            return *this;
        }
        auto& add_back(
          text_type text, function_type func = {},
          const WORD intensity_attrs = console_text::foreground_blue | console_text::foreground_green,
          const WORD default_attrs   = console_text::foreground_white )
        {
            lines_.emplace_back( text, func, default_attrs, is_empty_function_( func ) ? default_attrs : intensity_attrs );
            return *this;
        }
        auto& insert(
          const std::size_t index, text_type text, function_type func = {},
          const WORD intensity_attrs = console_text::foreground_green | console_text::foreground_blue,
          const WORD default_attrs   = console_text::foreground_white )
        {
            lines_.emplace(
              lines_.cbegin() + index, text, func, default_attrs, is_empty_function_( func ) ? default_attrs : intensity_attrs );
            return *this;
        }
        auto& set_text( const std::size_t index, text_type text )
        {
            if constexpr ( is_debugging_build ) {
                lines_.at( index ).text = std::move( text );
            } else {
                lines_[ index ].text = std::move( text );
            }
            return *this;
        }
        auto& set_func( const std::size_t index, function_type func )
        {
            if constexpr ( is_debugging_build ) {
                lines_.at( index ).func = std::move( func );
            } else {
                lines_[ index ].func = std::move( func );
            }
            return *this;
        }
        auto& set_intensity_attrs( const std::size_t index, const WORD intensity_attrs )
        {
            if constexpr ( is_debugging_build ) {
                lines_.at( index ).intensity_attrs = intensity_attrs;
            } else {
                lines_[ index ].intensity_attrs = intensity_attrs;
            }
            return *this;
        }
        auto& set_default_attrs( const std::size_t index, const WORD default_attrs )
        {
            if constexpr ( is_debugging_build ) {
                lines_.at( index ).default_attrs = default_attrs;
            } else {
                lines_[ index ].default_attrs = default_attrs;
            }
            return *this;
        }
        auto& remove_front() noexcept
        {
            lines_.erase( lines_.cbegin() );
            return *this;
        }
        auto& remove_back() noexcept
        {
            lines_.pop_back();
            return *this;
        }
        auto& remove( const std::size_t begin, const std::size_t length )
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
            if ( empty() ) [[unlikely]] {
                return *this;
            }
            con_.show_cursor( false );
            con_.lock_text( true );
            init_pos_();
            INPUT_RECORD record;
            const auto& mouse_event{ record.Event.MouseEvent };
            auto func_return_value{ func_back };
            while ( func_return_value == func_back ) {
                if ( try_get_event_( record ) == false || record.EventType != MOUSE_EVENT ) {
                    continue;
                }
                if ( mouse_event.dwEventFlags == mouse::move ) {
                    refresh_( mouse_event.dwMousePosition );
                    continue;
                }
                if ( mouse_event.dwEventFlags == mouse::click && mouse_event.dwButtonState != 0 ) {
                    func_return_value = invoke_func_( mouse_event );
                }
            }
            con_.clear( lines_.get_allocator().resource() );
            return *this;
        }
        auto operator=( const console_ui& ) noexcept -> console_ui& = default;
        auto operator=( console_ui&& ) noexcept -> console_ui&      = default;
        console_ui( const console& console_info, std::pmr::memory_resource* const resource = std::pmr::get_default_resource() ) noexcept
          : lines_{ resource }
          , con_{ console_info }
        { }
        console_ui( console&&, std::pmr::memory_resource* const = std::pmr::get_default_resource() ) noexcept = delete;
        console_ui( const console_ui& ) noexcept                                                              = default;
        console_ui( console_ui&& ) noexcept                                                                   = default;
        ~console_ui() noexcept                                                                                = default;
    };
#else
# error "must be compiled on the windows os"
#endif
}
#pragma once
#include<stdio.h>
#include<windows.h>
#include<vector>
#include<string>
#include<functional>
#define CONSOLE_MOUSE_BUTTON_LEFT FROM_LEFT_1ST_BUTTON_PRESSED
#define CONSOLE_MOUSE_BUTTON_MIDDLE FROM_LEFT_2ND_BUTTON_PRESSED
#define CONSOLE_MOUSE_BUTTON_RIGHT RIGHTMOST_BUTTON_PRESSED
#define CONSOLE_MOUSE_CLICK 0x0
#define CONSOLE_MOUSE_CLICK_DOUBLE DOUBLE_CLICK
#define CONSOLE_MOUSE_MOVE MOUSE_MOVED
#define CONSOLE_MOUSE_WHEEL MOUSE_WHEELED
#define CONSOLE_TEXT_WHITE 0x07
#define CONSOLE_TEXT_BLUE 0x09
#define CONSOLE_TEXT_RED 0x0c
class console_ui final{
public:
    struct fn_args final{
        const DWORD button_state,ctrl_key_state,event_flag;
        console_ui *const ui;
        inline explicit fn_args():
          button_state{CONSOLE_MOUSE_BUTTON_LEFT},
          ctrl_key_state{},
          event_flag{},
          ui{}
        {}
        inline explicit fn_args(const MOUSE_EVENT_RECORD _mouse_event,console_ui *const _ui):
          button_state{_mouse_event.dwButtonState},
          ctrl_key_state{_mouse_event.dwControlKeyState},
          event_flag{_mouse_event.dwEventFlags},
          ui{_ui}
        {}
        inline ~fn_args(){}
    };
private:
    using size_type_=std::size_t;
    using callback_=std::function<bool(fn_args)>;
    struct ui_item_ final{
        const char *text;
        short default_color,highlight_color,last_color;
        COORD position;
        callback_ func;
        inline explicit ui_item_():
          text{},
          default_color{CONSOLE_TEXT_WHITE},
          highlight_color{CONSOLE_TEXT_BLUE},
          last_color{CONSOLE_TEXT_WHITE},
          position{},
          func{}
        {}
        inline explicit ui_item_(
            const char *const _text,
            const short _default_color,
            const short _highlight_color,
            const callback_ _func
        ):text{_text},
          default_color{_default_color},
          highlight_color{_highlight_color},
          last_color{CONSOLE_TEXT_WHITE},
          position{},
          func{_func}
        {}
        inline ~ui_item_(){}
        inline auto set_color(short _color){
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),_color);
            last_color=_color;
        }
        inline auto operator==(const COORD &_mouse_position)const{
            return (position.Y==_mouse_position.Y)&&
                   (position.X<=_mouse_position.X)&&
                   (_mouse_position.X<(position.X+static_cast<short>(strlen(text))));
        }
        inline auto operator!=(const COORD &_mouse_position)const{
            return !operator==(_mouse_position);
        }
    };
    std::vector<ui_item_> item_;
    short width_,height_;
    enum console_attrs_op_{v_normal=0,v_lock_text=1,v_lock_all=2};
    inline auto show_cursor_(const bool _mode){
        CONSOLE_CURSOR_INFO cursor;
        GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor);
        cursor.bVisible=_mode;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor);
    }
    inline auto edit_console_attrs_(const console_attrs_op_ _mode){
        DWORD attrs;
        GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),&attrs);
        switch(_mode){
            case v_normal:{
                attrs|=ENABLE_QUICK_EDIT_MODE,
                attrs|=ENABLE_INSERT_MODE,
                attrs|=ENABLE_MOUSE_INPUT;
                break;
            }case v_lock_text:{
                attrs&=~ENABLE_QUICK_EDIT_MODE,
                attrs&=~ENABLE_INSERT_MODE,
                attrs|=ENABLE_MOUSE_INPUT;
                break;
            }case v_lock_all:{
                attrs&=~ENABLE_QUICK_EDIT_MODE,
                attrs&=~ENABLE_INSERT_MODE,
                attrs&=~ENABLE_MOUSE_INPUT;
                break;
            }
        }
        SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),attrs);
    }
    inline auto get_cursor_(){
        CONSOLE_SCREEN_BUFFER_INFO console;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&console);
        return console.dwCursorPosition;
    }
    inline auto set_cursor_(const COORD &_position){
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),_position);
    }
    inline auto wait_mouse_event_(const bool _move=true){
        INPUT_RECORD record;
        DWORD reg;
        while(true){
            Sleep(10);
            ReadConsoleInputA(GetStdHandle(STD_INPUT_HANDLE),&record,1,&reg);
            if(
                (record.EventType==MOUSE_EVENT)&&
                (_move|(record.Event.MouseEvent.dwEventFlags!=MOUSE_MOVED))
            ){
                return record.Event.MouseEvent;
            }
        }
    }
    inline auto get_console_size_(){
        CONSOLE_SCREEN_BUFFER_INFO console;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&console);
        height_=console.dwSize.Y;
        width_=console.dwSize.X;
    }
    inline auto cls_(){
        get_console_size_();
        set_cursor_({0,0});
        printf("%s",std::string(width_*height_,' ').c_str());
        set_cursor_({0,0});
    }
    inline auto write_(const char *const _text,const bool _is_endl=false){
        printf("%s",_text);
        if(_is_endl){
            printf("\n");
        }
    }
    inline auto rewrite_(const COORD &_position,const char *const _text){
        set_cursor_({0,_position.Y});
        write_(std::string(_position.X,' ').c_str());
        set_cursor_({0,_position.Y});
        write_(_text);
        set_cursor_({0,_position.Y});
    }
    inline auto init_pos_(){
        cls_();
        for(auto &line:item_){
            line.position=get_cursor_();
            line.set_color(line.default_color);
            write_(line.text,true);
        }
    }
    inline auto refresh_(const COORD &_hang_position){
        for(auto &line:item_){
            if((line==_hang_position)&&(line.last_color!=line.highlight_color)){
                line.set_color(line.highlight_color);
                rewrite_(line.position,line.text);
            }
            if((line!=_hang_position)&&(line.last_color!=line.default_color)){
                line.set_color(line.default_color);
                rewrite_(line.position,line.text);
            }
        }
    }
    inline auto call_func_(const MOUSE_EVENT_RECORD &_mouse_event){
        bool is_exit{};
        for(auto &line:item_){
            if(line==_mouse_event.dwMousePosition){
                if(line.func!=nullptr){
                    cls_();
                    line.set_color(line.default_color);
                    edit_console_attrs_(v_lock_all);
                    is_exit=line.func(fn_args{_mouse_event,this});
                    show_cursor_(false);
                    edit_console_attrs_(v_lock_text);
                    init_pos_();
                }
                break;
            }
        }
        return is_exit;
    }
public:
    inline auto size(){
        return item_.size();
    }
    inline auto &set_console(
        const UINT _code_page,
        const CHAR *const _title,
        const SHORT _width,
        const SHORT _height,
        const bool _fix_size,
        const bool _minimize_ctrl,
        const bool _close_window_ctrl,
        const BYTE _transparency
    ){
        SetConsoleOutputCP(_code_page);
        SetConsoleCP(_code_page);
        SetConsoleTitleA(_title);
        using namespace std::string_literals;
        system(
            "mode con cols="s
              .append(std::to_string(_width))
              .append(" lines=")
              .append(std::to_string(_height))
              .c_str()
        );
        SetWindowLongPtrA(
            GetConsoleWindow(),GWL_STYLE,
            (_fix_size)
              ?(GetWindowLongPtrA(GetConsoleWindow(),GWL_STYLE)&~WS_SIZEBOX&~WS_MAXIMIZEBOX)
              :(GetWindowLongPtrA(GetConsoleWindow(),GWL_STYLE)|WS_SIZEBOX|WS_MAXIMIZEBOX)
        );
        SetWindowLongPtrA(
            GetConsoleWindow(),GWL_STYLE,
            (_minimize_ctrl)
              ?(GetWindowLongPtrA(GetConsoleWindow(),GWL_STYLE)|WS_MINIMIZEBOX)
              :(GetWindowLongPtrA(GetConsoleWindow(),GWL_STYLE)&~WS_MINIMIZEBOX)
        );
        EnableMenuItem(
            GetSystemMenu(GetConsoleWindow(),FALSE),
            SC_CLOSE,
            (_close_window_ctrl)
              ?(MF_BYCOMMAND|MF_ENABLED)
              :(MF_BYCOMMAND|MF_DISABLED|MF_GRAYED)
        );
        SetLayeredWindowAttributes(GetConsoleWindow(),0,_transparency,LWA_ALPHA);
        return *this;
    }
    inline auto &lock(const bool _is_hide_cursor,const bool _is_lock_text){
        show_cursor_(!_is_hide_cursor);
        edit_console_attrs_(
            (_is_lock_text)
              ?(v_lock_all)
              :(v_normal)
        );
        return *this;
    }
    inline auto &add(
        const char *const _text,
        const callback_ _func=nullptr,
        const short _highlight_color=CONSOLE_TEXT_BLUE,
        const short _default_color=CONSOLE_TEXT_WHITE
    ){
        item_.emplace_back(
            ui_item_{
                _text,
                _default_color,
                (_func==nullptr)
                  ?(_default_color)
                  :(_highlight_color),
                _func
            }
        );
        return *this;
    }
    inline auto &insert(
        const size_type_ _index,
        const char *const _text,
        const callback_ _func=nullptr,
        const short _highlight_color=CONSOLE_TEXT_BLUE,
        const short _default_color=CONSOLE_TEXT_WHITE
    ){
        item_.emplace(
            item_.begin()+_index,
            ui_item_{
                _text,
                _default_color,
                (_func==nullptr)
                  ?(_default_color)
                  :(_highlight_color),
                _func
            }
        );
        return *this;
    }
    inline auto &edit(
        const size_type_ _index,
        const char *const _text,
        const callback_ _func=nullptr,
        const short _highlight_color=CONSOLE_TEXT_BLUE,
        const short _default_color=CONSOLE_TEXT_WHITE
    ){
        item_.at(_index)=ui_item_{
            _text,
            _default_color,
            (_func==nullptr)
              ?(_default_color)
              :(_highlight_color),
            _func
        };
        return *this;
    }
    inline auto &revert(){
        item_.pop_back();
        return *this;
    }
    inline auto &remove(const size_type_ _begin,const size_type_ _end){
        item_.erase(item_.begin()+_begin,item_.begin()+_end);
        return *this;
    }
    inline auto &clear(){
        item_.clear();
        return *this;
    }
    inline auto &show(){
        show_cursor_(false);
        edit_console_attrs_(v_lock_text);
        MOUSE_EVENT_RECORD mouse_event;
        init_pos_();
        bool is_exit{};
        while(!is_exit){
            mouse_event=wait_mouse_event_();
            switch(mouse_event.dwEventFlags){
                case CONSOLE_MOUSE_MOVE:{
                    refresh_(mouse_event.dwMousePosition);
                    break;
                }case CONSOLE_MOUSE_CLICK:{
                    if((mouse_event.dwButtonState)&&(mouse_event.dwButtonState!=CONSOLE_MOUSE_WHEEL)){
                        is_exit=call_func_(mouse_event);
                    }
                    break;
                }
            }
            Sleep(10);
        }
        cls_();
        return *this;
    }
    inline explicit console_ui():
      item_{},
      width_{},
      height_{}
    {}
    inline explicit console_ui(const console_ui &_obj):
      item_{_obj.item_},
      width_{},
      height_{}
    {}
    inline explicit console_ui(const console_ui &&_obj):
      item_{std::move(_obj.item_)},
      width_{},
      height_{}
    {}
    inline ~console_ui(){}
};
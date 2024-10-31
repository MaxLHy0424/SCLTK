#pragma once
#include"def.hpp"
#define MOUSE_BUTTON_LEFT FROM_LEFT_1ST_BUTTON_PRESSED
#define MOUSE_BUTTON_MIDDLE FROM_LEFT_2ND_BUTTON_PRESSED
#define MOUSE_BUTTON_RIGHT RIGHTMOST_BUTTON_PRESSED
#define MOUSE_CLICK 0x0
#define MOUSE_CLICK_DOUBLE DOUBLE_CLICK
#define MOUSE_MOVE MOUSE_MOVED
#define MOUSE_WHEEL MOUSE_WHEELED
#define CONSOLE_WHITE 0x07
#define CONSOLE_BLUE 0x09
#define CONSOLE_RED 0x0c
class console_ui;
struct ui_data final{
    const DWORD button_state,ctrl_key_state,event_flag;
    console_ui *const ui;
    inline explicit ui_data():
        button_state{MOUSE_BUTTON_LEFT},
        ctrl_key_state{},
        event_flag{},
        ui{}
    {}
    inline explicit ui_data(const MOUSE_EVENT_RECORD _mouseEvent,console_ui *const _ui):
        button_state{_mouseEvent.dwButtonState},
        ctrl_key_state{_mouseEvent.dwControlKeyState},
        event_flag{_mouseEvent.dwEventFlags},
        ui{_ui}
    {}
    inline ~ui_data(){}
};
class console_ui final{
private:
    using callback=std::function<bool(ui_data)>;
    struct ui_item final{
        const char *text;
        short default_color,highlight_color,last_color;
        COORD position;
        callback function;
        inline explicit ui_item():
            text{},
            default_color{CONSOLE_WHITE},
            highlight_color{CONSOLE_BLUE},
            last_color{CONSOLE_WHITE},
            position{},
            function{}
        {}
        inline explicit ui_item(
            const char *const _text,
            const short _default_color,
            const short _highlight_color,
            const callback _function
        ):text{_text},
          default_color{_default_color},
          highlight_color{_highlight_color},
          last_color{CONSOLE_WHITE},
          position{},
          function{_function}
        {}
        inline ~ui_item(){}
        inline auto set_color(short _color){
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),_color);
            last_color=_color;
        }
        inline auto operator==(const COORD &_mouse_position)const{
            return (position.Y==_mouse_position.Y)&&
                   (position.X<=_mouse_position.X)&&
                   (_mouse_position.X<(position.X+(short)strlen(text)));
        }
        inline auto operator!=(const COORD &_mouse_position)const{
            return !operator==(_mouse_position);
        }
    };
    std::vector<ui_item> m_item;
    short m_wnd_height,m_wnd_width;
    enum m_ui_item_attrs_op{t_add='+',t_remove='-'};
    inline auto m_show_cursor(const bool _mode){
        CONSOLE_CURSOR_INFO cursor_info;
        GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
        cursor_info.bVisible=_mode;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
    }
    inline auto m_edit_attrs(const m_ui_item_attrs_op _mode){
        DWORD attrs;
        GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),&attrs);
        switch(_mode){
            case t_add:{
                attrs|=ENABLE_QUICK_EDIT_MODE,
                attrs|=ENABLE_INSERT_MODE,
                attrs|=ENABLE_MOUSE_INPUT;
                break;
            }case t_remove:{
                attrs&=~ENABLE_QUICK_EDIT_MODE,
                attrs&=~ENABLE_INSERT_MODE,
                attrs|=ENABLE_MOUSE_INPUT;
                break;
            }
        }
        SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),_mode);
    }
    inline auto m_get_cursor(){
        CONSOLE_SCREEN_BUFFER_INFO console_info;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&console_info);
        return console_info.dwCursorPosition;
    }
    inline auto m_set_cursor(const COORD &_position){
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),_position);
    }
    inline auto m_wait_mouse_event(const bool _move=true){
        INPUT_RECORD record;
        DWORD reg;
        while(true){
            Sleep(10);
            ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE),&record,1,&reg);
            if((record.EventType==MOUSE_EVENT)&&(_move|(record.Event.MouseEvent.dwEventFlags!=MOUSE_MOVED))){
                return record.Event.MouseEvent;
            }
        }
    }
    inline auto m_get_console_size(){
        CONSOLE_SCREEN_BUFFER_INFO console_info;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&console_info);
        m_wnd_height=console_info.dwSize.Y;
        m_wnd_width=console_info.dwSize.X;
    }
    inline auto m_cls(){
        m_get_console_size();
        m_set_cursor({0,0});
        printf("%s",std::string(m_wnd_width*m_wnd_height,' ').c_str());
        m_set_cursor({0,0});
    }
    inline auto m_write(const char *const _text,const bool _isEndl=false){
        printf("%s",_text);
        if(_isEndl){
            printf("\n");
        }
    }
    inline auto m_rewrite(const COORD &_position,const char *const &_text){
        m_set_cursor({0,_position.Y});
        m_write(std::string(_position.X,' ').c_str());
        m_set_cursor({0,_position.Y});
        m_write(_text);
        m_set_cursor({0,_position.Y});
    }
    inline auto m_init_pos(){
        m_cls();
        for(auto &line:m_item){
            line.position=m_get_cursor();
            line.set_color(line.default_color);
            m_write(line.text,true);
        }
    }
    inline auto m_refresh(const COORD &_hang_position){
        for(auto &line:m_item){
            if((line==_hang_position)&&(line.last_color!=line.highlight_color)){
                line.set_color(line.highlight_color);
                m_rewrite(line.position,line.text);
            }
            if((line!=_hang_position)&&(line.last_color!=line.default_color)){
                line.set_color(line.default_color);
                m_rewrite(line.position,line.text);
            }
        }
    }
    inline auto m_impl(const MOUSE_EVENT_RECORD &_mouseEvent){
        bool isExit{};
        for(auto &line:m_item){
            if(line==_mouseEvent.dwMousePosition){
                if(line.function!=nullptr){
                    m_cls();
                    line.set_color(line.default_color);
                    m_edit_attrs(t_add);
                    m_show_cursor(true);
                    isExit=line.function(ui_data{_mouseEvent,this});
                    m_edit_attrs(t_remove);
                    m_show_cursor(false);
                    m_init_pos();
                }
                break;
            }
        }
        return isExit;
    }
public:
    inline explicit console_ui():
        m_item{},
        m_wnd_height{},
        m_wnd_width{}
    {}
    inline explicit console_ui(const console_ui &_obj):
        m_item{_obj.m_item},
        m_wnd_height{},
        m_wnd_width{}
    {}
    inline explicit console_ui(const console_ui &&_obj)=delete;
    inline ~console_ui(){}
    inline auto size(){
        return m_item.size();
    }
    inline auto &add(
        const char *const _text,
        const callback _function=nullptr,
        const short _highlight_color=CONSOLE_BLUE,
        const short _default_color=CONSOLE_WHITE
    ){
        m_item.emplace_back(
            ui_item{
                _text,
                _default_color,
                (_function==nullptr)?(_default_color):(_highlight_color),
                _function
            }
        );
        return *this;
    }
    inline auto &insert(
        const size_t _index,
        const char *const _text,
        const callback _function=nullptr,
        const short _highlight_color=CONSOLE_BLUE,
        const short _default_color=CONSOLE_WHITE
    ){
        m_item.emplace(
            m_item.begin()+_index,
            ui_item{
                _text,
                _default_color,
                (_function==nullptr)?(_default_color):(_highlight_color),
                _function
            }
        );
        return *this;
    }
    inline auto &edit(
        const size_t _index,
        const char *const _text,
        const callback _function=nullptr,
        const short _highlight_color=CONSOLE_BLUE,
        const short _default_color=CONSOLE_WHITE
    ){
        m_item.at(_index)=ui_item{
            _text,
            _default_color,
            (_function==nullptr)?(_default_color):(_highlight_color),
            _function
        };
        return *this;
    }
    inline auto &remove(){
        m_item.pop_back();
        return *this;
    }
    inline auto &remove(const size_t _begin,const size_t _end){
        m_item.erase(m_item.begin()+_begin,m_item.begin()+_end);
        return *this;
    }
    inline auto &clear(){
        m_item.clear();
        return *this;
    }
    inline auto show(){
        m_edit_attrs(t_remove);
        m_show_cursor(false);
        MOUSE_EVENT_RECORD mouse_event;
        m_init_pos();
        bool is_exit{};
        while(!is_exit){
            mouse_event=m_wait_mouse_event();
            switch(mouse_event.dwEventFlags){
                case MOUSE_MOVE:{
                    m_refresh(mouse_event.dwMousePosition);
                    break;
                }case MOUSE_CLICK:{
                    if((mouse_event.dwButtonState)&&(mouse_event.dwButtonState!=MOUSE_WHEEL)){
                        is_exit=m_impl(mouse_event);
                    }
                    break;
                }
            }
            Sleep(10);
        }
        m_cls();
    }
};
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
    inline explicit ui_data(const MOUSE_EVENT_RECORD _mouse_event,console_ui *const _ui):
        button_state{_mouse_event.dwButtonState},
        ctrl_key_state{_mouse_event.dwControlKeyState},
        event_flag{_mouse_event.dwEventFlags},
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
    std::vector<ui_item> item_;
    short console_height,console_width;
    enum ui_item_attrs_op_{t_add='+',t_remove='-'};
    inline auto show_cursor_(const bool _mode){
        CONSOLE_CURSOR_INFO cursor;
        GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor);
        cursor.bVisible=_mode;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor);
    }
    inline auto edit_attrs_(const ui_item_attrs_op_ _mode){
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
            ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE),&record,1,&reg);
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
        console_height=console.dwSize.Y;
        console_width=console.dwSize.X;
    }
    inline auto cls_(){
        get_console_size_();
        set_cursor_({0,0});
        printf("%s",std::string(console_width*console_height,' ').c_str());
        set_cursor_({0,0});
    }
    inline auto write_(const char *const _text,const bool _isEndl=false){
        printf("%s",_text);
        if(_isEndl){
            printf("\n");
        }
    }
    inline auto rewrite_(const COORD &_position,const char *const &_text){
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
    inline auto run_fn_(const MOUSE_EVENT_RECORD &_mouse_event){
        bool isExit{};
        for(auto &line:item_){
            if(line==_mouse_event.dwMousePosition){
                if(line.function!=nullptr){
                    cls_();
                    line.set_color(line.default_color);
                    edit_attrs_(t_add);
                    show_cursor_(true);
                    isExit=line.function(ui_data{_mouse_event,this});
                    edit_attrs_(t_remove);
                    show_cursor_(false);
                    init_pos_();
                }
                break;
            }
        }
        return isExit;
    }
public:
    inline explicit console_ui():
        item_{},
        console_height{},
        console_width{}
    {}
    inline explicit console_ui(const console_ui &_obj):
        item_{_obj.item_},
        console_height{},
        console_width{}
    {}
    inline explicit console_ui(const console_ui &&_obj):
        item_{std::move(_obj.item_)},
        console_height{},
        console_width{}
    {}
    inline ~console_ui(){}
    inline auto size(){
        return item_.size();
    }
    inline auto &add(
        const char *const _text,
        const callback _function=nullptr,
        const short _highlight_color=CONSOLE_BLUE,
        const short _default_color=CONSOLE_WHITE
    ){
        item_.emplace_back(
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
        item_.emplace(
            item_.begin()+_index,
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
        item_.at(_index)=ui_item{
            _text,
            _default_color,
            (_function==nullptr)?(_default_color):(_highlight_color),
            _function
        };
        return *this;
    }
    inline auto &remove(){
        item_.pop_back();
        return *this;
    }
    inline auto &remove(const size_t _begin,const size_t _end){
        item_.erase(item_.begin()+_begin,item_.begin()+_end);
        return *this;
    }
    inline auto &clear(){
        item_.clear();
        return *this;
    }
    inline auto show(){
        edit_attrs_(t_remove);
        show_cursor_(false);
        MOUSE_EVENT_RECORD mouse_event;
        init_pos_();
        bool is_exit{};
        while(!is_exit){
            mouse_event=wait_mouse_event_();
            switch(mouse_event.dwEventFlags){
                case MOUSE_MOVE:{
                    refresh_(mouse_event.dwMousePosition);
                    break;
                }case MOUSE_CLICK:{
                    if((mouse_event.dwButtonState)&&(mouse_event.dwButtonState!=MOUSE_WHEEL)){
                        is_exit=run_fn_(mouse_event);
                    }
                    break;
                }
            }
            Sleep(10);
        }
        cls_();
    }
};
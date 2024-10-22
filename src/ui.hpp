#pragma once
#include"def.hpp"
#define MOUSE_BUTTON_LEFT FROM_LEFT_1ST_BUTTON_PRESSED
#define MOUSE_BUTTON_MIDDLE FROM_LEFT_2ND_BUTTON_PRESSED
#define MOUSE_BUTTON_RIGHT RIGHTMOST_BUTTON_PRESSED
#define MOUSE_CLICK 0x0
#define MOUSE_CLICK_DOUBLE DOUBLE_CLICK
#define MOUSE_MOVE MOUSE_MOVED
#define MOUSE_WHEEL MOUSE_WHEELED
#define COLOR_WHITE 0x07
#define COLOR_BLUE 0x09
#define COLOR_RED 0x0c
class UI;
struct Data final{
    const DWORD stateButton,stateCtrlKey,flagEvent;
    UI *const ui;
    inline explicit Data():
        stateButton{MOUSE_BUTTON_LEFT},stateCtrlKey{},flagEvent{},ui{}{}
    inline explicit Data(const MOUSE_EVENT_RECORD mouseEvent,UI *const ui):
        stateButton{mouseEvent.dwButtonState},stateCtrlKey{mouseEvent.dwControlKeyState},
        flagEvent{mouseEvent.dwEventFlags},ui{ui}{}
    inline ~Data(){}
};
class UI final{
private:
    using callback=std::function<bool(Data)>;
    struct Item final{
        const char *text;
        short colorDef,colorHighlight,colorLast;
        COORD position;
        callback function;
        inline explicit Item():
            text{},colorDef{COLOR_WHITE},colorHighlight{COLOR_BLUE},
            colorLast{COLOR_WHITE},position{},function{}{}
        inline explicit Item(
            const char *const text,
            const short colorDef,
            const short colorHighlight,
            const callback function
        ):text{text},colorDef{colorDef},colorHighlight{colorHighlight},
          colorLast{COLOR_WHITE},position{},function{function}{}
        inline ~Item(){}
        inline auto setColor(const char mod){
            switch(mod){
                case 'd':{
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),colorDef);
                    colorLast=colorDef;
                    break;
                }case 'h':{
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),colorHighlight);
                    colorLast=colorHighlight;
                    break;
                }
            }
        }
        inline auto operator==(const COORD &mousePosition)const{
            return (position.Y==mousePosition.Y)&&
                   (position.X<=mousePosition.X)&&
                   (mousePosition.X<(position.X+(short)strlen(text)));
        }
        inline auto operator!=(const COORD &mousePosition)const{
            return !operator==(mousePosition);
        }
    };
    short height,width;
    std::vector<Item> item;
    inline auto opCursor(const char mod){
        CONSOLE_CURSOR_INFO infoCursor;
        GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&infoCursor);
        switch(mod){
            case 'h':{
                infoCursor.bVisible=false;
                break;
            }case 's':{
                infoCursor.bVisible=true;
                break;
            }
        }
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&infoCursor);
    }
    inline auto opAttrs(const char mod){
        DWORD mode;
        GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),&mode);
        switch(mod){
            case '+':{
                mode|=ENABLE_QUICK_EDIT_MODE,mode|=ENABLE_INSERT_MODE,mode|=ENABLE_MOUSE_INPUT;
                break;
            }case '-':{
                mode&=~ENABLE_QUICK_EDIT_MODE,mode&=~ENABLE_INSERT_MODE,mode|=ENABLE_MOUSE_INPUT;
                break;
            }
        }
        SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),mode);
    }
    inline auto getCursor(){
        CONSOLE_SCREEN_BUFFER_INFO infoConsole;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&infoConsole);
        return infoConsole.dwCursorPosition;
    }
    inline auto setCursor(const COORD &position){
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),position);
    }
    inline auto waitMouseEvent(const bool move=true){
        INPUT_RECORD record;
        DWORD reg;
        while(true){
            Sleep(10);
            ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE),&record,1,&reg);
            if((record.EventType==MOUSE_EVENT)&&(move|(record.Event.MouseEvent.dwEventFlags!=MOUSE_MOVED))){
                return record.Event.MouseEvent;
            }
        }
    }
    inline auto getConsoleSize(){
        CONSOLE_SCREEN_BUFFER_INFO infoConsole;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&infoConsole);
        height=infoConsole.dwSize.Y;
        width=infoConsole.dwSize.X;
    }
    inline auto cls(){
        getConsoleSize();
        setCursor({0,0});
        printf("%s",std::string(width*height,' ').c_str());
        setCursor({0,0});
    }
    inline auto write(const char *const text,const bool isEndl=false){
        printf("%s",text);
        if(isEndl){
            printf("\n");
        }
    }
    inline auto rewrite(const COORD &position,const char *const &text){
        setCursor({0,position.Y});
        write(std::string(position.X,' ').c_str());
        setCursor({0,position.Y});
        write(text);
        setCursor({0,position.Y});
    }
    inline auto initPos(){
        cls();
        for(auto &line:item){
            line.position=getCursor();
            line.setColor('d');
            write(line.text,true);
        }
    }
    inline auto refresh(const COORD &hangPosition){
        for(auto &line:item){
            if((line==hangPosition)&&(line.colorLast!=line.colorHighlight)){
                line.setColor('h');
                rewrite(line.position,line.text);
            }
            if((line!=hangPosition)&&(line.colorLast!=line.colorDef)){
                line.setColor('d');
                rewrite(line.position,line.text);
            }
        }
    }
    inline auto impl(const MOUSE_EVENT_RECORD &mouseEvent){
        bool isExit{};
        for(auto &line:item){
            if(line==mouseEvent.dwMousePosition){
                if(line.function!=nullptr){
                    cls();
                    line.setColor('d');
                    opAttrs('+');
                    opCursor('s');
                    isExit=line.function(Data{mouseEvent,this});
                    opAttrs('-');
                    opCursor('h');
                    initPos();
                }
                break;
            }
        }
        return isExit;
    }
public:
    inline explicit UI():
        height{},width{}{}
    inline explicit UI(const UI &ui)=delete;
    inline explicit UI(const UI &&ui)=delete;
    inline ~UI(){}
    inline auto size(){
        return item.size();
    }
    inline auto &add(
        const char *const text,
        const callback function=nullptr,
        const short colorHighlight=COLOR_BLUE,
        const short colorDef=COLOR_WHITE
    ){
        item.emplace_back(Item{text,colorDef,(function==nullptr)?(colorDef):(colorHighlight),function});
        return *this;
    }
    inline auto &insert(
        const size_t index,
        const char *const text,
        const callback function=nullptr,
        const short colorHighlight=COLOR_BLUE,
        const short colorDef=COLOR_WHITE
    ){
        item.emplace(item.begin()+index,Item{text,colorDef,(function==nullptr)?(colorDef):(colorHighlight),function});
        return *this;
    }
    inline auto &edit(
        const size_t index,
        const char *const text,
        const callback function=nullptr,
        const short colorHighlight=COLOR_BLUE,
        const short colorDef=COLOR_WHITE
    ){
        item.at(index)=Item{text,colorDef,(function==nullptr)?(colorDef):(colorHighlight),function};
        return *this;
    }
    inline auto &remove(){
        item.pop_back();
        return *this;
    }
    inline auto &remove(const size_t begin,const size_t end){
        item.erase(item.begin()+begin,item.begin()+end);
        return *this;
    }
    inline auto &clear(){
        item.clear();
        return *this;
    }
    inline auto show(){
        opAttrs('-');
        opCursor('h');
        MOUSE_EVENT_RECORD mouseEvent;
        initPos();
        bool isExit{};
        while(!isExit){
            mouseEvent=waitMouseEvent();
            switch(mouseEvent.dwEventFlags){
                case MOUSE_MOVE:{
                    refresh(mouseEvent.dwMousePosition);
                    break;
                }case MOUSE_CLICK:{
                    if((mouseEvent.dwButtonState)&&(mouseEvent.dwButtonState!=MOUSE_WHEEL)){
                        isExit=impl(mouseEvent);
                    }
                    break;
                }
            }
            Sleep(10);
        }
        cls();
    }
};
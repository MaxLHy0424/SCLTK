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
class Ui;
struct Data final{
    const DWORD stateButton,stateCtrlKey,flagEvent;
    Ui *const ui;
    inline explicit Data():
        stateButton{MOUSE_BUTTON_LEFT},stateCtrlKey{},flagEvent{},ui{}
    {}
    inline explicit Data(const MOUSE_EVENT_RECORD mouseEvent,Ui *const ui):
        stateButton{mouseEvent.dwButtonState},stateCtrlKey{mouseEvent.dwControlKeyState},
        flagEvent{mouseEvent.dwEventFlags},ui{ui}
    {}
    inline ~Data(){}
};
class Ui final{
private:
    using Callback=std::function<bool(Data)>;
    struct Item final{
        const char *text;
        short colorDef,colorHighlight,colorLast;
        COORD position;
        Callback function;
        inline explicit Item():
            text{},colorDef{CONSOLE_WHITE},colorHighlight{CONSOLE_BLUE},
            colorLast{CONSOLE_WHITE},position{},function{}
        {}
        inline explicit Item(
            const char *const text,
            const short colorDef,
            const short colorHighlight,
            const Callback function
        ):text{text},colorDef{colorDef},colorHighlight{colorHighlight},
          colorLast{CONSOLE_WHITE},position{},function{function}
        {}
        inline ~Item(){}
        inline auto setColor(short color){
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color);
            colorLast=color;
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
    std::vector<Item> item;
    short height,width;
    enum AttrsOp{Add='+',Remove='-'};
    inline auto showCursor(const bool mod){
        CONSOLE_CURSOR_INFO infoCursor;
        GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&infoCursor);
        infoCursor.bVisible=mod;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&infoCursor);
    }
    inline auto editAttrs(const AttrsOp mod){
        DWORD mode;
        GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),&mode);
        switch(mod){
            case Add:{
                mode|=ENABLE_QUICK_EDIT_MODE,
                mode|=ENABLE_INSERT_MODE,
                mode|=ENABLE_MOUSE_INPUT;
                break;
            }case Remove:{
                mode&=~ENABLE_QUICK_EDIT_MODE,
                mode&=~ENABLE_INSERT_MODE,
                mode|=ENABLE_MOUSE_INPUT;
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
            line.setColor(line.colorDef);
            write(line.text,true);
        }
    }
    inline auto refresh(const COORD &hangPosition){
        for(auto &line:item){
            if((line==hangPosition)&&(line.colorLast!=line.colorHighlight)){
                line.setColor(line.colorHighlight);
                rewrite(line.position,line.text);
            }
            if((line!=hangPosition)&&(line.colorLast!=line.colorDef)){
                line.setColor(line.colorDef);
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
                    line.setColor(line.colorDef);
                    editAttrs(Add);
                    showCursor(true);
                    isExit=line.function(Data{mouseEvent,this});
                    editAttrs(Remove);
                    showCursor(false);
                    initPos();
                }
                break;
            }
        }
        return isExit;
    }
public:
    inline explicit Ui():
        item{},height{},width{}
    {}
    inline explicit Ui(const Ui &obj):
        item{obj.item},height{},width{}
    {}
    inline explicit Ui(const Ui &&obj)=delete;
    inline ~Ui(){}
    inline auto size(){
        return item.size();
    }
    inline auto &add(
        const char *const text,
        const Callback function=nullptr,
        const short colorHighlight=CONSOLE_BLUE,
        const short colorDef=CONSOLE_WHITE
    ){
        item.emplace_back(Item{text,colorDef,(function==nullptr)?(colorDef):(colorHighlight),function});
        return *this;
    }
    inline auto &insert(
        const size_t index,
        const char *const text,
        const Callback function=nullptr,
        const short colorHighlight=CONSOLE_BLUE,
        const short colorDef=CONSOLE_WHITE
    ){
        item.emplace(item.begin()+index,Item{text,colorDef,(function==nullptr)?(colorDef):(colorHighlight),function});
        return *this;
    }
    inline auto &edit(
        const size_t index,
        const char *const text,
        const Callback function=nullptr,
        const short colorHighlight=CONSOLE_BLUE,
        const short colorDef=CONSOLE_WHITE
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
        editAttrs(Remove);
        showCursor(false);
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
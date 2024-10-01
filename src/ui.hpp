#pragma once
#include"def.hpp"
#define MOUSE_BUTTON_LEFT FROM_LEFT_1ST_BUTTON_PRESSED
#define MOUSE_BUTTON_MIDDLE FROM_LEFT_2ND_BUTTON_PRESSED
#define MOUSE_BUTTON_RIGHT RIGHTMOST_BUTTON_PRESSED
#define MOUSE_CLICK 0x0
#define MOUSE_CLICK_DOUBLE DOUBLE_CLICK
#define MOUSE_MOVE MOUSE_MOVED
#define MOUSE_WHEEL MOUSE_WHEELED
#define WC_WHITE 0x07
#define WC_BLUE 0x09
#define WC_RED 0x0c
class UI;
struct Data final{
    const DWORD buttonState,ctrlKeyState,eventFlag;
    UI *const ui;
    std::any args;
    Data():
        buttonState{MOUSE_BUTTON_LEFT},ctrlKeyState{},eventFlag{},ui{},args{}{}
    Data(const MOUSE_EVENT_RECORD mouseEvent,UI *const ui,const std::any args):
        buttonState{mouseEvent.dwButtonState},ctrlKeyState{mouseEvent.dwControlKeyState},
        eventFlag{mouseEvent.dwEventFlags},ui{ui},args{args}{}
    ~Data(){}
};
class UI final{
private:
    using call=bool(*)(Data);
    struct Item final{
        const i8 *text;
        i16 colorDef,colorHighlight,colorLast;
        COORD pos;
        call fn;
        std::any args;
        Item():
            text{},colorDef{WC_WHITE},colorHighlight{WC_BLUE},
            colorLast{WC_WHITE},pos{},fn{},args{}{}
        Item(
            const i8 *const text,
            const i16 def,const i16 highlight,
            const call fn,const std::any args
        ):text{text},colorDef{def},colorHighlight{highlight},
          colorLast{WC_WHITE},pos{},fn{fn},args{args}{}
        ~Item(){}
        auto setColor(const i8 key){
            switch(key){
                case 'D':{
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),colorDef);
                    colorLast=colorDef;
                    break;
                }case 'H':{
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),colorHighlight);
                    colorLast=colorHighlight;
                    break;
                }
            }
        }
        auto operator==(const COORD &refMousePos)const{
            return (pos.Y==refMousePos.Y)&&(pos.X<=refMousePos.X)&&(refMousePos.X<(pos.X+(i16)strlen(text)));
        }
        auto operator!=(const COORD &refMousePos)const{
            return !operator==(refMousePos);
        }
    };
    i16 height,width;
    std::vector<Item> item;
    auto opCursor(const i8 key){
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursorInfo);
        switch(key){
            case 'H':{
                cursorInfo.bVisible=false;
                break;
            }case 'S':{
                cursorInfo.bVisible=true;
                break;
            }
        }
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursorInfo);
    }
    auto opAttrs(const i8 key){
        DWORD mode;
        GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),&mode);
        switch(key){
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
    auto getCursor(){
        CONSOLE_SCREEN_BUFFER_INFO tmp;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&tmp);
        return tmp.dwCursorPosition;
    }
    auto setCursor(const COORD &tmp={0,0}){
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),tmp);
    }
    auto waitMouseEvent(const bool move=true){
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
    auto getConsoleSize(){
        CONSOLE_SCREEN_BUFFER_INFO tmp;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&tmp);
        height=tmp.dwSize.Y;
        width=tmp.dwSize.X;
    }
    auto cls(){
        getConsoleSize();
        setCursor({0,0});
        printf("%s",std::string(width*height,' ').c_str());
        setCursor({0,0});
    }
    auto write(const i8 *const text,const bool isEndl=false){
        printf("%s",text);
        if(isEndl){
            printf("\n");
        }
    }
    auto rewrite(const COORD &refPos,const i8 *const &refText){
        setCursor({0,refPos.Y});
        for(i16 j{};j<refPos.X;++j){
            write(" ");
        }
        setCursor({0,refPos.Y});
        write(refText);
        setCursor({0,refPos.Y});
    }
    auto initPos(){
        cls();
        for(auto &ref:item){
            ref.pos=getCursor();
            ref.setColor('D');
            write(ref.text,true);
        }
    }
    auto refresh(const COORD &hangPos){
        for(auto &ref:item){
            if((ref==hangPos)&&(ref.colorLast!=ref.colorHighlight)){
                ref.setColor('H');
                rewrite(ref.pos,ref.text);
            }
            if((ref!=hangPos)&&(ref.colorLast!=ref.colorDef)){
                ref.setColor('D');
                rewrite(ref.pos,ref.text);
            }
        }
    }
    auto impl(const MOUSE_EVENT_RECORD &mouseEvent){
        bool isExit{};
        for(auto &ref:item){
            if(ref==mouseEvent.dwMousePosition){
                if(ref.fn!=nullptr){
                    cls();
                    ref.setColor('D');
                    opAttrs('+');
                    opCursor('S');
                    isExit=ref.fn(Data(mouseEvent,this,ref.args));
                    opAttrs('-');
                    opCursor('H');
                    initPos();
                }
                break;
            }
        }
        return isExit;
    }
public:
    UI():
        height{},width{}{}
    ~UI(){}
    auto size(){
        return item.size();
    }
    auto &add(
        const i8 *const text,
        const call fn=nullptr,const std::any args={},
        const i16 colorHighlight=WC_BLUE,const i16 colorDef=WC_WHITE
    ){
        item.emplace_back(Item(text,colorDef,(fn==nullptr)?(colorDef):(colorHighlight),fn,args));
        return *this;
    }
    auto &insert(
        const i32 index,const i8 *const text,
        const call fn=nullptr,const std::any args={},
        const i16 colorHighlight=WC_BLUE,const i16 colorDef=WC_WHITE
    ){
        item.emplace(item.begin()+index,Item(text,colorDef,(fn==nullptr)?(colorDef):(colorHighlight),fn,args));
        return *this;
    }
    auto &edit(
        const i32 index,const i8 *const text,
        const call fn=nullptr,const std::any args={},
        const i16 colorHighlight=WC_BLUE,const i16 colorDef=WC_WHITE
    ){
        item[index]=Item(text,colorDef,(fn==nullptr)?(colorDef):(colorHighlight),fn,args);
        return *this;
    }
    auto &remove(){
        item.pop_back();
        return *this;
    }
    auto &remove(const i32 begin,const i32 end){
        item.erase(item.begin()+begin,item.begin()+end);
        return *this;
    }
    auto &clear(){
        item.clear();
        return *this;
    }
    auto show(){
        opAttrs('-');
        opCursor('H');
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
            Sleep(50);
        }
        cls();
    }
};
#undef MOUSE_BUTTON_LEFT
#undef MOUSE_BUTTON_MIDDLE
#undef MOUSE_BUTTON_RIGHT
#undef MOUSE_CLICK
#undef MOUSE_CLICK_DOUBLE
#undef MOUSE_MOVE
#undef MOUSE_WHEEL
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
#define WC_GREEN 0x0a
#define WC_RED 0x0c
class UI;
struct Data final{
    const DWORD buttonState,ctrlKeyState,eventFlag;
    UI *const ui;
    explicit Data():
        buttonState{MOUSE_BUTTON_LEFT},ctrlKeyState{},eventFlag{},ui{}{}
    explicit Data(MOUSE_EVENT_RECORD mouseEvent,UI *ui):
        buttonState{mouseEvent.dwButtonState},ctrlKeyState{mouseEvent.dwControlKeyState},
        eventFlag{mouseEvent.dwEventFlags},ui{ui}{}
    ~Data(){}
};
class UI final{
private:
    using callback=std::function<bool(Data)>;
    struct Item final{
        const char *text;
        short colorDef,colorHighlight,colorLast;
        COORD pos;
        callback fn;
        explicit Item():
            text{},colorDef{WC_WHITE},colorHighlight{WC_BLUE},
            colorLast{WC_WHITE},pos{},fn{}{}
        explicit Item(const char *text,short def,short highlight,callback fn):
            text{text},colorDef{def},colorHighlight{highlight},
            colorLast{WC_WHITE},pos{},fn{fn}{}
        ~Item(){}
        auto setColor(char mod){
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
        auto operator==(const COORD &refMousePos)const{
            return (pos.Y==refMousePos.Y)&&(pos.X<=refMousePos.X)&&(refMousePos.X<(pos.X+(short)strlen(text)));
        }
        auto operator!=(const COORD &refMousePos)const{
            return !operator==(refMousePos);
        }
    };
    short height,width;
    std::vector<Item> item;
    auto opCursor(char mod){
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursorInfo);
        switch(mod){
            case 'h':{
                cursorInfo.bVisible=false;
                break;
            }case 's':{
                cursorInfo.bVisible=true;
                break;
            }
        }
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursorInfo);
    }
    auto opAttrs(char mod){
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
    auto getCursor(){
        CONSOLE_SCREEN_BUFFER_INFO tmp;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&tmp);
        return tmp.dwCursorPosition;
    }
    auto setCursor(const COORD &tmp={0,0}){
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),tmp);
    }
    auto waitMouseEvent(bool move=true){
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
    auto write(const char *text,bool isEndl=false){
        printf("%s",text);
        if(isEndl){
            printf("\n");
        }
    }
    auto rewrite(COORD &refPos,const char *&refText){
        setCursor({0,refPos.Y});
        for(short j{};j<refPos.X;++j){
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
            ref.setColor('d');
            write(ref.text,true);
        }
    }
    auto refresh(COORD &hangPos){
        for(auto &ref:item){
            if((ref==hangPos)&&(ref.colorLast!=ref.colorHighlight)){
                ref.setColor('h');
                rewrite(ref.pos,ref.text);
            }
            if((ref!=hangPos)&&(ref.colorLast!=ref.colorDef)){
                ref.setColor('d');
                rewrite(ref.pos,ref.text);
            }
        }
    }
    auto impl(MOUSE_EVENT_RECORD &mouseEvent){
        bool isExit{};
        for(auto &ref:item){
            if(ref==mouseEvent.dwMousePosition){
                if(ref.fn!=nullptr){
                    cls();
                    ref.setColor('d');
                    opAttrs('+');
                    opCursor('s');
                    isExit=ref.fn(Data(mouseEvent,this));
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
    explicit UI():
        height{},width{}{}
    ~UI(){}
    auto size(){
        return item.size();
    }
    auto &add(
        const char *text,callback fn=nullptr,
        short colorHighlight=WC_BLUE,short colorDef=WC_WHITE
    ){
        item.emplace_back(Item(text,colorDef,(fn==nullptr)?(colorDef):(colorHighlight),fn));
        return *this;
    }
    auto &insert(
        size_t index,const char *text,callback fn=nullptr,
        short colorHighlight=WC_BLUE,short colorDef=WC_WHITE
    ){
        item.emplace(item.begin()+index,Item(text,colorDef,(fn==nullptr)?(colorDef):(colorHighlight),fn));
        return *this;
    }
    auto &edit(
        size_t index,const char *text,callback fn=nullptr,
        short colorHighlight=WC_BLUE,short colorDef=WC_WHITE
    ){
        item.at(index)=Item(text,colorDef,(fn==nullptr)?(colorDef):(colorHighlight),fn);
        return *this;
    }
    auto &remove(){
        item.pop_back();
        return *this;
    }
    auto &remove(size_t begin,size_t end){
        item.erase(item.begin()+begin,item.begin()+end);
        return *this;
    }
    auto &clear(){
        item.clear();
        return *this;
    }
    auto show(){
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
#undef MOUSE_BUTTON_LEFT
#undef MOUSE_BUTTON_MIDDLE
#undef MOUSE_BUTTON_RIGHT
#undef MOUSE_CLICK
#undef MOUSE_CLICK_DOUBLE
#undef MOUSE_MOVE
#undef MOUSE_WHEEL
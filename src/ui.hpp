#pragma once
#include"def.hpp"
#define MOUSE_BUTTON_LEFT FROM_LEFT_1ST_BUTTON_PRESSED
#define MOUSE_BUTTON_MIDDLE FROM_LEFT_2ND_BUTTON_PRESSED
#define MOUSE_BUTTON_RIGHT RIGHTMOST_BUTTON_PRESSED
#define MOUSE_CLICK 0x0
#define MOUSE_CLICK_DOUBLE DOUBLE_CLICK
#define MOUSE_MOVE MOUSE_MOVED
#define MOUSE_WHEEL MOUSE_WHEELED
#define CON_WHITE 0x07
#define CON_BLUE 0x09
#define CON_RED_PALE 0x0c
class CUI;
struct Data final{
    DWORD buttonState,ctrlKeyState,eventFlag;
    CUI *ui;
    void *args;
    Data():
        buttonState{MOUSE_BUTTON_LEFT},ctrlKeyState{},eventFlag{},ui{},args{}{}
    Data(MOUSE_EVENT_RECORD mouseEvent,CUI *ui,void *args):
        buttonState{mouseEvent.dwButtonState},ctrlKeyState{mouseEvent.dwControlKeyState},
        eventFlag{mouseEvent.dwEventFlags},ui{ui},args{args}{}
};
class CUI final{
    private:
        using call=bool(*)(Data);
        struct Item final{
            const i8 *text;
            i16 colorDef,colorHighlight,colorLast;
            COORD pos;
            call fn;
            void *args;
            Item():
                text{},colorDef{CON_WHITE},colorHighlight{CON_BLUE},
                colorLast{CON_WHITE},pos{},fn{},args{}{}
            Item(const i8 *text,i16 def,i16 highlight,call fn,void *args):
                text{text},colorDef{def},colorHighlight{highlight},
                colorLast{CON_WHITE},pos{},fn{fn},args{args}{}
            auto setColor(i8 f){
                switch(f){
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
            auto operator==(const COORD &mousePos)const{
                return (pos.Y==mousePos.Y)&&(pos.X<=mousePos.X)&&(mousePos.X<(pos.X+(i16)strlen(text)));
            }
            auto operator!=(const COORD &mousePos)const{
                return !operator==(mousePos);
            }
        };
        i16 height,width;
        std::vector<Item> items;
        auto opCursor(char f){
            CONSOLE_CURSOR_INFO cursorInfo;
            GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursorInfo);
            switch(f){
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
        auto opAttrs(char f){
            DWORD mode;
            GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),&mode);
            switch(f){
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
            for(i16 i{};i<height;++i){
                for(i16 j{};j<width;++j){
                    printf(" ");
                }
            }
            setCursor({0,0});
        }
        auto write(const i8 *text,bool isEndl=false){
            printf("%s",text);
            if(isEndl){
                printf("\n");
            }
        }
        auto rewrite(Item &ref){
            setCursor({0,ref.pos.Y});
            for(i16 j{};j<ref.pos.X;++j){
                write(" ");
            }
            setCursor({0,ref.pos.Y});
            write(ref.text);
            setCursor({0,ref.pos.Y});
        }
        auto initPos(){
            cls();
            for(auto &ref:items){
                ref.pos=getCursor();
                ref.setColor('D');
                write(ref.text,true);
            }
        }
        auto refresh(COORD &hangPos){
            for(auto &ref:items){
                if((ref==hangPos)&&(ref.colorLast!=ref.colorHighlight)){
                    ref.setColor('H');
                    rewrite(ref);
                }
                if((ref!=hangPos)&&(ref.colorLast!=ref.colorDef)){
                    ref.setColor('D');
                    rewrite(ref);
                }
            }
        }
        auto impl(MOUSE_EVENT_RECORD &mouseEvent){
            bool isExit{};
            for(auto &ref:items){
                if(ref==mouseEvent.dwMousePosition){
                    if(ref.fn!=nullptr){
                        cls();
                        ref.setColor('D');
                        opAttrs('+');
                        opCursor('S');
                        isExit=ref.fn(Data{mouseEvent,this,ref.args});
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
        CUI():
            height{},width{}{}
        ~CUI(){}
        auto add(const i8 *text,call fn=nullptr,void *args=nullptr,i16 colorHighlight=CON_BLUE,i16 colorDef=CON_WHITE)->CUI&{
            items.push_back(Item{text,colorDef,(fn==nullptr)?(colorDef):(colorHighlight),fn,args});
            return *this;
        }
        auto rm()->CUI&{
            items.pop_back();
            return *this;
        }
        auto clear()->CUI&{
            items.clear();
            return *this;
        }
        auto show(){
            opAttrs('-');
            opCursor('H');
            MOUSE_EVENT_RECORD mouseEvent;
            Sleep(100);
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
            Sleep(100);
        }
};
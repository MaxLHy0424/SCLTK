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
class UI;
struct Data final{
    DWORD buttonState,ctrlKeyState,eventFlag;
    UI *ui;
    void *args;
    Data():
        buttonState{MOUSE_BUTTON_LEFT},ctrlKeyState{},eventFlag{},ui{},args{}{}
    Data(MOUSE_EVENT_RECORD mouseEvent,UI *ui,void *args):
        buttonState{mouseEvent.dwButtonState},ctrlKeyState{mouseEvent.dwControlKeyState},
        eventFlag{mouseEvent.dwEventFlags},ui{ui},args{args}{}
};
class UI final{
    private:
        using call=bool(*)(Data);
        struct Item final{
            const i8 *text;
            i16 colorDef,colorHighlight,colorLast;
            COORD position;
            call function;
            void *args;
            Item():
                text{},colorDef{CON_WHITE},colorHighlight{CON_BLUE},
                colorLast{CON_WHITE},position{},function{},args{}{}
            Item(const i8 *text,i16 def,i16 highlight,call function,void *args):
                text{text},colorDef{def},colorHighlight{highlight},
                colorLast{CON_WHITE},position{},function{function},args{args}{}
            auto setColor(i8 k){
                switch(k){
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
            auto operator==(const COORD &mousePos)const{
                return (position.Y==mousePos.Y)&&(position.X<=mousePos.X)&&(mousePos.X<(position.X+(i16)strlen(text)));
            }
            auto operator!=(const COORD &mousePos)const{
                return !operator==(mousePos);
            }
        };
        i16 height,width;
        std::vector<Item> item;
        auto opCursor(char k){
            CONSOLE_CURSOR_INFO cursorInfo;
            GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursorInfo);
            switch(k){
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
        auto opAttrs(char k){
            DWORD mode;
            GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),&mode);
            switch(k){
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
        auto write(const i8 *text,bool isEndl=false){
            printf("%s",text);
            if(isEndl){
                printf("\n");
            }
        }
        auto rewrite(Item &ref){
            setCursor({0,ref.position.Y});
            for(i16 j{};j<ref.position.X;++j){
                write(" ");
            }
            setCursor({0,ref.position.Y});
            write(ref.text);
            setCursor({0,ref.position.Y});
        }
        auto initPos(){
            cls();
            for(auto &ref:item){
                ref.position=getCursor();
                ref.setColor('d');
                write(ref.text,true);
            }
        }
        auto refresh(COORD &hangPos){
            for(auto &ref:item){
                if((ref==hangPos)&&(ref.colorLast!=ref.colorHighlight)){
                    ref.setColor('h');
                    rewrite(ref);
                }
                if((ref!=hangPos)&&(ref.colorLast!=ref.colorDef)){
                    ref.setColor('d');
                    rewrite(ref);
                }
            }
        }
        auto impl(MOUSE_EVENT_RECORD &mouseEvent){
            bool isExit{};
            for(auto &ref:item){
                if(ref==mouseEvent.dwMousePosition){
                    if(ref.function!=nullptr){
                        cls();
                        ref.setColor('d');
                        opAttrs('+');
                        opCursor('s');
                        isExit=ref.function(Data(mouseEvent,this,ref.args));
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
        UI():
            height{},width{}{}
        ~UI(){}
        auto size(){
            return item.size();
        }
        auto &add(const i8 *text,call function=nullptr,void *args=nullptr,i16 colorHighlight=CON_BLUE,i16 colorDef=CON_WHITE){
            item.emplace_back(Item(text,colorDef,(function==nullptr)?(colorDef):(colorHighlight),function,args));
            return *this;
        }
        auto &insert(int index,const i8 *text,call function=nullptr,void *args=nullptr,i16 colorHighlight=CON_BLUE,i16 colorDef=CON_WHITE){
            item.emplace(item.begin()+index,Item(text,colorDef,(function==nullptr)?(colorDef):(colorHighlight),function,args));
            return *this;
        }
        auto &edit(int index,const i8 *text,call function=nullptr,void *args=nullptr,i16 colorHighlight=CON_BLUE,i16 colorDef=CON_WHITE){
            item[index]=Item(text,colorDef,(function==nullptr)?(colorDef):(colorHighlight),function,args);
            return *this;
        }
        auto &remove(){
            item.pop_back();
            return *this;
        }
        auto &remove(int begin,int end){
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
                Sleep(50);
            }
            cls();
        }
};
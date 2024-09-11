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
#define CON_BLUE 0x03
#define CON_RED_PALE 0x0c
class CUI;
struct Data{
    DWORD buttonState,ctrlKeyState,eventFlag;
    CUI *ui;
    void *argv;
    Data():
        buttonState{MOUSE_BUTTON_LEFT},ctrlKeyState{},eventFlag{},ui{nullptr},argv{nullptr}{}
    Data(MOUSE_EVENT_RECORD mouseEvent,CUI *ui,void *argv):
        buttonState{mouseEvent.dwButtonState},ctrlKeyState{mouseEvent.dwControlKeyState},
        eventFlag{mouseEvent.dwEventFlags},ui{ui},argv{argv}{}
    ~Data(){}
};
using callback=bool(*)(Data);
class CUI final{
    private:
        struct Item{
            const i8 *text;
            i16 colorDef,colorHighlight,colorLast;
            COORD pos;
            callback fn;
            void *argv;
            Item():
                text{},colorDef{CON_WHITE},colorHighlight{CON_BLUE},colorLast{CON_WHITE},pos{},fn{nullptr}{}
            Item(const i8 *text,i16 def,i16 highlight,callback fn,void *argv):
                text{text},colorDef{def},colorHighlight{highlight},colorLast{CON_WHITE},pos{},fn{fn},argv{argv}{}
            ~Item(){}
            auto setColor(i8 m){
                switch(m){
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
        std::vector<Item> lineData;
        auto opCursor(char m){
            CONSOLE_CURSOR_INFO cursorInfo;
            GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursorInfo);
            switch(m){
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
        auto opAttrs(char m){
            DWORD mode;
            GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),&mode);
            switch(m){
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
        auto getCursor()->COORD{
            CONSOLE_SCREEN_BUFFER_INFO tmp;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&tmp);
            return tmp.dwCursorPosition;
        }
        auto setCursor(const COORD &tmp={0,0}){
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),tmp);
        }
        auto waitMouseEvent(bool move=true)->MOUSE_EVENT_RECORD{
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
        auto rewrite(Item &data){
            setCursor({0,data.pos.Y});
            for(i16 j{};j<data.pos.X;++j){
                write(" ");
            }
            setCursor({0,data.pos.Y});
            write(data.text);
            setCursor({0,data.pos.Y});
        }
        auto initPos(){
            cls();
            for(auto &data:lineData){
                data.pos=getCursor();
                data.setColor('D');
                write(data.text,true);
            }
        }
        auto refresh(COORD &hangPos){
            for(auto &data:lineData){
                if((data==hangPos)&&(data.colorLast!=data.colorHighlight)){
                    data.setColor('H');
                    rewrite(data);
                }
                if((data!=hangPos)&&(data.colorLast!=data.colorDef)){
                    data.setColor('D');
                    rewrite(data);
                }
            }
        }
        auto impl(MOUSE_EVENT_RECORD &mouseEvent){
            bool isExit{};
            for(auto &data:lineData){
                if(data==mouseEvent.dwMousePosition){
                    if(data.fn!=nullptr){
                        system("cls");
                        cls();
                        data.setColor('D');
                        opAttrs('+');
                        opCursor('S');
                        isExit=data.fn(Data{mouseEvent,this,data.argv});
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
        auto push(const i8 *text,callback fn=nullptr,void *argv=nullptr,i16 colorHighlight=CON_BLUE,i16 colorDef=CON_WHITE)->CUI&{
            lineData.push_back(Item{text,colorDef,(fn==nullptr)?(colorDef):(colorHighlight),fn,argv});
            return *this;
        }
        auto pop()->CUI&{
            lineData.pop_back();
            return *this;
        }
        auto clear()->CUI&{
            lineData.clear();
            return *this;
        }
        auto show(){
            opAttrs('-');
            opCursor('H');
            MOUSE_EVENT_RECORD mouseEvent;
            Sleep(50);
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
            Sleep(50);
        }
};
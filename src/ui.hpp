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
struct Color{
    i16 def,highlight,lastColor;
    Color():
        def{CON_WHITE},highlight{CON_BLUE},lastColor{CON_WHITE}{}
    Color(i16 def=CON_WHITE,i16 highlight=CON_BLUE):
        def{def},highlight{highlight},lastColor{CON_WHITE}{}
    ~Color(){}
    auto setDefault(){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),def);
        lastColor=def;
    }
    auto setHighlight(){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),highlight);
        lastColor=highlight;
    }
};
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
struct Text{
    const i8 *text;
    Color color;
    COORD pos;
    callback fn;
    void *argv;
    Text():
        text{},color{Color{0,0}},pos{},fn{nullptr}{}
    Text(const i8 *text,Color color,callback fn,void *argv):
        text{text},color{color},pos{},fn{fn},argv{argv}{}
    ~Text(){}
    auto operator==(const COORD &mousePosition)const{
        return (pos.Y==mousePosition.Y)&&(pos.X<=mousePosition.X)&&(mousePosition.X<(pos.X+(i16)strlen(text)));
    }
    auto operator!=(const COORD &mousePosition)const{
        return !(operator==(mousePosition));
    }
};
class CUI{
    private:
        i16 height,width;
        std::vector<Text> lineData;
    protected:
        auto hideCursor(){
            CONSOLE_CURSOR_INFO cursorInfo;
            GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursorInfo);
            cursorInfo.bVisible=false;
            SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursorInfo);
        }
        auto showCursor(){
            CONSOLE_CURSOR_INFO cursorInfo;
            GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursorInfo);
            cursorInfo.bVisible=true;
            SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursorInfo);
        }
        auto removeAttrs(){
            DWORD mode;
            GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),&mode);
            mode&=~ENABLE_QUICK_EDIT_MODE,mode&=~ENABLE_INSERT_MODE,mode|=ENABLE_MOUSE_INPUT;
            SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),mode);
        }
        auto addAttrs(){
            DWORD mode;
            GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),&mode);
            mode|=ENABLE_QUICK_EDIT_MODE,mode|=ENABLE_INSERT_MODE,mode|=ENABLE_MOUSE_INPUT;
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
        auto rewrite(Text &data){
            setCursor({0,data.pos.Y});
            for(i16 j{};j<data.pos.X;++j){
                write(" ");
            }
            setCursor({0,data.pos.Y});
            write(data.text);
            setCursor({0,data.pos.Y});
        }
        auto initPosition(){
            cls();
            for(auto &data:lineData){
                data.pos=getCursor();
                data.color.setDefault();
                write(data.text,true);
            }
        }
        auto refresh(COORD &hangPosition){
            for(auto &data:lineData){
                if((data==hangPosition)&&(data.color.lastColor!=data.color.highlight)){
                    data.color.setHighlight();
                    rewrite(data);
                }
                if((data!=hangPosition)&&(data.color.lastColor!=data.color.def)){
                    data.color.setDefault();
                    rewrite(data);
                }
            }
        }
        auto impl(MOUSE_EVENT_RECORD &mouseEvent){
            bool isExit{};
            for(auto &data:lineData){
                if(data==mouseEvent.dwMousePosition){
                    if(data.fn!=nullptr){
                        cls();
                        data.color.setDefault();
                        addAttrs();
                        showCursor();
                        isExit=data.fn(Data{mouseEvent,this,data.argv});
                        removeAttrs();
                        hideCursor();
                        initPosition();
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
            lineData.push_back(Text{text,Color{colorDef,(fn==nullptr)?(colorDef):(colorHighlight)},fn,argv});
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
            removeAttrs();
            hideCursor();
            MOUSE_EVENT_RECORD mouseEvent;
            Sleep(50);
            initPosition();
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
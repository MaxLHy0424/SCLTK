#ifndef MENU_HPP
#define MENU_HP
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
auto removeAttributes(){
    DWORD mode;
    GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),&mode);
    mode&=~ENABLE_QUICK_EDIT_MODE,mode&=~ENABLE_INSERT_MODE,mode|=ENABLE_MOUSE_INPUT;
    SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),mode);
}
auto addAttributes(){
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
auto setCursor(const COORD& tmp={0,0}){
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),tmp);
}
auto waitMouseEvent(bool move=true)->MOUSE_EVENT_RECORD{
    INPUT_RECORD record;
    DWORD reg;
    while(true){
        Sleep(10ul);
        ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE),&record,1ul,&reg);
        if(!(record.EventType^MOUSE_EVENT)&&(move|(record.Event.MouseEvent.dwEventFlags^MOUSE_MOVED))){
            return record.Event.MouseEvent;
        }
    }
}
#define BLACK_WHITE 0x07
#define BLACK_BLUE 0x03
struct Color{
    short def,highlight,lastColor;
    Color():def(BLACK_WHITE),highlight(BLACK_BLUE),lastColor(BLACK_WHITE){}
    Color(short def=BLACK_WHITE,short highlight=BLACK_BLUE):def(def),highlight(highlight),lastColor(BLACK_WHITE){}
    auto setDefault(){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),def);
        lastColor=def;
    }
    auto setHighlight(){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),highlight);
        lastColor=highlight;
    }
};
class CUI;
#define MOUSE_LEFT_BUTTON FROM_LEFT_1ST_BUTTON_PRESSED
#define MOUSE_MIDDLE_BUTTON FROM_LEFT_2ND_BUTTON_PRESSED
#define MOUSE_RIGHT_BUTTON RIGHTMOST_BUTTON_PRESSED
#define MOUSE_CLICK 0x0
#define MOUSE_MOVE MOUSE_MOVED
#define MOUSE_DOUBLE_CLICK DOUBLE_CLICK
#define MOUSE_WHEEL MOUSE_WHEELED
struct Parameter{
    DWORD buttonState,ctrlKeyState,eventFlag;
    CUI* ui;
    Parameter():buttonState(MOUSE_LEFT_BUTTON),ctrlKeyState(0ul),eventFlag(0ul),ui(nullptr){}
    Parameter(MOUSE_EVENT_RECORD mouseEvent,CUI* ui):buttonState(mouseEvent.dwButtonState),ctrlKeyState(mouseEvent.dwControlKeyState),eventFlag(mouseEvent.dwEventFlags),ui(ui){}
};
typedef bool(*callback)(Parameter);
struct Text{
    std::string text;
    Color color;
    COORD position;
    callback function;
    Text():text(""),color(Color(0,0)),position({0,0}),function(nullptr){}
    Text(std::string text,Color color,callback function):text(text),color(color),position({0,0}),function(function){}
    bool operator==(const COORD& mousePosition)const{
        return (!(position.Y^mousePosition.Y)&&(position.X<=mousePosition.X)&&(mousePosition.X<(position.X+(short)text.size())))?(true):(false);
    }
    bool operator!=(const COORD& mousePosition)const{
        return !(operator==(mousePosition));
    }
};
class CUI{
    private:
        DWORD sleepTime;
        short height,width;
        std::vector<Text>lineData;
    protected:
        auto getConsoleSize(){
            CONSOLE_SCREEN_BUFFER_INFO tmp;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&tmp);
            height=tmp.dwSize.Y;
            width=tmp.dwSize.X;
        }
        auto clearScreen(){
            getConsoleSize();
            setCursor({0,0});
            for(short i{};i<height;++i){
                for(short j{};j<width;++j){
                    printf(" ");
                }
            }
            setCursor({0,0});
        }
        auto write(std::string text,bool isEndl=false){
            printf("%s",text.c_str());
            if(isEndl){
                printf("\n");
            }
        }
        auto rewrite(Text data){
            setCursor({0,data.position.Y});
            for(short j{};j<data.position.X;++j){
                write(" ");
            }
            setCursor({0,data.position.Y});
            write(data.text);
            setCursor({0,data.position.Y});
        }
        auto initPosition(){
            clearScreen();
            for(auto& data:lineData){
                data.position=getCursor();
                data.color.setDefault();
                write(data.text,true);
            }
        }
        auto refresh(COORD hangPosition){
            for(auto& data:lineData){
                if((data==hangPosition)&&(data.color.lastColor^data.color.highlight)){
                    data.color.setHighlight();
                    rewrite(data);
                }
                if((data!=hangPosition)&&(data.color.lastColor^data.color.def)){
                    data.color.setDefault();
                    rewrite(data);
                }
            }
        }
        auto implement(MOUSE_EVENT_RECORD mouseEvent){
            bool isExit{};
            for(auto& data:lineData){
                if(data==mouseEvent.dwMousePosition){
                    if(data.function!=nullptr){
                        clearScreen();
                        data.color.setDefault();
                        addAttributes();
                        showCursor();
                        isExit=data.function(Parameter(mouseEvent,this));
                        removeAttributes();
                        hideCursor();
                        initPosition();
                    }
                    break;
                }
            }
            return isExit;
        }
    public:
        CUI():sleepTime(50ul),height(0),width(0){}
        ~CUI(){}
        auto push(std::string text,callback function=nullptr,short colorHighlight=BLACK_BLUE,short colorDef=BLACK_WHITE)->CUI&{
            lineData.push_back(Text(text,Color(colorDef,((function==nullptr)?(colorDef):(colorHighlight))),function));
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
            removeAttributes();
            hideCursor();
            MOUSE_EVENT_RECORD mouseEvent;
            Sleep(100ul);
            initPosition();
            bool isExit{};
            while(!isExit){
                Sleep(sleepTime);
                mouseEvent=waitMouseEvent();
                switch(mouseEvent.dwEventFlags){
                    case MOUSE_MOVE:{
                        refresh(mouseEvent.dwMousePosition);
                        break;
                    }case MOUSE_CLICK:{
                        if((mouseEvent.dwButtonState)&&(mouseEvent.dwButtonState^MOUSE_WHEEL)){
                            isExit=implement(mouseEvent);
                        }
                        break;
                    }
                }
            }
            clearScreen();
            Sleep(100ul);
        }
};
auto _exit(Parameter){
    return true;
}
#define EXIT _exit
#endif
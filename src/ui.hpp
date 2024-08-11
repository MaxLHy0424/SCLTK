#ifndef MENU_HPP
#define MENU_HP
auto HideCursor(){
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursorInfo);
    cursorInfo.bVisible=false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursorInfo);
}
auto ShowCursor(){
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursorInfo);
    cursorInfo.bVisible=true;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursorInfo);
}
auto RemoveAttributes(){
    DWORD mode;
    GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),&mode);
    mode&=~ENABLE_QUICK_EDIT_MODE,mode&=~ENABLE_INSERT_MODE,mode|=ENABLE_MOUSE_INPUT;
    SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),mode);
}
auto AddAttributes(){
    DWORD mode;
    GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),&mode);
    mode|=ENABLE_QUICK_EDIT_MODE,mode|=ENABLE_INSERT_MODE,mode|=ENABLE_MOUSE_INPUT;
    SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),mode);
}
auto GetCursorPos()->COORD{
    CONSOLE_SCREEN_BUFFER_INFO t;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&t);
    return t.dwCursorPosition;
}
auto SetCursor(const COORD& t={0,0}){
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),t);
}
auto WaitMouseEvent(bool move=true)->MOUSE_EVENT_RECORD{
    INPUT_RECORD record;
    DWORD reg;
    while(true){
        Sleep(10ul);
        ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE),&record,1,&reg);
        if(!(record.EventType^MOUSE_EVENT)&&(move|(record.Event.MouseEvent.dwEventFlags^MOUSE_MOVED))){
            return record.Event.MouseEvent;
        }
    }
}
#define BLACK_WHITE 0x07
#define BLACK_BLUE 0x03
#define YELLOW_BLUE 0xe9
struct Color{
    short def,highlight,lastColor;
    Color():def(BLACK_WHITE),highlight(BLACK_BLUE),lastColor(BLACK_WHITE){}
    Color(short def=BLACK_WHITE,short highlight=BLACK_BLUE):def(def),highlight(highlight),lastColor(BLACK_WHITE){}
    auto SetDef(){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),def);
        lastColor=def;
    }
    auto SetHighlight(){
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
    DWORD buttonState,controlKeyState,eventFlag;
    CUI* ui;
    Parameter():buttonState(MOUSE_LEFT_BUTTON),controlKeyState(0ul),eventFlag(0ul),ui(nullptr){}
    Parameter(MOUSE_EVENT_RECORD mouseEvent,CUI* ui):buttonState(mouseEvent.dwButtonState),controlKeyState(mouseEvent.dwControlKeyState),eventFlag(mouseEvent.dwEventFlags),ui(ui){}
};
typedef bool(*callback)(Parameter);
struct Text{
    std::string text;
    Color color;
    COORD position;
    callback function;
    Text():text(""),color(Color(0,0)),position({0,0}),function(nullptr){}
    Text(std::string text,Color color,callback function):text(text),color(color),position({0,0}),function(function){}
    bool operator==(const COORD&mousePosition)const{
        return (((!(position.Y^mousePosition.Y))&&(position.X<=mousePosition.X)&&(mousePosition.X<(position.X+(short)text.size())))?(true):(false));
    }
    bool operator!=(const COORD&mousePosition)const{
        return !operator==(mousePosition);
    }
};
class CUI{
    private:
        DWORD sleepTime;
        short height,width;
        std::vector<Text>lineData;
    protected:
        auto GetConsoleSize(){
            CONSOLE_SCREEN_BUFFER_INFO t;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&t);
            height=t.dwSize.Y;
            width=t.dwSize.X;
        }
        auto ClearScreen(){
            GetConsoleSize();
            SetCursor({0,0});
            for(short i{};i<height;++i){
                for(short j{};j<width;++j){
                    printf(" ");
                }
            }
            SetCursor({0,0});
        }
        auto Write(std::string text,bool isEndl=false){
            printf("%s",text.c_str());
            if(isEndl){
                printf("\n");
            }
        }
        auto Rewrite(Text data){
            SetCursor({0,data.position.Y});
            for(short j{};j<data.position.X;++j){
                Write(" ");
            }
            SetCursor({0,data.position.Y});
            Write(data.text);
            SetCursor({0,data.position.Y});
        }
        auto InitPosition(){
            ClearScreen();
            for(auto &data:lineData){
                data.position=GetCursorPos();
                data.color.SetDef();
                Write(data.text,true);
            }
        }
        auto Refresh(COORD hangPosition){
            for(auto &data:lineData){
                if((data==hangPosition)&&(data.color.lastColor^data.color.highlight)){
                    data.color.SetHighlight();
                    Rewrite(data);
                }
                if((data!=hangPosition)&&(data.color.lastColor^data.color.def)){
                    data.color.SetDef();
                    Rewrite(data);
                }
            }
        }
        auto Implement(MOUSE_EVENT_RECORD mouseEvent){
            bool isExit{};
            for(auto& data:lineData){
                if(data==mouseEvent.dwMousePosition){
                    if(data.function!=nullptr){
                        ClearScreen();
                        data.color.SetDef();
                        AddAttributes();
                        ShowCursor();
                        isExit=data.function(Parameter(mouseEvent,this));
                        RemoveAttributes();
                        HideCursor();
                        InitPosition();
                    }
                    break;
                }
            }
            return isExit;
        }
    public:
        CUI():sleepTime(50ul),height(0),width(0){}
        ~CUI(){}
        auto Push(std::string text,callback function=nullptr,short colorHighlight=BLACK_BLUE,short colorDef=BLACK_WHITE)->CUI&{
            lineData.push_back(Text(text,Color(colorDef,((function==nullptr)?(colorDef):(colorHighlight))),function));
            return *this;
        }
        auto Pop()->CUI&{
            lineData.pop_back();
            return *this;
        }
        auto Clear()->CUI&{
            lineData.clear();
            return *this;
        }
        auto Show(){
            RemoveAttributes();
            HideCursor();
            MOUSE_EVENT_RECORD mouseEvent;
            Sleep(100ul);
            InitPosition();
            bool isExit{};
            while(!isExit){
                Sleep(sleepTime);
                mouseEvent=WaitMouseEvent();
                switch(mouseEvent.dwEventFlags){
                    case MOUSE_MOVE:{
                        Refresh(mouseEvent.dwMousePosition);
                        break;
                    }case MOUSE_CLICK:{
                        if((mouseEvent.dwButtonState)&&(mouseEvent.dwButtonState^MOUSE_WHEEL)){
                            isExit=Implement(mouseEvent);
                        }
                        break;
                    }
                }
            }
            ClearScreen();
            Sleep(100ul);
        }
}UI;
auto Exit(Parameter){
    return true;
}
#endif
#ifndef MENU_HPP
#define MENU_HP
void HideCursor(){
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&CursorInfo);
    CursorInfo.bVisible=false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&CursorInfo);
}
void ShowCursor(){
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&CursorInfo);
    CursorInfo.bVisible=true;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&CursorInfo);
}
void RemoveAttributes(){
    DWORD mode;
    GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),&mode);
    mode&=~ENABLE_QUICK_EDIT_MODE,mode&=~ENABLE_INSERT_MODE,mode|=ENABLE_MOUSE_INPUT;
    SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),mode);
}
void AddAttributes(){
    DWORD mode;
    GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),&mode);
    mode|=ENABLE_QUICK_EDIT_MODE,mode|=ENABLE_INSERT_MODE,mode|=ENABLE_MOUSE_INPUT;
    SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),mode);
}
COORD GetTheCursor(){
    CONSOLE_SCREEN_BUFFER_INFO tmp;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&tmp);
    return tmp.dwCursorPosition;
}
void SetCursor(const COORD&tmp={0,0}){
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),tmp);
}
MOUSE_EVENT_RECORD WaitMouseEvent(bool move=true){
    INPUT_RECORD record;
    DWORD reg;
    while(true){
        Sleep(10UL);
        ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE),&record,1,&reg);
        if(!(record.EventType^MOUSE_EVENT)&&(move|(record.Event.MouseEvent.dwEventFlags!=MOUSE_MOVED))){
            return record.Event.MouseEvent;
        }
    }
}
#define BlackWhite 0x07
#define BlackBlue 0x03
#define YellowBlue 0xe9
struct Color{
    short def,highlight,lastColor;
    Color():def(BlackWhite),highlight(BlackBlue),lastColor(BlackWhite){}
    Color(short def=BlackWhite,short highlight=BlackBlue):def(def),highlight(highlight),lastColor(BlackWhite){}
    void SetDef(){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),def);
        lastColor=def;
    }
    void SetHighlight(){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),highlight);
        lastColor=highlight;
    }
};
class Menu;
#define MouseLeftButton FROM_LEFT_1ST_BUTTON_PRESSED
#define MouseMiddleButton FROM_LEFT_2ND_BUTTON_PRESSED
#define MouseRightButton RIGHTMOST_BUTTON_PRESSED
#define MouseClick 0x0
#define MouseMove MOUSE_MOVED
#define MouseDoubleClick DOUBLE_CLICK
#define MouseWheel MOUSE_WHEELED
struct Parameter{
    DWORD buttonState,controlKeyState,eventFlag;
    Menu* menu;
    Parameter():buttonState(MouseLeftButton),controlKeyState(0UL),eventFlag(0UL),menu(nullptr){}
    Parameter(MOUSE_EVENT_RECORD mouseEvent,Menu* menu):buttonState(mouseEvent.dwButtonState),controlKeyState(mouseEvent.dwControlKeyState),eventFlag(mouseEvent.dwEventFlags),menu(menu){}
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
        return ((!(position.Y^mousePosition.Y)&&(position.X<=mousePosition.X)&&(mousePosition.X<(position.X+(short)text.length())))?(true):(false));
    }
    bool operator!=(const COORD&mousePosition)const{
        return !operator==(mousePosition);
    }
};
class Menu{
    private:
        unsigned long sleepTime;
        short consoleHeight,consoleWidth;
        std::vector<Text>lineData;
    protected:
        void GetConsoleSize(){
            CONSOLE_SCREEN_BUFFER_INFO tmp;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&tmp);
            consoleHeight=tmp.dwSize.Y;
            consoleWidth=tmp.dwSize.X;
        }
        void ClearScreen(){
            GetConsoleSize();
            SetCursor({0,0});
            for(short i{};i<consoleHeight;++i){
                for(short j{};j<consoleWidth;++j){
                    printf(" ");
                }
            }
            SetCursor({0,0});
        }
        void Write(std::string text,bool isEndl=false){
            printf("%s",text.c_str());
            if(isEndl){
                printf("\n");
            }
        }
        void Rewrite(Text data){
            SetCursor({0,data.position.Y});
            for(short j{};j<data.position.X;++j){
                Write(" ");
            }
            SetCursor({0,data.position.Y});
            Write(data.text);
            SetCursor({0,data.position.Y});
        }
        void InitPosition(){
            ClearScreen();
            for(auto &data:lineData){
                data.position=GetTheCursor();
                data.color.SetDef();
                Write(data.text,true);
            }
        }
        void Refresh(COORD hangPosition){
            for(auto &data:lineData){
                if((data==hangPosition)&&(data.color.lastColor!=data.color.highlight)){
                    data.color.SetHighlight();
                    Rewrite(data);
                }
                if((data!=hangPosition)&&(data.color.lastColor!=data.color.def)){
                    data.color.SetDef();
                    Rewrite(data);
                }
            }
        }
        bool implement(MOUSE_EVENT_RECORD mouseEvent){
            bool isExit{};
            for(auto &data:lineData){
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
        Menu():sleepTime(50UL),consoleHeight(0),consoleWidth(0){}
        ~Menu(){}
        Menu& Push(std::string text="",callback function=nullptr,short colorHighlight=BlackBlue,short colordef=BlackWhite){
            lineData.push_back(Text(text,Color(colordef,((function==nullptr)?(colordef):(colorHighlight))),function));
            return *this;
        }
        Menu& Pop(){
            lineData.pop_back();
            return *this;
        }
        Menu& Clear(){
            lineData.clear();
            return *this;
        }
        void Show(){
            RemoveAttributes();
            HideCursor();
            MOUSE_EVENT_RECORD mouseEvent;
            Sleep(100UL);
            InitPosition();
            bool isExit{};
            while(!isExit){
                Sleep(sleepTime);
                mouseEvent=WaitMouseEvent();
                switch(mouseEvent.dwEventFlags){
                    case MouseMove:{
                        Refresh(mouseEvent.dwMousePosition);
                        break;
                    }case MouseClick:{
                        if((mouseEvent.dwButtonState)&&(mouseEvent.dwButtonState!=MouseWheel)){
                            isExit=implement(mouseEvent);
                        }
                        break;
                    }
                }
            }
            ClearScreen();
            Sleep(100UL);
        }
}UI;
bool _Exit(Parameter){
    return true;
}
#define Exit _Exit
#endif
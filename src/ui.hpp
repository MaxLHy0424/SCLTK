#ifndef MENU_HPP
#define MENU_HP
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
    Color():def(CON_WHITE),highlight(CON_BLUE),lastColor(CON_WHITE){}
    Color(i16 def=CON_WHITE,i16 highlight=CON_BLUE):def(def),highlight(highlight),lastColor(CON_WHITE){}
    auto setDefault(){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),def);
        lastColor=def;
    }
    auto setHighlight(){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),highlight);
        lastColor=highlight;
    }
};
struct Parameter{
    DWORD buttonState,ctrlKeyState,eventFlag;
    CUI* ui;
    Parameter():buttonState(MOUSE_BUTTON_LEFT),ctrlKeyState(0),eventFlag(0),ui(nullptr){}
    Parameter(MOUSE_EVENT_RECORD mouseEvent,CUI* ui):buttonState(mouseEvent.dwButtonState),ctrlKeyState(mouseEvent.dwControlKeyState),eventFlag(mouseEvent.dwEventFlags),ui(ui){}
};
typedef bool (*fnptr)(Parameter);
struct Text{
    cstr txt;
    Color color;
    COORD position;
    fnptr fn;
    Text():txt(""),color(Color(0,0)),position({0,0}),fn(nullptr){}
    Text(cstr txt,Color color,fnptr fn):txt(txt),color(color),position({0,0}),fn(fn){}
    bool operator==(const COORD& mousePosition)const{
        return ((position.Y==mousePosition.Y)&&(position.X<=mousePosition.X)&&(mousePosition.X<(position.X+(i16)strlen(txt))))?(true):(false);
    }
    bool operator!=(const COORD& mousePosition)const{
        return !(operator==(mousePosition));
    }
};
class CUI{
    private:
        i16 height,width;
        std::vector<Text>lineData;
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
            CONSOLE_SCREEN_BUFFER_INFO t;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&t);
            return t.dwCursorPosition;
        }
        auto setCursor(const COORD& t={0,0}){
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),t);
        }
        auto waitMouseEvent(bool move=true)->MOUSE_EVENT_RECORD{
            INPUT_RECORD record;
            DWORD reg;
            while(true){
                Sleep(10ul);
                ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE),&record,1,&reg);
                if((record.EventType==MOUSE_EVENT)&&(move|(record.Event.MouseEvent.dwEventFlags!=MOUSE_MOVED))){
                    return record.Event.MouseEvent;
                }
            }
        }
        auto getConsoleSize(){
            CONSOLE_SCREEN_BUFFER_INFO t;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&t);
            height=t.dwSize.Y;
            width=t.dwSize.X;
        }
        auto clearScreen(){
            getConsoleSize();
            setCursor({0,0});
            for(i16 i{};i<height;++i){
                for(i16 j{};j<width;++j){
                    printf(" ");
                }
            }
            setCursor({0,0});
        }
        auto write(cstr txt,bool isEndl=false){
            printf("%s",txt);
            if(isEndl){
                printf("\n");
            }
        }
        auto rewrite(Text data){
            setCursor({0,data.position.Y});
            for(i16 j{};j<data.position.X;++j){
                write(" ");
            }
            setCursor({0,data.position.Y});
            write(data.txt);
            setCursor({0,data.position.Y});
        }
        auto initPosition(){
            clearScreen();
            for(auto& data:lineData){
                data.position=getCursor();
                data.color.setDefault();
                write(data.txt,true);
            }
        }
        auto refresh(COORD hangPosition){
            for(auto& data:lineData){
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
        auto implement(MOUSE_EVENT_RECORD mouseEvent){
            bool isExit{};
            for(auto& data:lineData){
                if(data==mouseEvent.dwMousePosition){
                    if(data.fn!=nullptr){
                        clearScreen();
                        data.color.setDefault();
                        addAttributes();
                        showCursor();
                        isExit=data.fn(Parameter(mouseEvent,this));
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
        CUI():height(0),width(0){}
        ~CUI(){}
        auto push(cstr txt,fnptr fn=nullptr,i16 colorHighlight=CON_BLUE,i16 colorDef=CON_WHITE)->CUI&{
            lineData.push_back(Text(txt,Color(colorDef,((fn==nullptr)?(colorDef):(colorHighlight))),fn));
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
            Sleep(100);
            initPosition();
            bool isExit{};
            while(!isExit){
                Sleep(50);
                mouseEvent=waitMouseEvent();
                switch(mouseEvent.dwEventFlags){
                    case MOUSE_MOVE:{
                        refresh(mouseEvent.dwMousePosition);
                        break;
                    }case MOUSE_CLICK:{
                        if((mouseEvent.dwButtonState)&&(mouseEvent.dwButtonState!=MOUSE_WHEEL)){
                            isExit=implement(mouseEvent);
                        }
                        break;
                    }
                }
            }
            clearScreen();
            Sleep(100);
        }
};
auto _exit(Parameter){
    return true;
}
#define EXIT _exit
#endif
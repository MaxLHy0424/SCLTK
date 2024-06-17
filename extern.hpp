#include<stdio.h>
#include<string>
#include<thread>
#include<ShlObj.h>
char Dat[3]{};
bool Continue(){
    printf("是否继续? (Y/N): ");
    while(true){
        scanf("%s",&Dat[1]);
        if((Dat[1]=='y'||Dat[1]=='Y'||Dat[1]=='n'||Dat[1]=='N')&&(Dat[2]==0)){
            break;
        }
        printf("输入错误, 请重试: ");
    }
    return ((bool)(Dat[1]=='Y'||Dat[1]=='y'));
}
void PinWnd(){
    HWND foreWnd{GetForegroundWindow()};
    DWORD foreId{GetWindowThreadProcessId(foreWnd,NULL)},curId{GetCurrentThreadId()};
    while(true){
        AttachThreadInput(curId,foreId,TRUE);
        ShowWindow(foreWnd,SW_SHOWNORMAL);
        SetWindowPos(foreWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
        SetWindowPos(foreWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
        SetForegroundWindow(foreWnd);
        AttachThreadInput(curId,foreId,FALSE);
        SetWindowPos(foreWnd,HWND_TOPMOST,0,0,100,100,SWP_NOMOVE|SWP_NOSIZE);
        Sleep(100UL);
    }
}
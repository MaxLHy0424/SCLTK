#include<stdio.h>
#include<string>
#include<ShlObj.h>
#include<thread>
char Dat[3]{};
bool Continue(){
    printf("是否继续? (Y/N): ");
    while(true){
        scanf("%s",&Dat[1]);
        if((Dat[1]=='Y'||Dat[1]=='y'||Dat[1]=='N'||Dat[1]=='n')&&(Dat[2]==0)){
            break;
        }
        printf("输入错误, 请重试: ");
    }
    return ((Dat[1]=='Y'||Dat[1]=='y')?(true):(false));
}
void ForceShow(){
    HWND ForeWnd{GetForegroundWindow()};
    DWORD ForeID{GetWindowThreadProcessId(ForeWnd,NULL)};
    DWORD CurID{GetCurrentThreadId()};
    while(true){
        AttachThreadInput(CurID,ForeID,TRUE);
        ShowWindow(ForeWnd,SW_SHOWNORMAL);
        SetWindowPos(ForeWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
        SetWindowPos(ForeWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
        SetForegroundWindow(ForeWnd);
        AttachThreadInput(CurID,ForeID,FALSE);
        SetWindowPos(ForeWnd,HWND_TOPMOST,0,0,100,100,SWP_NOMOVE|SWP_NOSIZE);
        Sleep(100UL);
    }
}
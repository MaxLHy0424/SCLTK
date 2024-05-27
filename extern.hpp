#include<stdio.h>
#include<string>
#include<thread>
#include<ShlObj.h>
#define KernelVersion (((DWORD)(LOBYTE(LOWORD(GetVersion()))))*10UL+((DWORD)(HIBYTE(LOWORD(GetVersion())))))
char InDat[3]{};
void ForceShow(){
    HWND ForeWnd{GetForegroundWindow()};
    DWORD ForeID{GetWindowThreadProcessId(ForeWnd,NULL)};
    DWORD CurID{GetCurrentThreadId()};
    for(;;){
        AttachThreadInput(CurID,ForeID,TRUE);
        ShowWindow(ForeWnd,SW_SHOWNORMAL);
        SetWindowPos(ForeWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
        SetWindowPos(ForeWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
        SetForegroundWindow(ForeWnd);
        AttachThreadInput(CurID,ForeID,FALSE);
        SetWindowPos(ForeWnd,HWND_TOPMOST,0,0,100,100,SWP_NOMOVE|SWP_NOSIZE);
        Sleep(250UL);
    }
}
bool Continue(){
    printf("是否继续? (Y/N): ");
    for(;;){
        scanf("%s",&InDat[1]);
        if((InDat[1]=='Y'||InDat[1]=='y'||InDat[1]=='N'||InDat[1]=='n')&&(InDat[2]==0)){
            break;
        }
        printf("输入错误, 请重试: ");
    }
    return (InDat[1]=='Y'||InDat[1]=='y');
}
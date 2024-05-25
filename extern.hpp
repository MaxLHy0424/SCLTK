#include<stdio.h>
#include<string>
#include<thread>
#include<ShlObj.h>
#define KernelVersion (((DWORD)(LOBYTE(LOWORD(GetVersion()))))*10UL+((DWORD)(HIBYTE(LOWORD(GetVersion())))))
char Input[3]{};
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
        Sleep(200UL);
    }
}
bool Continue(){
    printf("是否继续? (Y/N): ");
    for(;;){
        scanf("%s",&Input[1]);
        if((Input[1]=='Y'||Input[1]=='y'||Input[1]=='N'||Input[1]=='n')&&(Input[2]==0)){
            break;
        }
        printf("输入错误, 请重试: ");
    }
    return (Input[1]=='Y'||Input[1]=='y');
}
#include<stdio.h>
#include<string>
#include<ShlObj.h>
#include<thread>
#define KernalVersion (((DWORD)(LOBYTE(LOWORD(GetVersion()))))*10UL+((DWORD)(HIBYTE(LOWORD(GetVersion())))))
char IptCh[3]{};
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
        Sleep(500UL);
    }
}
bool ContinueOps(){
    printf("是否继续? (Y/N): ");
    for(;;){
        scanf("%s",&IptCh[1]);
        if((IptCh[1]=='Y'||IptCh[1]=='y'||IptCh[1]=='N'||IptCh[1]=='n')&&(IptCh[2]==0)){
            break;
        }
        printf("键入错误, 请重试: ");
    }
    return (IptCh[1]=='Y'||IptCh[1]=='y');
}
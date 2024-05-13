#pragma once
#include<stdio.h>
#include<string>
#include<ShlObj.h>
#include<thread>
char IptDat[3]{};
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
bool ConfirmOp(){
    printf("是否继续? (Y/N): ");
    for(;;){
        scanf("%s",&IptDat[1]);
        if((IptDat[1]=='Y'||IptDat[1]=='y'||IptDat[1]=='N'||IptDat[1]=='n')&&(IptDat[2]==0)){
            break;
        }
        printf("键入错误, 请重试: ");
    }
    return ((bool)(IptDat[1]=='Y'||IptDat[1]=='y'));
}
DWORD KernalVersion(){
    DWORD version{GetVersion()};
    DWORD major{(DWORD)(LOBYTE(LOWORD(version)))},minor{(DWORD)(HIBYTE(LOWORD(version)))};
    return (major*10UL+minor);
}
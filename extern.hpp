#pragma once
#include<stdio.h>
#include<string>
#include<ShlObj.h>
#include<thread>
#define KernalVersion ((DWORD)(LOBYTE(LOWORD(GetVersion())))*10+(DWORD)(HIBYTE(LOWORD(GetVersion()))))
char IosDat[3]{};
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
        scanf("%s",&IosDat[1]);
        if((IosDat[1]=='Y'||IosDat[1]=='y'||IosDat[1]=='N'||IosDat[1]=='n')&&(IosDat[2]==0)){
            break;
        }
        printf("键入错误, 请重试: ");
    }
    return ((IosDat[1]=='Y'||IosDat[1]=='y')?(true):(false));
}
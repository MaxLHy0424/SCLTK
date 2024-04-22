#pragma once
#include<stdio.h>
#include<fstream>
#include<ShlObj.h>
#include<thread>
char Code[3]{};
std::string Config[3]{"","UNDEFINED",""};
DWORD KernalVersion(){
    DWORD ver{GetVersion()};
    DWORD major{(DWORD)(LOBYTE(LOWORD(ver)))},minor{(DWORD)(HIBYTE(LOWORD(ver)))};
    return major*10UL+minor;
}
bool Confirm(){
    printf("是否继续? (Y/N): ");
    for(;;){
        scanf("%s",&Code[1]);
        if((Code[1]=='Y'||Code[1]=='y'||Code[1]=='N'||Code[1]=='n')&&(Code[2]==0)){
            break;
        }
        printf("输入错误, 请重新输入: ");
    }
    return (Code[1]=='Y'||Code[1]=='y');
}
void ForceShow(){
    for(;;){
        if(Config[2]!="1"){
            SetWindowPos(GetForegroundWindow(),HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
            return;
        }
        HWND ForeWnd{GetForegroundWindow()};
        DWORD ForeID{GetWindowThreadProcessId(ForeWnd,NULL)};
        DWORD CurID{GetCurrentThreadId()};
        AttachThreadInput(CurID,ForeID,TRUE);
        ShowWindow(ForeWnd,SW_SHOWNORMAL);
        SetWindowPos(ForeWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
        SetWindowPos(ForeWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
        SetForegroundWindow(ForeWnd);
        AttachThreadInput(CurID,ForeID,FALSE);
        SetWindowPos(GetForegroundWindow(),HWND_TOPMOST,0,0,100,100,SWP_NOMOVE|SWP_NOSIZE);
        Sleep(500UL);
    }
}
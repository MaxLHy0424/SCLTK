#pragma once
#include<stdio.h>
#include<fstream>
#include<thread>
#include<ShlObj.h>
char Code[3]{};
std::string CfgDat[3]{"\0","UNDEFINED","0"};
DWORD KernalVersion(){
    DWORD version{GetVersion()};
    DWORD major{(DWORD)(LOBYTE(LOWORD(version)))},minor{(DWORD)(HIBYTE(LOWORD(version)))};
    return major*10UL+minor;
}
bool Confirm(){
    printf("是否继续? (Y/N): ");
    for(;;){
        scanf("%s",&Code[1]);
        if((Code[1]=='Y'||Code[1]=='y'||Code[1]=='N'||Code[1]=='n')&&Code[2]==0){
            break;
        }
        printf("输入错误, 请重新输入: ");
    }
    return (Code[1]=='Y'||Code[1]=='y')?(true):(false);
}
void OnForceShow(HWND hWnd){
    for(;;){
        if(CfgDat[2]!="1"){
            return;
        }
	    HWND ForeWnd{::GetForegroundWindow()};
	    DWORD ForeID{::GetWindowThreadProcessId(ForeWnd, NULL)};
	    DWORD CurID{::GetCurrentThreadId()};
	    ::AttachThreadInput(CurID,ForeID,TRUE);
	    ::ShowWindow(hWnd,SW_SHOWNORMAL);
	    ::SetWindowPos(hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	    ::SetWindowPos(hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	    ::SetForegroundWindow(hWnd);
	    ::AttachThreadInput(CurID,ForeID,FALSE);
        ::SetWindowPos(::GetForegroundWindow(),HWND_TOPMOST,0,0,100,100,SWP_NOMOVE|SWP_NOSIZE);
        Sleep(500);
    }
}
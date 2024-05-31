#include<stdio.h>
#include<string>
#include<ShlObj.h>
#include<thread>
#define Pause system("Pause")
#define Clear system("CLS")
char Data[3]{};
bool Continue(){
    printf("是否继续? (Y/N): ");
    for(;;){
        scanf("%s",&Data[1]);
        if((Data[1]=='Y'||Data[1]=='y'||Data[1]=='N'||Data[1]=='n')&&(Data[2]==0)){
            break;
        }
        printf("输入错误, 请重试: ");
    }
    return (Data[1]=='Y'||Data[1]=='y');
}
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
        Sleep(100UL);
    }
}
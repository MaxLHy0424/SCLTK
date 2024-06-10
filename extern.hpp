#include<stdio.h>
#include<ShlObj.h>
#include<string>
#include<thread>
char Dat[3]{};
bool Continue(){
    printf("是否继续? (Y/N): ");
    while(true){
        scanf("%s",&Dat[1]);
        if((Dat[1]=='y'||Dat[1]=='n'||Dat[1]=='Y'||Dat[1]=='N')&&(Dat[2]==0)){
            break;
        }
        printf("输入错误, 请重试: ");
    }
    return (Dat[1]=='Y'||Dat[1]=='y');
}
void ForceShow(){
    HWND ForeWin{GetForegroundWindow()};
    DWORD ForeId{GetWindowThreadProcessId(ForeWin,NULL)},CurId{GetCurrentThreadId()};
    while(true){
        AttachThreadInput(CurId,ForeId,TRUE);
        ShowWindow(ForeWin,SW_SHOWNORMAL);
        SetWindowPos(ForeWin,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
        SetWindowPos(ForeWin,HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
        SetForegroundWindow(ForeWin);
        AttachThreadInput(CurId,ForeId,FALSE);
        SetWindowPos(ForeWin,HWND_TOPMOST,0,0,100,100,SWP_NOMOVE|SWP_NOSIZE);
        Sleep(100UL);
    }
}
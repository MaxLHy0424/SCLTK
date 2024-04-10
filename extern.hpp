#pragma once
#include<stdio.h>
#include<fstream>
#include<ShlObj.h>
char Code[3]{};
std::string CfgDat[3]{"\0","NULL","true"};
DWORD KernalVersion(){
    DWORD version{GetVersion()};
    DWORD major{(DWORD)(LOBYTE(LOWORD(version)))};
    DWORD minor{(DWORD)(HIBYTE(LOWORD(version)))};
    return major*10U+minor;
}
bool Confirm(){
    printf("是否继续? (Y/N): ");
    for(;;){
        scanf("%s",&Code[1]);
        if((Code[1]=='Y'||Code[1]=='y'||Code[1]=='N'||Code[1]=='n')&&Code[2]==0){
            break;
        }else{
            printf("输入错误, 请重新输入: ");
        }
    }
    return (Code[1]=='Y'||Code[1]=='y')?true:false;
}
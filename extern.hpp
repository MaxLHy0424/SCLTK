#pragma once
#include<stdio.h>
#include<fstream>
#include<ShlObj.h>
#include<windows.h>
char Code[3];
std::string CfgDat[3]{"","","UNDEFINED"};
DWORD KernalVersion(){
	DWORD version{GetVersion()};
	DWORD major{(DWORD)(LOBYTE(LOWORD(version)))};
	DWORD minor{(DWORD)(HIBYTE(LOWORD(version)))};
    return major*10UL+minor;
}
bool PauseOp(){
	printf("是否继续? (Y/N): ");
    for(;;){
        scanf("%s",&Code[1]);
        if((Code[1]=='Y'||Code[1]=='y'||Code[1]=='N'||Code[1]=='n')&&Code[2]==0){
            break;
        }else{
            printf("输入错误, 请重新输入: ");
        }
    }
	if(Code[1]=='Y'||Code[1]=='y'){
        return true;
    }
    return false;
}
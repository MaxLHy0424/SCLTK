#pragma once
#include<stdio.h>
#include<fstream>
#include<ShlObj.h>
#include<windows.h>
char CODE[3];
std::string CONFIG[4]{"","","UNDEFINED",""};
DWORD KernalVersion(){
	DWORD version{GetVersion()};
	DWORD major{(DWORD)(LOBYTE(LOWORD(version)))};
	DWORD minor{(DWORD)(HIBYTE(LOWORD(version)))};
    return major*10UL+minor;
}
bool PauseOp(){
	printf("是否继续? (Y/N): ");
    for(;;){
        scanf("%s",&CODE[1]);
        if((CODE[1]=='Y'||CODE[1]=='y'||CODE[1]=='N'||CODE[1]=='n')&&CODE[2]==0){
            break;
        }else{
            printf("输入错误, 请重试: ");
        }
    }
	if(CODE[1]=='Y'||CODE[1]=='y'){
        return true;
    }
    return false;
}
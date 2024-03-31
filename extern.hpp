#pragma once
#include<stdio.h>
#include<fstream>
#include<ShlObj.h>
#include<windows.h>
char Code[3];
std::string Config[4]{"","","UNDEFINED",""};
DWORD KernalVersion(){
	DWORD version{GetVersion()};
	DWORD major{(DWORD)(LOBYTE(LOWORD(version)))};
	DWORD minor{(DWORD)(HIBYTE(LOWORD(version)))};
    return major*10UL+minor;
}
bool PauseOp(){
	printf("是否继续 (Y/N): ");
	scanf("%s",&Code[1]);
	while((Code[1]!='Y'&&Code[1]!='y'&&Code[1]!='N'&&Code[1]!='n')||Code[2]!=0){
        printf("输入错误, 请重新输入 ");
        scanf("%s",&Code[1]);
    }
	if(Code[1]=='Y'||Code[1]=='y'){
        return true;
    }
	return false;
}
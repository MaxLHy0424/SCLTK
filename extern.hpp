#pragma once
#include<stdio.h>
#include<fstream>
#include<ShlObj.h>
#include<windows.h>
char Code[3];
std::string CfgDat[4]{"","CRCSN","1","$UNDEFINED$"};
DWORD SysKernalVersion(){
	DWORD version{GetVersion()};
	DWORD major{(DWORD)(LOBYTE(LOWORD(version)))};
	DWORD minor{(DWORD)(HIBYTE(LOWORD(version)))};
    return major*10U+minor;
}
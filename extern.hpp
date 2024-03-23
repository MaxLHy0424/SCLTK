#pragma once
#include<stdio.h>
#include<fstream>
#include<ShlObj.h>
#include<windows.h>
char Code[3];
std::string CfgDat[4]{"","CRCSN","1","$UNDEFINED$"};
DWORD SysKernalVer(){
    typedef void(__stdcall*NTPROC)(DWORD*,DWORD*,DWORD*);
    HINSTANCE inst{LoadLibrary(TEXT("ntdll.dll"))};
    NTPROC GetSysKernalVer{(NTPROC)GetProcAddress(inst,"RtlGetNtVersionNumbers")};
    DWORD major,minor;
    GetSysKernalVer(&major,&minor,NULL);
    return major*10U+minor;
}
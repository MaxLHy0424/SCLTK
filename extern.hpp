#pragma once
#include<stdio.h>
#include<string>
#include<fstream>
#include<ShlObj.h>
#include<windows.h>
DWORD GetSysKernalVersion(){
    typedef void(__stdcall*NTPROC)(DWORD*,DWORD*,DWORD*);
    HINSTANCE inst{LoadLibrary(TEXT("ntdll.dll"))};
    NTPROC getSysKernalVersion{(NTPROC)GetProcAddress(inst,"RtlGetNtVersionNumbers")};
    DWORD major,minor;
    getSysKernalVersion(&major,&minor,NULL);
    return major*10U+minor;
}
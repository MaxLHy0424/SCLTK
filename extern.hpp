#pragma once
#include<stdio.h>
#include<fstream>
#include<ShlObj.h>
#include<windows.h>
DWORD SysKernalVersion(){
    typedef void(__stdcall*NTPROC)(DWORD*,DWORD*,DWORD*);
    HINSTANCE inst{LoadLibrary(TEXT("ntdll.dll"))};
    NTPROC GetKernalVersion{(NTPROC)GetProcAddress(inst,"RtlGetNtVersionNumbers")};
    DWORD major,minor;
    GetKernalVersion(&major,&minor,NULL);
    return major*10U+minor;
}
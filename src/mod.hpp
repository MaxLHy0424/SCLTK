#pragma once
#include"def.hpp"
#include"ui.hpp"
#define DEFAULT_OPTIONS false,false,true,false
struct{
    bool frontShow,alphaWnd,fixWndSize,compactView;
}opt{DEFAULT_OPTIONS};
namespace Mod{
    auto init(){
        system("chcp 936 > nul");
        #if CHANNEL==0
            SetConsoleTitle("CRCSN");
        #elif CHANNEL==1
            SetConsoleTitle("[Preview] CRCSN");
        #elif CHANNEL==2
            SetConsoleTitle("[DevSnap] CRCSN");
        #else
            SetConsoleTitle(CUSTOM_TITLE);
        #endif
        if(opt.fixWndSize){
            SetWindowLongPtr(GetConsoleWindow(),GWL_STYLE,GetWindowLongPtr(GetConsoleWindow(),GWL_STYLE)&~WS_SIZEBOX&~WS_MAXIMIZEBOX);
        }
        system("mode con cols=50 lines=25");
        if(opt.alphaWnd){
            SetLayeredWindowAttributes(GetConsoleWindow(),0,204,LWA_ALPHA);
        }
    }
    auto frontShow(){
        HWND foreWnd{GetConsoleWindow()};
        DWORD foreId{GetWindowThreadProcessId(foreWnd,NULL)},curId{GetCurrentThreadId()};
        while(true){
            AttachThreadInput(curId,foreId,TRUE);
            ShowWindow(foreWnd,SW_SHOWNORMAL);
            SetWindowPos(foreWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
            SetWindowPos(foreWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
            SetForegroundWindow(foreWnd);
            AttachThreadInput(curId,foreId,FALSE);
            SetWindowPos(foreWnd,HWND_TOPMOST,0,0,100,100,SWP_NOMOVE|SWP_NOSIZE);
            Sleep(100);
        }
    }
    auto exit(Parameter){
        SetWindowLongPtr(GetConsoleWindow(),GWL_STYLE,GetWindowLongPtr(GetConsoleWindow(),GWL_STYLE)|WS_SIZEBOX|WS_MAXIMIZEBOX);
        SetLayeredWindowAttributes(GetConsoleWindow(),0,255,LWA_ALPHA);
        return true;
    }
    auto cmd(Parameter){
        system("cls & cmd & cls");
        init();
        return false;
    }
    struct{
        struct{
            std::vector<cstr> exe,svc;
        }mythware;
        struct{
            std::vector<cstr> exe,svc;
        }lenovo;
    }rule{
        {
            {
                "StudentMain","DispcapHelper","VRCwPlayer",
                "InstHelpApp","InstHelpApp64","TDOvrSet",
                "GATESRV","ProcHelper64","MasterHelper"
            },{
                "STUDSRV","TDNetFilter","TDFileFilter"
            }
        },{
            {
                "vncviewer","tvnserver32","WfbsPnpInstall",
                "WFBSMon","WFBSMlogon","WFBSSvrLogShow",
                "ResetIp","FuncForWIN64","CertMgr",
                "Fireware","BCDBootCopy","refreship",
                "lenovoLockScreen","PortControl64","DesktopCheck",
                "DeploymentManager","DeploymentAgent","XYNTService"
            },{
                "BSAgentSvr","tvnserver","WFBSMlogon"
            }
        }
    };
    auto op(i8 mode,std::vector<cstr>& exe,std::vector<cstr>& svc){
        system("cls");
        std::string cmd;
        switch(mode){
            case 'c':{
                for(auto i:exe){
                    cmd="taskKill /f /im "+(std::string)i+".exe";
                    system(cmd.c_str());
                }
                for(auto i:exe){
                    cmd="reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution options\\"\
                        +(std::string)i+".exe\" /f /t reg_sz /v debugger /d ?";
                    system(cmd.c_str());
                }
                for(auto i:svc){
                    cmd="net stop "+(std::string)i+" /y";
                    system(cmd.c_str());
                }
                break;
            }case 'r':{
                for(auto i:exe){
                    cmd="reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution options\\"\
                        +(std::string)i+".exe\" /f";
                    system(cmd.c_str());
                }
                for(auto i:svc){
                    cmd="net start "+(std::string)i;
                    system(cmd.c_str());
                }
                break;
            }
        }
        system("cls");
    }
    namespace Crack{
        auto mythware(Parameter){
            op('c',rule.mythware.exe,rule.mythware.svc);
            return false;
        }
        auto lenovo(Parameter){
            op('c',rule.lenovo.exe,rule.lenovo.svc);
            return false;
        }
    }
    namespace Recovery{
        auto mythware(Parameter){
            op('r',rule.mythware.exe,rule.mythware.svc);
            return false;
        }
        auto lenovo(Parameter){
            op('r',rule.lenovo.exe,rule.lenovo.svc);
            return false;
        }
    }
}
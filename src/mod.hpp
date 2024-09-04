#pragma once
#include"def.hpp"
#include"ui.hpp"
struct{
    bool frontShow,alphaWnd,wndSizeBox,largeView;
}opt{};
namespace Mod{
    auto init(){
        system("chcp 936 > nul");
        if constexpr(CHANNEL==0){
            SetConsoleTitle(L"CRCSN");
        }else if constexpr(CHANNEL==1){
            SetConsoleTitle(L"[Evaluate] CRCSN");
        }else if constexpr(CHANNEL==2){
            SetConsoleTitle(L"[Snapshot] CRCSN");
        }else{
            SetConsoleTitle(CUSTOM_TITLE);
        }
        if(!opt.wndSizeBox){
            SetWindowLongPtr(GetConsoleWindow(),GWL_STYLE,GetWindowLongPtr(GetConsoleWindow(),GWL_STYLE)&~WS_SIZEBOX&~WS_MAXIMIZEBOX);
        }
        system("mode con cols=50 lines=25");
        if(opt.alphaWnd){
            SetLayeredWindowAttributes(GetConsoleWindow(),0,204,LWA_ALPHA);
        }
    }
    auto frontShow(){
        HWND foreWnd{GetConsoleWindow()};
        DWORD foreId{GetWindowThreadProcessId(foreWnd,nullptr)},curId{GetCurrentThreadId()};
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
    auto exit(Argv){
        SetLayeredWindowAttributes(GetConsoleWindow(),0,255,LWA_ALPHA);
        SetWindowLongPtr(GetConsoleWindow(),GWL_STYLE,GetWindowLongPtr(GetConsoleWindow(),GWL_STYLE)|WS_SIZEBOX|WS_MAXIMIZEBOX);
        return true;
    }
    auto cmd(Argv){
        system("cmd & cls");
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
    struct OpArg{
        i8 m;
        std::vector<cstr> &exe,&svc;
    };
    auto op(Argv argv){
        system("cls");
        std::string cmd;
        switch(((OpArg*)argv.fArg)->m){
            case 'c':{
                for(const auto &i:((OpArg*)argv.fArg)->exe){
                    cmd="taskKill /f /im "+(std::string)i+".exe";
                    system(cmd.c_str());
                }
                for(const auto &i:((OpArg*)argv.fArg)->exe){
                    cmd="reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution options\\"
                        +(std::string)i+".exe\" /f /t reg_sz /v debugger /d ?";
                    system(cmd.c_str());
                }
                for(const auto &i:((OpArg*)argv.fArg)->svc){
                    cmd="net stop "+(std::string)i+" /y";
                    system(cmd.c_str());
                }
                break;
            }case 'r':{
                for(const auto &i:((OpArg*)argv.fArg)->exe){
                    cmd="reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution options\\"
                        +(std::string)i+".exe\" /f";
                    system(cmd.c_str());
                }
                for(const auto &i:((OpArg*)argv.fArg)->svc){
                    cmd="net start "+(std::string)i;
                    system(cmd.c_str());
                }
                break;
            }
        }
        system("cls");
        return false;
    }
}
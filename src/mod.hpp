#pragma once
#include"def.hpp"
#include"ui.hpp"
struct{
    bool wndFrontShow,wndAlpha,wndSizeBox,viewWide;
}opt{};
namespace Mod{
    auto init(){
        if constexpr(CHANNEL==0){
            SetConsoleTitle(L"CRCSN");
        }else if constexpr(CHANNEL==1){
            SetConsoleTitle(L"[Preview] CRCSN");
        }else if constexpr(CHANNEL==2){
            SetConsoleTitle(L"[DevSnap] CRCSN");
        }else{
            SetConsoleTitle(CUSTOM_TITLE);
        }
        SetWindowLongPtr(
            GetConsoleWindow(),GWL_STYLE,(opt.wndSizeBox)?
            (GetWindowLongPtr(GetConsoleWindow(),GWL_STYLE)|WS_SIZEBOX|WS_MAXIMIZEBOX):
            (GetWindowLongPtr(GetConsoleWindow(),GWL_STYLE)&~WS_SIZEBOX&~WS_MAXIMIZEBOX)
        );
        system("mode con cols=50 lines=25");
        SetLayeredWindowAttributes(GetConsoleWindow(),0,(opt.wndAlpha)?(204):(255),LWA_ALPHA);
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
    auto exit(Data){
        return true;
    }
    auto cmd(Data){
        system("cmd");
        return false;
    }
    struct{
        struct{
            std::vector<const i8*> exe,svc;
        }mythware;
        struct{
            std::vector<const i8*> exe,svc;
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
    struct ArgvOp{
        i8 m;
        std::vector<const i8*> &exe,&svc;
    };
    auto op(Data data){
        std::string cmd;
        switch(((ArgvOp*)data.argv)->m){
            case 'c':{
                for(const auto &ref:((ArgvOp*)data.argv)->exe){
                    cmd="reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution options\\"
                        +(std::string)ref+".exe\" /f /t reg_sz /v debugger /d ?";
                    system(cmd.c_str());
                    cmd="taskKill /f /im "+(std::string)ref+".exe";
                    system(cmd.c_str());
                }
                for(const auto &ref:((ArgvOp*)data.argv)->svc){
                    cmd="net stop "+(std::string)ref+" /y";
                    system(cmd.c_str());
                }
                break;
            }case 'r':{
                for(const auto &ref:((ArgvOp*)data.argv)->exe){
                    cmd="reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution options\\"
                        +(std::string)ref+".exe\" /f";
                    system(cmd.c_str());
                }
                for(const auto &ref:((ArgvOp*)data.argv)->svc){
                    cmd="net start "+(std::string)ref;
                    system(cmd.c_str());
                }
                break;
            }
        }
        return false;
    }
}
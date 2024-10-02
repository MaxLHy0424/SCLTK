#pragma once
#include"def.hpp"
#include"ui.hpp"
struct{
    bool wndFrontShow,wndAlpha,wndCtrls;
}opt{};
bool optError{};
namespace Mod{
    auto init(){
        SetConsoleTitle(WINDOW_TITLE);
        SetWindowLongPtr(
            GetConsoleWindow(),GWL_STYLE,(opt.wndCtrls)
            ?(GetWindowLongPtr(GetConsoleWindow(),GWL_STYLE)|WS_SIZEBOX|WS_MAXIMIZEBOX|WS_MINIMIZEBOX)
            :(GetWindowLongPtr(GetConsoleWindow(),GWL_STYLE)&~WS_SIZEBOX&~WS_MAXIMIZEBOX&~WS_MINIMIZEBOX)
        );
        system("mode con cols=50 lines=25");
        SetLayeredWindowAttributes(GetConsoleWindow(),0,(opt.wndAlpha)?(230):(255),LWA_ALPHA);
    }
    auto frontShow(){
        const HWND foreWnd{GetConsoleWindow()};
        const DWORD foreId{GetWindowThreadProcessId(foreWnd,nullptr)},curId{GetCurrentThreadId()};
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
    auto info(Data){
        UI ui;
        ui.add("                    < 信  息 >\n\n")
          .add(" < 返回 ",Mod::exit,{},WC_RED)
          .add("\n[  名  称  ]\n")
          .add(" " INFO_NAME)
          .add("\n[  版  本  ]\n")
          .add(" " INFO_VERSION)
          .add("\n[  仓  库  ]\n")
          .add(" " INFO_REPO_URL)
          .add("\n[ 许 可 证 ]\n")
          .add(" " INFO_LICENSE)
          .add(" (C) 2023 " INFO_PUBLISHER ". All Rights Reserved.")
          .show();
        return false;
    }
    auto cmd(Data){
        system("cmd");
        if(!opt.wndCtrls){
            init();
        }
        return false;
    }
    struct{
        struct{
            const std::vector<const i8*> exe,svc;
        }mythware;
        struct{
            const std::vector<const i8*> exe,svc;
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
    struct ArgsOp final{
        const i8 key;
        const std::vector<const i8*> &exe,&svc;
        ArgsOp(
            const i8 key,
            const std::vector<const i8*> &exe,const std::vector<const i8*> &svc
        ):key{key},exe{exe},svc{svc}{}
        ~ArgsOp(){}
    };
    auto op(Data data){
        UI ui;
        ui.add("                 < 破 解 | 恢 复 >\n\n");
        if(IsUserAnAdmin()){
            auto base{[&data](Data){
                ArgsOp args{std::any_cast<ArgsOp>(data.args)};
                std::string cmd;
                switch(args.key){
                    case 'C':{
                        for(const auto &ref:args.exe){
                            cmd="reg add "
                                "\"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution options\\"
                                +(std::string)ref+".exe\" /f /t reg_sz /v debugger /d ?";
                            system(cmd.c_str());
                            cmd="taskKill /f /im "+(std::string)ref+".exe";
                            system(cmd.c_str());
                        }
                        for(const auto &ref:args.svc){
                            cmd="net stop "+(std::string)ref+" /y";
                            system(cmd.c_str());
                        }
                        break;
                    }case 'R':{
                        for(const auto &ref:args.exe){
                            cmd="reg delete "
                                "\"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution options\\"
                                +(std::string)ref+".exe\" /f";
                            system(cmd.c_str());
                        }
                        for(const auto &ref:args.svc){
                            cmd="net start "+(std::string)ref;
                            system(cmd.c_str());
                        }
                        break;
                    }
                }
                return true;
            }};
            ui.add(" (i) 是否继续?\n")
              .add(" > 是 ",base)
              .add(" > 否 ",exit);
        }else{
            ui.add(" < 返回 ",exit,{},WC_RED);
            ui.add("\n (i) 需要管理员权限.");
        }
        ui.show();
        return false;
    }
}
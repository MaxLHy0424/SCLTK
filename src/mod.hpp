#pragma once
#include"def.hpp"
#include"ui.hpp"
struct{
    bool wndFrontShow,wndAlpha,wndCtrls;
}settings{};
bool settingsError{};
namespace Mod{
    auto isRunAsAdmin(){  
        BOOL isAdmin{};
        PSID adminsGroup{};
        SID_IDENTIFIER_AUTHORITY ntAuthority{SECURITY_NT_AUTHORITY};
        if(
            AllocateAndInitializeSid(
                &ntAuthority,2,SECURITY_BUILTIN_DOMAIN_RID,
                DOMAIN_ALIAS_RID_ADMINS,0,0,0,0,0,0,&adminsGroup
            )
        ){
            CheckTokenMembership(NULL,adminsGroup,&isAdmin);
            FreeSid(adminsGroup);
        }
        return isAdmin;
    }
    auto init(){
        system("chcp 936 > nul");
        SetConsoleTitle(WINDOW_TITLE);
        SetWindowLongPtr(
            GetConsoleWindow(),GWL_STYLE,(settings.wndCtrls)
                ?(GetWindowLongPtr(GetConsoleWindow(),GWL_STYLE)|WS_SIZEBOX|WS_MAXIMIZEBOX|WS_MINIMIZEBOX)
                :(GetWindowLongPtr(GetConsoleWindow(),GWL_STYLE)&~WS_SIZEBOX&~WS_MAXIMIZEBOX&~WS_MINIMIZEBOX)
        );
        system("mode con cols=50 lines=25");
        SetLayeredWindowAttributes(GetConsoleWindow(),0,(settings.wndAlpha)?(230):(255),LWA_ALPHA);
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
          .add("\n[名称]\n")
          .add(" " INFO_NAME)
          .add("\n[版本]\n")
          .add(" " INFO_VERSION)
          .add("\n[仓库]\n")
          .add(" " INFO_REPO_URL)
          .add("\n[许可证]\n")
          .add(" " INFO_LICENSE)
          .add(" (C) 2023 " INFO_DEVELOPER ". All Rights Reserved.")
          .show();
        return false;
    }
    auto cmd(Data){
        system("cmd");
        if(!settings.wndCtrls){
            init();
        }
        return false;
    }
#ifdef _DEV_
    auto settingsRead(){
        std::ifstream file("settings.ini",std::ios::in);
        if(!file.is_open()){
            goto END;
        }
        {
            std::string item{};
            while(std::getline(file,item)){
                if(item=="wndAlpha"){
                    settings.wndAlpha=true;
                }else if(item=="wndCtrls"){
                    settings.wndCtrls=true;
                }else if(item=="wndFrontShow"){
                    settings.wndFrontShow=true;
                }else{
                    settings={};
                    settingsError=true;
                    break;
                }
            }
        }
    END:
        file.close();
        return;
    }
    auto settingsEdit(Data){
        auto save{[](Data){
            std::string item;
            if(settings.wndAlpha){
                item=item+"wndAlpha\n";
            }if(settings.wndCtrls){
                item=item+"wndCtrls\n";
            }if(settings.wndFrontShow){
                item=item+"wndFrontShow\n";
            }
            if(item[item.size()-1]=='\n'){
                item=item.substr(0,item.size()-1);
            }
            std::ofstream file("settings.ini",std::ios::out|std::ios::trunc);
            file.write(item.c_str(),item.size());
            file.close();
            return true;
        }};
        auto reset{[](Data){
            std::ofstream file("settings.ini",std::ios::out|std::ios::trunc);
            file.close();
            return true;
        }};
        UI ui;
        ui.add("                    < 设  置 >\n\n")
          .add(" (i) 下次启动时生效.\n")
          .add(" < 保存并返回 ",save,{},WC_RED)
          .add(" < 重置并返回 ",reset,{},WC_RED)
          .add("\n[半透明窗口]\n")
          .add(" > 启用 ",[](Data){settings.wndAlpha=true;return false;})
          .add(" > 禁用 ",[](Data){settings.wndAlpha=false;return false;})
          .add("\n[置顶窗口]\n")
          .add(" > 启用 ",[](Data){settings.wndFrontShow=true;return false;})
          .add(" > 禁用 ",[](Data){settings.wndFrontShow=false;return false;})
          .add("\n[窗口控件]\n")
          .add(" > 启用 ",[](Data){settings.wndCtrls=true;return false;})
          .add(" > 禁用 ",[](Data){settings.wndCtrls=false;return false;})
          .show();
        return false;
    }
#endif
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
    #ifndef _DEV_
        if(isRunAsAdmin()){
    #endif
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
    #ifndef _DEV_
        }else{
            ui.add(" < 返回 ",exit,{},WC_RED);
            ui.add("\n (i) 需要管理员权限.");
        }
    #endif
        ui.show();
        return false;
    }
}
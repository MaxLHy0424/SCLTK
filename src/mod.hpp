#pragma once
#include"def.hpp"
#include"ui.hpp"
struct{
    bool wndFrontShow,wndAlpha,wndCtrls;
}config{};
bool configError{};
namespace Mod{
    inline auto isRunAsAdmin(){
        BOOL isAdmin{};
        PSID adminsGroup{};
        SID_IDENTIFIER_AUTHORITY ntAuthority{SECURITY_NT_AUTHORITY};
        if(
            AllocateAndInitializeSid(
                &ntAuthority,2,SECURITY_BUILTIN_DOMAIN_RID,
                DOMAIN_ALIAS_RID_ADMINS,0,0,0,0,0,0,&adminsGroup
            )
        ){
            CheckTokenMembership(nullptr,adminsGroup,&isAdmin);
            FreeSid(adminsGroup);
        }
        return isAdmin;
    }
    inline auto init(){
        system("chcp 936 > nul");
        SetConsoleTitle(WINDOW_TITLE);
        SetWindowLongPtr(
            GetConsoleWindow(),GWL_STYLE,(config.wndCtrls)
            ?(GetWindowLongPtr(GetConsoleWindow(),GWL_STYLE)|WS_SIZEBOX|WS_MAXIMIZEBOX|WS_MINIMIZEBOX)
            :(GetWindowLongPtr(GetConsoleWindow(),GWL_STYLE)&~WS_SIZEBOX&~WS_MAXIMIZEBOX&~WS_MINIMIZEBOX)
        );
        system("mode con cols=50 lines=25");
        SetLayeredWindowAttributes(GetConsoleWindow(),0,(config.wndAlpha)?(230):(255),LWA_ALPHA);
    }
    inline auto frontShow(){
        const HWND wndThis{GetConsoleWindow()};
        const DWORD idForeground{GetWindowThreadProcessId(wndThis,nullptr)},
                    idCurrent{GetCurrentThreadId()};
        while(true){
            AttachThreadInput(idCurrent,idForeground,TRUE);
            ShowWindow(wndThis,SW_SHOWNORMAL);
            SetWindowPos(wndThis,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
            SetWindowPos(wndThis,HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
            SetForegroundWindow(wndThis);
            AttachThreadInput(idCurrent,idForeground,FALSE);
            SetWindowPos(wndThis,HWND_TOPMOST,0,0,100,100,SWP_NOMOVE|SWP_NOSIZE);
            Sleep(100);
        }
    }
    inline auto exit(Data){
        return true;
    }
    inline auto info(Data){
        UI ui;
        ui.add("                    [ 关  于 ]\n\n")
          .add(" < 返回 ",Mod::exit,COLOR_RED)
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
    inline auto cmd(Data){
        system("cmd");
        if(!config.wndCtrls){
            init();
        }
        return false;
    }
#ifdef _THE_NEXT_MAJOR_UPDATE_
    inline auto configRead(){
        std::ifstream configFile("config.ini",std::ios::in);
        if(!configFile.is_open()){
            goto END;
        }
        {
            std::string line;
            while(std::getline(configFile,line)){
                if(line.at(0)=='#'){
                    continue;
                }
                if(line=="wndAlpha"){
                    config.wndAlpha=true;
                }else if(line=="wndCtrls"){
                    config.wndCtrls=true;
                }else if(line=="wndFrontShow"){
                    config.wndFrontShow=true;
                }else{
                    config={};
                    configError=true;
                    break;
                }
            }
        }
    END:
        configFile.close();
        return;
    }
    inline auto configEdit(Data){
        auto save{[](Data){
            std::string text;
            if(config.wndAlpha){
                text.append("wndAlpha\n");
            }if(config.wndCtrls){
                text.append("wndCtrls\n");
            }if(config.wndFrontShow){
                text.append("wndFrontShow\n");
            }
            std::ofstream configFile("config.ini",std::ios::out|std::ios::trunc);
            configFile.write(text.c_str(),text.size());
            configFile.close();
            return true;
        }};
        UI ui;
        ui.add("                    [ 设  置 ]\n\n")
          .add(" (i) 下次启动时生效.\n")
          .add(" < 保存并返回 ",save,COLOR_RED)
          .add("\n[半透明窗口]\n")
          .add(" > 启用 ",[](Data){config.wndAlpha=true;return false;})
          .add(" > 禁用 ",[](Data){config.wndAlpha=false;return false;})
          .add("\n[置顶窗口]\n")
          .add(" > 启用 ",[](Data){config.wndFrontShow=true;return false;})
          .add(" > 禁用 ",[](Data){config.wndFrontShow=false;return false;})
          .add("\n[窗口控件]\n")
          .add(" > 启用 ",[](Data){config.wndCtrls=true;return false;})
          .add(" > 禁用 ",[](Data){config.wndCtrls=false;return false;})
          .show();
        return false;
    }
#endif
    struct Rule final{
        std::vector<const char*> exe,svc;
    };
    struct{
        const Rule mythware,lenovo;
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
    class Op final{
    private:
        const char mod;
        const Rule &rule;
    public:
        inline explicit Op(const char mod,const Rule &rule):
            mod{mod},rule{rule}{}
        inline ~Op(){}
        inline auto operator()(Data){
#ifndef _THE_NEXT_MAJOR_UPDATE_
            if(!isRunAsAdmin()){
                puts(" (!) 请以管理员权限运行.\n");
                for(short i{3};i>0;--i){
                    printf("%d 秒后自动退出.\r",i);
                    Sleep(1000);
                }
                return false;
            }
#endif
            puts("=> 生成命令.");
            std::string cmd;
            switch(mod){
                case 'c':{
                    for(const auto &item:rule.exe){
                        cmd.append("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution options\\")
                           .append(item)
                           .append(".exe\" /f /t reg_sz /v debugger /d ? & taskKill /f /im \"")
                           .append(item)
                           .append(".exe\" & ");
                    }
                    for(const auto &item:rule.svc){
                        cmd.append("net stop ")
                           .append(item)
                           .append(" /y & ");
                    }
                    break;
                }case 'r':{
                    for(const auto &item:rule.exe){
                        cmd.append("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution options\\")
                           .append(item)
                           .append(".exe\" /f & ");
                    }
                    for(const auto &item:rule.svc){
                        cmd.append("net start ")
                           .append(item)
                           .append(" & ");
                    }
                    break;
                }
            }
            puts("=> 执行命令.");
            puts(std::string(50,'-').c_str());
            system(cmd.c_str());
            puts(std::string(50,'-').c_str());
            puts("=> 释放内存.");
            return false;
        }
    };
}
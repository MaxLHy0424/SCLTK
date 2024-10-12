#pragma once
#include"def.hpp"
#include"ui.hpp"
struct{
    bool wndFrontShow,wndAlpha,wndCtrls;
}config{};
bool configError{};
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
            CheckTokenMembership(nullptr,adminsGroup,&isAdmin);
            FreeSid(adminsGroup);
        }
        return isAdmin;
    }
    auto init(){
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
        ui.add("                    [ 关  于 ]\n\n")
          .add(" < 返回 ",Mod::exit,WCC_RED)
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
        if(!config.wndCtrls){
            init();
        }
        return false;
    }
#ifdef _THE_NEXT_MAJOR_UPDATE_
    auto configRead(){
        std::ifstream fs("config.ini",std::ios::in);
        if(!fs.is_open()){
            goto END;
        }
        {
            std::string text;
            while(std::getline(fs,text)){
                if(text.at(0)=='#'){
                    continue;
                }
                if(text=="wndAlpha"){
                    config.wndAlpha=true;
                }else if(text=="wndCtrls"){
                    config.wndCtrls=true;
                }else if(text=="wndFrontShow"){
                    config.wndFrontShow=true;
                }else{
                    config={};
                    configError=true;
                    break;
                }
            }
        }
    END:
        fs.close();
        return;
    }
    auto configEdit(Data){
        class Save final{
        public:
            explicit Save(){}
            ~Save(){}
            auto operator()(Data){
                std::string text;
                if(config.wndAlpha){
                    text.append("wndAlpha\n");
                }if(config.wndCtrls){
                    text.append("wndCtrls\n");
                }if(config.wndFrontShow){
                    text.append("wndFrontShow\n");
                }
                std::ofstream fs("config.ini",std::ios::out|std::ios::trunc);
                fs.write(text.c_str(),text.size());
                fs.close();
                return true;
            }
        };
        UI ui;
        ui.add("                    [ 设  置 ]\n\n")
          .add(" (i) 下次启动时生效.\n")
          .add(" < 放弃并返回 ",exit,WCC_RED)
          .add(" < 保存并返回 ",Save(),WCC_GREEN)
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
    struct{
        struct{
            const std::vector<const char*> exe,svc;
        }mythware;
        struct{
            const std::vector<const char*> exe,svc;
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
    class Op final{
    private:
        const char mod;
        const std::vector<const char*> &exe,&svc;
    public:
        explicit Op(
            const char mod,
            const std::vector<const char*> &exe,
            const std::vector<const char*> &svc
        ):mod{mod},exe{exe},svc{svc}{}
        ~Op(){}
        auto operator()(Data){
            puts("=> 生成命令.");
            std::string cmd;
            switch(mod){
                case 'c':{
                    for(const auto &ref:exe){
                        cmd.append("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution options\\")
                           .append(ref)
                           .append(".exe\" /f /t reg_sz /v debugger /d ? & taskKill /f /im ")
                           .append(ref)
                           .append(".exe & ");
                    }
                    for(const auto &ref:svc){
                        cmd.append("net stop ")
                           .append(ref)
                           .append(" /y & ");
                    }
                    break;
                }case 'r':{
                    for(const auto &ref:exe){
                        cmd.append("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution options\\")
                           .append(ref)
                           .append(".exe\" /f & ");
                    }
                    for(const auto &ref:svc){
                        cmd.append("net start ")
                           .append(ref)
                           .append(" & ");
                    }
                    break;
                }
            }
            puts("=> 执行命令.");
            printf("%s\n",std::string(50,'-').c_str());
            system(cmd.c_str());
            printf("%s\n",std::string(50,'-').c_str());
            puts("=> 释放内存.");
            return false;
        }
    };
}
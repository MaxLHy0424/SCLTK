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
            CheckTokenMembership(nullptr,adminsGroup,&isAdmin);
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
        ui.add("                    < 关  于 >\n\n")
          .add(" < 返回 ",Mod::exit,WC_RED)
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
#ifdef _THE_NEXT_MAJOR_UPDATE_
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
        class Save final{
        public:
            explicit Save(){}
            ~Save(){}
            auto operator()(Data){
                std::string item;
                if(settings.wndAlpha){
                    item=item+"wndAlpha\n";
                }if(settings.wndCtrls){
                    item=item+"wndCtrls\n";
                }if(settings.wndFrontShow){
                    item=item+"wndFrontShow\n";
                }
                std::ofstream file("settings.ini",std::ios::out|std::ios::trunc);
                file.write(item.c_str(),item.size());
                file.close();
                return true;
            }
        };
        UI ui;
        ui.add("                    < 设  置 >\n\n")
          .add(" (i) 下次启动时生效.\n")
          .add(" < 放弃并返回 ",exit,WC_RED)
          .add(" < 保存并返回 ",Save(),WC_GREEN)
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
        const char key;
        const std::vector<const char*> &exe,&svc;
    public:
        explicit Op(
            const char key,
            const std::vector<const char*> &exe,const std::vector<const char*> &svc
        ):key{key},exe{exe},svc{svc}{}
        ~Op(){}
        auto operator()(Data){
            std::string cmd;
            switch(key){
                case 'c':{
                    for(const auto &ref:exe){
                        cmd=std::string()
                            +"reg add "
                            "\"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution options\\"
                            +ref+".exe\" /f /t reg_sz /v debugger /d ?"
                            " && taskKill /f /im "+ref+".exe";
                        system(cmd.c_str());
                    }
                    for(const auto &ref:svc){
                        cmd=std::string()+"net stop "+ref+" /y";
                        system(cmd.c_str());
                    }
                    break;
                }case 'r':{
                    for(const auto &ref:exe){
                        cmd=std::string()
                            +"reg delete "
                            "\"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution options\\"
                            +ref+".exe\" /f";
                        system(cmd.c_str());
                    }
                    for(const auto &ref:svc){
                        cmd=std::string()+"net start "+ref;
                        system(cmd.c_str());
                    }
                    break;
                }
            }
            return false;
        }
    };
}
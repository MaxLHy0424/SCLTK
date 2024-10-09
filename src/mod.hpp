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
        if(!config.wndCtrls){
            init();
        }
        return false;
    }
#ifdef _THE_NEXT_UPDATE_
    auto configRead(){
        std::ifstream fs("config.ini",std::ios::in);
        if(!fs.is_open()){
            goto END;
        }
        {
            std::string item;
            while(std::getline(fs,item)){
                if(item.at(0)=='#'){
                    continue;
                }
                if(item=="wndAlpha"){
                    config.wndAlpha=true;
                }else if(item=="wndCtrls"){
                    config.wndCtrls=true;
                }else if(item=="wndFrontShow"){
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
                std::string item;
                if(config.wndAlpha){
                    item=item+"wndAlpha\n";
                }if(config.wndCtrls){
                    item=item+"wndCtrls\n";
                }if(config.wndFrontShow){
                    item=item+"wndFrontShow\n";
                }
                std::ofstream fs("config.ini",std::ios::out|std::ios::trunc);
                fs.write(item.c_str(),item.size());
                fs.close();
                return true;
            }
        };
        UI ui;
        ui.add("                    < 设  置 >\n\n")
          .add(" (i) 下次启动时生效.\n")
          .add(" < 放弃并返回 ",exit,WC_RED)
          .add(" < 保存并返回 ",Save(),WC_GREEN)
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
            std::vector<const char*> exe,svc;
        }mythware;
        struct{
            std::vector<const char*> exe,svc;
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
        char mod;
        std::vector<const char*> &exe,&svc;
    public:
        explicit Op(char mod,std::vector<const char*> &exe,std::vector<const char*> &svc):
            mod{mod},exe{exe},svc{svc}{}
        ~Op(){}
        auto operator()(Data){
            using namespace std::string_literals;
            std::string cmd;
            switch(mod){
                case 'c':{
                    for(auto &ref:exe){
                        cmd="reg add "
                        "\"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution options\\"s
                        +ref+".exe\" /f /t reg_sz /v debugger /d ? && taskKill /f /im "+ref+".exe";
                        system(cmd.c_str());
                    }
                    for(auto &ref:svc){
                        cmd="net stop "s+ref+" /y";
                        system(cmd.c_str());
                    }
                    break;
                }case 'r':{
                    for(auto &ref:exe){
                        cmd="reg delete "
                        "\"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution options\\"s
                        +ref+".exe\" /f";
                        system(cmd.c_str());
                    }
                    for(auto &ref:svc){
                        cmd="net start "s+ref;
                        system(cmd.c_str());
                    }
                    break;
                }
            }
            return false;
        }
    };
}
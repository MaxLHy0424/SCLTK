#pragma once
#include"def.hpp"
#include"ui.hpp"
struct{
#ifdef _NEXT_
    bool frontShow,alpha,hideCloseCtrl;
#else
    bool frontShow,alpha,wndCtrls;
#endif
}attrsWnd{};
#ifndef _NEXT_
bool configError{};
#endif
namespace Mod{
#ifdef _NEXT_
    class StringForOpRule final{
    private:
        char *const str;
    public:
        inline StringForOpRule()=delete;
        inline StringForOpRule(const char *s):
            str{new char[strlen(s)+1]}
        {
            strcpy(this->str,s);
        }
        inline ~StringForOpRule(){
            delete[] str;
        }
        inline const auto get()const{
            return str;
        }
    };
#endif
    struct Rule final{
#ifdef _NEXT_
        std::vector<StringForOpRule> exe,svc;
#else
        std::vector<const char*> exe,svc;
#endif
    };
    struct{
        const Rule mythware,lenovo;
#ifdef _NEXT_
        Rule custom;
#endif
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
#ifdef _NEXT_
        ,{}
#endif
    };
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
#ifdef _NEXT_
        SetWindowLongPtr(
            GetConsoleWindow(),GWL_STYLE,
            GetWindowLongPtr(GetConsoleWindow(),GWL_STYLE)&~WS_SIZEBOX&~WS_MAXIMIZEBOX&~WS_MINIMIZEBOX
        );
        EnableMenuItem(
            GetSystemMenu(GetConsoleWindow(),(attrsWnd.hideCloseCtrl)?(FALSE):(TRUE)),
            SC_CLOSE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED
        );
#else
        SetWindowLongPtr(
            GetConsoleWindow(),GWL_STYLE,(attrsWnd.wndCtrls)
            ?(GetWindowLongPtr(GetConsoleWindow(),GWL_STYLE)|WS_SIZEBOX|WS_MAXIMIZEBOX|WS_MINIMIZEBOX)
            :(GetWindowLongPtr(GetConsoleWindow(),GWL_STYLE)&~WS_SIZEBOX&~WS_MAXIMIZEBOX&~WS_MINIMIZEBOX)
        );
#endif
        system("mode con cols=50 lines=25");
        SetLayeredWindowAttributes(GetConsoleWindow(),0,(attrsWnd.alpha)?(230):(255),LWA_ALPHA);
    }
#ifndef _NEXT_
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
#endif
    inline auto exit(Data){
        return true;
    }
    inline auto info(Data){
#ifdef _NEXT_
        auto visitRepoWebPage{[](Data){
            ShellExecute(nullptr,"",INFO_REPO_URL,nullptr,nullptr,SW_SHOWNORMAL);
            return false;
        }};
#endif
        Ui ui;
        ui.add("                    [ 关  于 ]\n\n")
          .add(" < 返回 ",Mod::exit,CONSOLE_RED)
          .add("\n[名称]\n")
          .add(" " INFO_NAME)
          .add("\n[版本]\n")
          .add(" " INFO_VERSION)
          .add("\n[仓库]\n")
#ifdef _NEXT_
          .add(" (i) 点击访问 URL.\n")
          .add(" " INFO_REPO_URL,std::move(visitRepoWebPage))
#else
          .add(" " INFO_REPO_URL)
#endif
          .add("\n[许可证]\n")
          .add(" " INFO_LICENSE)
          .add(" (C) 2023 " INFO_DEVELOPER ". All Rights Reserved.")
          .show();
        return false;
    }
    inline auto cmd(Data){
        system("cmd");
#ifdef _NEXT_
        init();
#else
        if(!attrsWnd.wndCtrls){
            init();
        }
#endif
        return false;
    }
#ifdef _NEXT_
    class OpConfig final{
    private:
        const char mod;
        bool isOnlyLoadCustomRule;
        inline auto load(){
            std::ifstream configFile{"config.ini",std::ios::in};
            if(!configFile.is_open()){
                goto END;
            }
            {
                puts("==> 加载配置文件.");
                if(!isOnlyLoadCustomRule){
                    attrsWnd={};
                }
                rule.custom.exe.clear(),rule.custom.svc.clear();
                std::string line;
                enum{Settings=0,RuleExe=1,RuleSvc=2} configItem{Settings};
                while(std::getline(configFile,line)){
                    if(line.empty()||line.at(0)=='#'){
                        continue;
                    }
                    if(line=="<Settings>"){
                        configItem=Settings;
                        continue;
                    }else if(line=="<RuleExe>"){
                        configItem=RuleExe;
                        continue;
                    }else if(line=="<RuleSvc>"){
                        configItem=RuleSvc;
                        continue;
                    }
                    switch(configItem){
                        case Settings:{
                            if(isOnlyLoadCustomRule){
                                continue;
                            }
                            if(line=="frontShow"){
                                attrsWnd.frontShow=true;
                            }else if(line=="alpha"){
                                attrsWnd.alpha=true;
                            }else if(line=="hideCloseCtrl"){
                                attrsWnd.hideCloseCtrl=true;
                            }
                            break;
                        }case RuleExe:{
                            rule.custom.exe.emplace_back(line.c_str());
                            break;
                        }case RuleSvc:{
                            rule.custom.svc.emplace_back(line.c_str());
                            break;
                        }
                    }
                }
            }
        END:
            configFile.close();
            return;
        }
        inline auto edit(){
            auto save{[&](Data){
                isOnlyLoadCustomRule=true;
                load();
                puts("==> 格式化保存配置文件.");
                std::string text;
                text.append("<Settings>\n");
                if(attrsWnd.frontShow){
                    text.append("frontShow\n");
                }
                if(attrsWnd.alpha){
                    text.append("alpha\n");
                }
                if(attrsWnd.hideCloseCtrl){
                    text.append("hideCloseCtrl\n");
                }
                text.append("<RuleExe>\n");
                if(!rule.custom.exe.empty()){
                    for(const auto &item:rule.custom.exe){
                        text.append(item.get()).push_back('\n');
                    }
                }
                text.append("<RuleSvc>\n");
                if(!rule.custom.exe.empty()){
                    for(const auto &item:rule.custom.svc){
                        text.append(item.get()).push_back('\n');
                    }
                }
                std::ofstream configFile{"config.ini",std::ios::out|std::ios::trunc};
                configFile.write(text.c_str(),text.size());
                configFile.close();
                return true;
            }};
            auto openConfigFile{[](Data){
                puts("==> 打开配置文件.");
                ShellExecute(nullptr,"","config.ini",nullptr,nullptr,SW_SHOWNORMAL);
                return false;
            }};
            Ui ui;
            ui.add("                    [ 配  置 ]\n\n")
              .add(" (i) 此处设置将在下次启动时生效.\n     可通过 <RuleExe> 与 <RuleSvc> 自定义规则.\n")
              .add(" < 格式化保存并返回 ",std::move(save),CONSOLE_RED)
              .add(" > 打开配置文件 ",std::move(openConfigFile))
              .add("\n[半透明窗口]\n")
              .add(" > 启用 ",[](Data){attrsWnd.alpha=true;return false;})
              .add(" > 禁用 ",[](Data){attrsWnd.alpha=false;return false;})
              .add("\n[置顶窗口]\n")
              .add(" > 启用 ",[](Data){attrsWnd.frontShow=true;return false;})
              .add(" > 禁用 ",[](Data){attrsWnd.frontShow=false;return false;})
              .add("\n[隐藏窗口关闭控件]\n")
              .add(" > 启用 ",[](Data){attrsWnd.hideCloseCtrl=true;return false;})
              .add(" > 禁用 ",[](Data){attrsWnd.hideCloseCtrl=false;return false;})
              .show();
            return false;
        }
    public:
        inline explicit OpConfig(const char mod):
            mod{mod},isOnlyLoadCustomRule{}
        {}
        inline ~OpConfig(){}
        inline auto operator()(Data){
            switch(mod){
                case 'r':{
                    load();
                    break;
                }case 'e':{
                    edit();
                    break;
                }
            }
            return false;
        }
    };
#endif
    class OpSys final{
    private:
        const char mod;
        const Rule &rule;
    public:
        inline explicit OpSys(const char mod,const Rule &rule):
            mod{mod},rule{rule}
        {}
        inline ~OpSys(){}
        inline auto operator()(Data)const{
#ifdef _NEXT_
            if((rule.exe.empty())&&(rule.svc.empty())){
                puts("\n (!) 规则为空.\n");
                for(unsigned short i{3};i>0;--i){
                    printf(" %hu 秒后返回.\r",i);
                    Sleep(1000);
                }
                return false;
            }
#else
            if(!isRunAsAdmin()){
                puts("\n (!) 需要提权.\n");
                for(unsigned short i{3};i>0;--i){
                    printf(" %hu 秒后返回.\r",i);
                    Sleep(1000);
                }
                return false;
            }
#endif
            puts("==> 生成命令.");
            std::string cmd;
            switch(mod){
                case 'c':{
                    for(const auto &item:rule.exe){
                        cmd.append("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution options\\")
#ifdef _NEXT_
                           .append(item.get())
#else
                           .append(item)
#endif
                           .append(".exe\" /f /t reg_sz /v debugger /d ? & taskKill /f /im \"")
#ifdef _NEXT_
                           .append(item.get())
#else
                           .append(item)
#endif
                           .append(".exe\" & ");
                    }
                    for(const auto &item:rule.svc){
                        cmd.append("net stop \"")
#ifdef _NEXT_
                           .append(item.get())
#else
                           .append(item)
#endif
                           .append("\" /y & ");
                    }
                    break;
                }case 'r':{
                    for(const auto &item:rule.exe){
                        cmd.append("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution options\\")
#ifdef _NEXT_
                           .append(item.get())
#else
                           .append(item)
#endif
                           .append(".exe\" /f & ");
                    }
                    for(const auto &item:rule.svc){
                        cmd.append("net start \"")
#ifdef _NEXT_
                           .append(item.get())
#else
                           .append(item)
#endif
                           .append("\" & ");
                    }
                    break;
                }
            }
            puts("==> 执行命令.");
            puts(std::string(50,'-').c_str());
            system(cmd.c_str());
            puts(std::string(50,'-').c_str());
            puts("==> 释放内存.");
            return false;
        }
    };
}
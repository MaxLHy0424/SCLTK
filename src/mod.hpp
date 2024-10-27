#pragma once
#include"def.hpp"
#include"ui.hpp"
struct{
#ifdef _THE_NEXT_MAJOR_UPDATE_
    bool wndFrontShow,wndAlpha,wndHideCloseCtrl;
#else
    bool wndFrontShow,wndAlpha,wndCtrls;
#endif
}config{};
bool configError{};
namespace Mod{
#ifdef _THE_NEXT_MAJOR_UPDATE_
    class StringForOpRule{
    private:
        char *const str;
    public:
        inline StringForOpRule()=delete;
        inline StringForOpRule(const char *s):
            str{new char[strlen(s)+1]}{
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
#ifdef _THE_NEXT_MAJOR_UPDATE_
        std::vector<StringForOpRule> exe,svc;
#else
        std::vector<const char*> exe,svc;
#endif
    };
    struct{
        const Rule mythware,lenovo;
#ifdef _THE_NEXT_MAJOR_UPDATE_
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
#ifdef _THE_NEXT_MAJOR_UPDATE_
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
#ifdef _THE_NEXT_MAJOR_UPDATE_
        SetWindowLongPtr(
            GetConsoleWindow(),GWL_STYLE,
            GetWindowLongPtr(GetConsoleWindow(),GWL_STYLE)&~WS_SIZEBOX&~WS_MAXIMIZEBOX&~WS_MINIMIZEBOX
        );
        EnableMenuItem(
            GetSystemMenu(GetConsoleWindow(),(config.wndHideCloseCtrl)?(FALSE):(TRUE)),
            SC_CLOSE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED
        );
#else
        SetWindowLongPtr(
            GetConsoleWindow(),GWL_STYLE,(config.wndCtrls)
            ?(GetWindowLongPtr(GetConsoleWindow(),GWL_STYLE)|WS_SIZEBOX|WS_MAXIMIZEBOX|WS_MINIMIZEBOX)
            :(GetWindowLongPtr(GetConsoleWindow(),GWL_STYLE)&~WS_SIZEBOX&~WS_MAXIMIZEBOX&~WS_MINIMIZEBOX)
        );
#endif
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
    inline auto cmd(Data){
        system("cmd");
#ifdef _THE_NEXT_MAJOR_UPDATE_
        init();
#else
        if(!config.wndCtrls){
            init();
        }
#endif
        return false;
    }
#ifdef _THE_NEXT_MAJOR_UPDATE_
    class OpConfig final{
    private:
        const char mod;
        bool isOnlyLoadCustomRule;
        inline auto read(){
            std::ifstream configFile{"config.ini",std::ios::in};
            if(!configFile.is_open()){
                goto END;
            }
            {
                puts("=> 读取配置文件.");
                if(!isOnlyLoadCustomRule){
                    config={};
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
                            if(line=="wndFrontShow"){
                                config.wndFrontShow=true;
                            }else if(line=="wndAlpha"){
                                config.wndAlpha=true;
                            }else if(line=="wndHideCloseCtrl"){
                                config.wndHideCloseCtrl=true;
                            }else{
                                configError=true;
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
                read();
                puts("=> 格式化保存配置文件.");
                std::string text;
                text.append("<Settings>\n");
                if(config.wndFrontShow){
                    text.append("wndFrontShow\n");
                }
                if(config.wndAlpha){
                    text.append("wndAlpha\n");
                }
                if(config.wndHideCloseCtrl){
                    text.append("wndHideCloseCtrl\n");
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
                puts("=> 打开配置文件.");
                ShellExecute(nullptr,"","config.ini",nullptr,nullptr,SW_SHOWNORMAL);
                return false;
            }};
            UI ui;
            ui.add("                    [ 配  置 ]\n\n")
              .add(" (i) 此处设置将在下次启动时生效.\n     可通过 <RuleExe> 与 <RuleSvc> 自定义规则.\n")
              .add(" < 格式化保存并返回 ",std::move(save),WCC_RED)
              .add(" > 打开配置文件 ",std::move(openConfigFile))
              .add("\n[半透明窗口]\n")
              .add(" > 启用 ",[](Data){config.wndAlpha=true;return false;})
              .add(" > 禁用 ",[](Data){config.wndAlpha=false;return false;})
              .add("\n[置顶窗口]\n")
              .add(" > 启用 ",[](Data){config.wndFrontShow=true;return false;})
              .add(" > 禁用 ",[](Data){config.wndFrontShow=false;return false;})
              .add("\n[隐藏窗口关闭控件]\n")
              .add(" > 启用 ",[](Data){config.wndHideCloseCtrl=true;return false;})
              .add(" > 禁用 ",[](Data){config.wndHideCloseCtrl=false;return false;})
              .show();
            return false;
        }
    public:
        inline explicit OpConfig(const char mod):
            mod{mod},isOnlyLoadCustomRule{false}{}
        inline ~OpConfig(){}
        inline auto operator()(Data){
            switch(mod){
                case 'r':{
                    read();
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
            mod{mod},rule{rule}{}
        inline ~OpSys(){}
        inline auto operator()(Data)const{
#ifdef _THE_NEXT_MAJOR_UPDATE_
            if((rule.exe.empty())&&(rule.svc.empty())){
                puts(" (!) 规则为空.\n");
                for(unsigned short i{3};i>0;--i){
                    printf("%hu 秒后返回.\r",i);
                    Sleep(1000);
                }
                return false;
            }
#else
            if(!isRunAsAdmin()){
                puts(" (!) 需要提权.\n");
                for(unsigned short i{3};i>0;--i){
                    printf("%hu 秒后返回.\r",i);
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
#ifdef _THE_NEXT_MAJOR_UPDATE_
                           .append(item.get())
#else
                           .append(item)
#endif
                           .append(".exe\" /f /t reg_sz /v debugger /d ? & taskKill /f /im \"")
#ifdef _THE_NEXT_MAJOR_UPDATE_
                           .append(item.get())
#else
                           .append(item)
#endif
                           .append(".exe\" & ");
                    }
                    for(const auto &item:rule.svc){
                        cmd.append("net stop \"")
#ifdef _THE_NEXT_MAJOR_UPDATE_
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
#ifdef _THE_NEXT_MAJOR_UPDATE_
                           .append(item.get())
#else
                           .append(item)
#endif
                           .append(".exe\" /f & ");
                    }
                    for(const auto &item:rule.svc){
                        cmd.append("net start \"")
#ifdef _THE_NEXT_MAJOR_UPDATE_
                           .append(item.get())
#else
                           .append(item)
#endif
                           .append("\" & ");
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
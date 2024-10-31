#pragma once
#include"def.hpp"
#include"console_ui.hpp"
struct{
#ifdef _NEXT_
    bool front_show_wnd,alpha_wnd,hide_wnd_close_ctrl;
#else
    bool front_show_wnd,alpha_wnd,wnd_ctrls;
#endif
}config_data{};
#ifndef _NEXT_
bool config_error{};
#endif
namespace mod{
#ifdef _NEXT_
    class simple_string final{
    private:
        char *const m_str;
    public:
        inline simple_string()=delete;
        inline simple_string(const char *_s):
            m_str{new char[strlen(_s)+1]}
        {
            strcpy(this->m_str,_s);
        }
        inline ~simple_string(){
            delete[] m_str;
        }
        inline const auto get()const{
            return m_str;
        }
    };
#endif
    struct sys_rule final{
#ifdef _NEXT_
        std::vector<simple_string> exe,svc;
#else
        std::vector<const char*> exe,svc;
#endif
    };
    struct{
        const sys_rule mythware,lenovo;
#ifdef _NEXT_
        sys_rule custom;
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
    inline auto is_run_as_admin(){
        BOOL is_admin{};
        PSID admins_group{};
        SID_IDENTIFIER_AUTHORITY nt_authority{SECURITY_NT_AUTHORITY};
        if(
            AllocateAndInitializeSid(
                &nt_authority,2,SECURITY_BUILTIN_DOMAIN_RID,
                DOMAIN_ALIAS_RID_ADMINS,0,0,0,0,0,0,&admins_group
            )
        ){
            CheckTokenMembership(nullptr,admins_group,&is_admin);
            FreeSid(admins_group);
        }
        return is_admin;
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
            GetSystemMenu(
                GetConsoleWindow(),
                (config_data.hide_wnd_close_ctrl)?(FALSE):(TRUE)
            ),
            SC_CLOSE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED
        );
#else
        SetWindowLongPtr(
            GetConsoleWindow(),GWL_STYLE,(config_data.wnd_ctrls)
            ?(GetWindowLongPtr(GetConsoleWindow(),GWL_STYLE)|WS_SIZEBOX|WS_MAXIMIZEBOX|WS_MINIMIZEBOX)
            :(GetWindowLongPtr(GetConsoleWindow(),GWL_STYLE)&~WS_SIZEBOX&~WS_MAXIMIZEBOX&~WS_MINIMIZEBOX)
        );
#endif
        system("mode con cols=50 lines=25");
        SetLayeredWindowAttributes(
            GetConsoleWindow(),0,
            (config_data.alpha_wnd)?(230):(255),
            LWA_ALPHA
        );
    }
#ifndef _NEXT_
    inline auto front_show_wnd(){
        const HWND this_wnd{GetConsoleWindow()};
        const DWORD foreground_id{GetWindowThreadProcessId(this_wnd,nullptr)},
                    current_id{GetCurrentThreadId()};
        while(true){
            AttachThreadInput(current_id,foreground_id,TRUE);
            ShowWindow(this_wnd,SW_SHOWNORMAL);
            SetWindowPos(this_wnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
            SetWindowPos(this_wnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
            SetForegroundWindow(this_wnd);
            AttachThreadInput(current_id,foreground_id,FALSE);
            SetWindowPos(this_wnd,HWND_TOPMOST,0,0,100,100,SWP_NOMOVE|SWP_NOSIZE);
            Sleep(100);
        }
    }
#endif
    inline auto exit(ui_data){
        return true;
    }
    inline auto info(ui_data){
#ifdef _NEXT_
        auto visit_repo_webpage{[](ui_data){
            ShellExecute(nullptr,"",INFO_REPO_URL,nullptr,nullptr,SW_SHOWNORMAL);
            return false;
        }};
#endif
        console_ui ui;
        ui.add("                    [ 关  于 ]\n\n")
          .add(" < 返回 ",mod::exit,CONSOLE_RED)
          .add("\n[名称]\n")
          .add(" " INFO_NAME)
          .add("\n[版本]\n")
          .add(" " INFO_VERSION)
          .add("\n[仓库]\n")
#ifdef _NEXT_
          .add(" (i) 点击访问 URL.\n")
          .add(" " INFO_REPO_URL,std::move(visit_repo_webpage))
#else
          .add(" " INFO_REPO_URL)
#endif
          .add("\n[许可证]\n")
          .add(" " INFO_LICENSE)
          .add(" (C) 2023 " INFO_DEVELOPER ". All Rights Reserved.")
          .show();
        return false;
    }
    inline auto cmd(ui_data){
        system("cmd");
#ifdef _NEXT_
        init();
#else
        if(!config_data.wnd_ctrls){
            init();
        }
#endif
        return false;
    }
#ifdef _NEXT_
    class config_op final{
    private:
        const char m_mode;
        bool m_is_only_load_custom_rule;
        inline auto load(){
            std::ifstream config_file{"config_data.ini",std::ios::in};
            if(!config_file.is_open()){
                goto END;
            }
            {
                puts("==> 加载配置文件.");
                if(!m_is_only_load_custom_rule){
                    config_data={};
                }
                rule.custom.exe.clear(),rule.custom.svc.clear();
                std::string line;
                enum{t_settings=0,t_rule_exe=1,t_rule_svc=2} config_item{t_settings};
                while(std::getline(config_file,line)){
                    if(line.empty()||line.at(0)=='#'){
                        continue;
                    }
                    if(line=="<settings>"){
                        config_item=t_settings;
                        continue;
                    }else if(line=="<rule_exe>"){
                        config_item=t_rule_exe;
                        continue;
                    }else if(line=="<rule_svc>"){
                        config_item=t_rule_svc;
                        continue;
                    }
                    switch(config_item){
                        case t_settings:{
                            if(m_is_only_load_custom_rule){
                                continue;
                            }
                            if(line=="front_show_wnd"){
                                config_data.front_show_wnd=true;
                            }else if(line=="alpha_wnd"){
                                config_data.alpha_wnd=true;
                            }else if(line=="hide_wnd_close_ctrl"){
                                config_data.hide_wnd_close_ctrl=true;
                            }
                            break;
                        }case t_rule_exe:{
                            rule.custom.exe.emplace_back(line.c_str());
                            break;
                        }case t_rule_svc:{
                            rule.custom.svc.emplace_back(line.c_str());
                            break;
                        }
                    }
                }
            }
        END:
            config_file.close();
            return;
        }
        inline auto edit(){
            auto save{[&](ui_data){
                m_is_only_load_custom_rule=true;
                load();
                puts("==> 格式化保存配置文件.");
                std::string text;
                text.append("<settings>\n");
                if(config_data.front_show_wnd){
                    text.append("front_show_wnd\n");
                }
                if(config_data.alpha_wnd){
                    text.append("alpha_wnd\n");
                }
                if(config_data.hide_wnd_close_ctrl){
                    text.append("hide_wnd_close_ctrl\n");
                }
                text.append("<rule_exe>\n");
                if(!rule.custom.exe.empty()){
                    for(const auto &item:rule.custom.exe){
                        text.append(item.get()).push_back('\n');
                    }
                }
                text.append("<rule_svc>\n");
                if(!rule.custom.exe.empty()){
                    for(const auto &item:rule.custom.svc){
                        text.append(item.get()).push_back('\n');
                    }
                }
                std::ofstream config_file{"config_data.ini",std::ios::out|std::ios::trunc};
                config_file.write(text.c_str(),text.size());
                config_file.close();
                return true;
            }};
            auto open_config_file{[](ui_data){
                puts("==> 打开配置文件.");
                ShellExecute(nullptr,"","config_data.ini",nullptr,nullptr,SW_SHOWNORMAL);
                return false;
            }};
            console_ui ui;
            ui.add("                    [ 配  置 ]\n\n")
              .add(" (i) 此处设置将在下次启动时生效.\n     可通过 <rule_exe> 与 <rule_svc> 自定义规则.\n")
              .add(" < 格式化保存并返回 ",std::move(save),CONSOLE_RED)
              .add(" > 打开配置文件 ",std::move(open_config_file))
              .add("\n[半透明窗口]\n")
              .add(" > 启用 ",[](ui_data){config_data.alpha_wnd=true;return false;})
              .add(" > 禁用 ",[](ui_data){config_data.alpha_wnd=false;return false;})
              .add("\n[置顶窗口]\n")
              .add(" > 启用 ",[](ui_data){config_data.front_show_wnd=true;return false;})
              .add(" > 禁用 ",[](ui_data){config_data.front_show_wnd=false;return false;})
              .add("\n[隐藏窗口关闭控件]\n")
              .add(" > 启用 ",[](ui_data){config_data.hide_wnd_close_ctrl=true;return false;})
              .add(" > 禁用 ",[](ui_data){config_data.hide_wnd_close_ctrl=false;return false;})
              .show();
            return false;
        }
    public:
        inline explicit config_op(const char _mode):
            m_mode{_mode},
            m_is_only_load_custom_rule{}
        {}
        inline ~config_op(){}
        inline auto operator()(ui_data){
            switch(m_mode){
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
    class sys_op final{
    private:
        const char m_mode;
        const sys_rule &m_rule;
    public:
        inline explicit sys_op(const char _mode,const sys_rule &_rule):
            m_mode{_mode},
            m_rule{_rule}
        {}
        inline ~sys_op(){}
        inline auto operator()(ui_data)const{
#ifdef _NEXT_
            if((m_rule.exe.empty())&&(m_rule.svc.empty())){
                puts("\n (!) 规则为空.\n");
                for(unsigned short i{3};i>0;--i){
                    printf(" %hu 秒后返回.\r",i);
                    Sleep(1000);
                }
                return false;
            }
#else
            if(!is_run_as_admin()){
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
            switch(m_mode){
                case 'c':{
                    for(const auto &item:m_rule.exe){
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
                    for(const auto &item:m_rule.svc){
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
                    for(const auto &item:m_rule.exe){
                        cmd.append("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution options\\")
#ifdef _NEXT_
                           .append(item.get())
#else
                           .append(item)
#endif
                           .append(".exe\" /f & ");
                    }
                    for(const auto &item:m_rule.svc){
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
#pragma once
#include"info.hpp"
#include"console_ui.hpp"
#include<windows.h>
#include<vector>
#include<string>
#ifdef _NEXT_
#include<fstream>
#endif
inline struct{
#ifdef _NEXT_
    bool front_show_window,translucent_window,hide_window_close_ctrl;
#else
    bool front_show_window,translucent_window,window_ctrls;
#endif
}config_data{};
#ifndef _NEXT_
inline bool config_error{};
#endif
namespace mod{
#ifdef _NEXT_
    class simple_string final{
    private:
        char *const str_;
    public:
        inline const auto get()const{
            return str_;
        }
        inline simple_string()=delete;
        inline simple_string(const char *_s):
            str_{new char[strlen(_s)+1]}
        {
            strcpy(this->str_,_s);
        }
        inline ~simple_string(){
            delete[] str_;
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
    inline struct{
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
#ifndef _NEXT_
    inline auto init(){
        system("chcp 936 > nul");
        SetConsoleTitle("CRCSN");
        SetWindowLongPtr(
            GetConsoleWindow(),GWL_STYLE,(config_data.window_ctrls)
            ?(GetWindowLongPtr(GetConsoleWindow(),GWL_STYLE)|WS_SIZEBOX|WS_MAXIMIZEBOX|WS_MINIMIZEBOX)
            :(GetWindowLongPtr(GetConsoleWindow(),GWL_STYLE)&~WS_SIZEBOX&~WS_MAXIMIZEBOX&~WS_MINIMIZEBOX)
        );
        system("mode con cols=50 lines=25");
        SetLayeredWindowAttributes(
            GetConsoleWindow(),0,
            (config_data.translucent_window)?(230):(255),
            LWA_ALPHA
        );
    }
    inline auto front_show_window(){
        const HWND this_window{GetConsoleWindow()};
        const DWORD foreground_id{GetWindowThreadProcessId(this_window,nullptr)},
                    current_id{GetCurrentThreadId()};
        while(true){
            AttachThreadInput(current_id,foreground_id,TRUE);
            ShowWindow(this_window,SW_SHOWNORMAL);
            SetWindowPos(this_window,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
            SetWindowPos(this_window,HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
            SetForegroundWindow(this_window);
            AttachThreadInput(current_id,foreground_id,FALSE);
            SetWindowPos(this_window,HWND_TOPMOST,0,0,100,100,SWP_NOMOVE|SWP_NOSIZE);
            Sleep(100);
        }
    }
#endif
    inline auto exit(console_ui::fn_args){
        return true;
    }
    inline auto info(console_ui::fn_args){
#ifdef _NEXT_
        auto visit_repo_webpage{[](console_ui::fn_args){
            ShellExecute(nullptr,"",INFO_REPO_URL,nullptr,nullptr,SW_SHOWNORMAL);
            return false;
        }};
#endif
        console_ui ui;
        ui.add("                    [ 关  于 ]\n\n")
          .add(" < 返回 ",mod::exit,CUI_TEXT_RED)
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
    inline auto cmd(console_ui::fn_args){
        system("cmd");
#ifdef _NEXT_
        console_ui{}.set_window(936,"CRCSN",50,25,true,false,true,255);
#else
        if(!config_data.window_ctrls){
            init();
        }
#endif
        return false;
    }
#ifdef _NEXT_
    class config_op final{
    private:
        const char mode_;
        bool is_only_load_custom_rule_;
        inline auto load_(){
            std::ifstream config_file{"config.ini",std::ios::in};
            if(!config_file.is_open()){
                goto END;
            }
            {
                puts("-> 加载配置文件.");
                if(!is_only_load_custom_rule_){
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
                            if(is_only_load_custom_rule_){
                                continue;
                            }
                            if(line=="front_show_window"){
                                config_data.front_show_window=true;
                            }else if(line=="translucent_window"){
                                config_data.translucent_window=true;
                            }else if(line=="hide_window_close_ctrl"){
                                config_data.hide_window_close_ctrl=true;
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
        inline auto edit_(){
            auto save{[&](console_ui::fn_args){
                is_only_load_custom_rule_=true;
                load_();
                puts("-> 格式化保存配置文件.");
                std::string text;
                text.append("<settings>\n");
                if(config_data.front_show_window){
                    text.append("front_show_window\n");
                }
                if(config_data.translucent_window){
                    text.append("translucent_window\n");
                }
                if(config_data.hide_window_close_ctrl){
                    text.append("hide_window_close_ctrl\n");
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
                std::ofstream config_file{"config.ini",std::ios::out|std::ios::trunc};
                config_file.write(text.c_str(),text.size());
                config_file.close();
                return true;
            }};
            auto open_config_file{[](console_ui::fn_args){
                puts("-> 打开配置文件.");
                ShellExecute(nullptr,"","config.ini",nullptr,nullptr,SW_SHOWNORMAL);
                return false;
            }};
            console_ui ui;
            ui.add("                    [ 配  置 ]\n\n")
              .add(" (i) 此处设置将在下次启动时生效.\n     可通过 <rule_exe> 与 <rule_svc> 自定义规则.\n")
              .add(" < 格式化保存并返回 ",std::move(save),CUI_TEXT_RED)
              .add(" > 打开配置文件 ",std::move(open_config_file))
              .add("\n[半透明窗口]\n")
              .add(" > 启用 ",[](console_ui::fn_args){config_data.translucent_window=true;return false;})
              .add(" > 禁用 ",[](console_ui::fn_args){config_data.translucent_window=false;return false;})
              .add("\n[置顶窗口]\n")
              .add(" > 启用 ",[](console_ui::fn_args){config_data.front_show_window=true;return false;})
              .add(" > 禁用 ",[](console_ui::fn_args){config_data.front_show_window=false;return false;})
              .add("\n[隐藏窗口关闭控件]\n")
              .add(" > 启用 ",[](console_ui::fn_args){config_data.hide_window_close_ctrl=true;return false;})
              .add(" > 禁用 ",[](console_ui::fn_args){config_data.hide_window_close_ctrl=false;return false;})
              .show();
            return false;
        }
    public:
        inline auto operator()(console_ui::fn_args){
            switch(mode_){
                case 'r':{
                    load_();
                    break;
                }case 'e':{
                    edit_();
                    break;
                }
            }
            return false;
        }
        inline explicit config_op(const char _mode):
            mode_{_mode},
            is_only_load_custom_rule_{}
        {}
        inline ~config_op(){}
    };
#endif
    class sys_op final{
    private:
        const char mode_;
        const sys_rule &rule_;
    public:
        inline auto operator()(console_ui::fn_args)const{
#ifdef _NEXT_
            if((rule_.exe.empty())&&(rule_.svc.empty())){
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
            puts("-> 生成命令.");
            std::string cmd;
            switch(mode_){
                case 'c':{
                    for(const auto &item:rule_.exe){
                        cmd.append(R"(reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution options\)")
#ifdef _NEXT_
                           .append(item.get())
#else
                           .append(item)
#endif
                           .append(R"(.exe" /f /t reg_sz /v debugger /d ? & taskKill /f /im ")")
#ifdef _NEXT_
                           .append(item.get())
#else
                           .append(item)
#endif
                           .append(R"(.exe" & )");
                    }
                    for(const auto &item:rule_.svc){
                        cmd.append(R"(net stop ")")
#ifdef _NEXT_
                           .append(item.get())
#else
                           .append(item)
#endif
                           .append(R"(" /y & )");
                    }
                    break;
                }case 'r':{
                    for(const auto &item:rule_.exe){
                        cmd.append(R"(reg delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution options\)")
#ifdef _NEXT_
                           .append(item.get())
#else
                           .append(item)
#endif
                           .append(R"(.exe" /f & )");
                    }
                    for(const auto &item:rule_.svc){
                        cmd.append(R"(net start ")")
#ifdef _NEXT_
                           .append(item.get())
#else
                           .append(item)
#endif
                           .append(R"(" & )");
                    }
                    break;
                }
            }
            puts("-> 执行命令.");
            puts(std::string(50,'-').c_str());
            system(cmd.c_str());
            puts(std::string(50,'-').c_str());
            puts("-> 释放内存.");
            return false;
        }
        inline explicit sys_op(const char _mode,const sys_rule &_rule):
            mode_{_mode},
            rule_{_rule}
        {}
        inline ~sys_op(){}
    };
}
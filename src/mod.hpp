#pragma once
#include"info.hpp"
#include"console_ui.hpp"
#ifdef _PREVIEW_
#include<fstream>
#endif
inline struct{
#ifdef _PREVIEW_
    bool enhanced_op,enhanced_window,repaired_mode;
#else
    bool front_show_window,translucent_window,window_ctrls;
#endif
}config_data{};
#ifndef _PREVIEW_
inline bool config_error{};
#endif
namespace mod{
    namespace rule_data{
        struct base final{
#ifdef _PREVIEW_
            std::vector<std::string> exe,svc;
#else
            std::vector<const char*> exe,svc;
#endif
        };
        inline const base mythware{
            {
                "StudentMain.exe",
                "DispcapHelper.exe",
                "VRCwPlayer.exe",
                "InstHelpApp.exe",
                "InstHelpApp64.exe",
                "TDOvrSet.exe",
                "GATESRV.exe",
                "ProcHelper64.exe",
                "MasterHelper.exe"
            },{
                "STUDSRV",
                "TDNetFilter",
                "TDFileFilter"
            }
        },lenovo{
            {
                "vncviewer.exe",
                "tvnserver32.exe",
                "WfbsPnpInstall.exe",
                "WFBSMon.exe",
                "WFBSMlogon.exe",
                "WFBSSvrLogShow.exe",
                "ResetIp.exe",
                "FuncForWIN64.exe",
                "CertMgr.exe",
                "Fireware.exe",
                "BCDBootCopy.exe",
                "refreship.exe",
                "lenovoLockScreen.exe",
                "PortControl64.exe",
                "DesktopCheck.exe",
                "DeploymentManager.exe",
                "DeploymentAgent.exe",
                "XYNTService.exe"
            },{
                "BSAgentSvr",
                "tvnserver",
                "WFBSMlogon"
            }
        };
#ifdef _PREVIEW_
        inline base customized{};
#endif
    }
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
    inline auto quit(console_ui::fn_args){
        return true;
    }
#ifdef _PREVIEW_
    inline auto relaunch_as_admin(console_ui::fn_args){
        char *const path{new char[MAX_PATH]{}};
        GetModuleFileNameA(nullptr,path,MAX_PATH);
        ShellExecuteA(nullptr,"runas",path,nullptr,nullptr,SW_SHOWNORMAL);
        delete[] path;
        return true;
    }
    inline auto info(console_ui::fn_args){
        auto view_repo_webpage{[](console_ui::fn_args){
            ShellExecuteA(nullptr,"open",INFO_REPO_URL,nullptr,nullptr,SW_SHOWNORMAL);
            return false;
        }};
        console_ui ui;
        ui.add("                    [ 信  息 ]\n\n")
          .add(" < 返回 ",quit,CONSOLE_TEXT_RED_WHITE)
          .add(
            "\n[名称]\n\n"
            " " INFO_NAME "\n"
            "\n[版本]\n\n"
            " " INFO_VERSION
          )
          .add("\n[仓库]\n")
          .add(" " INFO_REPO_URL,view_repo_webpage)
          .add(
            "\n[许可证]\n\n"
            " " INFO_LICENSE "\n"
            " (C) 2023 " INFO_DEVELOPER ". All Rights Reserved."
          )
          .show();
        return false;
    }
    inline auto toolkit(console_ui::fn_args){
        auto launch_cmd{[](console_ui::fn_args _args){
            _args.ui->lock(false,false);
            _args.ui->set_console(
                936,
                "CRCSN",
                WINDOW_WIDTH*2,
                WINDOW_HEIGHT,
                true,
                false,
                !config_data.enhanced_window,
                (config_data.enhanced_window)
                  ?(230)
                  :(255)
            );
            SetConsoleScreenBufferSize(
                GetStdHandle(STD_OUTPUT_HANDLE),
                {
                    WINDOW_WIDTH*2,
                    32766
                }
            );
            system("cmd");
            _args.ui->set_console(
                936,
                "CRCSN",
                WINDOW_WIDTH,
                WINDOW_HEIGHT,
                true,
                false,
                !config_data.enhanced_window,
                (config_data.enhanced_window)
                  ?(230)
                  :(255)
            );
            return false;
        }};
        class exec_cmd{
        private:
            const char *const cmd_;
        public:
            auto operator=(const exec_cmd &)=delete;
            auto operator()(console_ui::fn_args){
                printf(
                    ":: 执行命令.\n%s\n",
                    std::string(WINDOW_WIDTH,'-').c_str()
                );
                system(cmd_);
                return false;
            }
            explicit exec_cmd(const char *const _cmd):
              cmd_{_cmd}
            {}
            explicit exec_cmd(const exec_cmd &_obj):
              cmd_{_obj.cmd_}
            {}
            ~exec_cmd(){}
        };
        const char *const cmds[]{
            R"(taskkill /f /im explorer.exe && timeout /t 3 /nobreak && start C:\Windows\explorer.exe)",
            R"(reg delete "HKLM\SOFTWARE\Policies\Google\Chrome" /f /v AllowDinosaurEasterEgg)",
            R"(reg delete "HKLM\SOFTWARE\Policies\Microsoft\Edge" /f /v AllowSurfGame)",
            R"(reg add "HKLM\SYSTEM\CurrentControlSet\Services\USBSTOR" /f /t reg_dword /v Start /d 3)"
        };
        console_ui ui;
        ui.add("                   [ 工 具 箱 ]\n\n")
          .add(" < 返回 ",quit,CONSOLE_TEXT_RED_WHITE)
          .add(" > 命令提示符 ",launch_cmd)
          .add("\n[快捷操作]\n")
          .add(" > 重启资源管理器 ",exec_cmd{cmds[0]})
          .add(" > 恢复 Google Chrome 离线游戏 ",exec_cmd{cmds[1]})
          .add(" > 恢复 Microsoft Edge 离线游戏 ",exec_cmd{cmds[2]})
          .add(" > 恢复 USB 设备访问 ",exec_cmd{cmds[3]})
          .show();
        return false;
    }
    class config_op final{
    private:
        const char mode_;
        mutable bool is_reload_;
        auto load_()const{
            std::ifstream config_file{"config.ini",std::ios::in};
            if(!config_file.is_open()){
                config_file.close();
                return;
            }
            if(is_reload_){
                puts(":: 同步更改.");
            }else{
                puts(":: 加载配置文件.");
            }
            rule_data::customized.exe.clear();
            rule_data::customized.svc.clear();
            std::string line;
            enum{
                v_unknown=-1,
                v_settings=0,
                v_rule_exe=1,
                v_rule_svc=2,
            }config_item{v_unknown};
            while(std::getline(config_file,line)){
                if((line.empty())||(line.front()=='#')){
                    continue;
                }
                if(line=="[settings]"){
                    config_item=v_settings;
                    continue;
                }else if(line=="[rule_exe]"){
                    config_item=v_rule_exe;
                    continue;
                }else if(line=="[rule_svc]"){
                    config_item=v_rule_svc;
                    continue;
                }else if((line.front()=='[')&&(line.back()==']')){
                    config_item=v_unknown;
                    continue;
                }
                switch(config_item){
                    case v_unknown:{
                        break;
                    }case v_settings:{
                        if(is_reload_){
                            continue;
                        }
                        if(line=="enhanced_op"){
                            config_data.enhanced_op=true;
                        }else if(line=="enhanced_window"){
                            config_data.enhanced_window=true;
                        }else if(line=="repaired_mode"){
                            config_data.repaired_mode=true;
                        }
                        break;
                    }case v_rule_exe:{
                        rule_data::customized.exe.emplace_back(std::move(line));
                        break;
                    }case v_rule_svc:{
                        rule_data::customized.svc.emplace_back(std::move(line));
                        break;
                    }
                }
            }
            config_file.close();
            return;
        }
        auto edit_()const{
            auto sync{[this](console_ui::fn_args){
                is_reload_=true;
                load_();
                puts(":: 保存更改.");
                std::string text;
                text.append("[settings]\n");
                if(config_data.enhanced_op){
                    text.append("enhanced_op\n");
                }
                if(config_data.enhanced_window){
                    text.append("enhanced_window\n");
                }
                if(config_data.repaired_mode){
                    text.append("repaired_mode\n");
                }
                text.append("[rule_exe]\n");
                if(!rule_data::customized.exe.empty()){
                    for(const auto &item:rule_data::customized.exe){
                        text.append(item).push_back('\n');
                    }
                }
                text.append("[rule_svc]\n");
                if(!rule_data::customized.svc.empty()){
                    for(const auto &item:rule_data::customized.svc){
                        text.append(item).push_back('\n');
                    }
                }
                std::ofstream config_file{"config.ini",std::ios::out|std::ios::trunc};
                config_file.write(text.c_str(),text.size());
                config_file.close();
                return true;
            }};
            auto open_config_file{[](console_ui::fn_args){
                if(std::ifstream{"config.ini",std::ios::in}.is_open()){
                    puts(":: 打开配置文件.");
                    ShellExecuteA(nullptr,"open","config.ini",nullptr,nullptr,SW_SHOWNORMAL);
                    return false;
                }
                puts(
                    "                    [ 配  置 ]\n\n\n"
                    " (i) 无法读取配置文件.\n"
                );
                for(unsigned short i{3};i>0;--i){
                    printf(" %hus 后返回.\r",i);
                    Sleep(1000);
                }
                return false;
            }};
            console_ui ui;
            ui.add("                    [ 配  置 ]\n\n")
              .add(" (i) 相关信息可参阅文档.\n")
              .add(" < 同步配置并返回 ",std::move(sync),CONSOLE_TEXT_RED_WHITE)
              .add(" > 打开配置文件 ",open_config_file)
              .add("\n[增强操作]\n")
              .add(" > 启用 ",[](console_ui::fn_args){config_data.enhanced_op=true;return false;})
              .add(" > 禁用 (默认) ",[](console_ui::fn_args){config_data.enhanced_op=false;return false;})
              .add("\n[增强窗口 (下次启动时生效)]\n")
              .add(" > 启用 ",[](console_ui::fn_args){config_data.enhanced_window=true;return false;})
              .add(" > 禁用 (默认) ",[](console_ui::fn_args){config_data.enhanced_window=false;return false;})
              .add("\n[修复模式 (下次启动时生效)]\n")
              .add(" > 启用 ",[](console_ui::fn_args){config_data.repaired_mode=true;return false;})
              .add(" > 禁用 (默认) ",[](console_ui::fn_args){config_data.repaired_mode=false;return false;})
              .show();
            return false;
        }
    public:
        auto operator()(console_ui::fn_args)const{
            switch(mode_){
                case 'r':{
                    load_();
                    break;
                }case 'w':{
                    edit_();
                    break;
                }
            }
            return false;
        }
        explicit config_op(const char _mode):
          mode_{_mode},
          is_reload_{}
        {}
        ~config_op(){}
    };
    class rule_op final{
    private:
        const char mode_;
        const rule_data::base &rule_data_;
    public:
        auto operator()(console_ui::fn_args)const{
            puts("                 [ 破 解 / 恢 复 ]\n\n");
            if((rule_data_.exe.empty())&&(rule_data_.svc.empty())){
                puts(" (i) 规则为空.\n");
                for(unsigned short i{3};i>0;--i){
                    printf(" %hus 后返回.\r",i);
                    Sleep(1000);
                }
                return false;
            }
            printf(
                ":: 生成并执行命令.\n%s\n",
                std::string(WINDOW_WIDTH,'-').c_str()
            );
            std::string cmd;
            switch(mode_){
                case 'c':{
                    if(config_data.enhanced_op){
                        for(const auto &item:rule_data_.exe){
                            cmd.append(R"(reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution options\)")
                               .append(item)
                               .append(R"(" /f /t reg_sz /v debugger /d _)");
                            system(cmd.c_str());
                            cmd.clear();
                        }
                        for(const auto &item:rule_data_.svc){
                            cmd.append("sc config ")
                               .append(item)
                               .append(" start= disabled");
                            system(cmd.c_str());
                            cmd.clear();
                        }
                    }
                    for(const auto &item:rule_data_.exe){
                        cmd.append(R"(taskkill /f /im ")")
                           .append(item)
                           .append(R"(")");
                        system(cmd.c_str());
                        cmd.clear();
                    }
                    for(const auto &item:rule_data_.svc){
                        cmd.append(R"(net stop ")")
                           .append(item)
                           .append(R"(" /y)");
                        system(cmd.c_str());
                        cmd.clear();
                    }
                    break;
                }case 'r':{
                    if(config_data.enhanced_op){
                        for(const auto &item:rule_data_.exe){
                            cmd.append(R"(reg delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution options\)")
                               .append(item)
                               .append(R"(" /f)");
                            system(cmd.c_str());
                            cmd.clear();
                        }
                        for(const auto &item:rule_data_.svc){
                            cmd.append("sc config ")
                               .append(item)
                               .append(" start= auto");
                            system(cmd.c_str());
                            cmd.clear();
                        }
                    }
                    for(const auto &item:rule_data_.svc){
                        cmd.append(R"(net start ")")
                           .append(item)
                           .append(R"(")");
                        system(cmd.c_str());
                        cmd.clear();
                    }
                    break;
                }
            }
            return false;
        }
        explicit rule_op(
            const char _mode,
            const rule_data::base &_rule_data
        ):mode_{_mode},
          rule_data_{_rule_data}
        {}
        ~rule_op(){}
    };
#else
    inline auto init(){
        system("chcp 936 > nul");
        SetConsoleTitleA("CRCSN");
        SetWindowLongPtrA(
            GetConsoleWindow(),GWL_STYLE,(config_data.window_ctrls)
            ?(GetWindowLongPtrA(GetConsoleWindow(),GWL_STYLE)|WS_SIZEBOX|WS_MAXIMIZEBOX|WS_MINIMIZEBOX)
            :(GetWindowLongPtrA(GetConsoleWindow(),GWL_STYLE)&~WS_SIZEBOX&~WS_MAXIMIZEBOX&~WS_MINIMIZEBOX)
        );
        system("mode con cols=50 lines=25");
        SetLayeredWindowAttributes(
            GetConsoleWindow(),0,
            (config_data.translucent_window)
              ?(230)
              :(255),
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
            SetForegroundWindow(this_window);
            AttachThreadInput(current_id,foreground_id,FALSE);
            SetWindowPos(this_window,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
            Sleep(100);
        }
    }
    inline auto info(console_ui::fn_args){
        console_ui ui;
        ui.add("                    [ 信  息 ]\n\n")
          .add(" < 返回 ",quit,CONSOLE_TEXT_RED_WHITE)
          .add(
            "\n[名称]\n\n"
            " " INFO_NAME "\n"
            "\n[版本]\n\n"
            " " INFO_VERSION "\n"
            "\n[仓库]\n\n"
            " " INFO_REPO_URL "\n"
            "\n[许可证]\n\n"
            " " INFO_LICENSE "\n"
            " (C) 2023 " INFO_DEVELOPER ". All Rights Reserved."
          )
          .show();
        return false;
    }
    inline auto cmd(console_ui::fn_args _args){
        _args.ui->lock(false,false);
        system("cmd");
        if(!config_data.window_ctrls){
            init();
        }
        return false;
    }
    class rule_op final{
    private:
        const char mode_;
        const rule_data::base &rule_;
    public:
        auto operator()(console_ui::fn_args)const{
            puts("                 [ 破 解 / 恢 复 ]\n\n");
            if(!is_run_as_admin()){
                puts("\n (i) 需要管理员权限.\n");
                for(unsigned short i{3};i>0;--i){
                    printf(" %hus 后返回.\r",i);
                    Sleep(1000);
                }
                return false;
            }
            puts(":: 生成命令.");
            std::string cmd;
            switch(mode_){
                case 'c':{
                    for(const auto &item:rule_.exe){
                        cmd.append(R"(reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution options\)")
                           .append(item)
                           .append(R"(" /f /t reg_sz /v debugger /d ? & taskkill /f /im ")")
                           .append(item)
                           .append(R"(" & )");
                    }
                    for(const auto &item:rule_.svc){
                        cmd.append(R"(net stop ")")
                           .append(item)
                           .append(R"(" /y & )");
                    }
                    break;
                }case 'r':{
                    for(const auto &item:rule_.exe){
                        cmd.append(R"(reg delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution options\)")
                           .append(item)
                           .append(R"(" /f & )");
                    }
                    for(const auto &item:rule_.svc){
                        cmd.append(R"(net start ")")
                           .append(item)
                           .append(R"(" & )");
                    }
                    break;
                }
            }
            printf(":: 执行命令.\n%s\n",std::string(50,'-').c_str());
            system(cmd.c_str());
            printf("%s\n::释放内存.",std::string(50,'-').c_str());
            return false;
        }
        explicit rule_op(const char _mode,const rule_data::base &_rule):
          mode_{_mode},
          rule_{_rule}
        {}
        ~rule_op(){}
    };
#endif
}
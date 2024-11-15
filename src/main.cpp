#ifdef _WIN32
#include"console_ui.hpp"
#include"mod.hpp"
#include<thread>
#ifdef _PREVIEW
auto main()->int{
    console_ui ui;
    ui.set_console(936,"CRCSN",WINDOW_WIDTH,WINDOW_HEIGHT,true,false,true,255)
      .lock(true,true);
    puts(":: 检测运行权限.");
    if(!mod::is_run_as_admin()){
        puts(":: 申请管理员权限.");
        mod::reboot_as_admin(console_ui::fn_args{});
        return 0;
    }
    mod::config_op{'r'}(console_ui::fn_args{});
#else
auto main(const int _argc,const char *const _argv[])->int{
    console_ui ui;
    if(_argc>1){
        std::string_view tmp;
        for(int i{1};i<_argc;++i){
            tmp=_argv[i];
            if((tmp.size()>2)&&(tmp.substr(0,2)=="-W")){
                for(const auto &sub:tmp.substr(2)){
                    switch(sub){
                        case 'f':{
                            config_data.front_show_window=true;
                            break;
                        }case 't':{
                            config_data.translucent_window=true;
                            break;
                        }case 'c':{
                            config_data.window_ctrls=true;
                            break;
                        }default:{
                            config_error=true;
                            goto END;
                        }
                    }
                }
            }else{
                config_error=true;
                break;
            }
        }
    END:
        if(config_error){
            config_data={};
        }
    }
    mod::init();
#endif
#ifdef _PREVIEW
    if(config_data.enhanced_window){
        std::thread{[](){
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
        }}.detach();
    }
    if(config_data.protected_mode){
        std::thread{[](){
            const char *const exe[]{
                "mode.com",
                "chcp.com",
                "reg.exe",
                "sc.exe",
                "taskkill.exe",
                "net.exe",
                "cmd.exe",
                "taskmgr.exe",
                "perfmon.exe",
                "regedit.exe",
                "mmc.exe"
            };
            std::string path;
            while(true){
                RegDeleteTreeA(HKEY_CURRENT_USER,R"(Software\Policies\Microsoft\Windows\System)");
                RegDeleteTreeA(HKEY_CURRENT_USER,R"(Software\Microsoft\Windows\CurrentVersion\Policies\System)");
                RegDeleteTreeA(HKEY_CURRENT_USER,R"(Software\Microsoft\Windows\CurrentVersion\Policies\Explorer)");
                for(const auto &item:exe){
                    path.append(R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\)")
                        .append(item);
                    RegDeleteTreeA(HKEY_LOCAL_MACHINE,path.c_str());
                    path.clear();
                }
                Sleep(1000);
            }
        }}.detach();
    }
#else
    if(config_data.front_show_window){
        std::thread{mod::front_show_window}.detach();
    }
#endif
    ui.add("                    [ 主  页 ]\n\n");
#ifndef _PREVIEW
    if(config_error){
        ui.add(" (!) 参数错误.\n");
    }
#endif
    ui.add(" < 退出 ",mod::exit,CONSOLE_TEXT_RED_DEFAULT)
#ifdef _PREVIEW
      .add(" < 重启 ",mod::reboot_as_admin,CONSOLE_TEXT_RED_DEFAULT)
#endif
      .add(" > 关于 ",mod::info)
#ifdef _PREVIEW
      .add(" > 配置 ",mod::config_op{'w'})
      .add(" > 工具箱 ",mod::toolkit)
#else
      .add(" > 命令提示符 ",mod::cmd)
#endif
      .add("\n[破解]\n")
      .add(" > 极域电子教室 ",mod::rule_op{'c',mod::rule_data::mythware})
      .add(" > 联想云教室 ",mod::rule_op{'c',mod::rule_data::lenovo})
#ifdef _PREVIEW
      .add(" > 自定义 ",mod::rule_op{'c',mod::rule_data::customize})
#endif
      .add("\n[恢复]\n")
      .add(" > 极域电子教室 ",mod::rule_op{'r',mod::rule_data::mythware})
      .add(" > 联想云教室 ",mod::rule_op{'r',mod::rule_data::lenovo})
#ifdef _PREVIEW
      .add(" > 自定义 ",mod::rule_op{'r',mod::rule_data::customize})
      .set_console(
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
      )
#endif
      .show()
      .lock(false,false);
    return 0;
}
#else
#error "must be compiled on the Windows OS platform."
#endif
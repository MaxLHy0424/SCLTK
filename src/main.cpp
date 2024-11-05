#ifdef _WIN32
#include"console_ui.hpp"
#include"mod.hpp"
#include<thread>
#ifdef _NEXT_
auto main()->int{
    console_ui ui;
    ui.set_window(936,"CRCSN",50,25,true,false,true,255)
      .lock_console(true);
    puts("-> 检测运行权限.");
    if(!mod::is_run_as_admin()){
        puts("-> 申请管理员权限.");
        mod::relaunch_as_admin(console_ui::args{});
        return 0;
    }
    mod::config_op{'r'}(console_ui::args{});
#else
auto main(const int _argc,const char *const _args[])->int{
    console_ui ui;
    if(_argc>1){
        std::string_view tmp;
        for(int i{1};i<_argc;++i){
            tmp=_args[i];
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
    if(config_data.front_show_window){
#ifdef _NEXT_
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
#else
        std::thread{mod::front_show_window}.detach();
#endif
    }
    ui.add("                    [ 主  页 ]\n\n");
#ifndef _NEXT_
    if(config_error){
        ui.add(" (!) 参数错误.\n");
    }
#endif
    ui.add(" < 退出 ",mod::exit,CUI_TEXT_RED)
#ifdef _NEXT_
      .add(" < 重启 ",mod::relaunch_as_admin,CUI_TEXT_RED)
#endif
      .add(" > 关于 ",mod::info)
#ifdef _NEXT_
      .add(" > 配置 ",mod::config_op{'e'})
#endif
      .add(" > 命令提示符 ",mod::cmd)
      .add("\n[破解]\n")
      .add(" > 极域电子教室 ",mod::sys_op{'c',mod::sys_rule::mythware})
      .add(" > 联想云教室 ",mod::sys_op{'c',mod::sys_rule::lenovo})
#ifdef _NEXT_
      .add(" > 自定义 ",mod::sys_op{'c',mod::sys_rule::custom})
#endif
      .add("\n[恢复]\n")
      .add(" > 极域电子教室 ",mod::sys_op{'r',mod::sys_rule::mythware})
      .add(" > 联想云教室 ",mod::sys_op{'r',mod::sys_rule::lenovo})
#ifdef _NEXT_
      .add(" > 自定义 ",mod::sys_op{'r',mod::sys_rule::custom})
#endif
#ifdef _NEXT_
      .set_window(
        936,
        "CRCSN",
        50,
        25,
        true,
        false,
        !config_data.hide_window_close_ctrl,
        (config_data.translucent_window)?(230):(255)
      )
#endif
      .show()
      .lock_console(false);
    return 0;
}
#endif
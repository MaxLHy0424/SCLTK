#ifdef _WIN32
#include<string>
#include<thread>
#include"console_ui.hpp"
#include"mod.hpp"
#ifdef _NEXT
auto main()->int{
    mod::init();
    puts("==> 检测运行权限.");
    if(!mod::is_run_as_admin()){
        puts("==> 申请管理员权限.");
        char *const path{new char[MAX_PATH]{}};
        GetModuleFileName(nullptr,path,MAX_PATH);
        ShellExecute(nullptr,"runAs",path,nullptr,nullptr,SW_SHOWNORMAL);
        delete[] path;
        return 0;
    }
    mod::config_op{'r'}(console_ui::fn_args{});
#else
auto main(const int _argc,const char *const _args[])->int{
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
                        }case 'a':{
                            config_data.alpha_window=true;
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
#endif
    mod::init();
    if(config_data.front_show_window){
#ifdef _NEXT
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
    console_ui ui;
    ui.add("                    [ 主  页 ]\n\n");
#ifndef _NEXT
    if(config_error){
        ui.add(" (!) 参数错误.\n");
    }
#endif
    ui.add(" < 退出 ",mod::exit,CONSOLE_RED)
      .add(" > 关于 ",mod::info)
#ifdef _NEXT
      .add(" > 配置 ",mod::config_op{'e'})
#endif
      .add(" > 命令提示符 ",mod::cmd)
      .add("\n[破解]\n")
      .add(" > 极域电子教室 ",mod::sys_op{'c',mod::rule.mythware})
      .add(" > 联想云教室 ",mod::sys_op{'c',mod::rule.lenovo})
#ifdef _NEXT
      .add(" > 自定义 ",mod::sys_op{'c',mod::rule.custom})
#endif
      .add("\n[恢复]\n")
      .add(" > 极域电子教室 ",mod::sys_op{'r',mod::rule.mythware})
      .add(" > 联想云教室 ",mod::sys_op{'r',mod::rule.lenovo})
#ifdef _NEXT
      .add(" > 自定义 ",mod::sys_op{'r',mod::rule.custom})
#endif
      .show();
    return 0;
}
#endif
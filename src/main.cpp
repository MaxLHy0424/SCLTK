#ifdef _WIN32
#include"def.hpp"
#include"ui.hpp"
#include"mod.hpp"
#ifdef _THE_NEXT_MAJOR_UPDATE_
auto main()->int{
    Mod::init();
    puts("=> 检测运行权限.");
    if(!Mod::isRunAsAdmin()){
        puts("=> 申请管理员权限.");
        char *const path{new char[MAX_PATH]{}};
        GetModuleFileName(nullptr,path,MAX_PATH);
        ShellExecute(nullptr,"runAs",path,nullptr,nullptr,SW_SHOWNORMAL);
        delete[] path;
        return 0;
    }
    Mod::OpConfig{'r'}(Data{});
#else
auto main(const int argc,const char *const args[])->int{
    if(argc>1){
        std::string_view tmp;
        for(int i{1};i<argc;++i){
            tmp=args[i];
            if((tmp.size()>2)&&(tmp.substr(0,2)=="-W")){
                for(const auto &sub:tmp.substr(2)){
                    switch(sub){
                        case 'f':{
                            config.wndFrontShow=true;
                            break;
                        }case 'a':{
                            config.wndAlpha=true;
                            break;
                        }case 'c':{
                            config.wndCtrls=true;
                            break;
                        }default:{
                            configError=true;
                            goto END;
                        }
                    }
                }
            }else{
                configError=true;
                break;
            }
        }
    END:
        if(configError){
            config={};
        }
    }
#endif
    Mod::init();
    if(config.wndFrontShow){
        std::thread{Mod::frontShow}.detach();
    }
    UI ui;
    ui.add("                    [ 主  页 ]\n\n");
    if(configError){
#ifdef _THE_NEXT_MAJOR_UPDATE_
        ui.add(" (!) 配置存在错误.\n");
#else
        ui.add(" (!) 参数错误.\n");
#endif
    }
    ui.add(" < 退出 ",Mod::exit,WCC_RED)
      .add(" > 关于 ",Mod::info)
#ifdef _THE_NEXT_MAJOR_UPDATE_
      .add(" > 配置 ",Mod::OpConfig{'e'})
#endif
      .add(" > 命令提示符 ",Mod::cmd)
      .add("\n[破解]\n")
      .add(" > 极域电子教室 ",Mod::OpSys{'c',Mod::rule.mythware})
      .add(" > 联想云教室 ",Mod::OpSys{'c',Mod::rule.lenovo})
#ifdef _THE_NEXT_MAJOR_UPDATE_
      .add(" > 自定义 ",Mod::OpSys{'c',Mod::rule.custom})
#endif
      .add("\n[恢复]\n")
      .add(" > 极域电子教室 ",Mod::OpSys{'r',Mod::rule.mythware})
      .add(" > 联想云教室 ",Mod::OpSys{'r',Mod::rule.lenovo})
#ifdef _THE_NEXT_MAJOR_UPDATE_
      .add(" > 自定义 ",Mod::OpSys{'r',Mod::rule.custom})
#endif
      .show();
    return 0;
}
#endif
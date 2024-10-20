#ifdef _WIN32
#include"def.hpp"
#include"ui.hpp"
#include"mod.hpp"
#ifdef _THE_NEXT_MAJOR_UPDATE_
auto main()->int{
    if(!Mod::isRunAsAdmin()){
        std::string path(MAX_PATH,0);
        GetModuleFileName(nullptr,path.data(),MAX_PATH);
        ShellExecute(nullptr,"runAs",path.c_str(),nullptr,nullptr,SW_SHOWNORMAL);
        return 0;
    }
    Mod::OpConfig('r')(Data());
#else
auto main(const int argc,const char *const args[])->int{
    if(argc>1){
        std::string tmp;
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
        std::thread(Mod::frontShow).detach();
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
    ui.add(" < 退出 ",Mod::exit,COLOR_RED)
      .add(" > 关于 ",Mod::info)
#ifdef _THE_NEXT_MAJOR_UPDATE_
      .add(" > 配置 ",Mod::OpConfig('e'))
#endif
      .add(" > 命令提示符 ",Mod::cmd)
      .add("\n[破解]\n")
      .add(" > 极域电子教室 ",Mod::OpSys('c',Mod::rule.mythware))
      .add(" > 联想云教室 ",Mod::OpSys('c',Mod::rule.lenovo))
      .add("\n[恢复]\n")
      .add(" > 极域电子教室 ",Mod::OpSys('r',Mod::rule.mythware))
      .add(" > 联想云教室 ",Mod::OpSys('r',Mod::rule.lenovo))
      .show();
    return 0;
}
#endif
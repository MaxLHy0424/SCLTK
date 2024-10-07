#ifdef _WIN32
#include"def.hpp"
#include"ui.hpp"
#include"mod.hpp"
#ifdef _THE_NEXT_UPDATE_
auto main()->int{
    if(!Mod::isRunAsAdmin()){
        char path[MAX_PATH];
        GetModuleFileName(nullptr,path,MAX_PATH);
        ShellExecute(nullptr,"runAs",path,nullptr,nullptr,SW_SHOWNORMAL);
        return 0;
    }
    Mod::configRead();
#else
auto main(int argc,char *args[])->int{
    if(argc>1){
        std::string tmp;
        for(int i{1};i<argc;++i){
            tmp=args[i];
            if((tmp.size()>2)&&(tmp.substr(0,2)=="-W")){
                for(auto &ref:tmp.substr(2,tmp.size()-1)){
                    switch(ref){
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
    ui.add("                    < 主  页 >\n\n");
    if(configError){
#ifdef _THE_NEXT_UPDATE_
        ui.add(" (!) 配置项目错误.\n");
#else
        ui.add(" (!) 参数错误.\n");
#endif
    }
    ui.add(" > 退出 ",Mod::exit,WC_RED)
      .add(" > 关于 ",Mod::info)
#ifdef _THE_NEXT_UPDATE_
      .add(" > 设置 ",Mod::configEdit)
#endif
      .add(" > 命令提示符 ",Mod::cmd)
      .add("\n[破解]\n")
      .add(" > 极域电子教室 ",Mod::Op('c',Mod::rule.mythware.exe,Mod::rule.mythware.svc))
      .add(" > 联想云教室 ",Mod::Op('c',Mod::rule.lenovo.exe,Mod::rule.lenovo.svc))
      .add("\n[恢复]\n")
      .add(" > 极域电子教室 ",Mod::Op('r',Mod::rule.mythware.exe,Mod::rule.mythware.svc))
      .add(" > 联想云教室 ",Mod::Op('r',Mod::rule.lenovo.exe,Mod::rule.lenovo.svc))
      .show();
    return 0;
}
#endif
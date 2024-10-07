#ifdef _WIN32
#include"def.hpp"
#include"ui.hpp"
#include"mod.hpp"
#ifdef _THE_NEXT_MAJOR_UPDATE_
auto main()->int{
    if(!Mod::isRunAsAdmin()){
        char path[MAX_PATH];
        GetModuleFileName(nullptr,path,MAX_PATH);
        ShellExecute(nullptr,"runAs",path,nullptr,nullptr,SW_SHOWNORMAL);
        return 0;
    }
    Mod::settingsRead();
#else
auto main(int argc,char *args[])->int{
    if(argc>1){
        std::string k;
        for(int i{1};i<argc;++i){
            k=args[i];
            if((k.size()>2)&&(k.substr(0,2)=="-W")){
                for(auto &ref:k.substr(2)){
                    switch(ref){
                        case 'f':{
                            settings.wndFrontShow=true;
                            break;
                        }case 'a':{
                            settings.wndAlpha=true;
                            break;
                        }case 'c':{
                            settings.wndCtrls=true;
                            break;
                        }default:{
                            settingsError=true;
                            goto END;
                        }
                    }
                }
            }else{
                settingsError=true;
                break;
            }
        }
    END:
        if(settingsError){
            settings={};
        }
    }
#endif
    Mod::init();
    if(settings.wndFrontShow){
        std::thread(Mod::frontShow).detach();
    }
    UI ui;
    ui.add("                    < 主  页 >\n\n");
    if(settingsError){
#ifdef _THE_NEXT_MAJOR_UPDATE_
        ui.add(" (!) 配置项目错误.\n");
#else
        ui.add(" (!) 参数错误.\n");
#endif
    }
    ui.add(" > 退出 ",Mod::exit,WC_RED)
      .add(" > 关于 ",Mod::info)
#ifdef _THE_NEXT_MAJOR_UPDATE_
      .add(" > 设置 ",Mod::settingsEdit)
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
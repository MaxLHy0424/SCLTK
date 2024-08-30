#include"header.hpp"
#include"ui.hpp"
CUI ui;
#include"mods.hpp"
auto main(i32 argc,cstr argv[])->i32{
    bool optErr{};
    if(argc>1){
        std::string tmp;
        for(i32 i{1};i<argc;++i){
            tmp=argv[i];
            if((tmp.substr(0,2)=="-W")&&(tmp.size()>2)){
                for(u64 j{2};j<tmp.size();++j){
                    switch(tmp[j]){
                        case 'f':{
                            opt.frontShow=true;
                            break;
                        }case 's':{
                            opt.wndSize=true;
                            break;
                        }case 'a':{
                            opt.alphaWnd=true;
                            break;
                        }default:{
                            optErr=true;
                            goto INIT;
                        }
                    }
                }
            }else if(tmp=="--loose-view"){
                opt.looseView=true;
            }else if(tmp=="/command"){
                continue;
            }else{
                optErr=true;
                break;
            }
        }
    INIT:
        if(optErr){
            opt={};
        }
    }
    Mod::init();
    if(opt.frontShow){
        std::thread(Mod::frontShow).detach();
    }
    ui.push("    [ Computer Room Control Software Nemesis ]");
    if(opt.looseView){
        ui.push("\0");
    }
    ui.push("                     v5.0.3");
    ui.push("       https://github.com/MaxLHy0424/CRCSN");
    ui.push("     (C) 2023 MaxLHy0424. All Rights Reserved.\n");
    if(optErr){
        ui.push(" (!) 参数错误.\n");
    }
    ui.push(" > 退出 ",Mod::exit,CON_RED_PALE);
    if(opt.looseView){
        ui.push("\0");
    }
    ui.push(" > 命令提示符 ",Mod::cmd);
    ui.push("\n[ 破 解 ]\n");
    if(IsUserAnAdmin()){
        ui.push(" > 极域电子教室 ",Mod::Crack::mythware);
        if(opt.looseView){
            ui.push("\0");
        }
        ui.push(" > 联想云教室 ",Mod::Crack::lenovo);
    }else{
        ui.push(" (i) 需要提权.");
    }
    ui.push("\n[ 恢 复 ]\n");
    if(IsUserAnAdmin()){
        ui.push(" > 极域电子教室 ",Mod::Recovery::mythware);
        if(opt.looseView){
            ui.push("\0");
        }
        ui.push(" > 联想云教室 ",Mod::Recovery::lenovo);
    }else{
        ui.push(" (i) 需要提权.");
    }
    ui.show();
    return 0;
}
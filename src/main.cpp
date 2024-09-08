#include"def.hpp"
#include"ui.hpp"
#include"mod.hpp"
auto main(i32 argc,i8 *argv[])->i32{
    CUI ui;
    bool optError{};
    if(argc>1){
        std::string tmp;
        for(i32 i{1};i<argc;++i){
            tmp=argv[i];
            if((tmp.size()>2)?(tmp.substr(0,2)=="-W"):(false)){
                for(const auto &j:tmp.substr(2)){
                    switch(j){
                        case 'f':{
                            opt.wndFrontShow=true;
                            break;
                        }case 'a':{
                            opt.wndAlpha=true;
                            break;
                        }case 's':{
                            opt.wndSizeBox=true;
                            break;
                        }default:{
                            optError=true;
                            goto INIT;
                        }
                    }
                }
            }else if(tmp=="--wide-view"){
                opt.viewWide=true;
            }else{
                optError=true;
                break;
            }
        }
    INIT:
        if(optError){
            opt={};
        }
    }
    Mod::init();
    if(opt.wndFrontShow){
        std::thread{Mod::frontShow}.detach();
    }
    ui.push("    < Computer Room Control Software Nemesis >\n");
    ui.push("                     v5.3.3");
    ui.push("       https://github.com/MaxLHy0424/CRCSN");
    ui.push("     (C) 2023 MaxLHy0424. All rights reserved.\n");
    if(optError){
        ui.push(" (!) 参数错误.\n");
    }
    ui.push(" > 退出 ",Mod::exit,nullptr,CON_RED_PALE);
    if(opt.viewWide){
        ui.push("\0");
    }
    ui.push(" > 命令提示符 ",Mod::cmd);
    ui.push("\n[破解]\n");
    if(IsUserAnAdmin()){
        ui.push(
            " > 极域电子教室 ",Mod::op,
            new Mod::ArgvOp{'c',Mod::rule.mythware.exe,Mod::rule.mythware.svc}
        );
        if(opt.viewWide){
            ui.push("\0");
        }
        ui.push(
            " > 联想云教室 ",Mod::op,
            new Mod::ArgvOp{'c',Mod::rule.lenovo.exe,Mod::rule.lenovo.svc}
        );
    }else{
        ui.push(" (i) 需要提权.");
    }
    ui.push("\n[恢复]\n");
    if(IsUserAnAdmin()){
        ui.push(
            " > 极域电子教室 ",Mod::op,
            new Mod::ArgvOp{'r',Mod::rule.mythware.exe,Mod::rule.mythware.svc}
        );
        if(opt.viewWide){
            ui.push("\0");
        }
        ui.push(
            " > 联想云教室 ",Mod::op,
            new Mod::ArgvOp{'r',Mod::rule.lenovo.exe,Mod::rule.lenovo.svc}
        );
    }else{
        ui.push(" (i) 需要提权.");
    }
    ui.show();
    return 0;
}
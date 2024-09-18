#include"def.hpp"
#include"ui.hpp"
#include"mod.hpp"
auto main(i32 argc,i8 *args[])->i32{
    bool optError{};
    if(argc>1){
        std::string tmp;
        for(i32 i{1};i<argc;++i){
            tmp=args[i];
            if((tmp.size()>2)?(tmp.substr(0,2)=="-W"):(false)){
                for(const auto &j:tmp.substr(2)){
                    switch(j){
                        case 'f':{
                            opt.wndFrontShow=true;
                            break;
                        }case 'a':{
                            opt.wndAlpha=true;
                            break;
                        }case 'c':{
                            opt.wndCtrls=true;
                            break;
                        }default:{
                            optError=true;
                            goto END;
                        }
                    }
                }
            }else{
                optError=true;
                break;
            }
        }
    END:
        if(optError){
            opt={};
        }
    }
    Mod::init();
    if(opt.wndFrontShow){
        std::thread{Mod::frontShow}.detach();
    }
    CUI ui;
    ui.add("                    < 主  页 >\n\n");
    if(optError){
        ui.add(" (!) 参数错误.\n");
    }
    ui.add(" > 退出 ",Mod::exit,nullptr,CON_RED_PALE)
      .add(" > 信息 ",Mod::info)
      .add(" > 命令提示符 ",Mod::cmd)
      .add("\n[破解]\n");
    if(IsUserAnAdmin()){
        ui.add(
            " > 极域电子教室 ",Mod::op,
            new Mod::ArgsOp{'C',Mod::rule.mythware.exe,Mod::rule.mythware.svc}
        ).add(
            " > 联想云教室 ",Mod::op,
            new Mod::ArgsOp{'C',Mod::rule.lenovo.exe,Mod::rule.lenovo.svc}
        );
    }else{
        ui.add(" (i) 需要提权.");
    }
    ui.add("\n[恢复]\n");
    if(IsUserAnAdmin()){
        ui.add(
            " > 极域电子教室 ",Mod::op,
            new Mod::ArgsOp{'R',Mod::rule.mythware.exe,Mod::rule.mythware.svc}
        ).add(
            " > 联想云教室 ",Mod::op,
            new Mod::ArgsOp{'R',Mod::rule.lenovo.exe,Mod::rule.lenovo.svc}
        );
    }else{
        ui.add(" (i) 需要提权.");
    }
    ui.show();
    return 0;
}
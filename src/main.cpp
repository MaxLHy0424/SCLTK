#ifdef _WIN32
#include"def.hpp"
#include"ui.hpp"
#include"mod.hpp"
auto main(const i32 argc,const i8 *const args[])->i32{
    if(argc>1){
        std::string tmp;
        for(i32 i{1};i<argc;++i){
            tmp=args[i];
            if((tmp.size()>2)&&(tmp.substr(0,2)=="-W")){
                for(const auto &ref:tmp.substr(2,tmp.size())){
                    switch(ref){
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
        std::thread(Mod::frontShow).detach();
    }
    UI ui;
    ui.add("                    < 主  页 >\n\n");
    if(optError){
        ui.add(" (!) 参数错误.\n");
    }
    ui.add(" > 退出 ",Mod::exit,{},WC_RED)
      .add(" > 信息 ",Mod::info)
      .add(" > 命令提示符 ",Mod::cmd)
      .add("\n[ 破 解 ]\n")
      .add(" > 极域电子教室 ",Mod::op,Mod::ArgsOp('C',Mod::rule.mythware.exe,Mod::rule.mythware.svc))
      .add(" > 联想云教室 ",Mod::op,Mod::ArgsOp('C',Mod::rule.lenovo.exe,Mod::rule.lenovo.svc))
      .add("\n[ 恢 复 ]\n")
      .add(" > 极域电子教室 ",Mod::op,Mod::ArgsOp('R',Mod::rule.mythware.exe,Mod::rule.mythware.svc))
      .add(" > 联想云教室 ",Mod::op,Mod::ArgsOp('R',Mod::rule.lenovo.exe,Mod::rule.lenovo.svc))
      .show();
    return 0;
}
#endif
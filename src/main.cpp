#include"header.hpp"
#include"ui.hpp"
CUI ui;
#include"modules.hpp"
auto main(i32 argc,cstr argv[])->i32{
    bool optError{};
    if(argc>1){
        std::string t;
        for(i32 i{1};i<argc;++i){
            t=argv[i];
            if((t.substr(0,2)=="-W")&&(t.length()>2ull)){
                for(u64 j{2ull};j<t.length();++j){
                    switch(t[j]){
                        case 'c':{
                            Pvt::opt.wndCtrls=true;
                            break;
                        }case 'f':{
                            Pvt::opt.frontShow=true;
                            break;
                        }case 'a':{
                            Pvt::opt.alphaWnd=true;
                            break;
                        }default:{
                            optError=true;
                            goto INIT;
                        }
                    }
                }
            }else if(t=="--sparse-view"){
                Pvt::opt.sparseView=true;
            }else if(t=="/command"){
                continue;
            }else{
                optError=true;
                break;
            }
        }
    INIT:
        if(optError){
            Pvt::opt={};
        }
    }
    Pvt::init(Pvt::opt.wndCtrls,Pvt::opt.alphaWnd);
    if(Pvt::opt.frontShow){
        std::thread(Pvt::frontShow).detach();
    }
    ui.push("    [ Computer Room Control Software Nemesis ]");
    if(Pvt::opt.sparseView){
        ui.push("\0");
    }
    ui.push("                     v5.0.0");
    ui.push("       https://github.com/MaxLHy0424/CRCSN");
    ui.push("     (C) 2023 MaxLHy0424. All Rights Reserved.\n");
    if(optError){
        ui.push(" (!) 命令行参数错误.\n");
    }
    ui.push(" > 退出 ",EXIT,BLACK_RED,BLACK_RED_PALE);
    if(Pvt::opt.sparseView){
        ui.push("\0");
    }
    ui.push(" > 命令提示符 ",Pvt::cmd);
    ui.push("\n[ 破 解 ]\n");
    if(IsUserAnAdmin()){
        ui.push(" > 极域电子教室 ",Pvt::Crack::mythware);
        if(Pvt::opt.sparseView){
            ui.push("\0");
        }
        ui.push(" > 联想云教室 ",Pvt::Crack::lenovo);
    }else{
        ui.push(" (i) 需要管理员权限.");
    }
    ui.push("\n[ 恢 复 ]\n");
    if(IsUserAnAdmin()){
        ui.push(" > 极域电子教室 ",Pvt::Recovery::mythware);
        if(Pvt::opt.sparseView){
            ui.push("\0");
        }
        ui.push(" > 联想云教室 ",Pvt::Recovery::lenovo);
    }else{
        ui.push(" (i) 需要管理员权限.");
    }
    ui.show();
    return 0;
}
#include"header.hpp"
#include"ui.hpp"
CUI ui;
#include"modules.hpp"
auto main(i32 argc,char* argv[])->i32{
    bool optError{};
    if(argc>1){
        std::string tmp;
        for(i32 i{1};i<argc;++i){
            tmp=argv[i];
            if((tmp.substr(0,2)=="-W")&&(tmp.size()>2ull)){
                for(u64 j{2ull};j<tmp.size();++j){
                    switch(tmp[j]){
                        case 'c':{
                            Lib::opt.wndCtrls=true;
                            break;
                        }case 'f':{
                            Lib::opt.frontShow=true;
                            break;
                        }case 'a':{
                            Lib::opt.alphaWnd=true;
                            break;
                        }default:{
                            optError=true;
                            goto INIT;
                        }
                    }
                }
            }else if(tmp=="--sparse-view"){
                Lib::opt.sparseView=true;
            }else if(tmp=="/command"){
                continue;
            }else{
                optError=true;
                break;
            }
        }
    INIT:
        if(optError){
            Lib::opt={};
        }
    }
    Lib::init(Lib::opt.wndCtrls,Lib::opt.alphaWnd);
    if(Lib::opt.frontShow){
        std::thread(Lib::frontShow).detach();
    }
    ui.push("    [ Computer Room Control Software Nemesis ]");
    if(Lib::opt.sparseView){
        ui.push("\0");
    }
    ui.push("                     v5.0.0");
    ui.push("       https://github.com/MaxLHy0424/CRCSN");
    ui.push("     (C) 2023 MaxLHy0424. All Rights Reserved.\n");
    if(optError){
        ui.push(" (!) 存在错误的命令行参数.\n");
    }
    ui.push(" > 退出 ",EXIT);
    if(Lib::opt.sparseView){
        ui.push("\0");
    }
    ui.push(" > 命令提示符 ",Lib::cmd);
    ui.push("\n[ 破 解 ]\n");
    if(IsUserAnAdmin()){
        ui.push(" > 极域电子教室 ",Lib::Crack::mythware);
        if(Lib::opt.sparseView){
            ui.push("\0");
        }
        ui.push(" > 联想云教室 ",Lib::Crack::lenovo);
    }else{
        ui.push(" (i) 需要管理员权限.");
    }
    ui.push("\n[ 恢 复 ]\n");
    if(IsUserAnAdmin()){
        ui.push(" > 极域电子教室 ",Lib::Recovery::mythware);
        if(Lib::opt.sparseView){
            ui.push("\0");
        }
        ui.push(" > 联想云教室 ",Lib::Recovery::lenovo);
    }else{
        ui.push(" (i) 需要管理员权限.");
    }
    ui.show();
    return 0;
}
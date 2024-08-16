#include"header.hpp"
#include"ui.hpp"
CUI ui;
#include"modules.hpp"
auto main(int argc,char* argv[])->int{
    bool errOpt{};
    {
        std::string tmp;
        if(argc==1){
            goto BEGIN;
        }
        for(int i{1};i<argc;++i){
            tmp=argv[i];
            if((tmp.substr(0,2)=="-W")&&(tmp.size()>2ull)){
                for(uint64_t j{2ull};j<tmp.size();++j){
                    switch(tmp[j]){
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
                            errOpt=true;
                        }
                    }
                }
            }else if(tmp=="--sparse-view"){
                Pvt::opt.sparseView=true;
            }else if(tmp=="/command"){
                continue;
            }else{
                errOpt=true;
                goto BEGIN;
            }
        }
    BEGIN:
        if(errOpt){
            Pvt::opt={};
        }
        Pvt::init(Pvt::opt.wndCtrls,Pvt::opt.alphaWnd);
        if(Pvt::opt.frontShow){
            std::thread(Pvt::frontShow).detach();
        }
    }
    ui.push("    [ Computer Room Control Software Nemesis ]");
    if(Pvt::opt.sparseView){
        ui.push("\0");
    }
    ui.push("                     v5.0.0");
    ui.push("       https://github.com/MaxLHy0424/CRCSN");
    ui.push("     (C) 2023 MaxLHy0424. All Rights Reserved.\n");
    if(errOpt){
        ui.push(" (!) 命令行参数错误.\n");
    }
    ui.push(" > 退出 ",EXIT);
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
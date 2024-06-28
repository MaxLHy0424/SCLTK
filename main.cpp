#include"head.hpp"
#include"ui.hpp"
Menu Activity;
#include"module.hpp"
int main(int argc,char* argv[]){
    CRCSN::Task::WndInit();
    unsigned char initState{};
    if(argc==1){
        goto BEGIN;
    }
    {
        struct Options{
            bool wndPin;
        }Opt{};
        std::string tmp{};
        for(int i{1};i<argc;++i){
            tmp=argv[i];
            if((Opt.wndPin==false)&&(tmp=="-Wp")){
                Opt.wndPin=true;
            }
            else{
                initState=1U;
                goto BEGIN;
            }
        }
        if(Opt.wndPin==true){
            std::thread(CRCSN::Task::WndPin).detach();
        }
    }
BEGIN:
    system("CLS");
    Activity.push_back("< Computer Room Control Software Nemesis >");
    Activity.push_back("                  24w27d");
    Activity.push_back("    https://github.com/MaxLHy0424/CRCSN");
    Activity.push_back(" (C) 2024 MaxLHy0424, All Rights Reserved.\n\n");
    if(initState==1U){
        Activity.push_back("(!) 启动参数错误.\n\n");
    }
    Activity.push_back(" |   退    出   | ",ExitMenu);
    Activity.push_back("\n[  破  解  ]\n");
    Activity.push_back(" | 极域电子教室 | ",CRCSN::Crack::ELearningClass);
    Activity.push_back(" |  联想云教室  | ",CRCSN::Crack::LenovoEClass);
    Activity.push_back("\n[  恢  复  ]\n");
    if(IsUserAnAdmin()){
        Activity.push_back(" | 极域电子教室 | ",CRCSN::Recovery::ELearningClass);
        Activity.push_back(" |  联想云教室  | ",CRCSN::Recovery::LenovoEClass);
    }else{
        Activity.push_back(" (i) 权限过低, 不可使用.");
    }
    Activity.push_back("\n[ 工 具 箱 ]\n");
    Activity.push_back(" (i) 正在开发.");
    Activity.ShowMenu();
    return 0;
}
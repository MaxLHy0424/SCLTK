#include"head.hpp"
#include"ui.hpp"
Menu Activity;
#include"module.hpp"
int main(int argc,char* argv[]){
    char error{};
    {
        struct Options{
            bool wndControls,wndAlpha,wndPin;
        }Opt{};
        if(argc==1){
            goto BEGIN;
        }
        for(int i{1};i<argc;++i){
            std::string tmp{argv[i]};
            if((tmp.length()>2)&&(tmp.substr(0,2)=="-W")){
                for(unsigned long long j{2};j<tmp.length();++j){
                    switch(tmp[j]){
                        case 'c':{
                            Opt.wndControls=true;
                            break;
                        }case 'a':{
                            Opt.wndAlpha=true;
                            break;
                        }case 'p':{
                            Opt.wndPin=true;
                            break;
                        }default:{
                            Opt={};
                            error=2;
                        }
                    }
                }
            }else{
                error=1;
                goto BEGIN;
            }
        }
BEGIN:
        CRCSN::Wnd::Init(Opt.wndControls,Opt.wndAlpha);
        if(Opt.wndPin==true){
            std::thread(CRCSN::Wnd::Pin).detach();
        }
    }
    system("CLS");
    Activity.PushBack("   | Computer Room Control Software Nemesis |");
    Activity.PushBack("                    24w28a");
    Activity.PushBack("     https://github.com/MaxLHy0424/CRCSN");
    Activity.PushBack("    (C) 2024 MaxLHy0424, All Rights Reserved.\n");
    if(error!=0){
        Activity.PushBack(" (!) 发生错误 "+std::to_string(error)+".\n");
    }
    Activity.PushBack(" > 退出 ",ExitProc);
    Activity.PushBack("\n[  破  解  ]\n");
    Activity.PushBack(" > 极域电子教室 ",CRCSN::Crack::ELearningClass);
    Activity.PushBack(" > 联想云教室 ",CRCSN::Crack::LenovoEClass);
    Activity.PushBack("\n[  恢  复  ]\n");
    if(IsUserAnAdmin()){
        Activity.PushBack(" > 极域电子教室 ",CRCSN::Recovery::ELearningClass);
        Activity.PushBack(" > 联想云教室 ",CRCSN::Recovery::LenovoEClass);
    }else{
        Activity.PushBack(" (i) 需要管理员权限.");
    }
    Activity.PushBack("\n[ 工 具 箱 ]\n");
    Activity.PushBack(" (i) 正在开发.");
    Activity.ShowMenu();
    return 0;
}
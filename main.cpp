#include"head.hpp"
#include"ui.hpp"
Menu Ui;
#include"module.hpp"
int main(int argc,char* argv[]){
    bool errArgv{};
    {
        struct Options{
            bool wndCtrls,wndAlpha,wndPin;
        }Opt{};
        std::string tmp{};
        if(argc==1){
            goto BEGIN;
        }
        for(int i{1};i<argc;++i){
            tmp=argv[i];
            if((tmp.length()>2)&&(tmp.substr(0,2)=="-W")){
                for(unsigned long long j{2};j<tmp.length();++j){
                    switch(tmp[j]){
                        case 'c':{
                            Opt.wndCtrls=true;
                            break;
                        }case 'p':{
                            Opt.wndPin=true;
                            break;
                        }case 'a':{
                            Opt.wndAlpha=true;
                            break;
                        }default:{
                            Opt={};
                            errArgv=true;
                        }
                    }
                }
            }else{
                errArgv=true;
                goto BEGIN;
            }
        }
BEGIN:
        CRCSN::Wnd::Init(Opt.wndCtrls,Opt.wndAlpha);
        if(Opt.wndPin==true){
            std::thread(CRCSN::Wnd::Pin).detach();
        }
    }
    system("cls");
    Ui.PushBack("   | Computer Room Control Software Nemesis |");
    Ui.PushBack("                    24w28c");
    Ui.PushBack("     https://github.com/MaxLHy0424/CRCSN");
    Ui.PushBack("    (C) 2024 MaxLHy0424, All Rights Reserved.\n");
    if(errArgv){
        Ui.PushBack(" (!) 参数错误.\n");
    }
    Ui.PushBack(" > 退出 ",ExitProc);
    Ui.PushBack("\n[  破  解  ]\n");
    Ui.PushBack(" > 极域电子教室 ",CRCSN::Crack::ELearningClass);
    Ui.PushBack(" > 联想云教室 ",CRCSN::Crack::LenovoEClass);
    Ui.PushBack("\n[  恢  复  ]\n");
    if(IsUserAnAdmin()){
        Ui.PushBack(" > 极域电子教室 ",CRCSN::Recovery::ELearningClass);
        Ui.PushBack(" > 联想云教室 ",CRCSN::Recovery::LenovoEClass);
    }else{
        Ui.PushBack(" (i) 需要管理员权限.");
    }
    Ui.PushBack("\n[ 工 具 箱 ]\n");
    Ui.PushBack(" (i) 正在开发.");
    Ui.ShowMenu();
    return 0;
}
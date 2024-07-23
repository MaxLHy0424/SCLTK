#include"head.hpp"
#include"ui.hpp"
#include"module.hpp"
int main(int argc,char* argv[]){
    bool errArgv{};
    {
        struct Options{
            bool wndCtrls,wndPin,wndAlpha;
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
        CRCSN::Tsk::Init(Opt.wndCtrls,Opt.wndAlpha);
        if(Opt.wndPin){
            std::thread(CRCSN::Tsk::Pin).detach();
        }
    }
    UI.PushBack("   | Computer Room Control Software Nemesis |");
    UI.PushBack("                    24w31b");
    UI.PushBack("     https://github.com/MaxLHy0424/CRCSN");
    UI.PushBack("    (C) 2024 MaxLHy0424, All Rights Reserved.\n");
    if(errArgv){
        UI.PushBack(" (!) 参数错误.\n");
    }
    UI.PushBack(" > 退出 ",ExitProc);
    UI.PushBack("\n[ 破 解 ]\n");
    if(!IsUserAnAdmin()){
        UI.PushBack(" (i) 当前权限破解可能无效.\n");
    }
    UI.PushBack(" > 极域电子教室 ",CRCSN::Crack::ELearningClass);
    UI.PushBack(" > 联想云教室 ",CRCSN::Crack::LenovoEClass);
    UI.PushBack("\n[ 恢 复 ]\n");
    if(IsUserAnAdmin()){
        UI.PushBack(" > 极域电子教室 ",CRCSN::Recovery::ELearningClass);
        UI.PushBack(" > 联想云教室 ",CRCSN::Recovery::LenovoEClass);
    }else{
        UI.PushBack(" (i) 需要管理员权限.");
    }
    UI.PushBack("\n[ 移 除 ]\n");
    UI.PushBack(" (i) 正在开发.");
    UI.Show();
    return 0;
}
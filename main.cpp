#include"head.hpp"
#include"ui.hpp"
#include"module.hpp"
int main(int argc,char* argv[]){
    bool errArgv{};
    {
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
                            CRCSN::Opt.wndCtrls=true;
                            break;
                        }case 'f':{
                            CRCSN::Opt.wndForceShow=true;
                            break;
                        }case 'a':{
                            CRCSN::Opt.wndAlpha=true;
                            break;
                        }default:{
                            CRCSN::Opt={};
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
        CRCSN::General::Init(CRCSN::Opt.wndCtrls,CRCSN::Opt.wndAlpha);
        if(CRCSN::Opt.wndForceShow){
            std::thread(CRCSN::General::ForceShow).detach();
        }
    }
    UI.PushBack("   | Computer Room Control Software Nemesis |");
    UI.PushBack("                    24w31c");
    UI.PushBack("     https://github.com/MaxLHy0424/CRCSN");
    UI.PushBack("    (C) 2023 MaxLHy0424. All Rights Reserved.\n");
    if(errArgv){
        UI.PushBack(" (!) 参数错误.\n");
    }
    UI.PushBack(" > 退出 ",Exit);
    UI.PushBack("\n[ 破 解 ]\n");
    if(!IsUserAnAdmin()){
        UI.PushBack(" (i) 当前权限破解可能无效.\n");
    }
    UI.PushBack(" > 极域电子教室 ",CRCSN::Crack::Mythware);
    UI.PushBack(" > 联想云教室 ",CRCSN::Crack::Lenovo);
    UI.PushBack("\n[ 恢 复 ]\n");
    if(IsUserAnAdmin()){
        UI.PushBack(" > 极域电子教室 ",CRCSN::Recovery::Mythware);
        UI.PushBack(" > 联想云教室 ",CRCSN::Recovery::Lenovo);
    }else{
        UI.PushBack(" (i) 需要管理员权限.");
    }
    UI.PushBack("\n[ 移 除 ]\n");
    UI.PushBack(" (i) 正在开发.");
    UI.Show();
    return 0;
}
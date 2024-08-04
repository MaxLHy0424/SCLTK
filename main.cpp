#include"hd.hpp"
#include"ui.hpp"
#include"module.hpp"
auto main(int argc,char* argv[])->int{
    bool errOpt{};
    {
        std::string t;
        if(!(argc^1)){
            goto BEGIN;
        }
        for(int i{1};i<argc;++i){
            t=argv[i];
            if((t.size()>2ULL)&&(t.substr(0,2)=="-W")){
                for(uint64_t j{2};j<t.size();++j){
                    switch(t[j]){
                        case 'c':{
                            CRCSN::Opt.ctrls=true;
                            break;
                        }case 'f':{
                            CRCSN::Opt.frontShow=true;
                            break;
                        }case 'a':{
                            CRCSN::Opt.alphaWnd=true;
                            break;
                        }default:{
                            CRCSN::Opt={};
                            errOpt=true;
                        }
                    }
                }
            }else{
                errOpt=true;
                goto BEGIN;
            }
        }
    BEGIN:
        CRCSN::Init(CRCSN::Opt.ctrls,CRCSN::Opt.alphaWnd);
        if(CRCSN::Opt.frontShow){
            std::thread(CRCSN::FrontShow).detach();
        }
    }
    UI.Push("   | Computer Room Control Software Nemesis |");
    UI.Push("                  v5.0.0-beta.1");
    UI.Push("       https://github.com/MaxLHy0424/CRCSN");
    UI.Push("    (C) 2023 MaxLHy0424. All Rights Reserved.\n");
    if(errOpt){
        UI.Push(" (!) 参数错误.\n");
    }
    UI.Push(" > 退出 ",Exit);
    UI.Push(" > 命令提示符 ",CRCSN::Cmd);
    UI.Push("\n[ 破 解 ]\n");
    if(IsUserAnAdmin()){
        UI.Push(" > 极域电子教室 ",CRCSN::Crack::Mythware);
        UI.Push(" > 联想云教室 ",CRCSN::Crack::Lenovo);
    }else{
        UI.Push(" (i) 需要管理员权限.");
    }
    UI.Push("\n[ 恢 复 ]\n");
    if(IsUserAnAdmin()){
        UI.Push(" > 极域电子教室 ",CRCSN::Recovery::Mythware);
        UI.Push(" > 联想云教室 ",CRCSN::Recovery::Lenovo);
    }else{
        UI.Push(" (i) 需要管理员权限.");
    }
    UI.Show();
    return 0;
}
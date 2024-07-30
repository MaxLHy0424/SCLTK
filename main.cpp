#include"head.hpp"
#include"ui.hpp"
Menu UI;
#include"module.hpp"
int main(int argc,char* argv[]){
    bool errOpt{};
    {
        std::string tmp;
        if(!(argc^1)){
            goto BEGIN;
        }
        for(int i{1};i<argc;++i){
            tmp=argv[i];
            if((tmp.length()>2UL)&&(tmp.substr(0,2)=="-W")){
                for(uint64_t j{2};j<tmp.length();++j){
                    switch(tmp[j]){
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
    UI.Push("                     24w32a");
    UI.Push("      https://github.com/MaxLHy0424/CRCSN");
    UI.Push("    (C) 2023 MaxLHy0424. All Rights Reserved.\n");
    if(errOpt){
        UI.Push(" (!) 参数错误.\n");
    }
    UI.Push(" > 退出 ",Exit);
    UI.Push(" > 命令提示符 ",CRCSN::Cmd);
    UI.Push("\n[ 破 解 ]\n");
    UI.Push(" > 极域电子教室 ",CRCSN::Crack::Mythware);
    UI.Push(" > 联想云教室 ",CRCSN::Crack::Lenovo);
    UI.Push("\n[ 恢 复 ]\n");
    if(IsUserAnAdmin()){
        UI.Push(" > 极域电子教室 ",CRCSN::Recovery::Mythware);
        UI.Push(" > 联想云教室 ",CRCSN::Recovery::Lenovo);
    }else{
        UI.Push(" (i) 需要 Admin 权限.");
    }
    UI.Show();
    return 0;
}
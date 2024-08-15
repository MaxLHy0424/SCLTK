#include"header.hpp"
#include"ui.hpp"
#include"module.hpp"
auto main(int argc,char* argv[])->int{
    bool errOpt{};
    {
        std::string t;
        if(argc==1){
            goto BEGIN;
        }
        for(int i{1};i<argc;++i){
            t=argv[i];
            if((t.size()>2ull)&&(t.substr(0,2)=="-W")){
                for(uint64_t j{2ull};j<t.size();++j){
                    switch(t[j]){
                        case 'c':{
                            CRCSN::Opt.wndCtrls=true;
                            break;
                        }case 'f':{
                            CRCSN::Opt.frontShow=true;
                            break;
                        }case 'a':{
                            CRCSN::Opt.alphaWnd=true;
                            break;
                        }default:{
                            errOpt=true;
                        }
                    }
                }
            }else if(t=="--compact-view"){
                CRCSN::Opt.compactView=true;
            }else if(t=="/command"){
                continue;
            }else{
                errOpt=true;
                goto BEGIN;
            }
        }
    BEGIN:
        if(errOpt){
            CRCSN::Opt={};
        }
        CRCSN::Init(CRCSN::Opt.wndCtrls,CRCSN::Opt.alphaWnd);
        if(CRCSN::Opt.frontShow){
            std::thread(CRCSN::FrontShow).detach();
        }
    }
    UI.Push("    [ Computer Room Control Software Nemesis ]");
    if(!CRCSN::Opt.compactView){
        UI.Push("\0");
    }
    UI.Push("                  v5.0.0-beta.2");
    UI.Push("       https://github.com/MaxLHy0424/CRCSN");
    UI.Push("     (C) 2023 MaxLHy0424. All Rights Reserved.\n");
    if(errOpt){
        UI.Push(" (!) 存在未知的命令行参数.\n");
    }
    UI.Push(" > 退出 ",EXIT);
    if(!CRCSN::Opt.compactView){
        UI.Push("\0");
    }
    UI.Push(" > 命令提示符 ",CRCSN::Cmd);
    UI.Push("\n[ 破 解 ]\n");
    if(IsUserAnAdmin()){
        UI.Push(" > 极域电子教室 ",CRCSN::Crack::Mythware);
        if(!CRCSN::Opt.compactView){
            UI.Push("\0");
        }
        UI.Push(" > 联想云教室 ",CRCSN::Crack::Lenovo);
    }else{
        UI.Push(" (i) 需要管理员权限.");
    }
    UI.Push("\n[ 恢 复 ]\n");
    if(IsUserAnAdmin()){
        UI.Push(" > 极域电子教室 ",CRCSN::Recovery::Mythware);
        if(!CRCSN::Opt.compactView){
            UI.Push("\0");
        }
        UI.Push(" > 联想云教室 ",CRCSN::Recovery::Lenovo);
    }else{
        UI.Push(" (i) 需要管理员权限.");
    }
    UI.Show();
    return 0;
}
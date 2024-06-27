#include"head.hpp"
#include"ui.hpp"
#include"module.hpp"
Menu Activity;
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
    Activity.push_back("| CRCSN |\n");
    if(initState==1U){
        Activity.push_back("(X) 启动参数错误\n");
    }
    Activity.push_back(" 退出 ",ExitMenu).push_back("\0");
    Activity.push_back(" 关于 ",(callback)CRCSN::About);
    Activity.push_back("\n[破解]\n");
    Activity.push_back(" 极域电子教室 ",(callback)CRCSN::Crack::MythwareClassroom);
    Activity.push_back(" 联想云教室 ",(callback)CRCSN::Crack::LenovoCloudClassroom);
    Activity.push_back("\n[恢复]\n");
    if(IsUserAnAdmin()){
        Activity.push_back(" 极域电子教室 ",(callback)CRCSN::Recovery::MythwareClassroom);
        Activity.push_back(" 联想云教室 ",(callback)CRCSN::Recovery::LenovoCloudClassroom);
    }else{
        Activity.push_back(" (i) 权限过低, 不可使用.");
    }
    Activity.push_back("\n[工具]\n");
    Activity.push_back(" (i) 正在开发.");
    Activity.ShowMenu();
    return 0;
}
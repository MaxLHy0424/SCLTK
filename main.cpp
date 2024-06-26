char Dat[3]{};
#include"head.hpp"
#include"module.hpp"
#include"ui.hpp"
void Info(){
    puts("[信息]\n");
    puts("  Computer Room Control Software Nemesis");
    puts("                 24w27a\n");
    puts("  https://github.com/MaxLHy0424/CRCSN");
    puts("  (C) 2024 MaxLHy0424, All Rights Reserved.\n");
    puts("---------------------------------------------\n");
    printf("按任意键返回.");
    system("Pause>NUL");
    return;
}
void Crack(){
BEGIN:
    puts("[破解]\n");
    puts("  - 返回");
    puts("  1 极域电子教室");
    puts("  2 联想云教室\n");
    printf("请输入: ");
    for(;;){
        scanf("%s",&Dat[0]);
        if((Dat[1]==0)&&(Dat[0]=='-'||Dat[0]=='1'||Dat[0]=='2')){
            break;
        }
        printf("输入错误, 请重试: ");
    }
    if(Dat[0]=='-'){
        return;
    }
    if(Continue()){
        puts("\n-----------------------------------\n");
        switch(Dat[0]){
            case '1':{
                system("TaskKill /F /IM StudentMain.exe");
                system("TaskKill /F /IM DispcapHelper.exe");
                system("TaskKill /F /IM InstHelpApp.exe");
                system("TaskKill /F /IM GATESRV.exe");
                system("TaskKill /F /IM ProcHelper64.exe");
                system("TaskKill /F /IM MasterHelper.exe");
                if(IsUserAnAdmin()){
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\StudentMain.exe\" /F /T REG_SZ /V debugger /D ?");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DispcapHelper.exe\" /F /T REG_SZ /V debugger /D ?");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\VRCwPlayer.exe\" /F /T REG_SZ /V debugger /D ?");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\InstHelpApp.exe\" /F /T REG_SZ /V debugger /D ?");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\InstHelpApp64.exe\" /F /T REG_SZ /V debugger /D ?");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\TDOvrSet.exe\" /F /T REG_SZ /V debugger /D ?");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\GATESRV.exe\" /F /T REG_SZ /V debugger /D ?");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\ProcHelper64.exe\" /F /T REG_SZ /V debugger /D ?");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\MasterHelper.exe\" /F /T REG_SZ /V debugger /D ?");
                    system("Net Stop STUDSRV /Y");
                    system("Net Stop TDNetFilter /Y");
                    system("Net Stop TDFileFilter /Y");
                }
                break;
            }case '2':{
                system("TaskKill /F /IM vncviewer.exe");
                system("TaskKill /F /IM tvnserver32.exe");
                system("TaskKill /F /IM WfbsPnpInstall.exe");
                system("TaskKill /F /IM WFBSMon.exe");
                system("TaskKill /F /IM WFBSMlogon.exe");
                system("TaskKill /F /IM WFBSSvrLogShow.exe");
                system("TaskKill /F /IM ResetIp.exe");
                system("TaskKill /F /IM Fireware.exe");
                system("TaskKill /F /IM BCDBootCopy.exe");
                system("TaskKill /F /IM refreship.exe");
                system("TaskKill /F /IM LenovoLockScreen.exe");
                system("TaskKill /F /IM PortControl64.exe");
                system("TaskKill /F /IM DesktopCheck.exe");
                system("TaskKill /F /IM DeploymentManager.exe");
                system("TaskKill /F /IM DeploymentAgent.exe");
                system("TaskKill /F /IM XYNTService.exe");
                if(IsUserAnAdmin()){
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\vncviewer.exe\" /F /T REG_SZ /V debugger /D ?");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\tvnserver32.exe\" /F /T REG_SZ /V debugger /D ?");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WfbsPnpInstall.exe\" /F /T REG_SZ /V debugger /D ?");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSMon.exe\" /F /T REG_SZ /V debugger /D ?");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSMlogon.exe\" /F /T REG_SZ /V debugger /D ?");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSSvrLogShow.exe\" /F /T REG_SZ /V debugger /D ?");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\ResetIp.exe\" /F /T REG_SZ /V debugger /D ?");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\FuncForWIN64.exe\" /F /T REG_SZ /V debugger /D ?");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\CertMgr.exe\" /F /T REG_SZ /V debugger /D ?");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\Fireware.exe\" /F /T REG_SZ /V debugger /D ?");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\BCDBootCopy.exe\" /F /T REG_SZ /V debugger /D ?");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\refreship.exe\" /F /T REG_SZ /V debugger /D ?");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\LenovoLockScreen.exe\" /F /T REG_SZ /V debugger /D ?");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\PortControl64.exe\" /F /T REG_SZ /V debugger /D ?");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DesktopCheck.exe\" /F /T REG_SZ /V debugger /D ?");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DeploymentManager.exe\" /F /T REG_SZ /V debugger /D ?");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DeploymentAgent.exe\" /F /T REG_SZ /V debugger /D ?");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\XYNTService.exe\" /F /T REG_SZ /V debugger /D ?");
                    system("Net Stop BSAgentSvr /Y");
                    system("Net Stop tvnserver /Y");
                    system("Net Stop WFBSMlogon /Y");
                }
                break;
            }
        }
        puts("\n-----------------------------------\n");
        printf("按任意键返回.");
        system("Pause>NUL");
    }
    system("CLS");
    goto BEGIN;
}
void Recovery(){
BEGIN:
    puts("[恢复]\n");
    if(!IsUserAnAdmin()){
        puts("请以管理员权限运行.\n");
        puts("--------------------\n");
        printf("按任意键返回.");
        system("Pause>NUL");
        return;
    }
    puts("  - 返回");
    puts("  1 极域电子教室");
    puts("  2 联想云教室\n");
    printf("请输入: ");
    for(;;){
        scanf("%s",&Dat[0]);
        if((Dat[1]==0)&&(Dat[0]=='-'||Dat[0]=='1'||Dat[0]=='2')){
            break;
        }
        printf("输入错误, 请重试: ");
    }
    if(Dat[0]=='-'){
        return;
    }
    if(Continue()){
        puts("\n-----------------------------------\n");
        switch(Dat[0]){
            case '1':{
                system("Reg Delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\StudentMain.exe\" /F");
                system("Reg Delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DispcapHelper.exe\" /F");
                system("Reg Delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\VRCwPlayer.exe\" /F");
                system("Reg Delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\InstHelpApp.exe\" /F");
                system("Reg Delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\InstHelpApp64.exe\" /F");
                system("Reg Delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\TDOvrSet.exe\" /F");
                system("Reg Delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\GATESRV.exe\" /F");
                system("Reg Delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\ProcHelper64.exe\" /F");
                system("Reg Delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\MasterHelper.exe\" /F");
                system("Net Start STUDSRV");
                system("Net Start TDNetFilter");
                system("Net Start TDFileFilter");
                break;
            }case '2':{
                system("Reg Delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\vncviewer.exe\" /F");
                system("Reg Delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\tvnserver32.exe\" /F");
                system("Reg Delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WfbsPnpInstall.exe\" /F");
                system("Reg Delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSMon.exe\" /F");
                system("Reg Delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSMlogon.exe\" /F");
                system("Reg Delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSSvrLogShow.exe\" /F");
                system("Reg Delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\ResetIp.exe\" /F");
                system("Reg Delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\FuncForWIN64.exe\" /F");
                system("Reg Delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\CertMgr.exe\" /F");
                system("Reg Delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\Fireware.exe\" /F");
                system("Reg Delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\BCDBootCopy.exe\" /F");
                system("Reg Delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\refreship.exe\" /F");
                system("Reg Delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\LenovoLockScreen.exe\" /F");
                system("Reg Delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\PortControl64.exe\" /F");
                system("Reg Delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DesktopCheck.exe\" /F");
                system("Reg Delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DeploymentManager.exe\" /F");
                system("Reg Delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DeploymentAgent.exe\" /F");
                system("Reg Delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\XYNTService.exe\" /F");
                system("Net Start BSAgentSvr");
                system("Net Start tvnserver");
                system("Net Start WFBSMlogon");
                break;
            }
        }
        puts("------------------------------------\n");
        printf("按任意键返回.");
        system("Pause>NUL");
    }
    system("CLS");
    goto BEGIN;
}
void Tools(){
    puts("[工具]\n");
    puts("正在开发.\n");
    puts("---------------\n");
    printf("按任意键返回.");
    system("Pause>NUL");
    return;
}
int main(int argc,char* argv[]){
    system("Title CRCSN");
    if(argc==1){
        goto BEGIN;
    }
    {
        struct Options{
            std::string color;
            bool wndPin;
        }Opt{};
        std::string tmp{};
        for(int i{1};i<argc;++i){
            tmp=argv[i];
            if((Opt.color=="")&&(tmp.substr(0,7)=="-color=")&&(tmp.length()>7)){
                tmp.erase(0,7);
                Opt.color="Color "+tmp;
            }else if((Opt.wndPin==false)&&(tmp=="-Wp")){
                Opt.wndPin=true;
            }
            else{
                puts("启动参数错误.\n");
                puts("---------------\n");
                printf("按任意键继续.");
                system("Pause>NUL");
                goto BEGIN;
            }
        }
        system(Opt.color.c_str());
        if(Opt.wndPin==true){
            std::thread(PinWnd).detach();
        }
    }
BEGIN:
    system("CLS");
    puts("[首页]\n");
    puts("  ? 信息");
    puts("  1 破解");
    puts("  2 恢复");
    puts("  3 工具\n");
    printf("请输入: ");
    for(;;){
        scanf("%s",&Dat[0]);
        if((Dat[1]==0)&&(Dat[0]=='?'||Dat[0]=='1'||Dat[0]=='2'||Dat[0]=='3')){
            break;
        }
        printf("输入错误, 请重试: ");
    }
    system("CLS");
    switch(Dat[0]){
        case '?':{
            Info();
            break;
        }case '1':{
            Crack();
            break;
        }case '2':{
            Recovery();
            break;
        }case '3':{
            Tools();
            break;
        }
    }
    goto BEGIN;
}
#include"extern.hpp"
void About(){
    puts("[关于]\n");
    puts("  Computer Room Control Software Nemesis");
    puts("\t\tv4.10.2\n");
    puts("  https://github.com/MaxLHy0424/CRCSN");
    puts("  (C) 2024 MaxLHy0424, All Rights Reserved.\n");
    puts("---------------------------------------------\n");
    printf("按任意键返回.");
    system("Pause > NUL");
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
        puts("\n------------------------------\n");
        switch(Dat[0]){
            case '1':{
                system("TaskKill /F /IM StudentMain.exe");
                system("TaskKill /F /IM DispcapHelper.exe");
                system("TaskKill /F /IM InstHelpApp.exe");
                system("TaskKill /F /IM GATESRV.exe");
                system("TaskKill /F /IM ProcHelper64.exe");
                system("TaskKill /F /IM MasterHelper.exe");
                if(IsUserAnAdmin()){
                    system("Reg Add /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\StudentMain.exe\" /T REG_SZ /V debugger /D ?");
                    system("Reg Add /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DispcapHelper.exe\" /T REG_SZ /V debugger /D ?");
                    system("Reg Add /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\VRCwPlayer.exe\" /T REG_SZ /V debugger /D ?");
                    system("Reg Add /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\InstHelpApp.exe\" /T REG_SZ /V debugger /D ?");
                    system("Reg Add /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\InstHelpApp64.exe\" /T REG_SZ /V debugger /D ?");
                    system("Reg Add /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\TDOvrSet.exe\" /T REG_SZ /V debugger /D ?");
                    system("Reg Add /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\GATESRV.exe\" /T REG_SZ /V debugger /D ?");
                    system("Reg Add /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\ProcHelper64.exe\" /T REG_SZ /V debugger /D ?");
                    system("Reg Add /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\MasterHelper.exe\" /T REG_SZ /V debugger /D ?");
                    system("Net Stop /Y STUDSRV");
                    system("Net Stop /Y TDNetFilter");
                    system("Net Stop /Y TDFileFilter");
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
                    system("Reg Add /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\vncviewer.exe\" /T REG_SZ /V debugger /D ?");
                    system("Reg Add /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\tvnserver32.exe\" /T REG_SZ /V debugger /D ?");
                    system("Reg Add /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WfbsPnpInstall.exe\" /T REG_SZ /V debugger /D ?");
                    system("Reg Add /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSMon.exe\" /T REG_SZ /V debugger /D ?");
                    system("Reg Add /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSMlogon.exe\" /T REG_SZ /V debugger /D ?");
                    system("Reg Add /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSSvrLogShow.exe\" /T REG_SZ /V debugger /D ?");
                    system("Reg Add /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\ResetIp.exe\" /T REG_SZ /V debugger /D ?");
                    system("Reg Add /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\FuncForWIN64.exe\" /T REG_SZ /V debugger /D ?");
                    system("Reg Add /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\CertMgr.exe\" /T REG_SZ /V debugger /D ?");
                    system("Reg Add /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\Fireware.exe\" /T REG_SZ /V debugger /D ?");
                    system("Reg Add /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\BCDBootCopy.exe\" /T REG_SZ /V debugger /D ?");
                    system("Reg Add /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\refreship.exe\" /T REG_SZ /V debugger /D ?");
                    system("Reg Add /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\LenovoLockScreen.exe\" /T REG_SZ /V debugger /D ?");
                    system("Reg Add /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\PortControl64.exe\" /T REG_SZ /V debugger /D ?");
                    system("Reg Add /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DesktopCheck.exe\" /T REG_SZ /V debugger /D ?");
                    system("Reg Add /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DeploymentManager.exe\" /T REG_SZ /V debugger /D ?");
                    system("Reg Add /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DeploymentAgent.exe\" /T REG_SZ /V debugger /D ?");
                    system("Reg Add /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\XYNTService.exe\" /T REG_SZ /V debugger /D ?");
                    system("Net Stop /Y BSAgentSvr");
                    system("Net Stop /Y tvnserver");
                    system("Net Stop /Y WFBSMlogon");
                }
                break;
            }
        }
        puts("\n------------------------------\n");
        printf("按任意键返回.");
        system("Pause > NUL");
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
        system("Pause > NUL");
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
        puts("\n------------------------------\n");
        switch(Dat[0]){
            case '1':{
                system("Reg Delete /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\StudentMain.exe\"");
                system("Reg Delete /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DispcapHelper.exe\"");
                system("Reg Delete /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\VRCwPlayer.exe\"");
                system("Reg Delete /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\InstHelpApp.exe\"");
                system("Reg Delete /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\InstHelpApp64.exe\"");
                system("Reg Delete /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\TDOvrSet.exe\"");
                system("Reg Delete /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\GATESRV.exe\"");
                system("Reg Delete /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\ProcHelper64.exe\"");
                system("Reg Delete /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\MasterHelper.exe\"");
                system("Net Start STUDSRV");
                system("Net Start TDNetFilter");
                system("Net Start TDFileFilter");
                break;
            }case '2':{
                system("Reg Delete /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\vncviewer.exe\"");
                system("Reg Delete /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\tvnserver32.exe\"");
                system("Reg Delete /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WfbsPnpInstall.exe\"");
                system("Reg Delete /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSMon.exe\"");
                system("Reg Delete /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSMlogon.exe\"");
                system("Reg Delete /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSSvrLogShow.exe\"");
                system("Reg Delete /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\ResetIp.exe\"");
                system("Reg Delete /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\FuncForWIN64.exe\"");
                system("Reg Delete /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\CertMgr.exe\"");
                system("Reg Delete /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\Fireware.exe\"");
                system("Reg Delete /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\BCDBootCopy.exe\"");
                system("Reg Delete /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\refreship.exe\"");
                system("Reg Delete /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\LenovoLockScreen.exe\"");
                system("Reg Delete /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\PortControl64.exe\"");
                system("Reg Delete /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DesktopCheck.exe\"");
                system("Reg Delete /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DeploymentManager.exe\"");
                system("Reg Delete /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DeploymentAgent.exe\"");
                system("Reg Delete /F \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\XYNTService.exe\"");
                system("Net Start BSAgentSvr");
                system("Net Start tvnserver");
                system("Net Start WFBSMlogon");
                break;
            }
        }
        puts("-------------------------------\n");
        printf("按任意键返回.");
        system("Pause > NUL");
    }
    system("CLS");
    goto BEGIN;
}
void Tools(){
    puts("[工具]\n");
    puts("即将实装.\n");
    puts("---------------\n");
    printf("按任意键返回.");
    system("Pause > NUL");
    return;
}
int main(int argc,char* argv[]){
    system("Title CRCSN");
    if(argc==1){
        goto BEGIN;
    }
    {
        std::string opt[2]{},tmp{};
        for(int i{1};i<argc;++i){
            tmp=argv[i];
            if((opt[0]=="")&&(tmp.substr(0,5)=="-clr=")&&(tmp.length()>5)){
                tmp.erase(0,5);
                opt[0]="Color "+tmp;
            }else if((opt[1]=="")&&(tmp=="-fs")){
                opt[1]="#";
            }
            else{
                puts("参数错误.\n");
                puts("---------------\n");
                printf("按任意键继续.");
                system("Pause > NUL");
                goto BEGIN;
            }
        }
        system(opt[0].c_str());
        if(opt[1]=="#"){
            std::thread(ForceShow).detach();
        }
    }
BEGIN:
    system("CLS");
    puts("[首页]\n");
    puts("  ? 关于");
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
            About();
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
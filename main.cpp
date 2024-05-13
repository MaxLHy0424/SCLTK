#include"extern.hpp"
void Info(){
    puts("| 关于 |\n");
    puts("   [软件名称] Computer Room Controlling Software Nemesis");
    puts("   [构建版本] v4.8.7");
    puts("   [项目仓库] https://github.com/MaxLHy0424/CRCSN\n");
    puts("   (C) 2023- MaxLHy0424, All Rights Reserved.\n");
    puts("========================================\n");
    puts("按任意键返回.\n");
    system("Pause");
    return;
}
void Cracker(){
BEGIN:
    puts("| 破解 |\n");
    puts("   [0] 返回");
    puts("   [1] 极域电子教室");
    puts("   [2] 联想云教室\n");
    printf("请键入: ");
    for(;;){
        scanf("%s",&IosDat[0]);
        if((IosDat[0]=='0'||IosDat[0]=='1'||IosDat[0]=='2')&&(IosDat[1]==0)){
            break;
        }
        printf("键入错误, 请重试: ");
    }
    if(IosDat[0]=='0'){
        return;
    }
    printf("已%s用高级破解, ",((IsUserAnAdmin())?("启"):("禁")));
    if(ConfirmOp()==true){
        system("Cls");
        puts("| 破解 > 执行操作 |\n");
        switch(IosDat[0]){
            case '1':{
                system("TaskKill /F /IM StudentMain.exe");
                system("TaskKill /F /IM DispcapHelper.exe");
                system("TaskKill /F /IM InstHelpApp.exe");
                system("TaskKill /F /IM GATESRV.exe");
                system("TaskKill /F /IM ProcHelper64.exe");
                system("TaskKill /F /IM MasterHelper.exe");
                if(IsUserAnAdmin()){
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\StudentMain.exe\" /F /T REG_SZ /V debugger /D \"BLOCK\"");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DispcapHelper.exe\" /F /T REG_SZ /V debugger /D \"BLOCK\"");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\VRCwPlayer.exe\" /F /T REG_SZ /V debugger /D \"BLOCK\"");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\InstHelpApp.exe\" /F /T REG_SZ /V debugger /D \"BLOCK\"");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\InstHelpApp64.exe\" /F /T REG_SZ /V debugger /D \"BLOCK\"");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\TDOvrSet.exe\" /F /T REG_SZ /V debugger /D \"BLOCK\"");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\GATESRV.exe\" /F /T REG_SZ /V debugger /D \"BLOCK\"");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\ProcHelper64.exe\" /F /T REG_SZ /V debugger /D \"BLOCK\"");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\MasterHelper.exe\" /F /T REG_SZ /V debugger /D \"BLOCK\"");
                    system("Net Stop TDNetFilter /Y");
                    system("Net Stop TDFileFilter /Y");
                    system("Net Stop STUDSRV /Y");
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
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\vncviewer.exe\" /F /T REG_SZ /V debugger /D \"BLOCK\"");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\tvnserver32.exe\" /F /T REG_SZ /V debugger /D \"BLOCK\"");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WfbsPnpInstall.exe\" /F /T REG_SZ /V debugger /D \"BLOCK\"");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSMon.exe\" /F /T REG_SZ /V debugger /D \"BLOCK\"");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSMlogon.exe\" /F /T REG_SZ /V debugger /D \"BLOCK\"");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSSvrLogShow.exe\" /F /T REG_SZ /V debugger /D \"BLOCK\"");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\ResetIp.exe\" /F /T REG_SZ /V debugger /D \"BLOCK\"");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\FuncForWIN64.exe\" /F /T REG_SZ /V debugger /D \"BLOCK\"");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\CertMgr.exe\" /F /T REG_SZ /V debugger /D \"BLOCK\"");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\Fireware.exe\" /F /T REG_SZ /V debugger /D \"BLOCK\"");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\BCDBootCopy.exe\" /F /T REG_SZ /V debugger /D \"BLOCK\"");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\refreship.exe\" /F /T REG_SZ /V debugger /D \"BLOCK\"");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\LenovoLockScreen.exe\" /F /T REG_SZ /V debugger /D \"BLOCK\"");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\PortControl64.exe\" /F /T REG_SZ /V debugger /D \"BLOCK\"");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DesktopCheck.exe\" /F /T REG_SZ /V debugger /D \"BLOCK\"");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DeploymentManager.exe\" /F /T REG_SZ /V debugger /D \"BLOCK\"");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DeploymentAgent.exe\" /F /T REG_SZ /V debugger /D \"BLOCK\"");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\XYNTService.exe\" /F /T REG_SZ /V debugger /D \"BLOCK\"");
                    system("Net Stop tvnserver /Y");
                    system("Net Stop BSAgentSvr /Y");
                    system("Net Stop WFBSMlogon /Y");
                }
                break;
            }
        }
        puts("========================================\n");
        puts("按任意键返回.\n");
        system("Pause");
    }
    system("Cls");
    goto BEGIN;
}
void Recoverer(){
BEGIN:
    puts("| 恢复 |\n");
    if(!IsUserAnAdmin()){
        puts("此功能在基本会话下不可用.\n");
        puts("========================================\n");
        puts("按任意键返回.\n");
        system("Pause");
        return;
    }
    puts("   [0] 返回");
    puts("   [1] 极域电子教室");
    puts("   [2] 联想云教室\n");
    printf("请键入: ");
    for(;;){
        scanf("%s",&IosDat[0]);
        if((IosDat[0]=='0'||IosDat[0]=='1'||IosDat[0]=='2')&&(IosDat[1]==0)){
            break;
        }
        printf("键入错误, 请重试: ");
    }
    if(IosDat[0]=='0'){
        return;
    }
    if(ConfirmOp()==true){
        system("Cls");
        puts("| 恢复 > 执行操作 |\n");
        switch(IosDat[0]){
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
                system("Net Start TDNetFilter");
                system("Net Start TDFileFilter");
                system("Net Start STUDSRV");
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
                system("Net Start tvnserver");
                system("Net Start BSAgentSvr");
                system("Net Start WFBSMlogon");
                break;
            }
        }
        puts("========================================\n");
        puts("按任意键返回.\n");
        system("Pause");
    }
    system("Cls");
    goto BEGIN;
}
void Tools(){
BEGIN:
    puts("| 工具箱 |\n");
    if(!IsUserAnAdmin()){
        puts("此功能在基本会话下不可用.\n");
        puts("========================================\n");
        puts("按任意键返回.\n");
        system("Pause");
        return;
    }
    puts("   [0] 返回");
    puts("   [/] 命令提示符");
    puts("   [1] 系统修复");
    puts("   [2] 磁盘清理");
    puts("   [3] 高级启动\n");
    printf("请键入: ");
    for(;;){
        scanf("%s",&IosDat[0]);
        if((IosDat[0]=='0'||IosDat[0]=='/'||IosDat[0]=='1'||IosDat[0]=='2'||IosDat[0]=='3')&&(IosDat[1]==0)){
            break;
        }
        printf("键入错误, 请重试: ");
    }
    system("Cls");
    switch(IosDat[0]){
        case '0':{
            return;
        }case '/':{
            puts("| 工具箱 > 命令提示符 |\n");
            puts("执行 \"Exit\" 退出.\n");
            system("Cmd");
            goto END;
        }case '1':{
            puts("| 工具箱 > 系统修复 |\n");
            if(ConfirmOp()==false){
                goto END;
            }
            if(KernalVersion>=62UL){
                system("DISM /Online /Cleanup-Image /RestoreHealth");
            }
            system("SFC /ScanNow");
            break;
        }case '2':{
            puts("| 工具箱 > 垃圾清理 |\n");
            if(ConfirmOp()==false){
                goto END;
            }
            if(KernalVersion>=100UL){
                system("DISM /Online /Set-ReservedStorageState /State:Disabled");
            }
            system("Del /F /S /Q %TEMP%");
            break;
        }case '3':{
            puts("| 工具箱 > 高级启动 |\n");
            if(KernalVersion<62UL){
                puts("仅支持 Windows 8+.");
                break;
            }
            if(ConfirmOp()==false){
                goto END;
            }
            puts("\n");
            system("Pause");
            system("Shutdown /R /O /T 0");
            break;
        }
    }
    puts("\n========================================\n");
    puts("按任意键返回.\n");
    system("Pause");
END:
    system("Cls");
    goto BEGIN;
}
int main(int argc,char* argv[]){
    if(IsUserAnAdmin()){
        system("Title [增强会话] CRCSN");
    }else{
        system("Title [基本会话] CRCSN");
    }
    if(argc==1){
        goto BEGIN;
    }
    {
        std::string opts[argc-1]{},tmp{};
        for(int i{1};i<argc;++i){
            tmp=argv[i];
            if((tmp.substr(0,5)=="-clr=")&&(opts[0]=="")){
                tmp.erase(0,5);
                opts[i-1]="Color "+tmp;
                continue;
            }else if((tmp=="-fs")&&(opts[1]=="")){
                opts[1]="1";
                continue;
            }
            puts("启动参数错误.\n");
            puts("========================================\n");
            puts("按任意键退出.\n");
            system("Pause");
            return 0;
        }
        system(opts[0].c_str());
        if(opts[1]=="1"){
            std::thread(ForceShow).detach();
        }
    }
BEGIN:
    puts("| 主页 |\n");
    puts("   [i] 关于");
    puts("   [1] 破解");
    puts("   [2] 恢复");
    puts("   [3] 工具箱\n");
    printf("请键入: ");
    for(;;){
        scanf("%s",&IosDat[0]);
        if((IosDat[0]=='i'||IosDat[0]=='I'||IosDat[0]=='1'||IosDat[0]=='2'||IosDat[0]=='3')&&(IosDat[1]==0)){
            if(IosDat[0]=='I'){
                IosDat[0]=='i';
            }
            break;
        }
        printf("键入错误, 请重试: ");
    }
    system("Cls");
    switch(IosDat[0]){
        case 'i':{
            Info();
            break;
        }case '1':{
            Cracker();
            break;
        }case '2':{
            Recoverer();
            break;
        }case '3':{
            Tools();
            break;
        }
    }
    system("Cls");
    goto BEGIN;
}
#include"extern.hpp"
void About(){
    puts("| Home / 关于 |\n");
    puts("   [软件名称] Computer Room Control Software Nemesis");
    puts("   [软件版本] v4.7.0");
    puts("   [项目作者] MaxLHy0424");
    puts("   [项目仓库] https://github.com/MaxLHy0424/CRCSN\n");
    printf("   [系统内核] NT %.1lf\n\n",KernalVersion()/10.0);
    puts("   (C) 2024 MaxLHy0424, 保留所有权利.\n");
    puts("========================================\n");
    puts("按任意键返回.\n");
    system("Pause");
}
void Configurator(bool rld){
    if(rld){
        puts("| Home / 重载配置 |\n");
    }
    std::ifstream ifs;
    ifs.open("cfg.ini",std::ios::in);
    if(!ifs.is_open()){
        puts("无法读取 cfg.ini.\n");
        puts("========================================\n");
        puts("按任意键继续.\n");
        system("Pause");
        system("Cls");
        ifs.close();
        return;
    }
    for(unsigned short i{};i<3U;++i){
        getline(ifs,CfgDt[i]);
    }
    ifs.close();
    CfgDt[0]="Color "+CfgDt[0];
    system(CfgDt[0].c_str());
    static bool FSRun{false};
    if((CfgDt[2]=="1")&&(FSRun==false)){
        FSRun=true;
        std::thread(ForceShow).detach();
    }else if(CfgDt[2]!="1"){
        FSRun=false;
    }
    if(rld){
        puts("重载完毕.\n");
        puts("========================================\n");
        puts("按任意键返回.\n");
        system("Pause");
    }
    return;
}
void Cracker(){
Begin:
    puts("| Home / 破解 |\n");
    puts("   [0] 返回");
    puts("   [1] 极域电子教室");
    puts("   [2] 联想云教室\n");
    printf("请输入: ");
    for(;;){
        scanf("%s",&Code[0]);
        if((Code[0]=='0'||Code[0]=='1'||Code[0]=='2')&&(Code[1]==0)){
            break;
        }
        printf("输入错误, 请重新输入: ");
    }
    if(Code[0]=='0'){
        return;
    }
    printf("已%s用 [高级破解], ",((IsUserAnAdmin())?("启"):("禁")));
    if(Confirm()==true){
        system("Cls");
        puts("| Home / 破解 / 执行操作 |\n");
        switch(Code[0]){
            case '1':{
                system("TaskKill /F /T /IM StudentMain.exe");
                system("TaskKill /F /T /IM DispcapHelper.exe");
                system("TaskKill /F /T /IM InstHelpApp.exe");
                system("TaskKill /F /T /IM GATESRV.exe");
                system("TaskKill /F /T /IM ProcHelper64.exe");
                system("TaskKill /F /T /IM MasterHelper.exe");
                if(IsUserAnAdmin()){
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\StudentMain.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DispcapHelper.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\VRCwPlayer.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\InstHelpApp.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\InstHelpApp64.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\TDOvrSet.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\GATESRV.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\ProcHelper64.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\MasterHelper.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                    system("Net Stop TDNetFilter /Y");
                    system("Net Stop TDFileFilter /Y");
                    system("Net Stop STUDSRV /Y");
                }
                break;
            }case '2':{
                system("TaskKill /F /T /IM vncviewer.exe");
                system("TaskKill /F /T /IM tvnserver32.exe");
                system("TaskKill /F /T /IM WfbsPnpInstall.exe");
                system("TaskKill /F /T /IM WFBSMon.exe");
                system("TaskKill /F /T /IM WFBSMlogon.exe");
                system("TaskKill /F /T /IM WFBSSvrLogShow.exe");
                system("TaskKill /F /T /IM ResetIp.exe");
                system("TaskKill /F /T /IM Fireware.exe");
                system("TaskKill /F /T /IM BCDBootCopy.exe");
                system("TaskKill /F /T /IM refreship.exe");
                system("TaskKill /F /T /IM LenovoLockScreen.exe");
                system("TaskKill /F /T /IM PortControl64.exe");
                system("TaskKill /F /T /IM DesktopCheck.exe");
                system("TaskKill /F /T /IM DeploymentManager.exe");
                system("TaskKill /F /T /IM DeploymentAgent.exe");
                system("TaskKill /F /T /IM XYNTService.exe");
                if(IsUserAnAdmin()){
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\vncviewer.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\tvnserver32.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WfbsPnpInstall.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSMon.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSMlogon.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSSvrLogShow.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\ResetIp.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\FuncForWIN64.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\CertMgr.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\Fireware.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\BCDBootCopy.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\refreship.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\LenovoLockScreen.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\PortControl64.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DesktopCheck.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DeploymentManager.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DeploymentAgent.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                    system("Reg Add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\XYNTService.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
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
    return;
}
void Recoverer(){
    puts("| Home / 恢复 |\n");
    if(!IsUserAnAdmin()){
        puts("基本会话下不可用.\n");
        puts("========================================\n");
        puts("按任意键返回.\n");
        system("Pause");
        return;
    }
    puts("   [0] 返回");
    puts("   [1] 极域电子教室");
    puts("   [2] 联想云教室\n");
    printf("请输入: ");
    for(;;){
        scanf("%s",&Code[0]);
        if((Code[0]=='0'||Code[0]=='1'||Code[0]=='2')&&(Code[1]==0)){
            break;
        }
        printf("输入错误, 请重新输入: ");
    }
    if(Code[0]=='0'){
        return;
    }
    if(Confirm()==true){
        system("Cls");
        puts("| Home / 恢复 / 执行操作 |\n");
        switch(Code[0]){
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
                system("Net Start tvnserver");
                system("Net Start BSAgentSvr");
                system("Net Start WFBSMlogon");
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
                system("Net Start TDNetFilter");
                system("Net Start TDFileFilter");
                system("Net Start STUDSRV");
                break;
            }
        }
        puts("========================================\n");
        puts("按任意键返回.\n");
        system("Pause");
    }
    return;
}
void Toolkit(){
Begin:
    puts("| Home / 工具箱 |\n");
    if(!IsUserAnAdmin()){
        puts("基本会话下不可用.\n");
        puts("========================================\n");
        puts("按任意键返回.\n");
        system("Pause");
        return;
    }
    puts("   [0] 返回");
    puts("   [$] 命令提示符");
    puts("   [1] 系统修复");
    puts("   [2] 垃圾清理");
    puts("   [3] 高级启动\n");
    printf("请输入: ");
    for(;;){
        scanf("%s",&Code[0]);
        if((Code[0]=='0'||Code[0]=='$'||Code[0]=='1'||Code[0]=='2'||Code[0]=='3')&&(Code[1]==0)){
            break;
        }
        printf("输入错误, 请重新输入: ");
    }
    system("Cls");
    switch(Code[0]){
        case '0':{
            return;
        }case '$':{
            puts("| Home / 工具箱 / 命令提示符 |\n");
            puts("\"Exit\" 退出.\n");
            system("Cmd");
            break;
        }case '1':{
            puts("| Home / 工具箱 / 系统修复 |\n");
            printf("请输入次数 (0 返回): ");
            unsigned short n{};
            scanf("%hu",&n);
            if(n==0U){
                system("Cls");
                goto Begin;
            }
            for(;n>0U;--n){
                if(KernalVersion()>=62UL){
                    system("DISM /Online /Cleanup-Image /RestoreHealth");
                }
                system("SFC /ScanNow");
            }
            break;
        }case '2':{
            puts("| Home / 工具箱 / 垃圾清理 |\n");
            if(Confirm()==true){
                puts("\n");
                system("Del /F /S /Q %TEMP%\\*");
                if(KernalVersion()>=100UL){
                    system("DISM /Online /Set-ReservedStorageState /State:Disabled");
                }
            }else{
                goto Begin;
            }
            break;
        }case '3':{
            puts("| Home / 工具箱 / 高级启动 |\n");
            if(KernalVersion()<62UL){
                puts("仅支持 Windows 8 及以上的 Windows 操作系统.");
                break;
            }
            if(Confirm()==false){
                break;
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
    system("Cls");
    goto Begin;
}
int main(){
    if(IsUserAnAdmin()){
        system("Title [增强会话] CRCSN");
    }else{
        system("Title [基本会话] CRCSN");
    }
    system("Color 9");
    Configurator(false);
Begin:
    puts("| Home |\n");
    if(CfgDt[1]!="NUL"){
        printf("%s\n\n",CfgDt[1].c_str());
    }
    puts("   [?] 关于");
    puts("   [0] 重载配置");
    puts("   [1] 破解");
    puts("   [2] 恢复");
    puts("   [3] 工具箱\n");
    printf("请输入: ");
    for(;;){
        scanf("%s",&Code[0]);
        if((Code[0]=='?'||Code[0]=='0'||Code[0]=='1'||Code[0]=='2'||Code[0]=='3')&&(Code[1]==0)){
            break;
        }
        printf("输入错误, 请重新输入: ");
    }
    system("Cls");
    switch(Code[0]){
        case '?':{
            About();
            break;
        }case '0':{
            Configurator(true);
            break;
        }case '1':{
            Cracker();
            break;
        }case '2':{
            Recoverer();
            break;
        }case '3':{
            Toolkit();
            break;
        }
    }
    system("Cls");
    goto Begin;
}
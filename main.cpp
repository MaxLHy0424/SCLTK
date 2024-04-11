#include"extern.hpp"
void Start();
void About(){
    puts("| 主菜单 > 关于 |\n");
    puts("   [软件名称] Computer Room Control Software Nemesis");
    puts("   [软件版本] v4.5.1");
    puts("   [项目作者] MaxLHy0424");
    puts("   [项目仓库] https://github.com/MaxLHy0424/CRCSN\n");
    puts("   (C) 2024 MaxLHy0424, 保留所有权利.\n");
    puts("========================================\n");
    puts("按任意键返回.\n");
    system("Pause");
}
void Configurator(BOOL rld){
    if(rld){
        puts("| 主菜单 > 配置工具 |\n");
    }
    std::ifstream fs;
    fs.open("dat.cfg",std::ios::in);
    if(!fs.is_open()){
        puts("无法读取 dat.cfg.\n");
        puts("========================================\n");
        puts("按任意键继续.\n");
        system("Pause");
        system("Cls");
        goto SKP;
    }
    for(unsigned short i{};i<3U;++i){
        getline(fs,CfgDt[i]);
    }
    CfgDt[0]="Color "+CfgDt[0];
    system(CfgDt[0].c_str());
    if(rld){
        puts("加载完毕.\n");
        puts("========================================\n");
        puts("按任意键返回主菜单.\n");
        system("Pause");
    }
SKP:
    fs.close();
}
void Cracker(){
BEGIN:
    puts("| 主菜单 > 破解 |\n");
    puts("   [0] 返回");
    puts("   [1] 单次");
    puts("   [2] 循环\n");
    printf("请输入: ");
    for(;;){
        scanf("%s",&Code[0]);
        if((Code[0]=='0'||Code[0]=='1'||Code[0]=='2')&&Code[1]==0){
            break;
        }else{
            printf("输入错误, 请重新输入: ");
        }
    }
    unsigned short pauseTime{};
    switch(Code[0]){
        case '0':{
            return;
        }case '2':{
            printf("请输入暂停时间 (ms, 0~10000): ");
            for(;;){
                scanf("%hu",&pauseTime);
                if(pauseTime<=10000U){
                    break;
                }else{
                    printf("输入错误, 请重新输入: ");
                }
            }
            break;
        }
    }
    system("Cls");
    puts("| 主菜单 > 破解 > 确认配置 |\n");
    printf("     [行为] ");
    switch(Code[0]){
        case '1':{
            puts("单次.");
            break;
        }case '2':{
            puts("循环.");
            break;
        }
    }
    printf(" [高级破解] ");
    if(IsUserAnAdmin()){
        puts("启用.");
    }else{
        puts("禁用.");
    }
    printf("     [暂停] %hums.\n\n",pauseTime);
    if(Confirm()==FALSE){
        system("Cls");
        goto BEGIN;
    }
    for(;;){
        system("Cls");
        puts("| 主菜单 > 破解 > 确认配置 > 执行操作 |\n");
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
        system("TaskKill /F /T /IM ProcHelper64.exe");
        system("TaskKill /F /T /IM StudentMain.exe");
        system("TaskKill /F /T /IM DispcapHelper.exe");
        system("TaskKill /F /T /IM InstHelpApp.exe");
        system("TaskKill /F /T /IM GATESRV.exe");
        system("TaskKill /F /T /IM MasterHelper.exe");
        if(IsUserAnAdmin()){
            if(CfgDt[2]=="true"){
                system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\vncviewer.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\tvnserver32.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WfbsPnpInstall.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSMon.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSMlogon.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSSvrLogShow.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\ResetIp.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\FuncForWIN64.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\CertMgr.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\Fireware.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\BCDBootCopy.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\refreship.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\LenovoLockScreen.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\PortControl64.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DesktopCheck.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DeploymentManager.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DeploymentAgent.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\XYNTService.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\ProcHelper64.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\StudentMain.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DispcapHelper.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\VRCwPlayer.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\InstHelpApp.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\InstHelpApp64.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\TDOvrSet.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\GATESRV.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
                system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\MasterHelper.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
            }
            system("SC Config tvnserver Start= Disabled");
            system("SC Config BSAgentSvr Start= Disabled");
            system("SC Config WFBSMlogon Start= Disabled");
            system("SC Config TDNetFilter Start= Disabled");
            system("SC Config TDFileFilter Start= Disabled");
            system("SC Config STUDSRV Start= Disabled");
            system("Net Stop tvnserver /Y");
            system("Net Stop BSAgentSvr /Y");
            system("Net Stop WFBSMlogon /Y");
            system("Net Stop TDNetFilter /Y");
            system("Net Stop TDFileFilter /Y");
            system("Net Stop STUDSRV /Y");
        }
        if(Code[0]=='1'){
            break;
        }
        puts("\n暂停中...");
        Sleep(pauseTime);
    }
    puts("========================================\n");
    puts("按任意键返回主菜单.\n");
    system("Pause");
}
void Recoverer(){
    puts("| 主菜单 > 恢复 |\n");
    if(!IsUserAnAdmin()){
        puts("基本会话下此功能不可用.\n");
        puts("========================================\n");
        puts("按任意键返回主菜单.\n");
        system("Pause");
        return;
    }
    if(Confirm()==FALSE){
        return;
    }
    system("Cls");
    puts("| 主菜单 > 恢复 > 执行操作 |\n");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\vncviewer.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\tvnserver32.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WfbsPnpInstall.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSMon.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSMlogon.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSSvrLogShow.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\ResetIp.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\FuncForWIN64.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\CertMgr.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\Fireware.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\BCDBootCopy.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\refreship.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\LenovoLockScreen.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\PortControl64.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DesktopCheck.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DeploymentManager.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DeploymentAgent.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\XYNTService.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\ProcHelper64.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\StudentMain.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DispcapHelper.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\VRCwPlayer.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\InstHelpApp.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\InstHelpApp64.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\TDOvrSet.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\GATESRV.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\MasterHelper.exe\" /F");
    system("SC Config tvnserver Start= Auto");
    system("SC Config BSAgentSvr Start= Auto");
    system("SC Config WFBSMlogon Start= Auto");
    system("SC Config TDNetFilter Start= Auto");
    system("SC Config TDFileFilter Start= Auto");
    system("SC Config STUDSRV Start= Auto");
    system("Net Start tvnserver");
    system("Net Start BSAgentSvr");
    system("Net Start WFBSMlogon");
    system("Net Start TDNetFilter");
    system("Net Start TDFileFilter");
    system("Net Start STUDSRV");
    puts("========================================\n");
    puts("按任意键返回主菜单.\n");
    system("Pause");
}
void Toolkit(){
BEGIN:
    puts("| 主菜单 > 工具箱 |\n");
    if(!IsUserAnAdmin()){
        puts("基本会话下此功能不可用.\n");
        puts("========================================\n");
        puts("按任意键返回主菜单.\n");
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
        if((Code[0]=='0'||Code[0]=='$'||Code[0]=='1'||Code[0]=='2'||Code[0]=='3')&&Code[1]==0){
            break;
        }else{
            printf("输入错误, 请重新输入: ");
        }
    }
    system("Cls");
    switch(Code[0]){
        case '0':{
            return;
        }case '$':{
            puts("| 主菜单 > 工具箱 > 命令提示符 |\n");
            puts("输入 \"Exit\" 退出命令提示符.\n");
            system("Cmd");
            break;
        }case '1':{
            puts("| 主菜单 > 工具箱 > 系统修复 |\n");
            printf("请输入执行次数 (0 返回): ");
            unsigned short n{};
            scanf("%hu",&n);
            if(n==0U){
                system("Cls");
                goto BEGIN;
            }
            for(;n>0U;--n){
                if(KernalVersion()>=62UL){
                    system("DISM /Online /Cleanup-Image /RestoreHealth");
                }
                system("SFC /ScanNow");
            }
            break;
        }case '2':{
            puts("| 主菜单 > 工具箱 > 垃圾清理 |\n");
            if(Confirm()==TRUE){
                puts("\n");
                system("Del /F /S /Q %TEMP%\\*");
                if(KernalVersion()>=100UL){
                    system("DISM /Online /Set-ReservedStorageState /State:Disabled");
                }
            }else{
                goto BEGIN;
            }
            break;
        }case '3':{
            puts("| 主菜单 > 工具箱 > 高级启动 |\n");
            if(KernalVersion()<62UL){
                puts("需要 Windows 8 及以上的 Windows 操作系统.\n");
                break;
            }
            for(unsigned short t{5U};;--t){
                printf("请保存文件, %hus 后可重启.",t);
                if(t==0U){
                    break;
                }
                Sleep(1000U);
                printf("\r");
            }
            puts("\n");
            system("Pause");
            puts("\n");
            system("ReAgentC /Enable");
            system("Shutdown /R /O /T 0");
            break;
        }
    }
    puts("\n========================================\n");
    puts("按任意键返回.\n");
    system("Pause");
    system("Cls");
    goto BEGIN;
}
void Start(){
BEGIN:
    puts("| 主菜单 |\n");
    if(CfgDt[1]!="NULL"){
        printf("%s\n\n",CfgDt[1].c_str());
    }
    puts("   [?] 关于");
    puts("   [0] 配置工具");
    puts("   [1] 破解");
    puts("   [2] 恢复");
    puts("   [3] 工具箱\n");
    printf("请输入: ");
    for(;;){
        scanf("%s",&Code[0]);
        if((Code[0]=='?'||Code[0]=='0'||Code[0]=='1'||Code[0]=='2'||Code[0]=='3')&&Code[1]==0){
            break;
        }else{
            printf("输入错误, 请重新输入: ");
        }
    }
    system("Cls");
    switch(Code[0]){
        case '?':{
            About();
            break;
        }case '0':{
            Configurator(TRUE);
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
    goto BEGIN;
}
int main(){
    if(IsUserAnAdmin()){
        system("Title [增强会话] CRCSN");
    }else{
        system("Title [基本会话] CRCSN");
    }
    system("Color 9");
    Configurator(FALSE);
    Start();
}
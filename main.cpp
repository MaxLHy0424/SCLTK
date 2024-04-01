#include"extern.hpp"
void Start();
void About(){
    puts("| 主菜单 > 关于 |\n");
    puts("   [软件名称] 机房控制软件克星 (Computer Room Control Software Nemesis)");
    puts("   [版本信息] v4.2.2");
    puts("   [项目作者] MaxLHy0424");
    puts("   [项目仓库] https://github.com/MaxLHy0424/CRCSN\n");
    puts("   (C) 2024 MaxLHy0424, 保留所有权利.\n");
    puts("========================================\n");
    puts("按任意键返回.\n");
    system("Pause");
}
void Configurator(bool rld){
    if(rld){
        puts("| 主菜单 > 重载配置 |\n");
    }
    std::ifstream fop;
    fop.open("config.ini",std::ios::in);
    if(!fop.is_open()){
        if(!rld){
            system("Color 9");
            if(IsUserAnAdmin()){
                system("Title [增强会话] CRCSN");
            }else{
                system("Title [基本会话] CRCSN");
            }
        }
        puts("无法读取 config.ini.\n");
        puts("========================================\n");
        puts("按任意键继续.\n");
        system("Pause");
        system("CLS");
        goto SKIP;
    }
    for(unsigned short i{};i<4U;++i){
        getline(fop,CONFIG[i]);
    }
    CONFIG[0]="Color "+CONFIG[0];
    system(CONFIG[0].c_str());
    if(CONFIG[1]=="1"){
        if(IsUserAnAdmin()){
            system("Title [增强会话] CRCSN");
        }else{
            system("Title [基本会话] CRCSN");
        }
    }else{
        system("Title CRCSN");
    }
    if(rld){
        puts("重载完毕.\n");
        puts("========================================\n");
        puts("按任意键返回主菜单.\n");
        system("Pause");
    }
SKIP:
    fop.close();
}
void Cracker(){
BEGIN:
    puts("| 主菜单 > 破解 |\n");
    puts("   [0] 返回");
    puts("   [1] 单次");
    puts("   [2] 循环\n");
    printf("请输入: ");
    for(;;){
        scanf("%s",&CODE[0]);
        if((CODE[0]=='0'||CODE[0]=='1'||CODE[0]=='2')&&CODE[1]==0){
            break;
        }else{
            printf("输入错误, 请重试: ");
        }
    }
    unsigned short pauseTime{};
    switch(CODE[0]){
        case '0':{
            return;
        }case '2':{
            printf("请输入暂停时间 (ms, 0~7500): ");
            for(;;){
                scanf("%hu",&pauseTime);
                if(pauseTime<=7500U){
                    break;
                }else{
                    printf("输入错误, 请重试: ");
                }
            }
            break;
        }
    }
    system("CLS");
    puts("| 主菜单 > 破解 > 确认配置 |\n");
    printf("     [行为] ");
    switch(CODE[0]){
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
    if(PauseOp()==false){
        system("CLS");
        goto BEGIN;
    }
    for(;;){
        system("CLS");
        puts("| 主菜单 > 破解 > 确认配置 > 执行操作 |\n");
        system("TaskKill /F /T /IM vncviewer.exe");
        system("TaskKill /F /T /IM tvnserver32.exe");
        system("TaskKill /F /T /IM WfbsPnpInstall.exe");
        system("TaskKill /F /T /IM WFBSMon.exe");
        system("TaskKill /F /T /IM WFBSMlogon.exe");
        system("TaskKill /F /T /IM WFBSSvrLogShow.exe");
        system("TaskKill /F /T /IM ResetIp.exe");
        system("TaskKill /F /T /IM FuncForWIN64.exe");
        system("TaskKill /F /T /IM CertMgr.exe");
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
        system("TaskKill /F /T /IM VRCwPlayer.exe");
        system("TaskKill /F /T /IM InstHelpApp.exe");
        system("TaskKill /F /T /IM TDOvrSet.exe");
        system("TaskKill /F /T /IM GATESRV.exe");
        system("TaskKill /F /T /IM MasterHelper.exe");
        if(IsUserAnAdmin()){
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
            system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\TDOvrSet.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
            system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\GATESRV.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
            system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\MasterHelper.exe\" /T REG_SZ /V debugger /D \"BLOCK\" /F");
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
        if(CODE[0]=='1'){
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
        puts("此功能在基本会话下不可用.\n");
        puts("========================================\n");
        puts("按任意键返回主菜单.\n");
        system("Pause");
        return;
    }
    if(PauseOp()==false){
        return;
    }
    system("CLS");
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
        puts("此功能在基本会话下不可用.\n");
        puts("========================================\n");
        puts("按任意键返回主菜单.\n");
        system("Pause");
        return;
    }
    puts("   [0] 返回");
    puts("   [/] 自定义命令");
    puts("   [1] 系统修复");
    puts("   [2] 垃圾清理");
    puts("   [3] 高级启动\n");
    printf("请输入: ");
    for(;;){
        scanf("%s",&CODE[0]);
        if((CODE[0]=='0'||CODE[0]=='/'||CODE[0]=='1'||CODE[0]=='2'||CODE[0]=='3')&&CODE[1]==0){
            break;
        }else{
            printf("输入错误, 请重试: ");
        }
    }
    system("CLS");
    switch(CODE[0]){
        case '0':{
            return;
        }case '/':{
            puts("| 主菜单 > 工具箱 > 自定义命令 |\n");
            std::ifstream fop;
            fop.open("config.ini",std::ios::in);
            if(!fop.is_open()){
                fop.close();
                puts("无法读取 config.ini.");
            }else if(CONFIG[3]=="UNDEFINED"){
                fop.close();
                puts("未配置此项, 请编辑配置.");
            }else{
                fop.close();
                system(CONFIG[3].c_str());
            }
            break;
        }case '1':{
            puts("| 主菜单 > 工具箱 > 系统修复 |\n");
            printf("请输入执行次数 (0 返回): ");
            unsigned short n{};
            scanf("%hu",&n);
            if(n==0U){
                system("CLS");
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
            if(PauseOp()==true){
                puts("\n");
                system("Del /F /S /Q %TEMP%\\*");
                system("CleanMgr");
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
                puts("此功能需要 Windows 8 及以上的 Windows 操作系统.\n");
                break;
            }
            for(unsigned short t{5U};;--t){
                printf("请保存文件, %hus 后可重启.",t);
                if(t==0U){
                    break;
                }
                Sleep(1000UL);
                printf("\r");
            }
            puts("\n");
            system("Pause");
            puts("\n");
            system("ReAgentC /Enable");
            system("Shutdown /F /R /O /T 0");
            break;
        }
    }
    puts("\n========================================\n");
    puts("按任意键返回.\n");
    system("Pause");
    system("CLS");
    goto BEGIN;
}
void Start(){
BEGIN:
    puts("| 主菜单 |\n");
    if(CONFIG[2]!="UNDEFINED"){
        printf("%s\n\n",CONFIG[2].c_str());
    }
    puts("   [?] 关于");
    puts("   [0] 重载配置");
    puts("   [1] 破解");
    puts("   [2] 恢复");
    puts("   [3] 工具箱\n");
    printf("请输入: ");
    for(;;){
        scanf("%s",&CODE[0]);
        if((CODE[0]=='?'||CODE[0]=='0'||CODE[0]=='1'||CODE[0]=='2'||CODE[0]=='3')&&CODE[1]==0){
            break;
        }else{
            printf("输入错误, 请重试: ");
        }
    }
    system("CLS");
    switch(CODE[0]){
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
    system("CLS");
    goto BEGIN;
}
int main(){
    Configurator(false);
    Start();
}
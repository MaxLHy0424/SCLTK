#include"extern.hpp"
void Start();
void Configuration(bool reload){
    if(reload){
        puts("| 主菜单 > 重载配置 |\n");
    }
    std::ifstream fOp;
    fOp.open("cfg.ini",std::ios::in);
    if(!fOp.is_open()){
        if(!reload){
            system("Color 9");
            Cfg[2]="0";
            if(!IsUserAnAdmin()){
                system("Title [基本会话] CRCSN");
            }else{
                system("Title [增强会话] CRCSN");
            }
        }
        puts("[x] 无法读取 cfg.ini.\n");
        puts("########################################\n");
        puts("按任意键继续.\n");
        system("Pause");
        ClearScreen();
        goto SKIP;
    }
    for(unsigned short counter{};counter<4U;++counter){
        getline(fOp,Cfg[counter]);
    }
    Cfg[0]="Color "+Cfg[0];
    system(Cfg[0].c_str());
    if(IsUserAnAdmin()){
        Cfg[1]="Title [增强会话] "+Cfg[1];
    }else{
        Cfg[1]="Title [基本会话] "+Cfg[1];
    }
    system(Cfg[1].c_str());
    if(reload){
        puts("重载配置完成.\n");
        puts("########################################\n");
        puts("按任意键返回主菜单.\n");
        system("Pause");
    }
SKIP:
    fOp.close();
}
void About(){
    puts("| 主菜单 > 关于 |\n");
    puts("   [软件名称] 机房控制软件克星 (Computer Room Control Software Nemesis)");
    puts("   [版本信息] v4.0.0_rc7");
    puts("   [软件作者] MaxLHy0424");
    puts("   [开源仓库] https://github.com/MaxLHy0424/Computer-Room-Control-Software-Nemesis\n");
    puts("   内部版本, 仅供测试和评估, 禁止泄露.");
    puts("   (C) 2024 MaxLHy0424, 保留所有权利.\n");
    puts("########################################\n");
    puts("按任意键返回.\n");
    system("Pause");
}
void Cracking(){
BEGIN:
    puts("| 主菜单 > 破解 |\n");
    puts("   [0] 返回");
    puts("   [1] 单次");
    puts("   [2] 循环\n");
    printf("请输入: ");
    scanf("%s",&Code[0]);
    while((Code[0]!='0'&&Code[0]!='1'&&Code[0]!='2')||Code[1]!=0){
        printf("输入错误, 请重新输入: ");
        scanf("%s",&Code[0]);
    }
    unsigned short sleepTime{};
    switch(Code[0]){
        case '0':{
            return;
        }case '2':{
            printf("请输入休眠时间 (毫秒, 0~5000): ");
            scanf("%hu",&sleepTime);
            while(sleepTime>5000U){
                printf("输入错误, 请重新输入: ");
                scanf("%hu",&sleepTime);
            }
            break;
        }
    }
    ClearScreen();
    puts("| 主菜单 > 破解 > 确认配置 |\n");
    printf("     [模式] ");
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
    printf("     [休眠] ");
    if(!sleepTime){
        puts("禁用.\n");
    }else{
        printf("启用, %hu 毫秒.\n",sleepTime);
    }
    printf("是否继续? (Y/N): ");
    scanf("%s",&Code[1]);
    while((Code[1]!='Y'&&Code[1]!='N')||Code[2]!=0){
        printf("输入错误, 请重新输入: ");
        scanf("%s",&Code[1]);
    }
    if(Code[1]=='N'){
        ClearScreen();
        goto BEGIN;
    }
    for(;;){
        ClearScreen();
        puts("| 主菜单 > 破解 > 确认配置 > 实施操作 |\n");
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
            system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\vncviewer.exe\" /T REG_SZ /V debugger /D \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
            system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\tvnserver32.exe\" /T REG_SZ /V debugger /D \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
            system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WfbsPnpInstall.exe\" /T REG_SZ /V debugger /D \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
            system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSMon.exe\" /T REG_SZ /V debugger /D \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
            system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSMlogon.exe\" /T REG_SZ /V debugger /D \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
            system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSSvrLogShow.exe\" /T REG_SZ /V debugger /D \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
            system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\ResetIp.exe\" /T REG_SZ /V debugger /D \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
            system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\FuncForWIN64.exe\" /T REG_SZ /V debugger /D \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
            system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\CertMgr.exe\" /T REG_SZ /V debugger /D \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
            system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\Fireware.exe\" /T REG_SZ /V debugger /D \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
            system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\BCDBootCopy.exe\" /T REG_SZ /V debugger /D \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
            system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\refreship.exe\" /T REG_SZ /V debugger /D \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
            system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\LenovoLockScreen.exe\" /T REG_SZ /V debugger /D \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
            system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\PortControl64.exe\" /T REG_SZ /V debugger /D \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
            system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DesktopCheck.exe\" /T REG_SZ /V debugger /D \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
            system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DeploymentManager.exe\" /T REG_SZ /V debugger /D \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
            system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DeploymentAgent.exe\" /T REG_SZ /V debugger /D \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
            system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\XYNTService.exe\" /T REG_SZ /V debugger /D \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
            system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\ProcHelper64.exe\" /T REG_SZ /V debugger /D \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
            system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\StudentMain.exe\" /T REG_SZ /V debugger /D \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
            system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DispcapHelper.exe\" /T REG_SZ /V debugger /D \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
            system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\VRCwPlayer.exe\" /T REG_SZ /V debugger /D \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
            system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\InstHelpApp.exe\" /T REG_SZ /V debugger /D \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
            system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\TDOvrSet.exe\" /T REG_SZ /V debugger /D \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
            system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\GATESRV.exe\" /T REG_SZ /V debugger /D \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
            system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\MasterHelper.exe\" /T REG_SZ /V debugger /D \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
            system("SC Cfg tvnserver Start= Disabled");
            system("SC Cfg BSAgentSvr Start= Disabled");
            system("SC Cfg WFBSMlogon Start= Disabled");
            system("SC Cfg TDNetFilter Start= Disabled");
            system("SC Cfg TDFileFilter Start= Disabled");
            system("SC Cfg STUDSRV Start= Disabled");
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
        puts("\n休眠中...");
        Sleep(sleepTime);
    }
    puts("########################################\n");
    puts("按任意键返回主菜单.\n");
    system("Pause");
}
void Recovering(){
    puts("| 主菜单 > 恢复 |\n");
    if(!IsUserAnAdmin()){
        puts("当前为基本会话, 此功能不可用.\n");
        puts("########################################\n");
        puts("按任意键返回主菜单.\n");
        system("Pause");
        return;
    }
    puts("用于恢复破解时的部分操作, 部分情况下可能无效.\n");
    printf("是否继续? (Y/N): ");
    scanf("%s",&Code[1]);
    while((Code[1]!='Y'&&Code[1]!='N')||Code[2]!=0){
        printf("输入错误, 请重新输入: ");
        scanf("%s",&Code[1]);
    }
    if(Code[1]=='N'){
        return;
    }
    ClearScreen();
    puts("| 主菜单 > 恢复 > 实施操作 |\n");
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
    system("SC Cfg tvnserver Start= Auto");
    system("SC Cfg BSAgentSvr Start= Auto");
    system("SC Cfg WFBSMlogon Start= Auto");
    system("SC Cfg TDNetFilter Start= Auto");
    system("SC Cfg TDFileFilter Start= Auto");
    system("SC Cfg STUDSRV Start= Auto");
    system("Net Start tvnserver");
    system("Net Start BSAgentSvr");
    system("Net Start WFBSMlogon");
    system("Net Start TDNetFilter");
    system("Net Start TDFileFilter");
    system("Net Start STUDSRV");
    puts("########################################\n");
    puts("按任意键返回主菜单.\n");
    system("Pause");
}
void Toolkit(){
BEGIN:
    puts("| 主菜单 > 工具箱 |\n");
    if(!IsUserAnAdmin()){
        puts("当前为基本会话, 此功能不可用.\n");
        puts("########################################\n");
        puts("按任意键返回主菜单.\n");
        system("Pause");
        return;
    }
    puts("   [0] 返回");
    puts("   [$] 自定义命令");
    puts("   [1] 系统修复");
    puts("   [2] 激活工具");
    puts("   [3] 高级启动\n");
    printf("请输入: ");
    scanf("%s",&Code[0]);
    while((Code[0]!='0'&&Code[0]!='$'&&Code[0]!='1'&&Code[0]!='2'&&Code[0]!='3')||Code[1]!=0){
        printf("输入错误, 请重新输入: ");
        scanf("%s",&Code[0]);
    }
    ClearScreen();
    switch(Code[0]){
        case '0':{
            return;
        }case '$':{
            puts("| 主菜单 > 工具箱 > 自定义命令 |\n");
            std::ifstream fOp;
            fOp.open("cfg.ini",std::ios::in);
            if(!fOp.is_open()){
                fOp.close();
                puts("[x] 无法读取 cfg.ini.");
            }else if(Cfg[3]=="$UNDEFINED$"){
                fOp.close();
                puts("未配置此项, 请编辑并重载配置.");
            }else{
                fOp.close();
                system(Cfg[3].c_str());
            }
            break;
        }case '1':{
            puts("| 主菜单 > 工具箱 > 系统修复 |\n");
            printf("请输入执行次数 (输入 0 返回): ");
            unsigned short counter{};
            scanf("%hu",&counter);
            if(!counter){
                ClearScreen();
                goto BEGIN;
            }
            for(;counter>0U;--counter){
                if(SysKernalVersion()>=62UL){
                    system("DISM /Online /Cleanup-Image /RestoreHealth");
                }
                system("SFC /ScanNow");
            }
            break;
        }case '2':{
            puts("| 主菜单 > 工具箱 > 激活工具 |\n");
            puts("此功能需要联网, 且系统内有 PowerShell. 脚本为 Microsoft Activation Scripts.\n");
            printf("是否继续? (Y/N): ");
            scanf("%s",&Code[1]);
            while((Code[1]!='Y'&&Code[1]!='N')||Code[2]!=0){
                printf("输入错误, 请重新输入: ");
                scanf("%s",&Code[1]);
            }
            if(Code[1]=='Y'){
                puts("\n请稍候...");
                system("PowerShell \"IRM https://massgrave.dev/get|IEX\"");
            }
            break;
        }case '3':{
            puts("| 主菜单 > 工具箱 > 高级启动 |\n");
            if(SysKernalVersion()<62UL){
                puts("此功能需要 Windows 8 及以上的 Windows 操作系统.\n");
                break;
            }
            for(unsigned short counter{5};;--counter){
                printf("[!] 请保存好文件, %hu 秒后可重启.\r",counter);
                if(!counter){
                    break;
                }
                Sleep(1000U);
            }
            puts("\033[?25h\n");
            system("Pause");
            puts("\n");
            system("ReAgentC /Enable");
            system("Shutdown /F /R /O /T 0");
            break;
        }
    }
    puts("\n########################################\n");
    puts("按任意键返回.\n");
    system("Pause");
    ClearScreen();
    goto BEGIN;
}
void Start(){
BEGIN:
    puts("| 主菜单 |\n");
    puts("   [?] 关于");
    puts("   [0] 重载配置");
    puts("   [1] 破解");
    puts("   [2] 恢复");
    puts("   [3] 工具箱\n");
    printf("请输入: ");
    scanf("%s",&Code[0]);
    while((Code[0]!='?'&&Code[0]!='0'&&Code[0]!='1'&&Code[0]!='2'&&Code[0]!='3')||Code[1]!=0){
        printf("输入错误, 请重新输入: ");
        scanf("%s",&Code[0]);
    }
    ClearScreen();
    switch(Code[0]){
        case '?':{
            About();
            break;
        }case '0':{
            Configuration(1);
            break;
        }case '1':{
            Cracking();
            break;
        }case '2':{
            Recovering();
            break;
        }case '3':{
            Toolkit();
            break;
        }
    }
    ClearScreen();
    goto BEGIN;
}
int main(){
    Configuration(0);
    Start();
}
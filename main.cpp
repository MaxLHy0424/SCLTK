#include"extern.hpp"
void Start();
void About(){
    puts("| 主菜单 > 关于 |\n");
    puts("   [软件名称] 机房控制软件克星 (Computer Room Control Software Nemesis)");
    puts("   [版本信息] v4.1.1");
    puts("   [项目作者] MaxLHy0424");
    puts("   [项目仓库] https://github.com/MaxLHy0424/CRCSN\n");
    puts("   (C) 2024 MaxLHy0424, 保留所有权利.\n");
    puts("########################################\n");
    puts("按任意键返回.\n");
    system("Pause");
}
void Configurator(bool reload){
    if(reload){
        puts("| 主菜单 > 重载配置 |\n");
    }
    std::ifstream fop;
    fop.open("config.ini",std::ios::in);
    if(!fop.is_open()){
        if(!reload){
            system("Color 3");
            if(IsUserAnAdmin()){
                system("Title [增强会话] CRCSN");
            }else{
                system("Title [基本会话] CRCSN");
            }
        }
        puts("无法读取 config.ini.\n");
        puts("########################################\n");
        puts("按任意键继续.\n");
        system("Pause");
        system("CLS");
        goto SKP;
    }
    for(unsigned short i{};i<4U;++i){
        getline(fop,Config[i]);
    }
    Config[0]="Color "+Config[0];
    system(Config[0].c_str());
    if(Config[2]=="1"){
        if(IsUserAnAdmin()){
            Config[1]="Title [增强会话] "+Config[1];
        }else{
            Config[1]="Title [基本会话] "+Config[1];
        }
    }else{
        Config[1]="Title "+Config[1];
    }
    system(Config[1].c_str());
    if(reload&&fop.is_open()){
        puts("重载完毕.\n");
        puts("########################################\n");
        puts("按任意键返回主菜单.\n");
        system("Pause");
    }
SKP:
    fop.close();
}
void Cracker(){
BEGIN:
    puts("| 主菜单 > 破解 |\n");
    puts("   [0] 返回");
    puts("   [1] 单次破解");
    puts("   [2] 循环破解\n");
    printf("请输入: ");
    scanf("%s",&Code[0]);
    while((Code[0]!='0'&&Code[0]!='1'&&Code[0]!='2')||Code[1]!=0){
        printf("输入错误, 请重新输入 ");
        scanf("%s",&Code[0]);
    }
    unsigned short pauseTime{};
    switch(Code[0]){
        case '0':{
            return;
        }case '2':{
            printf("请输入暂停时间 (毫秒, 0~10000): ");
            scanf("%hu",&pauseTime);
            while(pauseTime>10000U){
                printf("输入错误, 请重新输入 ");
                scanf("%hu",&pauseTime);
            }
            break;
        }
    }
    system("CLS");
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
    printf("     [暂停] ");
    if(pauseTime==0U){
        puts("禁用.\n");
    }else{
        printf("启用, %hu 毫秒.\n\n",pauseTime);
    }
    printf("是否继续? (Y/N): ");
    scanf("%s",&Code[1]);
    while((Code[1]!='Y'&&Code[1]!='y'&&Code[1]!='N'&&Code[1]!='n')||Code[2]!=0){
        printf("输入错误, 请重新输入 ");
        scanf("%s",&Code[1]);
    }
    if(Code[1]=='N'||Code[1]=='n'){
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
        if(Code[0]=='1'){
            break;
        }
        puts("\n暂停中...");
        Sleep(pauseTime);
    }
    puts("########################################\n");
    puts("按任意键返回主菜单.\n");
    system("Pause");
}
void Recoverer(){
    puts("| 主菜单 > 恢复 |\n");
    if(!IsUserAnAdmin()){
        puts("此功能在基本会话下不可用.\n");
        puts("########################################\n");
        puts("按任意键返回主菜单.\n");
        system("Pause");
        return;
    }
    printf("是否继续? (Y/N): ");
    scanf("%s",&Code[1]);
    while((Code[1]!='Y'&&Code[1]!='y'&&Code[1]!='N'&&Code[1]!='n')||Code[2]!=0){
        printf("输入错误, 请重新输入 ");
        scanf("%s",&Code[1]);
    }
    if(Code[1]=='N'||Code[1]=='n'){
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
    puts("########################################\n");
    puts("按任意键返回主菜单.\n");
    system("Pause");
}
void Toolkit(){
BEGIN:
    puts("| 主菜单 > 工具箱 |\n");
    if(!IsUserAnAdmin()){
        puts("此功能在基本会话下不可用.\n");
        puts("########################################\n");
        puts("按任意键返回主菜单.\n");
        system("Pause");
        return;
    }
    puts("   [0] 返回");
    puts("   [-] 自定义命令");
    puts("   [1] 系统修复");
    puts("   [2] 激活工具");
    puts("   [3] 高级启动\n");
    printf("请输入: ");
    scanf("%s",&Code[0]);
    while((Code[0]!='0'&&Code[0]!='-'&&Code[0]!='1'&&Code[0]!='2'&&Code[0]!='3')||Code[1]!=0){
        printf("输入错误, 请重新输入 ");
        scanf("%s",&Code[0]);
    }
    system("CLS");
    switch(Code[0]){
        case '0':{
            return;
        }case '-':{
            puts("| 主菜单 > 工具箱 > 自定义命令 |\n");
            std::ifstream fop;
            fop.open("config.ini",std::ios::in);
            if(!fop.is_open()){
                fop.close();
                puts("无法读取 config.ini.");
            }else if(Config[3]=="UNDEFINED"){
                fop.close();
                puts("未配置此项, 请编辑配置.");
            }else{
                fop.close();
                system(Config[3].c_str());
            }
            break;
        }case '1':{
            puts("| 主菜单 > 工具箱 > 系统修复 |\n");
            printf("请输入执行次数 (0 返回): ");
            unsigned short i{};
            scanf("%hu",&i);
            if(i==0){
                system("CLS");
                goto BEGIN;
            }
            for(;i>0U;--i){
                if(KernalVersion()>=62UL){
                    system("DISM /Online /Cleanup-Image /RestoreHealth");
                }
                system("SFC /ScanNow");
            }
            break;
        }case '2':{
            puts("| 主菜单 > 工具箱 > 激活工具 |\n");
            puts("此功能需要联网且安装 PowerShell, 使用 Microsoft Activation Scripts.\n");
            printf("是否继续? (Y/N): ");
            scanf("%s",&Code[1]);
            while((Code[1]!='Y'&&Code[1]!='y'&&Code[1]!='N'&&Code[1]!='n')||Code[2]!=0){
                printf("输入错误, 请重新输入 ");
                scanf("%s",&Code[1]);
            }
            if(Code[1]=='Y'||Code[1]=='y'){
                puts("\n请稍候...");
                system("PowerShell \"IRM https://massgrave.dev/get|IEX\"");
                puts("\n已退出.");
            }else{
                system("CLS");
                goto BEGIN;
            }
            break;
        }case '3':{
            puts("| 主菜单 > 工具箱 > 高级启动 |\n");
            if(KernalVersion()<62UL){
                puts("此功能需要 Windows 8 及以上的 Windows 操作系统.\n");
                break;
            }
            for(unsigned short i{5};;--i){
                printf("请保存文件, %hu 秒后可重启.\r",i);
                if(i==0){
                    break;
                }
                Sleep(1000UL);
            }
            puts("\n");
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
    system("CLS");
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
        printf("输入错误, 请重新输入 ");
        scanf("%s",&Code[0]);
    }
    system("CLS");
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
    system("CLS");
    goto BEGIN;
}
int main(){
    Configurator(false);
    Start();
}
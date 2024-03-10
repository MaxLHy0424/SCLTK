#include"extern.hpp"
char Code[3];
std::string Config[4];
unsigned short Start();
bool Configuration(bool reCfg){
    if(reCfg){
        printf("| 主菜单 > 重载配置 |\n\n");
    }
    std::ifstream fin;
    fin.open("config.ini",std::ios::in);
    if(!fin.is_open()){
        if(!reCfg){
            system("Color B");
            if(!IsUserAnAdmin()){
                system("Title [受限模式] CRCSN");
            }else{
                system("Title CRCSN");
            }
        }
        printf("[错误] 无法读取 config.ini !\n\n");
        printf("########################################\n\n");
        printf("按任意键继续.\n\n");
        system("Pause");
        system("CLS");
        goto skip;
    }
    {
        for(unsigned short i{};i<4;i++){
            getline(fin,Config[i]);
        }
        Config[0]="Color "+Config[0];
        system(Config[0].c_str());
        if(!IsUserAnAdmin()){
            Config[1]="Title [受限模式] "+Config[1];
        }else{
            Config[1]="Title "+Config[1];
        }
        system(Config[1].c_str());
        if(reCfg&&Config[2]=="1"){
            printf("[提示] 重载配置完成!\n\n");
            printf("########################################\n\n");
            printf("按任意键返回主菜单.\n\n");
            system("Pause");
        }
    }
    fin.close();
skip:
    system("CLS");
    if(!reCfg){
        Start();
    }
    return reCfg;
}
void About(){
    printf("| 主菜单 > 关于 |\n\n");
    printf("    [软件名称] 机房控制软件克星 (Computer Room Control Software Nemesis)\n");
    printf("    [版本信息] v3.1.0_Dev17\n");
    printf("    [软件作者] MaxLHy0424\n");
    printf("    [主 仓 库] https://github.com/MaxLHy0424/Computer-Room-Control-Software-Nemesis\n\n");
    printf("    (C) Copyright 2023-2024 MaxLHy0424, all rights reserved.\n\n");
    printf("########################################\n\n");
    printf("按任意键返回.\n\n");
    system("Pause");
    system("CLS");
    Start();
}
void Cracking(){
    printf("| 主菜单 > 破解 |\n\n");
    printf("   [0] 返回\n");
    printf("   [1] 单次模式\n");
    printf("   [2] 循环模式\n\n");
    printf("请输入: ");
    scanf("%s",&Code[0]);
    while((Code[0]!='0'&&Code[0]!='1'&&Code[0]!='2')||Code[1]!=0){
        printf("输入错误, 请重新输入: ");
        scanf("%s",&Code[0]);
    }
    unsigned short sleepTime{};
    switch(Code[0]){
        case '0':{
            system("CLS");
            Start();
            break;
        }case '2':{
            printf("请输入休眠时间 (毫秒, 范围: 0~10000): ");
            scanf("%hu",&sleepTime);
            while(sleepTime<0U||sleepTime>10000U){
                printf("输入错误, 请重新输入: ");
                scanf("%hu",&sleepTime);
            }
        }
    }
    system("CLS");
    printf("| 主菜单 > 破解 > 确认配置 |\n\n");
    printf("     [行为] ");
    switch(Code[0]){
        case '1':{
            printf("单次.\n");
            break;
        }case '2':{
            printf("循环.\n");
            break;
        }
    }
    printf(" [受限模式] ");
    if(IsUserAnAdmin()){
        printf("禁用.\n");
    }else{
        printf("启用.\n");
    }
    printf("     [休眠] ");
    if(!sleepTime){
        printf("禁用.\n\n");
    }else{
        printf("启用, %hu 毫秒.\n\n",sleepTime);
    }
    printf("请确认 (Y: 继续, N: 放弃并返回): ");
    scanf("%s",&Code[1]);
    while((Code[1]!='Y'&&Code[1]!='N')||Code[2]!=0){
        printf("输入错误, 请重新输入: ");
        scanf("%s",&Code[1]);
    }
    switch(Code[1]){
        case 'N':{
            system("CLS");
            Cracking();
            break;
        }
    }
    for(;;){
        system("CLS");
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
        printf("\n休眠中...\n");
        Sleep(sleepTime);
    }
    printf("\n########################################\n\n");
    printf("按任意键返回主菜单.\n\n");
    system("Pause");
    system("CLS");
    Start();
}
void Recoverying(){
    printf("| 主菜单 > 恢复 |\n\n");
    if(!IsUserAnAdmin()){
        printf("[提示] 当前为受限模式, 已禁用此功能.\n\n");
        printf("########################################\n\n");
        printf("按任意键返回主菜单.\n\n");
        system("Pause");
        system("CLS");
        Start();
    }
    printf("用于恢复破解时的部分操作, 部分情况下可能无效.\n\n");
    printf("请确认 (Y: 继续, N: 返回主菜单): ");
    scanf("%s",&Code[1]);
    while((Code[1]!='Y'&&Code[1]!='N')||Code[2]!=0){
        printf("输入错误, 请重新输入: ");
        scanf("%s",&Code[1]);
    }
    switch(Code[1]){
        case 'N':{
            system("CLS");
            goto back;
            break;
        }
    }
    system("CLS");
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
    system("Net Start tvnserver /Y");
    system("Net Start BSAgentSvr /Y");
    system("Net Start WFBSMlogon /Y");
    system("Net Start TDNetFilter /Y");
    system("Net Start TDFileFilter /Y");
    system("Net Start STUDSRV /Y");
    printf("\n########################################\n");
    printf("    联想云教室: vncviewer.exe, tvnserver32.exe, WFDeskShow.exe, WfbsPnpInstall.exe, WFBSMon.exe, WFBSMlogon.exe, refreship.exe, LenovoLockScreen.exe, DeploymentManager.exe, WFBSSvrLogShow.exe, ResetIp.exe, FuncForWIN64.exe, CertMgr.exe, Fireware.exe, BCDBootCopy.exe, PortControl64.exe, DesktopCheck.exe, DeploymentAgent.exe, XYNTService.exe\n");
    printf("    极域电子教室: StudentMain.exe, GATESRV.exe, MasterHelper.exe, ProcHelper64.exe, DispcapHelper.exe, VRCwPlayer.exe\n\n");
    printf("请手动启动以上可执行文件.\n");
    printf("########################################\n\n");
    Sleep(2500U);
    printf("按任意键返回主菜单.\n\n");
    system("Pause");
back:
    system("CLS");
    Start();
}
#pragma GCC diagnostic ignored "-Winfinite-recursion"
void Toolkit(){
    printf("| 主菜单 > 工具箱 |\n\n");
    if(!IsUserAnAdmin()){
        printf("[提示] 当前为受限模式, 已禁用此功能.\n\n");
        printf("########################################\n\n");
        printf("按任意键返回主菜单.\n\n");
        system("Pause");
        system("CLS");
        Start();
    }
    printf("    [0] 返回\n");
    printf("    [$] 自定义命令\n");
    printf("    [1] 修复系统文件\n");
    printf("    [2] 重启至 RE\n\n");
    printf("请输入: ");
    scanf("%s",&Code[0]);
    while((Code[0]!='0'&&Code[0]!='$'&&Code[0]!='1'&&Code[0]!='2')||Code[1]!=0){
        printf("输入错误, 请重新输入: ");
        scanf("%s",&Code[0]);
    }
    system("CLS");
    switch(Code[0]){
        case '0':{
            Start();
            break;
        }case '$':{
            printf("| 主菜单 > 工具箱 > 自定义命令 |\n\n");
            std::ifstream fin;
            fin.open("config.ini",std::ios::in);
            if(!fin.is_open()){
                printf("[错误] 无法读取 config.ini !\n\n");
                fin.close();
            }else if(Config[3]=="$NULL$"){
                fin.close();
                printf("[提示] 配置选项为 $NULL$ , 请编辑并重载配置.\n\n");
            }else{
                fin.close();
                system(Config[3].c_str());
            }
            break;
        }case '1':{
            printf("| 主菜单 > 工具箱 > 修复系统文件|\n\n");
            printf("请输入修复次数 (输入 0 返回): ");
            unsigned short tmp{};
            scanf("%hu",&tmp);
            if(!tmp){
                goto back;
                break;
            }
            for(;tmp>0;tmp--){
                if(GetSysKernalVersion()>=62UL){
                    system("DISM /Online /Cleanup-Image /RestoreHealth");
                }
                system("SFC /ScanNow");
            }
            break;
        }case '2':{
            if(GetSysKernalVersion()<62UL){
                printf("| 主菜单 > 工具箱 > 重启至 RE |\n\n");
                printf("[提示] 此功能需要 Windows 8 及以上的 Windows 操作系统.\n\n");
                break;
            }
            for(unsigned short tmp{5};tmp>=0;tmp--){
                system("CLS");
                printf("| 主菜单 > 工具箱 > 重启至 RE |\n\n");
                printf("[警告] 请保存好文件, %hu 秒后可重启至 RE.\n",tmp);
                if(!tmp){
                    printf("\n");
                    break;
                }
                Sleep(1000U);
            }
            system("Pause");
            system("ReAgentC /Enable");
            system("Shutdown /F /R /O /T 0");
            break;
        }
    }
    printf("########################################\n\n");
    printf("按任意键返回.\n\n");
    system("Pause");
back:
    system("CLS");
    Toolkit();
}
#pragma GCC diagnostic pop
unsigned short Start(){
    printf("| 主菜单 |\n\n");
    printf("    [?] 关于\n");
    printf("    [0] 重载配置\n");
    printf("    [1] 破解\n");
    printf("    [2] 恢复\n");
    printf("    [3] 工具箱\n\n");
    printf("请输入: ");
    scanf("%s",&Code[0]);
    while((Code[0]!='?'&&Code[0]!='0'&&Code[0]!='1'&&Code[0]!='2'&&Code[0]!='3')||Code[1]!=0){
        printf("输入错误, 请重新输入: ");
        scanf("%s",&Code[0]);
    }
    system("CLS");
    switch(Code[0]){
        case '?':{
            About();
            break;
        }case '0':{
            Configuration(true);
            break;
        }case '1':{
            Cracking();
            break;
        }case '2':{
            Recoverying();
            break;
        }case '3':{
            Toolkit();
            break;
        }
    }
    return 0;
}
int main(){
    Configuration(false);
    Start();
}
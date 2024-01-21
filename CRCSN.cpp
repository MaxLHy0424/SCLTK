#include<stdio.h>
#include<unistd.h>
#include<ShlObj.h>
char Code[3]{0,0,0};
int Start();
void About(){
    Code[0]=0,Code[1]=0,Code[2]=0;
    printf("[关于]\n");
    printf("   名称: 机房控制软件克星 (英文名 Computer Room Control Software Nemesis, 简称 CRCSN)\n");
    printf("   版本: v3.0_Dev8\n");
    printf("   项目仓库: https://github.com/MaxLHy0424/Computer-Room-Control-Software-Nemesis\n");
    printf("   作者: MaxLHy0424\n");
    printf("   作者 B 站账号 UID: 1678066522\n");
    printf("   作者邮箱: MaxLHy974413@outlook.com\n\n");
    printf("   (C) Copyright 2024 MaxLHy0424, All Rights Reserved.\n\n");
    printf("########################################\n\n");
    printf("按任意键返回上一级!\n\n");
    system("pause");
    system("cls");
    Start();
}
void CrackingTool(){
    if(!IsUserAnAdmin()){
        printf("[提示] 当前处于\"受限模式\", 此页面已被禁用...\n");
        printf("\n########################################\n\n");
        printf("按任意键返回上一级!\n\n");
        system("pause");
        system("cls");
        Start();
    }
    Code[0]=0,Code[1]=0,Code[2]=0;
    printf("[破解工具]\n");
    printf("   [0] 返回上一级\n");
    printf("   [1] 单次模式\n");
    printf("   [2] 循环模式\n");
    printf("请输入: ");
    scanf("%s",&Code[0]);
    while(Code[0]!='0'&&Code[0]!='1'&&Code[0]!='2'){
        printf("输入有误, 请重新输入: ");
        scanf("%s",&Code[0]);
    }
    double *SleepTimes{new double};
    switch(Code[0]){
        case '0':{
            system("cls");
            Start();
            break;
        }case '2':{
            printf("请输入 \"休眠\" 时间 (在每次执行操作后暂停执行一段时间, 单位 秒, 数值 0.0  ~ 10.0): ");
            scanf("%lf",SleepTimes);
            while(*SleepTimes>10.0||*SleepTimes<0.0){
                printf("输入错误, 请重新输入: ");
                scanf("%lf",SleepTimes);
            }
        }
    }
    system("cls");
    printf("配置:\n    模式: ");
    switch(Code[0]){
        case '1':{
            printf("单次模式.\n");
            printf("   \"休眠\": 已禁用.\n\n");
            break;
        }case '2':{
            printf("循环模式.\n");
            if(*SleepTimes==0){
                printf("已禁用.\n\n");
            }else{
                printf("已启用, %lg 秒.\n\n",*SleepTimes);
            }
            break;
        }
    }
    printf("请确认 (Y: 继续, N: 返回上一级): ");
    scanf("%s",&Code[1]);
    while((Code[1]!='Y'&&Code[1]!='N')||Code[2]!=0){
        printf("输入有误, 请重新输入: ");
        scanf("%s",&Code[1]);
    }
    switch(Code[1]){
        case 'Y':{
            break;
        }case 'N':{
            system("cls");
            CrackingTool();
            break;
        }
    }
    Code[0]=0,Code[1]=0,Code[2]=0;
    for(;;){
        system("cls");
        system("TaskKill /F /T /IM uninstallCnt.exe");
        system("TaskKill /F /T /IM Install64.exe");
        system("TaskKill /F /T /IM Install32.exe");
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
        system("TaskKill /F /T /IM SpecialSet.exe");
        system("TaskKill /F /T /IM VRCwPlayer.exe");
        system("TaskKill /F /T /IM InstHelpApp.exe");
        system("TaskKill /F /T /IM TDOvrSet.exe");
        system("TaskKill /F /T /IM GATESRV.exe");
        system("TaskKill /F /T /IM MasterHelper.exe");
        system("SC Config netprofm Start= Disabled");
        system("SC Config NlaSvc Start= Disabled");
        system("SC Config EventLog Start= Disabled");
        system("SC Config tvnserver Start= Disabled");
        system("SC Config TDNetFilter Start= Disabled");
        system("SC Config TDFileFilter Start= Disabled");
        system("SC Config STUDSRV Start= Disabled");
        system("SC Config BSAgentSvr Start= Disabled");
        system("SC Config WFBSMlogon Start= Disabled");
        system("Net Stop netprofm");
        system("Net Stop NlaSvc");
        system("Net Stop EventLog");
        system("Net Stop tvnserver");
        system("Net Stop TDNetFilter");
        system("Net Stop TDFileFilter");
        system("Net Stop STUDSRV");
        system("Net Stop BSAgentSvr");
        system("Net Stop WFBSMlogon");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\uninstallCnt.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\Install64.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\Install32.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\vncviewer.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\tvnserver32.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WfbsPnpInstall.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSMon.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSMlogon.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSSvrLogShow.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\ResetIp.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\FuncForWIN64.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\CertMgr.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\Fireware.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\BCDBootCopy.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\refreship.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\LenovoLockScreen.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\PortControl64.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DesktopCheck.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DeploymentManager.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DeploymentAgent.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\XYNTService.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\ProcHelper64.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\StudentMain.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DispcapHelper.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\SpecialSet.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\VRCwPlayer.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\InstHelpApp.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\TDOvrSet.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\GATESRV.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        system("Reg Add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\MasterHelper.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
        if(Code[0]=='1'){
            break;
        }
        printf("\n休眠中...\n");
        sleep(*SleepTimes);
    }
    switch(Code[0]){
        case '1':{
            printf("\n########################################\n");
            printf("按任意键返回 [主界面]!\n\n");
            system("pause");
            system("cls");
            delete SleepTimes;
            Start();
            break;
        }
    }
}
void RecoveringMode(){
    if(!IsUserAnAdmin()){
        printf("[提示] 当前处于\"受限模式\", 此页面已被禁用...\n");
        printf("\n########################################\n\n");
        printf("按任意键返回上一级!\n\n");
        system("pause");
        system("cls");
        Start();
    }
    printf("[恢复工具]\n\n");
    printf("说明: 本功能用于恢复破解时的部分操作, 部分情况下可能无法产生效果, 执行完毕后请手动开启控制软件.\n\n");
    printf("请确认 (Y: 继续, N: 返回上一级): ");
    scanf("%s",&Code[1]);
    while((Code[1]!='Y'&&Code[1]!='N')||Code[2]!=0){
        printf("输入有误, 请重新输入: ");
        scanf("%s",&Code[1]);
    }
    switch(Code[1]){
        case 'Y':{
            break;
        }case 'N':{
            system("cls");
            Start();
            break;
        }
    }
    Code[1]=0,Code[1]=0,Code[2]=0;
    system("cls");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\uninstallCnt.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\Install64.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\Install32.exe\" /F");
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
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\SpecialSet.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\VRCwPlayer.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\InstHelpApp.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\TDOvrSet.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\GATESRV.exe\" /F");
    system("Reg Delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\MasterHelper.exe\" /F");
    system("SC Config netprofm Start= Auto");
    system("SC Config NlaSvc Start= Auto");
    system("SC Config EventLog Start= Auto");
    system("SC Config tvnserver Start= Auto");
    system("SC Config BSAgentSvr Start= Auto");
    system("SC Config WFBSMlogon Start= Auto");
    system("SC Config TDNetFilter Start= Auto");
    system("SC Config TDFileFilter Start= Auto");
    system("SC Config STUDSRV Start= Auto");
    system("Net Start netprofm");
    system("Net Start NlaSvc");
    system("Net Start EventLog");
    system("Net Start tvnserver");
    system("Net Start BSAgentSvr");
    system("Net Start WFBSMlogon");
    system("Net Start TDNetFilter");
    system("Net Start TDFileFilter");
    system("Net Start STUDSRV");
    printf("\n########################################\n");
    printf("    联想云教室: vncviewer.exe, tvnserver32.exe, WFDeskShow.exe, WfbsPnpInstall.exe, WFBSMon.exe, WFBSMlogon.exe, refreship.exe, LenovoLockScreen.exe, DeploymentManager.exe, WFBSSvrLogShow.exe, ResetIp.exe, FuncForWIN64.exe, CertMgr.exe, Fireware.exe, BCDBootCopy.exe, PortControl64.exe, DesktopCheck.exe, DeploymentAgent.exe, XYNTService.exe\n");
    printf("  极域电子教室: StudentMain.exe, GATESRV.exe, MasterHelper.exe, ProcHelper64.exe, DispcapHelper.exe, VRCwPlayer.exe\n\n");
    printf("请手动开启以上程序!\n");
    printf("########################################\n\n");
    printf("按任意键返回上一级!\n\n");
    system("pause");
    system("cls");
    Start();
}
int Start(){
    Code[0]=0,Code[1]=0,Code[2]=0;
    printf("欢迎使用 机房控制软件克星!\n");
    printf("   [x] 退出程序\n");
    printf("   [0] 关于\n");
    printf("   [1] 破解工具\n");
    printf("   [2] 恢复工具\n");
    printf("请输入: ");
    scanf("%s",&Code[0]);
    while((Code[0]!='x'&&Code[0]!='0'&&Code[0]!='1'&&Code[0]!='2')||Code[1]!=0){
        printf("输入有误, 请重新输入: ");
        scanf("%s",&Code[0]);
    }
    system("cls");
    switch(Code[0]){
        case 'x':{
            exit(0);
        }case '0':{
            About();
            break;
        }case '1':{
            CrackingTool();
            break;
        }case '2':{
            RecoveringMode();
            break;
        }
    }
    return 0;
}
int main(){
    system("set path=%path%;C:\\Windows\\System32\\");
    system("title 机房控制软件克星");
    system("color b");
    if(!IsUserAnAdmin()){
        printf("[提示] 建议以 Administrator 权限运行本程序...\n");
        printf("\n########################################\n\n");
        printf("按任意键进入 \"受限模式\"!\n\n");
        system("pause");
        system("cls");
    }
    Start();
    return 0;
}
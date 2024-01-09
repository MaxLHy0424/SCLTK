#include<stdio.h>
#include<unistd.h>
#include<ShlObj.h>
struct ProgramRuntimeData{
    BOOL StartupMode;
    char Code[2],Back[2];
    double SleepTimes;
}Data;
int Start();
void About(){
    Data.Code[0]=0,Data.Code[1]=0;
    printf("[关于]\n");
    printf("   名称: 机房控制软件克星 (英文名 Computer Room Control Software Nemesis, 简称 CRCSN)\n");
    printf("   版本: v3.0\n");
    printf("   项目仓库: https://github.com/MaxLHy0424/Computer-Room-Control-Software-Nemesis\n");
    printf("   作者: MaxLHy0424\n");
    printf("   作者 B 站账号 UID: 1678066522\n");
    printf("   作者邮箱: MaxLHy974413@outlook.com\n\n");
    printf("   (C) Copyright 2023-2024 MaxLHy0424, All Rights Reserved.\n\n");
    printf("########################################\n\n");
    printf("按任意键返回上一级!\n\n");
    system("pause");
    system("cls");
    Start();
}
void CrackingMode(){
    Data.Code[0]=0,Data.Code[1]=0;
    printf("[破解模式]\n");
    printf("   [0] 返回上一级\n");
    printf("   [1] 主方案\n");
    printf("   [2] 备用方案\n");
    printf("请输入: ");
    scanf("%s",&Data.Code[0]);
    while(Data.Code[0]!='0'&&Data.Code[0]!='1'&&Data.Code[0]!='2'){
        printf("输入有误, 请重新输入: ");
        scanf("%s",&Data.Code[0]);
    }
    switch(Data.Code[0]){
        case '0':{
            system("cls");
            Start();
            break;
        }case '1':{
            if(!Data.StartupMode){
                system("cls");
                printf("当前为 \"受限模式\", 此方案已被禁用. 按任意键返回上一级!\n\n");
                system("pause");
                system("cls");
                CrackingMode();
                break;
            }
        }
    }
    printf("请输入 \"休眠\" 时间 (在每次执行操作后暂停执行一段时间, 单位 秒, 数值 0.0  ~ 15.0): ");
    scanf("%lf",&Data.SleepTimes);
        while(Data.SleepTimes>15.0||Data.SleepTimes<0.0){
            printf("输入错误, 请重新输入: ");
            scanf("%lf",&Data.SleepTimes);
        }
    system("cls");
    printf("配置:\n          方案: ");
    switch(Data.Code[0]){
        case '1':{
            printf("主方案.\n");
            break;
        }case '2':{
            printf("备用方案.\n");
            break;
        }
    }
    printf("   \"休眠\" 时间: ");
    if(Data.SleepTimes==0){
        printf("已禁用.\n\n");
    }else{
        printf("已启用, %lg 秒.\n\n",Data.SleepTimes);
    }
    printf("请确认 (Y: 继续, N: 返回上一级): ");
    scanf("%s",&Data.Back[0]);
    while((Data.Back[0]!='Y'&&Data.Back[0]!='N')||Data.Back[1]!=0){
        printf("输入有误, 请重新输入: ");
        scanf("%s",&Data.Back[0]);
    }
    switch(Data.Back[0]){
        case 'Y':{
            break;
        }case 'N':{
            system("cls");
            CrackingMode();
            break;
        }
    }
    switch(Data.Code[0]){
        case '1':{
            for(;;){
                system("cls");
                printf("尝试禁用服务 tvnserver ...\n\n");
                system("sc config tvnserver start= Disabled");
                printf("\n尝试禁用服务 TDNetFilter ...\n\n");
                system("sc config TDNetFilter start= Disabled");
                printf("\n尝试禁用服务 TDFileFilter ...\n\n");
                system("sc config TDFileFilter start= Disabled");
                printf("\n尝试停止服务 tvnserver ...\n\n");
                system("net stop tvnserver");
                printf("\n尝试停止服务 TDNetFilter ...\n\n");
                system("net stop TDNetFilter");
                printf("\n尝试停止服务 TDFileFilter ...\n\n");
                system("net stop TDFileFilter");
                printf("\n尝试结束进程 PortControl64.exe ...\n\n");
                system("TaskKill /F /T /IM PortControl64.exe");
                printf("\n尝试结束进程 DesktopCheck.exe ...\n\n");
                system("TaskKill /F /T /IM DesktopCheck.exe");
                printf("\n尝试结束进程 DeploymentAgent.exe ...\n\n");
                system("TaskKill /F /T /IM DeploymentAgent.exe");
                printf("\n尝试结束进程 XYNTService.exe ...\n\n");
                system("TaskKill /F /T /IM XYNTService.exe");
                printf("\n尝试结束进程 ProcHelper64.exe ...\n\n");
                system("TaskKill /F /T /IM ProcHelper64.exe");
                printf("\n尝试结束进程 StudentMain.exe ...\n\n");
                system("TaskKill /F /T /IM StudentMain.exe");
                printf("\n尝试结束进程 GATESRV.exe ...\n\n");
                system("TaskKill /F /T /IM GATESRV.exe");
                printf("\n尝试结束进程 MasterHelper.exe ...\n\n");
                system("TaskKill /F /T /IM MasterHelper.exe");
                printf("\n休眠中...\n");
                sleep(Data.SleepTimes);
            }
            break;
        }case '2':{
            for(;;){
                system("cls");
                printf("尝试结束进程 PortControl64.exe ...\n\n");
                system("TaskKill /F /T /IM PortControl64.exe");
                printf("\n尝试结束进程 DesktopCheck.exe ...\n\n");
                system("TaskKill /F /T /IM DesktopCheck.exe");
                printf("\n尝试结束进程 DeploymentAgent.exe ...\n\n");
                system("TaskKill /F /T /IM DeploymentAgent.exe");
                printf("\n尝试结束进程 XYNTService.exe ...\n\n");
                system("TaskKill /F /T /IM XYNTService.exe");
                printf("\n尝试结束进程 ProcHelper64.exe ...\n\n");
                system("TaskKill /F /T /IM ProcHelper64.exe");
                printf("\n尝试结束进程 StudentMain.exe ...\n\n");
                system("TaskKill /F /T /IM StudentMain.exe");
                printf("\n尝试结束进程 GATESRV.exe ...\n\n");
                system("TaskKill /F /T /IM GATESRV.exe");
                printf("\n尝试结束进程 MasterHelper.exe ...\n\n");
                system("TaskKill /F /T /IM MasterHelper.exe");
                printf("\n休眠中...\n");
                sleep(Data.SleepTimes);
            }
            break;
        }
    }
}
void RecoveringMode(){
    if(!Data.StartupMode){
        system("cls");
        printf("当前为 \"受限模式\", 此功能已被禁用...\n\n");
        printf("########################################\n\n");
        printf("按任意键返回上一级!\n\n");
        system("pause");
        system("cls");
        Start();
    }
    Data.Code[0]=0,Data.Code[1]=0;
    Data.Back[0]=0,Data.Back[1]=0;
    printf("[恢复模式]\n\n");
    printf("说明: 本功能用于恢复破解时的部分操作, 部分情况下可能无法产生效果, 执行完毕后请手动开启控制软件.\n\n");
    printf("请确认 (Y: 继续, N: 返回上一级): ");
    scanf("%s",&Data.Back[0]);
    while((Data.Back[0]!='Y'&&Data.Back[0]!='N')||Data.Back[1]!=0){
        printf("输入有误, 请重新输入: ");
        scanf("%s",&Data.Back[0]);
    }
    switch(Data.Back[0]){
        case 'Y':{
            break;
        }case 'N':{
            system("cls");
            Start();
            break;
        }
    }
    Data.Back[0]=0,Data.Back[1]=0;
    system("cls");
    printf("尝试启用服务 tvnserver ...\n\n");
    system("sc config tvnserver start= Auto");
    printf("\n尝试启用服务 TDNetFilter ...\n\n");
    system("sc config TDNetFilter start= Auto");
    printf("\n尝试启用服务 TDFileFilter ...\n\n");
    system("sc config TDFileFilter start= Auto");
    printf("\n尝试启动服务 tvnserver ...\n\n");
    system("net start tvnserver");
    printf("\n尝试启动服务 TDNetFilter ...\n\n");
    system("net start TDNetFilter");
    printf("\n尝试启动服务 TDFileFilter ...\n\n");
    system("net start TDFileFilter");
    printf("\n########################################\n");
    printf("    联想云教室: PortControl64.exe , DesktopCheck.exe , DeploymentAgent.exe , XYNTService.exe\n");
    printf("  极域电子教室: StudentMain.exe , GATESRV.exe , MasterHelper.exe , ProcHelper64.exe\n\n");
    printf("请手动开启以上程序!\n");
    printf("########################################\n\n");
    printf("按任意键返回上一级!\n\n");
    system("pause");
    system("cls");
    Start();
}
void ToolBox(){
    if(!Data.StartupMode){
        system("cls");
        printf("当前为 \"受限模式\", 此功能已被禁用...\n\n");
        printf("########################################\n\n");
        printf("按任意键返回上一级!\n\n");
        system("pause");
        system("cls");
        Start();
    }
    Data.Code[0]=0,Data.Code[1]=0;
    printf("[工具箱]\n");
    printf("   [0] 返回上一级\n");
    printf("   [1] 打开 \"极域电子教室\" 默认安装目录\n");
    printf("   [2] 打开 \"联想云教室\" 默认安装目录\n");
    printf("   [3] 劫持工具\n");
    printf("   [4] 查询 \"极域电子教室\" 万能密码\n");
    printf("请输入: ");
    scanf("%s",&Data.Code[0]);
    while((Data.Code[0]!='0'&&Data.Code[0]!='1'&&Data.Code[0]!='2'&&Data.Code[0]!='3'&&Data.Code[0]!='4')||Data.Code[1]!=0){
        printf("输入有误, 请重新输入: ");
        scanf("%s",&Data.Code[0]);
    }
    system("cls");
    switch(Data.Code[0]){
        case '0':{
            Start();
            break;
        }case '1':{
            printf("尝试打开...\n\n");
            system("explorer \"C:\\Program Files (x86)\\Mythware\\\"");
            printf("########################################\n\n");
            printf("按任意键返回上一级!\n\n");
            system("pause");
            system("cls");
            ToolBox();
            break;
        }case '2':{
            printf("尝试打开...\n\n");
            system("explorer \"C:\\Program Files (x86)\\Lenovo\\\"");
            printf("########################################\n\n");
            printf("按任意键返回上一级!\n\n");
            system("pause");
            system("cls");
            ToolBox();
            break;
        }case '3':{
            printf("[工具箱 > 劫持工具]\n");
            printf("\"劫持工具\" 支持劫持 \"极域电子教室\" 和 \"联想云教室\".\n");
            printf("   [0] 返回上一级\n");
            printf("   [1] 应用劫持\n");
            printf("   [2] 撤销劫持\n");
            printf("请输入: ");
            scanf("%s",Data.Code);
            while((Data.Code[0]!='0'&&Data.Code[0]!='1'&&Data.Code[0]!='2')||Data.Code[1]!=0){
                printf("输入有误, 请重新输入: ");
                scanf("%s",Data.Code);
            }
            system("cls");
            switch(Data.Code[0]){
                case '0':{
                    ToolBox();
                    break;
                }case '1':{
                    printf("[工具箱 > 劫持工具 > 应用劫持]\n\n");
                    system("reg add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\PortControl64.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
                    system("reg add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DesktopCheck.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
                    system("reg add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DeploymentAgent.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
                    system("reg add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\XYNTService.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
                    system("reg add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\ProcHelper64.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
                    system("reg add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\StudentMain.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
                    system("reg add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\GATESRV.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
                    system("reg add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\MasterHelper.exe\" /t REG_SZ -v debugger /d \"THIS_PROGRAM_HAS_BEEN_BLOCKED\" /F");
                    printf("\n\n已成功劫持!\n\n");
                    printf("########################################\n\n");
                    printf("按任意键返回 [工具箱]!\n\n");
                    system("pause");
                    system("cls");
                    ToolBox();
                    break;
                }case '2':{
                    printf("[工具箱 > 劫持工具 > 撤销劫持]\n\n");
                    system("reg delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\PortControl64.exe\" /F");
                    system("reg delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DesktopCheck.exe\" /F");
                    system("reg delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DeploymentAgent.exe\" /F");
                    system("reg delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\XYNTService.exe\" /F");
                    system("reg delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\ProcHelper64.exe\" /F");
                    system("reg delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\StudentMain.exe\" /F");
                    system("reg delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\GATESRV.exe\" /F");
                    system("reg delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\MasterHelper.exe\" /F");
                    printf("\n\n已撤销劫持!\n\n");
                    printf("########################################\n\n");
                    printf("按任意键返回 [工具箱]!\n\n");
                    system("pause");
                    system("cls");
                    ToolBox();
                    break;
                }
            }
        }case '4':{
            printf("[工具箱 > 查询 \"极域电子教室\" 万能密码\n\n");
            printf("\"极域电子教室\" 万能密码:\n");
            printf("mythware_super_password\n");
            printf("########################################\n\n");
            printf("按任意键返回上一级!\n\n");
            system("pause");
            system("cls");
            ToolBox();
            break;
        }
    }
}
int Start(){
    Data.Code[0]=0,Data.Code[1]=0;
    printf("欢迎使用 机房控制软件克星!\n");
    printf("   [x] 退出程序\n");
    printf("   [0] 关于\n");
    printf("   [1] 破解模式\n");
    printf("   [2] 恢复模式\n");
    printf("   [3] 工具箱\n");
    printf("请输入: ");
    scanf("%s",&Data.Code[0]);
    while((Data.Code[0]!='x'&&Data.Code[0]!='0'&&Data.Code[0]!='1'&&Data.Code[0]!='2'&&Data.Code[0]!='3')||Data.Code[1]!=0){
        printf("输入有误, 请重新输入: ");
        scanf("%s",&Data.Code[0]);
    }
    system("cls");
    switch(Data.Code[0]){
        case 'x':{
            exit(0);
        }case '0':{
            About();
            break;
        }case '1':{
            CrackingMode();
            break;
        }case '2':{
            RecoveringMode();
            break;
        }case '3':{
            ToolBox();
            break;
        }
    }
    return 0;
}
int main(){
    Data.StartupMode=IsUserAnAdmin();//StartupMode: 0 - "受限模式"   1 - "完整模式"
    system("set path=%path%;C:\\Windows\\System32\\");
    system("color b");
    if(!Data.StartupMode){
        system("title 机房控制软件克星 (受限模式)");
        printf("[Error] 当前权限为 User, 建议 Administrator 权限运行本程序...\n");
        printf("\n########################################\n\n");
        printf("按任意键进入 \"受限模式\"...\n\n");
        system("pause");
        system("cls");
    }else{
        system("title 机房控制软件克星");
    }
    Start();
}
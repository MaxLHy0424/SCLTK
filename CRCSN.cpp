#include<stdio.h>
#include<unistd.h>
#include<ShlObj.h>
struct ProgramRuntimeData{
    BOOL EnableFullMode;
    char FeatureCode[2];
    char FeatureCodeForCrackingMode[2];
    char Back[2];
    double SleepTimes;
    char ToolCode[2];
}Data;
int Start();
void About(){
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
    printf("[破解模式]\n");
    printf("   [0] 返回上一级\n");
    printf("   [1] 主方案\n");
    printf("   [2] 备用方案\n");
    printf("请输入: ");
    scanf("%s",&Data.FeatureCodeForCrackingMode[0]);
    while(Data.FeatureCodeForCrackingMode[0]!='0'&&Data.FeatureCodeForCrackingMode[0]!='1'&&Data.FeatureCodeForCrackingMode[0]!='2'){
        printf("输入有误: ");
        scanf("%s",&Data.FeatureCodeForCrackingMode[0]);
    }
    switch(Data.FeatureCodeForCrackingMode[0]){
        case '0':{
            system("cls");
            Start();
            break;
        }case '1':{
            if(!Data.EnableFullMode){
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
    {
        unsigned short WrongInputNumber{0};
        while(Data.SleepTimes>15.0||Data.SleepTimes<0.0){
            if(WrongInputNumber>=5){
                printf("\n########################################\n\n");
                printf("[Warning] 输入次数达到限制, 按任意键禁用 \"休眠\" 并继续...");
                printf("########################################\n\n");
                system("pause");
                Data.SleepTimes=0;
                WrongInputNumber=0;
                break;
            }
            printf("输入错误, 请重新输入: ");
            scanf("%lf",&Data.SleepTimes);
            WrongInputNumber++;
        }
    }
    system("cls");
    printf("配置:\n          方案: ");
    switch(Data.FeatureCodeForCrackingMode[0]){
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
        printf("输入有误: ");
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
    switch(Data.FeatureCodeForCrackingMode[0]){
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
                printf("\n尝试结束进程 PortControl64.exe ...\n\n");
                system("TaskKill /F /T /IM PortControl64.exe");
                printf("\n尝试结束进程 DesktopCheck.exe ...\n\n");
                system("TaskKill /F /T /IM DesktopCheck.exe");
                printf("\n尝试结束进程 DeploymentAgent.exe ...\n\n");
                system("TaskKill /F /T /IM DeploymentAgent.exe");
                printf("\n尝试结束进程 XYNTService.exe ...\n\n");
                system("TaskKill /F /T /IM XYNTService.exe");
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
    if(!Data.EnableFullMode){
        system("cls");
        printf("当前为 \"受限模式\", 此功能已被禁用. 按任意键返回上一级!\n\n");
        system("pause");
        system("cls");
        Start();
    }
    printf("[恢复模式]\n\n");
    printf("说明: 本功能用于恢复破解时的部分操作, 部分情况下可能无法产生效果, 执行完毕后请手动开启控制软件.\n\n");
    printf("请确认 (Y: 继续, N: 返回上一级): ");
    scanf("%s",&Data.Back[0]);
    while((Data.Back[0]!='Y'&&Data.Back[0]!='N')||Data.Back[1]!=0){
        printf("输入有误: ");
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
    printf("  极域电子教室: StudentMain.exe , GATESRV.exe , MasterHelper.exe\n\n");
    printf("请手动开启以上程序!\n");
    printf("########################################\n\n");
    printf("按任意键返回上一级!\n\n");
    system("pause");
    system("cls");
    Start();
}
void ToolBox(){
    printf("[工具箱]\n");
    printf("   [0] 返回上一级\n");
    printf("   [1] 打开 \"极域电子教室\" 默认安装目录\n");
    printf("   [2] 打开 \"联想云教室\" 默认安装目录\n");
    printf("   [3] 读取 \"极域电子教室\" 配置密码\n");
    printf("   [4] 删除 \"极域电子教室\" 和 \"联想云教室\" 注册表\n");
    printf("   [5] 劫持 \"极域电子教室\" 和 \"联想云教室\"\n");
    printf("请输入: ");
    scanf("%s",&Data.ToolCode[0]);
}
int Start(){
    printf("欢迎使用 机房控制软件克星!\n");
    printf("   [x] 退出程序\n");
    printf("   [0] 关于\n");
    printf("   [1] 破解模式\n");
    printf("   [2] 恢复模式\n");
    printf("   [3] 工具箱\n");
    printf("请输入: ");
    scanf("%s",&Data.FeatureCode[0]);
    while((Data.FeatureCode[0]!='x'&&Data.FeatureCode[0]!='0'&&Data.FeatureCode[0]!='1'&&Data.FeatureCode[0]!='2'&&Data.FeatureCode[0]!='3')||Data.FeatureCode[1]!=0){
        printf("输入有误: ");
        scanf("%s",&Data.FeatureCode[0]);
    }
    system("cls");
    switch(Data.FeatureCode[0]){
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
            printf("正在开发...\n");
            system("pause");
            system("cls");
            Start();
            break;
        }
    }
    return 0;
}
int main(){
    Data.EnableFullMode=IsUserAnAdmin();
    system("set path=%path%;C:\\Windows\\System32\\");
    system("color b");
    if(!Data.EnableFullMode){
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
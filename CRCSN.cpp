/*
感谢您查看本程序的源代码!
本程序以 MIT 协议开源 (具体看仓库下 LICENSE 文件), 请在符合 MIT 协议的情况下使用.
欢迎对本程序提出建议, 这对我很重要!
*/
#include<cstdio>
#include<unistd.h>//函数 Sleep 依赖头文件, 用于实现 "休眠" 功能
#include<stdlib.h>
#include<ShlObj.h>//函数 IsUerAnAdmin() 依赖头文件, 用于检测是否为管理员权限运行程序
void PermissionDetection(void){
    BOOL RunAsAdmin{IsUserAnAdmin()};//当是管理员权限时为 True, 否则为 False
    if(!RunAsAdmin){//对变量 RunAsAdmin 进行非运算, 及将非管理员权限运行时的值 False 转为 True 以进行 if 语句内的操作
        printf("[Error] 权限不足, 请以管理员权限运行...\n");
        printf("\n########################################\n\n");
        system("pause");
        exit(0);
    }
}
void About(void){
    printf("软件名称: 机房控制软件克星 (英文名 Computer Room Control Software Nemesis, 简称 CRCSN)\n");
    printf("软件版本: v2.0_Stable\n");
    printf("软件项目仓库: https://github.com/MaxLHy0424/Computer-Room-Control-Software-Nemesis\n");
    printf("作者: MaxLHy0424\n");
    printf("作者 B 站账号 UID: 1678066522\n");
    printf("作者反馈邮箱: MaxLHy974413@outlook.com / 1097268127@qq.com\n\n");
    printf("(C) Copyright 2023-2024 CRCSN. 保留所有权利.\n");
}
double Mode1(double SleepTime){
    printf("您选择了 \"TaskKill 方案\".\n");
    printf("提示: 已尝试临时修复环境变量, 在部分情况下可能出现找不到命令的情况, 如无法使用, 暂无解决方案.\n");
    printf("按任意键将清空以上内容并继续!\n\n");
    system("pause");
    for(;;){//无限循环, 解决存在守护进程的问题, 资源消耗并不高
        system("cls");//清空控制台屏幕
        printf("尝试禁用服务 tvnserver ...\n\n");
        system("sc config tvnserver start= Disabled");//禁用 "联想云教室" 的主要服务
        printf("\n尝试停止服务 tvnserver ...\n\n");
        system("sc stop tvnserver");//停止 "联想云教室" 的主要服务
        printf("\n尝试禁用服务 TDNetFilter ...\n\n");
        system("sc config TDNetFilter start= Disabled");//禁用 "联想云教室" 的阻止网页访问的服务 (暂未测试)
        printf("\n尝试禁用服务 TDFileFilter ...\n\n");
        system("sc config TDFileFilter start= Disabled");//禁用 "极域电子教室" 的 U 盘锁服务 (暂未测试)
        printf("\n尝试停止服务 TDNetFilter ...\n\n");
        system("sc stop TDNetFilter");//停止 "极域电子教室" 的网络限制服务 (暂未测试)
        printf("\n尝试停止服务 TDFileFilter ...\n\n");
        system("sc stop TDFileFilter");//停止 "极域电子教室" 的 U 盘锁服务 (暂未测试)
        printf("\n尝试结束进程 PortControl64.exe ...\n\n");
        system("TaskKill /F /T /IM PortControl64.exe");//结束 "联想云教室" 的相关进程
        printf("\n尝试结束进程 DesktopCheck.exe ...\n\n");
        system("TaskKill /F /T /IM DesktopCheck.exe");//结束 "联想云教室" 的相关进程
        printf("\n尝试结束进程 DeploymentAgent.exe ...\n\n");
        system("TaskKill /F /T /IM DeploymentAgent.exe");//结束 "联想云教室" 的相关进程
        printf("\n尝试结束进程 XYNTService.exe ...\n\n");
        system("TaskKill /F /T /IM XYNTService.exe");//结束 "联想云教室" 的相关进程
        printf("\n尝试结束进程 StudentMain.exe ...\n\n");
        system("TaskKill /F /T /IM StudentMain.exe");//结束 "极域电子教室" 的进程
        printf("\n尝试结束进程 GATESRV.exe ...\n\n");
        system("TaskKill /F /T /IM GATESRV.exe");//结束 "极域电子教室" 阻止网页访问的进程 (暂未测试)
        printf("\n尝试结束进程 MasterHelper.exe ...\n\n");
        system("TaskKill /F /T /IM MasterHelper.exe");//结束 "极域电子教室" 阻止网页访问的进程 (暂未测试)
        printf("\n休眠中...\n");
        sleep(SleepTime);
    }
    return 0;
}
double Mode2(double SleepTime){
    printf("您选择了 \"TsKill 方案\".\n");
    printf("提示 1: 此方案推荐用于 \"TaskKill 方案\" 无法使用时的备选方案, 另外在部分情况下可能出现找不到命令的情况.\n");
    printf("提示 2: 已尝试临时修复环境变量, 如还是无法使用, 暂无解决方案.\n");
    printf("按任意键将清空以上内容并继续!\n\n");
    system("pause");//
    for(;;){//无限循环, 解决存在守护进程的问题, 资源消耗并不高
        system("cls");//清空控制台屏幕
        printf("尝试禁用服务 tvnserver ...\n\n");
        system("sc config tvnserver start= Disabled");//禁用 "联想云教室" 的主要服务
        printf("\n尝试停止服务 tvnserver ...\n\n");
        system("sc stop tvnserver");//停止 "联想云教室" 的主要服务
        printf("\n尝试禁用服务 TDNetFilter ...\n\n");
        system("sc config TDNetFilter start= Disabled");//禁用 "联想云教室" 的组织网页访问的服务 (暂未测试)
        printf("\n尝试禁用服务 TDFileFilter ...\n\n");
        system("sc config TDFileFilter start= Disabled");//禁用 "极域电子教室" 的 U 盘锁服务 (暂未测试)
        printf("\n尝试停止服务 TDNetFilter ...\n\n");
        system("sc stop TDNetFilter");//解除 "极域电子教室" 的阻止网页访问的服务 (暂未测试)
        printf("\n尝试停止服务 TDFileFilter ...\n\n");
        system("sc stop TDFileFilter");//解除 "极域电子教室" 的 U 盘锁 (暂未测试)
        printf("\n尝试结束进程 PortControl64.exe ...\n\n");
        system("TsKill PortControl64 /A /V");//结束 "联想云教室" 的相关进程
        printf("\n尝试结束进程 DesktopCheck.exe ...\n\n");
        system("TsKill DesktopCheck /A /V");//结束 "联想云教室" 的相关进程
        printf("\n尝试结束进程 DeploymentAgent.exe ...\n\n");
        system("TsKill DeploymentAgent /A /V");//结束 "联想云教室" 的相关进程
        printf("\n尝试结束进程 XYNTService.exe ...\n\n");
        system("TsKill XYNTService /A /V");//结束 "联想云教室" 的相关进程
        printf("\n尝试结束进程 StudentMain.exe ...\n\n");
        system("TsKill StudentMain /A /V");//结束 "极域电子教室" 的进程
        printf("\n尝试结束进程 GATESRV.exe ...\n\n");
        system("TsKill GATESRV /A /V");//结束 "极域电子教室" 的相关进程 (暂未测试)
        printf("\n尝试结束进程 MasterHelper.exe ...\n\n");
        system("TsKill MasterHelper /A /V");//结束 "极域电子教室" 的相关进程 (暂未测试)
        printf("\n休眠中...\n");
        sleep(SleepTime);
    }
    return 0;
}
double Start(unsigned short InputWrongNumber,double SleepTime){//用处: 自定义函数类型来解决 main 必须是 int 型的限制
    char FeatureCode[5];//用于存储功能代码 (全局定义纯粹是作者太懒)
    printf("欢迎使用 机房控制软件克星 !\n\n");
    printf("功能列表:\n");
    printf("  [0] 关于\n");
    printf("  [1] TaskKill 方案\n");
    printf("  [2] TsKill 方案\n");
    printf("请输入功能代码: ");
    scanf("%s",&FeatureCode[0]);
    while((FeatureCode[0]!='0'&&FeatureCode[0]!='1'&&FeatureCode[0]!='2')||FeatureCode[1]!='\0'){
        printf("输入错误, 请重新输入: ");
        scanf("%s",&FeatureCode[0]);
    }
    if(FeatureCode[0]=='1'||FeatureCode[0]=='2'){
        printf("请输入 \"休眠\" 的时间 (\"休眠\" 在每次执行命令后暂停一定时间, 单位 秒, 最大 30, 不可为负数): ");
        scanf("%lf",&SleepTime);
        while(SleepTime>30.0||SleepTime<0.0){
            if(InputWrongNumber>=5){
                printf("\n########################################\n");
                printf("[Error] 输入次数过多, 程序将退出...\n");
                printf("\n########################################\n\n");
                system("pause");
                exit(0);
            }
            printf("输入错误, 请按照上面的要求输入: ");
            scanf("%lf",&SleepTime);
            InputWrongNumber++;
        }
    }
    system("set path=%path%;C:\\Windows\\System32\\");//临时设置环境变量 (解决部分情况下 cmd 指令无法使用的问题)
    printf("\n########################################\n\n");
    switch(FeatureCode[0]){
        case '0':{
            About();
            break;
        }case '1':{
            Mode1(SleepTime);
            break;
        }case '2':{
            Mode2(SleepTime);
            break;
        }
    }
    printf("\n########################################\n\n");
    system("pause");
    return 0;
}
int main(void){
    system("title 机房控制软件克星 (CRCSN)");//修改窗口名称为 "机房控制软件克星 (CRCSN)"
    system("color b");//调整控制台字体颜色为青蓝色
    PermissionDetection();//调用权限检测函数
    Start(0,0);
    return 0;
}
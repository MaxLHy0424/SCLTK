#include<iostream>
#include<string>
#include<ShlObj.h>
std::string FeatureMode{0};
void PermissionDetection(){
    BOOL RunAsAdmin{IsUserAnAdmin()};
    if(!RunAsAdmin){
        printf("[Error] 权限不足, 请以管理员权限运行...\n");
        printf("\n########################################\n\n");
        system("pause");
        exit(0);
    }
}
void OptionScreen(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    printf("欢迎使用 机房控制软件克星 !\n\n");
    printf("功能列表:\n");
    printf("   [0] 关于\n");
    printf("   [1] TaskKill 方案\n");
    printf("   [2] TsKill 方案\n");
    printf("请输入功能代码: ");
    std::cin>>FeatureMode;
    while(FeatureMode!="0"&&FeatureMode!="1"&&FeatureMode!="2"){
        printf("输入错误, 请重新输入: ");
        std::cin>>FeatureMode;
    }
}
void About(){
    printf("软件名称: 机房控制软件克星 (英文名 Computer Room Control Software Nemesis, 简称 CRCSN)\n");
    printf("软件版本: v2.0_Stable\n");
    printf("软件项目仓库: https://github.com/MaxLHy0424/Computer-Room-Control-Software-Nemesis\n");
    printf("作者: MaxLHy0424\n");
    printf("作者 B 站账号 UID: 1678066522\n");
    printf("作者反馈邮箱: MaxLHy974413@outlook.com / 1097268127@qq.com\n\n");
    printf("(C) Copyright 2023-2024, 保留所有权利.\n");
}
void Mode1(){
    printf("您选择了 \"TaskKill 方案\".\n");
    printf("提示: 已尝试临时修复环境变量, 如无法使用则是管理员修改了注册表, 暂无方案解决此问题.\n");
    printf("按任意键将清空以上内容并继续!\n\n");
    system("pause");
    for(;;){
        system("cls");
        system("sc config tvnserver start= Disabled");
        system("net stop tvnserver");
        system("TaskKill /F /T /IM PortControl64.exe");
        system("TaskKill /F /T /IM DesktopCheck.exe");
        system("TaskKill /F /T /IM DeploymentAgent.exe");
        system("TaskKill /F /T /IM XYNTService.exe");
        system("TaskKill /F /T /IM StudentMain.exe");
    }
}
void Mode2(){
    printf("您选择了 \"TsKill 方案\".\n");
    printf("提示 1: 此方案推荐用于 \"TaskKill 方案\" 无法使用时的备选方案.\n");
    printf("提示 2: 已尝试临时修复环境变量, 如无法使用则是管理员修改了注册表, 暂无方案解决此问题.\n");
    printf("按任意键将清空以上内容并继续!\n\n");
    system("pause");
    for(;;){
        system("cls");
        system("sc config tvnserver start= Disabled");
        system("net stop tvnserver");
        system("TsKill PortControl64 /A /V");
        system("TsKill DesktopCheck /A /V");
        system("TsKill DeploymentAgent /A /V");
        system("TsKill XYNTService /A /V");
        system("TsKill StudentMain /A /V");
    }
}
int main(){
    system("color b");
    PermissionDetection();
    OptionScreen();
    system("set path=%path%;C:\\Windows\\System32\\");
    printf("\n########################################\n\n");
    if(FeatureMode=="0"){
        About();
    }else if(FeatureMode=="1"){
        Mode1();
    }else if(FeatureMode=="2"){
        Mode2();
    }
    printf("\n########################################\n\n");
    system("pause");
    return 0;
}
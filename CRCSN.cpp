/*
感谢您查看本程序的源代码!
本程序以 MIT 协议开源 (具体看仓库下 LICENSE 文件), 请在符合 MIT 协议的情况下使用, 谢谢配合!
欢迎对本程序提出意见或报告 Bugs, 这对我很重要!
*/
#include<iostream>
#include<string>
#include<ShlObj.h>//函数 IsUerAnAdmin 依赖头文件, 用于检测是否为管理员权限运行程序
using namespace std;
string FeatureCode{0};//用于存储功能代码 (全局定义纯粹是作者太懒)
void PermissionDetection(){
    BOOL RunAsAdmin{IsUserAnAdmin()};//当是管理员权限时为 True, 否则为 False
    if(!RunAsAdmin){//对变量 RunAsAdmin 进行非运算, 及将非管理员权限运行时的值 False 转为 True 以进行 if 语句内的操作
        printf("[Error] 权限不足, 请以管理员权限运行...\n");
        printf("\n########################################\n\n");
        system("pause");
        exit(0);//退出程序
    }
}
void OptionScreen(){
    ios::sync_with_stdio(false);//解除 cin 与 stdin 的绑定, 提升效率
    cin.tie(nullptr);//接触 cin 与 cout 之间的绑定, 提升效率
    printf("欢迎使用 机房控制软件克星 !\n\n");
    printf("功能列表:\n");
    printf("   [0] 关于\n");
    printf("   [1] TaskKill 方案\n");
    printf("   [2] TsKill 方案\n");
    printf("请输入功能代码: ");
    cin>>FeatureCode;
    while(FeatureCode!="0"&&FeatureCode!="1"&&FeatureCode!="2"){
        printf("输入错误, 请重新输入: ");
        cin>>FeatureCode;
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
    printf("提示: 已尝试临时修复环境变量, 如还是无法使用, 暂无解决方案.\n");
    printf("按任意键将清空以上内容并继续!\n\n");
    system("pause");
    for(;;){//无限循环, 解决存在守护进程的问题, 资源消耗并不高
        system("cls");//清空控制台屏幕
        system("sc config tvnserver start= Disabled");//禁用 "联想云教室" 的主要服务
        system("net stop tvnserver");//停止 "联想云教室" 的主要服务
        system("TaskKill /F /T /IM PortControl64.exe");//结束 "联想云教室" 的相关进程
        system("TaskKill /F /T /IM DesktopCheck.exe");//结束 "联想云教室" 的相关进程
        system("TaskKill /F /T /IM DeploymentAgent.exe");//结束 "联想云教室" 的相关进程
        system("TaskKill /F /T /IM XYNTService.exe");//结束 "联想云教室" 的相关进程
        system("TaskKill /F /T /IM StudentMain.exe");//结束 "极域电子教室" 的进程
    }
}
void Mode2(){
    printf("您选择了 \"TsKill 方案\".\n");
    printf("提示 1: 此方案未经测试, 推荐用于 \"TaskKill 方案\" 无法使用时的备选方案.\n");
    printf("提示 2: 已尝试临时修复环境变量, 如还是无法使用, 暂无解决方案.\n");
    printf("按任意键将清空以上内容并继续!\n\n");
    system("pause");
    for(;;){//无限循环, 解决存在守护进程的问题, 资源消耗并不高
        system("cls");//清空控制台屏幕
        system("sc config tvnserver start= Disabled");//禁用 "联想云教室" 的主要服务
        system("net stop tvnserver");//停止 "联想云教室" 的主要服务
        system("TsKill PortControl64 /A /V");//结束 "联想云教室" 的相关进程
        system("TsKill DesktopCheck /A /V");//结束 "联想云教室" 的相关进程
        system("TsKill DeploymentAgent /A /V");//结束 "联想云教室" 的相关进程
        system("TsKill XYNTService /A /V");//结束 "联想云教室" 的相关进程
        system("TsKill StudentMain /A /V");//结束 "极域电子教室" 的进程
    }
}
int main(){
    system("color b");//调整控制台字体颜色为青蓝色
    PermissionDetection();
    OptionScreen();
    system("set path=%path%;C:\\Windows\\System32\\");//临时设置环境变量 (解决部分情况下 cmd 指令无法使用的问题)
    printf("\n########################################\n\n");
    if(FeatureCode=="0"){
        About();
    }else if(FeatureCode=="1"){
        Mode1();
    }else if(FeatureCode=="2"){
        Mode2();
    }
    printf("\n########################################\n\n");
    system("pause");
    return 0;
}
#include<iostream>
#include<string>
#include<ShlObj.h>
using namespace std;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    BOOL RunAsAdmin = IsUserAnAdmin();
    if(!RunAsAdmin){
        printf("请以管理员权限运行...\n");
        printf("########################################\n");
        system("pause");
        exit(0);
    }
    printf("欢迎使用 Computer Room Control Software Nemesis (机房控制软件克星)!\n\n");
    printf("  当前版本: v1.0_Dev3\n\n");
    printf("     0. 访问 GitHub 仓库页面\n");
    printf("     1. 普通模式\n");
    printf("     2. 强力模式\n\n");
    printf("模式说明: \n  %c普通模式%c 仅会强制关闭 %c极域电子教室%c 的相关进程, 可能存在重新开启的情况, 但只会执行一次, 资源占用较少.\n  %c强力模式%c 会重复结束 %c极域电子教室%c 和 %c联想智能云教室%c 的相关进程, 效果较强, 但资源消耗较大, 推荐联想机型选择此模式.\n\n",34,34,34,34,34,34,34,34,34,34);
    string FeatureMode{0};
    printf("请输入模式代码: ");
    cin>>FeatureMode;
    while(FeatureMode!="0"&&FeatureMode!="1"&&FeatureMode!="2"){
        printf("输入错误, 请重新输入: ");
        cin>>FeatureMode;
    }
    printf("########################################\n");
    if(FeatureMode=="0"){
        printf("感谢!\n");
        system("start https://github.com/MaxLHy0424/Computer-Room-Control-Software-Nemesis");
    }else if(FeatureMode=="1"){
        printf("您选择了 %c普通模式%c.\n",34,34);
        system("taskkill /F /IM StudentMain.exe");
    }else if(FeatureMode=="2"){
        printf("您选择了 %c强力模式%c.\n",34,34);
        printf("如发现资源占用过高, 出现卡顿, 请改用 %c普通模式%c\n",34,34);
        printf("继续将会清空以上所有内容. 如出现文字闪烁, 属于正常现象.\n");
        system("pause");
        for(;;){
            system("cls");
            system("taskkill /F /IM StudentMain.exe");//还未完成，等待机房实机测试
        }
    }
    printf("########################################\n");
    system("pause");
    return 0;
}
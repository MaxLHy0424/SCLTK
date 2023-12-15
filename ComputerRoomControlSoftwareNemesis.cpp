#include<stdio.h>
#include<iostream>
int main(){
    printf("欢迎使用 Computer Room Control Software Nemesis (机房控制软件克星)!\n\n");
    printf("模式列表:\n");
    printf("  1. 普通模式\n");
    printf("  2. 强力模式\n\n");
    printf("模式说明: \n  %c普通模式%c 仅会强制关闭 %c极域电子教室%c 的相关进程, 可能存在重新开启的情况, 但只会执行一次, 资源占用较少.\n  %c强力模式%c 会重复结束 %c极域电子教室%c 和 %c联想智能云教室%c 的相关进程, 效果较强, 但资源消耗较大, 推荐联想机型选择此模式.\n\n",34,34,34,34,34,34,34,34,34,34);
    unsigned short FeatureMode{0},InputError{0};
    scanf("%hu",&FeatureMode);
    while(FeatureMode!=1&&FeatureMode!=2){
        if(InputError>=5){
            printf("输入错误次数过多, 程序退出.\n");
            goto ExitProgram;
        }
        printf("输入错误, 请重新输入: ");
        scanf("%hu",&FeatureMode);
    }
    switch(FeatureMode){
        case 1:{
            printf("\n您选择了 %c普通模式%c.\n",34,34);
            system("taskkill /F /IM StudentMain.exe");
            printf("执行成功, 如出现复发请使用 %c强力模式%c",34,34);
            break;
        }
        case 2:{
            printf("\n您选择了 %c强力模式%c.\n",34,34);
            printf("如发现资源占用过高, 出现卡顿, 请改用 %c普通模式%c\n",34,34);
            printf("继续将产生大量输出, 属于正常现象.\n");
            system("pause");
            for(;;){
                system("taskkill /F /IM StudentMain.exe");//还未完成，等待机房实机测试
            }
            break;
        }
    }
ExitProgram:
    system("pause");
    return 0;
}
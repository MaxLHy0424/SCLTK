# 简介

你是否因为无法脱离机房老师的控制, 而无法欢快使用机房的电脑?

这个软件可以帮到你!

Computer Room Control Software Nemesis (中文名 机房控制软件克星 , 简称 CRCSN), 可以帮助你通过简单的操作快速 (间接) 脱离控制!

# 技术细节

其原理是通过结束进程和禁止服务的方式破解控制.

具体实现方式:

使用 ``ShlObj.h`` 头文件中的 system 函数运行 CMD 命令.

主要是用了以下的命令:
````batch
sc config tvnserver start= Disabled
sc config TDNetFilter start= Disabled
sc config TDFileFilter start= Disabled
net stop tvnserver
net stop TDNetFilter
net stop TDFileFilter
TaskKill /F /T /IM PortControl64.exe
TaskKill /F /T /IM DesktopCheck.exe
TaskKill /F /T /IM DeploymentAgent.exe
TaskKill /F /T /IM XYNTService.exe
TaskKill /F /T /IM StudentMain.exe
TaskKill /F /T /IM GATESRV.exe
TaskKill /F /T /IM MasterHelper.exe
````
运行了以上几个命令后, 基本可以解决大部分的控制了.

"恢复模式" 的实现方法则是将上面的三个服务启用, 达到恢复的效果.

另外, 比较细心的话会发现以 User 权限运行, 会退出程序. 原理是使用了头文件 ``ShlObj.h`` 中的 ``IsUserAnAdmin()`` 函数, 赋值给一个叫 ``RunAsAdmin`` 的变量. 接着对这个变量进行判断, 如果为 0, 则返回退出程序的消息, 否则将继续运行程序.

还有破解时的 "休眠" 功能, 对应头文件 ``unistd.h`` 的 ``sleep`` 函数, 将 "休眠" 的时间存储在双精度浮点类型变量 ``SleepTimes`` 中, 然后在程序中使用 ``sleep(SleepTimes);`` 实现 "休眠".

# 注意事项

此软件仅可用于 Windows 平台.

此软件仅用于学习交流, 禁止用于商业用途.

此软件按照 MIT 协议开源, 请在遵守 MIT 协议的情况下使用.

如果此软件对计算机造成影响 (包括但不限于: 卡顿, 死机), 请勿使用.

使用时请以管理员身份运行, 否则可能出现异常. (v1.0_Dev3 及之后的版本已添加管理员身份的识别, v1.0_Dev1 和 v1.0_Dev2 请手动使用管理员权限运行)

# 常见问题

Q1: 打开程序时提示 "命令提示符已被管理员禁用" 之类的提示, 如何解决?

A1: 这说明 命令提示符 已被禁用. 可以试试打开注册表编辑器, 定位到 ``HKEY_CURRENT_USER\Software\Policies\Microsoft\Windows\System`` 下, 看看有没有一个叫 ``DisableCMD`` 的 DWORD 值, 有的话就删掉. 再次打开软件看看是否能成功. (如果还是打不开, 那我也不知道了 awa)

Q2: 程序无法在 32 位 Windows 系统中打开, 如何解决?

A2: 程序发布版本暂无 32 位版本. 可以先下载程序源代码, 然后删除以下代码:
````c++
    {
        BOOL RunAsAdmin{IsUserAnAdmin()};
        if(!RunAsAdmin){
            printf("[Error] 当前权限为 User, 请以 Administrator 权限运行本程序...\n");
            printf("\n########################################\n\n");
            printf("按任意键退出程序!\n\n");
            system("pause");
            exit(0);
        }
    }
````
然后找到 ``#include<ShlObj.h>`` , 改为 ``#include<stdlib.h>``, 保存文件. 接着使用 MinGW-w32 编译源代码文件, 正常使用即可. 

# 新闻
1. CRCSN 会在本年推出特别版本, 比如愚人节特别版.
2. CRCSN v2.x 版本将在几个小版本后趋于稳定. 接下来的 v3.x 更新内容暂未确定, 欢迎大家发送 E-mail 至 MaxLHy974413@outlook.com 提交你的想法.

# 鸣谢

支持本项目的所有网友, 所有测试人员, 以及我的老师.

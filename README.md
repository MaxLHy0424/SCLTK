![logo](logo.png)\
**Computer Room Controlling Software Nemesis: 轻松破解机房控制.**\
*©️ 2024 MaxLHy0424, 保留所有权利.*

# 使用方式

**CRCSN 涉及到修改注册表等操作, 使用时建议关闭防病毒软件. 如果 CRCSN 被报为恶意软件, 请添加排除项.**

> 适用于 v4.8.0-2 及以上版本.

## 0 启动

一般情况下, 运行`launcher.cmd`即可启动 CRCSN.

如果默认的启动命令不满足您的需求, 可以根据下文修改.

### 0-1 以管理员权运行

找到这一行:

````Cmd
: MsHta Vbscript:CreateObject("Shell.Application").ShellExecute("%~S0","goto :RunAs","","RunAs",1)(Window.Close)&Goto:EOF
````

将开头的`: `去掉即可. 再运行 CRCSN, 将会强制以管理员权限运行. 

如需取消, 将`: `加回去头即可.

### 0-2 修改控制台颜色

找到最后一行, 编辑`-clr=`后面的字符, 例如:

````Cmd
.\bin\main.exe -clr=F0 其他选项
````

具体参数详见`Color`命令帮助.

### 0-3 启用/禁用实验性功能

找到最后一行. 如需启用, 请在末尾加上`-beta`, 修改后见下:

````Cmd
.\bin\main.exe -beta 其他选项
````

如需禁用, 将`-beta`删去即可.

当前的实验性功能如下:

- 窗口置顶

## 1 破解

输入`1`, 回车, 根据需要破解的控制软件输入对应的字符, 回车, 输入`Y`, 回车, 开始破解.

## 2 恢复

输入`2`, 回车, 根据需要恢复的控制软件输入对应的字符, 回车, 输入`Y`, 回车, 开始恢复.

# 注意事项

1. 软件优先适配 Windows 8.x/10/11, Windows 7 及以下的 Windows 操作系统的适配可能会延后.
2. 请勿在执行操作时关闭窗口, 否则可能影响效果.
3. 请在遵守 MIT 许可证的情况下使用软件.

# 常见问题

## 1 运行软件时控制台总是输出 "命令提示符已被管理员禁用", 无法正常使用.

可以运行注册表编辑器, 定位到`HKEY_CURRENT_USER\Software\Policies\Microsoft\Windows\System`下, 看看有没有一个叫`DisableCMD`的值, 有的话就删掉, 再试试.

## 2 软件无法在 32 位 Windows 系统中使用.

软件暂无计划发布 32 位版本. 如有需要, 请使用 MinGW 自行编译.

## 3 破解后一些软件运行时提示 "找不到文件" 之类的错误, 而运行的软件并没有损坏.

在不影响软件正常运行的情况下, 可以给软件文件修改一个名称, 再试试. 或者打开注册表编辑器, 定位到`HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\`, 找到和要运行的软件的文件名称相同的项, 删除即可.

# 鸣谢

所有的测试人员, 所有提出建议的网友, 以及我的老师.
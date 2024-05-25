![logo](logo.png)\
**Computer Room Controlling Software Nemesis: 轻松破解机房控制.**\
*©️ 2023- MaxLHy0424, 保留所有权利.*

[下载最新发行版 (v4.8.14).](https://github.com/MaxLHy0424/CRCSN/releases/download/v4.8.14/CRCSN_v4-8-14_x64.7z)

# 使用方式

**CRCSN 涉及到修改注册表等操作, 使用时建议关闭防病毒软件. 若 CRCSN 被报为恶意软件, 请添加排除项.**

> 仅为最新正式发行版编写, 其他版本可能有所不同.

## 0 启动

一般情况下, 运行`launcher.cmd`即可启动 CRCSN.

如果默认的启动命令不满足您的需求, 可以根据下文修改.

### 0-1 以管理员权运行

找到:
````Cmd
: SetLocal EnableDelayedExpansion
: %1 %2
: MsHta VbScript:CreateObject("Shell.Application").ShellExecute("%~S0","GoTo:RunAs","","RunAs",1)(Window.Close) && GoTo:EOF
````

将每行开头的`: `去掉即可. 再运行 CRCSN, 将会强制以管理员权限运行. 

如需禁用, 将`: `加回去头即可.

### 0-2 修改控制台颜色

找到最后一行, 在末尾加上`-clr=`, 并在后面添加参数 (详见`Color`命令帮助), 例如:
````Cmd
.\bin\main.exe -clr=9
````

### 0-3 窗口置顶

找到最后一行. 如需启用, 请在末尾加上`-fs`, 见下:
````Cmd
.\bin\main.exe -fs
````

如需禁用, 将`-fs`删去即可.

## 1 破解

输入`1`, 回车, 根据需要破解的控制软件输入对应的字符, 回车, 输入`Y`, 回车, 开始破解.

## 2 恢复

输入`2`, 回车, 根据需要恢复的控制软件输入对应的字符, 回车, 输入`Y`, 回车, 开始恢复.

# 常见问题

## 1 运行软件时控制台总是输出 "命令提示符已被管理员禁用", 无法正常使用.

可以运行注册表编辑器, 定位到`HKEY_CURRENT_USER\Software\Policies\Microsoft\Windows\System`下, 看看有没有一个叫`DisableCMD`的值, 有的话就删掉, 再试试.

## 2 软件无法在 32 位 Windows 系统中使用.

软件暂无计划发布 32 位版本. 如有需要, 请使用 MinGW 自行编译.

## 3 破解后一些软件运行时提示 "找不到文件" 之类的错误, 而运行的软件并没有损坏.

在不影响软件正常运行的情况下, 可以给软件文件修改一个名称, 再试试. 或者打开注册表编辑器, 定位到`HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\`, 找到和要运行的软件的文件名称相同的项, 删除即可.

# 贡献代码

**CRCSN 是个开源项目, 欢迎各位贡献代码!** 所有贡献者将会在末尾列出, 以示感谢. 但请务必遵守以下规则:
1. 提交代码请创建 PR, 不要直接提交;
2. 修改`README.md`时, 注意语句通顺, 不要有错误内容;
3. 修改软件代码时, 标识符名称一律遵守驼峰命名法, 代码风格与项目保持统一, 尽可能提升运行效率, 同时可通过 MinGW-w64 编译, 而不附带警告.

# 贡献者

- [MaxLHy0424](https://github.com/MaxLHy0424)
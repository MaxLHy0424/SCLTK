![logo](logo.png)\
**Computer Room Control Software Nemesis: 轻松破解机房控制.**\
©️ 2024 MaxLHy0424, All Rights Reserved.

<!-- [下载最新发行版 (v5.0.0).](https://github.com/MaxLHy0424/CRCSN/releases/download/v5.0.0/CRCSN_v5-0-0_x64.7z) -->

# 使用方法

**CRCSN 涉及到修改注册表等操作, 使用时建议关闭防病毒软件. 若 CRCSN 被报为恶意软件, 请添加排除项.**

> 仅为最新发行版编写, 其他版本可能有所不同.

## 0 启动

一般情况下, 运行`launcher.cmd`即可启动 CRCSN.

如果默认的启动方式不满足您的需求, 可以根据下文修改.

### 0.1 强制以管理员限运行

找到:
```Batch
: msHta vbScript:createObject("shell.application").shellExecute("%~s0","goTo:runAs","","runAs",1)(window.close) & goTo:EOF
```

将开头的`: `去掉即可. 再运行启动脚本, 将会强制以管理员限运行. 如需禁用, 将`: `加回去头即可.

### 0.2 修改软件编译版本

找到:
```Batch
Set bin = "msvcrt64"
```

软件编译版本如下:
 - `msvcrt64`(默认): 使用 MSYS2 的`mingw-w64-x86_64-toolchain`编译, Runtime 已过时, 支持大部分 Windows 操作系统.
 - `ucrt64`(推荐): 使用 MSYS2 的`mingw-w64-ucrt-x86_64-toolchain`编译, Runtime 受支持, 支持 Windows 10 以上的 Windows 操作系统.

根据上述内容, 选择需要版本的对应字符串填入引号内即可.

### 0.3 窗口操作

> 此项参数区分大小写.

窗口操作的主参数为`-W`, 附加参数如下:
 - `c`: 启用窗口控件, 允许缩放窗口 (可能导致意外行为);
 - `a`: 将窗口不透明度设为 80%;
 - `p`: 置顶窗口并每间隔 100ms 将窗口设为焦点.

附加参数的顺序不影响应用效果.

使用示例:
```Batch
.\bin\main.exe -Wc
```
```Batch
.\bin\main.exe -Wbp
```
```Batch
.\bin\main.exe -Wpcb
```

## 1 破解

找到`[  破  解  ]`一栏, 选择需要破解的控制软件, 点击即可.

## 2 恢复

找到`[  恢  复  ]`一栏, 选择需要恢复的控制软件, 点击即可.

# 常见问题

## 1 运行软件时控制台总是输出 "命令提示符已被管理员禁用", 无法正常使用.

可以运行注册表编辑器, 定位到`HKEY_CURRENT_USER\Software\Policies\Microsoft\Windows\System`下, 看看有没有一个叫`DisableCMD`的值, 有的话就删掉, 再试试.

## 2 软件无法在 x86 Windows 系统中使用.

受限于开发环境, 编译的二进制文件仅可是 64 位.

## 3 破解后一些软件运行时提示 "找不到文件" 之类的错误, 而运行的软件并没有损坏.

在不影响软件正常运行的情况下, 可以给软件文件修改一个名称, 再试试. 或者打开注册表编辑器, 定位到`HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\`, 找到和要运行的软件的文件名称相同的项, 删除即可.

# 贡献代码

**欢迎各位贡献代码!** 所有贡献者将会在下方列出, 以示感谢. 但务必遵守以下规则:
- 提交代码请创建拉取请求, 不要直接提交;
- 修改`README.md`时, 注意语句通顺, 不要有错误内容;
- 提交的代码的风格请与项目保持统一, 尽可能提升运行效率, 同时可通过最新 MinGW-w64 GNU GCC (MSYS2: mingw-w64-ucrt-x86_64-toolchain && mingw-w64-x86_64-toolchain) 编译, 并且不附带警告.

## 贡献者

- [MaxLHy0424 (Mingxu Ye)](https://github.com/MaxLHy0424)
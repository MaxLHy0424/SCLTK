![logo](logo.png)\
**Computer Room Control Software Nemesis: 轻松破解机房控制.**\
©️ 2023 MaxLHy0424. All Rights Reserved.

# 下载

- [GitHub Release (v5.0.0, 即将推出)](https://github.com/MaxLHy0424/CRCSN/releases/download/v5.0.0/CRCSN-v5.0.0.7z)
- [GitHub Release (v4.10.7)](https://github.com/MaxLHy0424/CRCSN/releases/download/v4.10.7/CRCSN_v4-10-7_x64.7z)
- [中国大陆镜像站点](https://www.123pan.com/s/UzthTd-MkTRh.html)

# 使用方法

**CRCSN 涉及到修改注册表等操作, 使用时建议关闭防病毒软件. 若 CRCSN 被报为恶意软件, 请添加排除项.**

> 仅适用于 CRCSN 24w32b+.

## 1 启动

一般情况下, 运行`launcher.cmd`即可启动 CRCSN.

如果默认的启动方式不满足您的需求, 可以根据下文修改.

### 1.1 修改软件运行时

找到:
```batch
set runtime=msvcrt
```

软件支持的 Runtime 如下:
 - `msvcrt`(默认): 使用 *MSYS2* `mingw-w64-x86_64-toolchain`编译, Runtime 为老旧的 *Microsoft Visual C Runtime*, 支持大部分 Windows OS.
 - `ucrt`(推荐): 使用 *MSYS2* `mingw-w64-ucrt-x86_64-toolchain`编译,  Runtime 为新式的 *Universal C Runtime*, 支持 Windows 10 以上的 Windows OS (部分 Windows OS 在安装最新补丁后支持运行).

根据上述内容, 选择需要版本的对应字符串替换即可.

### 1.2 窗口操作

> 此项参数区分大小写.

窗口操作的主参数为`-W`, 附加参数如下:
 - `f`: 置顶窗口并每间隔 100ms 将窗口设为焦点.
 - `c`: 启用窗口控件, 允许缩放窗口 (可能导致意外行为);
 - `a`: 将窗口不透明度设为 80% (仅支持使用新版控制台的 Windows OS, 例如 Windows 10);

附加参数的顺序不影响应用效果.

使用示例:
```batch
.\bin\%branch%-%arch%-%runtime%.exe -Wa
```
```batch
.\bin\%branch%-%arch%-%runtime%.exe -Wfac
```

## 1.3 自定义启动脚本

软件启动脚本属于 Windows 命令脚本, 可以根据需要修改启动脚本.

## 2 功能使用及说明

### 2.1 软件信息

位于软件窗口顶部往下几行, 从上到下, 依次显示: 软件名称, 软件版本, 仓库网址, 版权信息.

### 2.2 快捷操作

位于 "软件信息" 下方. 通过鼠标点击按钮`> 退出`可以直接退出软件, 点击`> 命令提示符`可以在当前软件窗口内打开命令提示符 (可以使用`exit`命令退出命令提示符).

### 2.3 破解 & 恢复

位于按钮`> 命令提示符`下方. 如果`[ 破 解 ]`和`[ 恢 复 ]`下输出的文本为`(i) 需要管理员权限.`, 请以管理员权限重新启动软件.

确认以管理员权限启动后, 使用鼠标在`[ 破 解 ]`下点击需要破解的控制软件, 以禁用破解. 如需恢复, 可以在`[ 恢 复 ]`下点击需要恢复的控制软件, 即可恢复控制.

# 常见问题

## 1 运行软件时控制台总是输出 "命令提示符已被管理员禁用", 无法正常使用.

可以运行注册表编辑器, 定位到`HKEY_CURRENT_USER\Software\Policies\Microsoft\Windows\System`下, 看看有没有一个叫`DisableCMD`的值, 有的话就删掉, 再试试.

## 2 软件无法在 x86 或 arm64 架构的 Windows OS 中使用.

受限于开发工具链, 编译的二进制文件架构仅支持 x86_64 架构.

## 3 破解后一些软件运行时提示 "找不到文件" 之类的错误, 而运行的软件并没有损坏.

在不影响软件正常运行的情况下, 可以给软件文件修改一个名称, 再试试. 或者打开注册表编辑器, 定位到`HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\`, 找到和要运行的软件的文件名称相同的项, 删除即可.

# 鸣谢

- Bilibili 用户[lateworker_晚工](https://space.bilibili.com/39337803)提供 Console UI 代码 (详见[此处](https://www.bilibili.com/video/BV1X14y1n7S4/), 软件有修改之处);
- Bilibili 用户[痕继痕迹](https://space.bilibili.com/39337803)指导软件 UI 设计.
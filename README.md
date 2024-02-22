
![logo](logo.png)

**机房控制软件克星 (Computer Room Control Software Nemesis): 轻松脱离机房控制.**

*By MaxLHy0424.*

# 使用方式

> 更新于 2024/02/15.\
> 适用于 v3.0.0 及以上版本.

建议先关闭操作系统上已安装的所有反病毒软件, 以免 CRCSN 的操作被拦截.

## 破解

以管理员权限运行软件, 依次输入 `1 1 Y`, 开始破解. 等到软件提示返回主时即代表完成破解.

## 恢复

以管理员权限运行软件, 依次输入 `2 Y`, 开始恢复.

接着根据提示, 手动打开列出的控制软件的安装目录, 分别找到每个可执行文件, 分别以管理员权限运行.

## * 编辑配置 (适用于 v3.1.0_Dev9 及以上版本)

> 该功能正在开发, 目前仅存在于预发布版.

打开软件目录下的 `config.ini` (推荐使用 Windows Notepad 或者 Visual Studio Code), 根据文件末尾的提示修改需要的选项. 

接下来打开软件查看效果, 如果此时软件已经开启, 需要输入 `0` 来重新载入配置.

> [注意] 如果配置存在错误, 软件也不会进行提示. 如发现配置效果不符合预期, 请仔细检查配置文件是否正确.

# 注意事项

软件仅为 Windows 8.x / 10 / 11 提供适配支持, Windows 7 及以下的 Windows 操作系统可使用软件, 但不会专门适配.

软件按照 MIT 协议开源, 请在遵守 MIT 协议的情况下使用.

# 常见问题

## 1. GitHub 在中国大陆地区难以访问, 获取软件非常不方便.

目前已经在国内云盘上传最新版本, 可以从[这里](https://www.123pan.com/s/HmR8jv-tZLN.html "点击跳转")下载.

## 2. 运行软件时控制台界面总是输出 "命令提示符已被管理员禁用" 之类的提示, 无法正常使用.

这说明 CMD 已被禁用. 可以试试打开注册表编辑器, 定位到 `HKEY_CURRENT_USER\Software\Policies\Microsoft\Windows\System` 下, 看看有没有一个叫 `DisableCMD` 的 DWORD 值, 有的话就删掉. 再次打开软件试试.

## 3. 软件无法在 32 位 Windows 系统中使用.

软件无计划发布 32 位版本. 可以克隆软件仓库, 使用 MinGW-w32 编译源代码文件, 正常使用即可.

## 4. 破解后一些软件打开提示 "找不到文件" 之类的错误, 并且打开的软件本身并没有损坏.

CRCSN v3.0.0 添加了通过注册表劫持控制软件, 间接达到禁止启动控制软件. 但弊端是和控制软件的文件名一样的软件文件也无法打开了. 在不影响软件正常运行的情况下, 可以试试给软件文件修改一个名称, 再打开试试. 实在不行就打开注册表编辑器, 定位到 `HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\` 下, 找到和要运行的软件的文件名称相同的项, 删除项即可.

## 5. 软件目录下存在 `config.ini`, 但软件提示无法读取.

请检查 `config.ini` 的属性 (右键 `config.ini` -> 属性 -> 安全), 点击 "编辑", 依次点击上方列出的所有用户, 将下方的 "允许" 一栏全部勾选, "拒绝" 一栏全部取消勾选 (每栏的最后一个选项 "特殊权限" 可能无法 (取消) 勾选, 但并不影响), 再次重载配置试试.

> [注意] 如果此条目的解决方案可行, 请检查是否有人动过您的电脑, 也可能是恶意软件导致. 如为恶意软件导致, 建议使用反病毒软件扫描整个硬盘 (包括您的移动存储设备).

## 6. 使用 msys2 安装的 gcc 编译 `main.cpp` 出的文件在其他 Windows 操作系统上报错缺少 DLL 文件.

请从 GitHub 下载本仓库的压缩包文件, 解压, 将 `DLL` 文件夹下的文件复制进 CRCSN 主程序 (`main.exe`) 所在的文件夹, 具体文件树如下:

> $ ...\ \
> |----`config.ini`\
> |----`libgcc_s_seh-1.dll`\
> |----`libstdc++-6.dll`\
> |----`libwinpthread-1.dll`\
> |----`main.exe`


# 鸣谢

所有的测试人员, 所有提出建议的网友, 以及我的老师.
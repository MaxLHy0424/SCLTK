![logo](img/title.png)\
**Computer Room Control Software Nemesis: 轻松破解机房控制.**\
©️ 2023 MaxLHy0424. All rights reserved.

# 获取

**最新发行版: v5.6.7.**\
**[更新日志](https://github.com/MaxLHy0424/CRCSN/releases/tag/v5.6.7) | [直链下载](https://github.com/MaxLHy0424/CRCSN/releases/download/v5.6.7/CRCSN.7z)**

或者:

- [GitHub Repository Release](https://github.com/MaxLHy0424/CRCSN/releases);
- [中国大陆云盘镜像站点 (不含预发行版)](https://www.123pan.com/s/UzthTd-MkTRh.html).

# 使用指南

**由于 CRCSN 存在敏感操作, 使用时建议关闭防病毒软件. 若 CRCSN 被报为恶意软件, 请添加排除项.**

> [!NOTE]
> 适用于 v5.6.7.

## 1 启动

一般情况下, 运行 `launcher.cmd` 即可启动 CRCSN.

如果默认的启动方式不满足您的需求, 可以根据下文修改.

### 1.1 修改 Runtime

找到:
```dos
set runtime=msvcrt
```

软件支持的 Runtime 如下:
- `msvcrt` (默认): 开发工具链为 *MSYS2* `mingw-w64-x86_64-toolchain`, Runtime 为老旧的 *Microsoft Visual C Runtime*, 支持大部分 Windows OS.
- `ucrt` (推荐): 开发工具链为 *MSYS2* `mingw-w64-ucrt-x86_64-toolchain`, Runtime 为新式的 *Universal C Runtime*, 支持 Windows 10 以上的 Windows OS (部分 Windows OS 在安装最新补丁后可以运行).

根据上述内容, 选择需要版本的对应字符串替换即可.

### 1.2 自动启动

启用时, 将会直接启动 CRCSN, 无需确认.

找到:
```dos
set opt.autoLaunch=0
```

将等号后面的数字改为 `0` 之外的其他数字, 即可启用. 禁用只需改回去即可.

### 1.3 详细信息

启用时, 启动脚本将会输出 CRCSN 二进制文件的详细信息.

找到:
```dos
set opt.showVerboseInfo=0
```

将等号后面的数字改为 `0` 之外的其他数字, 即可启用. 禁用只需改回去即可.

### 1.4 窗口操作

> [!IMPORTANT]
> 此参数区分大小写.

找到:
```dos
set args=
```

窗口操作的主参数为 `-W`, 附加参数如下:
- `f`: 置顶窗口并每间隔 100ms 将窗口设为焦点;
- `a`: 将窗口不透明度设为 90% (仅支持新版控制台);
- `c`: 允许缩放 / 最大化 / 最小化窗口 (可能导致意外行为).

可以有多个附加参数, 其顺序不影响应用效果, 但不可以仅使用主参数.

使用示例:
```dos
set args=-Waf
```

### 1.5 深度自定义

启动脚本的本质是 Windows 命令脚本, 可以根据需求深度自定义.

## 2 功能使用及说明


### 2.1 快捷操作

位于 "软件信息" 区域下方. 通过鼠标点击按钮 `> 退出` 可以直接退出软件, 点击 `> 信息` 查看软件信息, 点击 `> 命令提示符` 可以在当前软件窗口内打开命令提示符.

> [!TIP]
> 启动命令提示符后, 可以使用命令 `mode con cols=行数 lines=列数` 调整窗口大小. 退出命令提示符后, 如果未追加 `-Wc` 参数, 将会重置窗口属性.

### 2.3 破解 & 恢复

位于 "快捷操作" 区域下方. 如果 `[ 破 解 ]` 和 `[ 恢 复 ]` 下输出的文本为 `(i) 需要提权.`, 请以管理员权限重新启动软件.

确认以管理员权限启动后, 使用鼠标在 `[ 破 解 ]` 下点击需要破解的控制软件, 以禁用破解. 如需恢复, 可以在 `[ 恢 复 ]` 下点击需要恢复的控制软件, 即可恢复控制.

# 许可证

CRCSN 使用 [MIT License](LICENSE), 详细内容请自行阅读.

# 二次开发

> [!NOTE]
> 适用于 v5.6.7.

非常欢迎大家二次开发 CRCSN!

二次开发时, 请务必修改 `def.hpp` 和 `launcher.cmd` 中的宏定义, 以做区分.

版权声明部分, 请在遵守许可证的情况下修改. 推荐将二次开发版本开源至 GitHub, 在许可证允许的情况下, 可能会被采纳.

> [!IMPORTANT]
> 不在 GitHub 上开源的二次开发版本的修改不会被采纳.

# 常见问题

## 1 软件无法非 x86_64 架构的 Windows OS 中使用.

受限于开发工具链, 构建出的二进制文件架构仅支持 x86_64 架构.

## 2 运行软件时控制台总是输出 "命令提示符已被管理员禁用", 无法正常使用.

可以运行注册表编辑器, 定位到 `HKEY_CURRENT_USER\Software\Policies\Microsoft\Windows\System` 下, 看看有没有一个叫 `DisableCMD` 的值, 有的话就删掉, 再试试.

## 3 破解后一些软件运行时报错 "找不到文件".

在不影响软件正常运行的情况下, 可以给软件文件修改一个名称, 再试试. 或者打开注册表编辑器, 定位到 `HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options`, 找到和要运行的软件的文件名称相同的项, 删除即可.

# 鸣谢

- Bilibili 用户 [lateworker_晚工](https://space.bilibili.com/454920362) 提供界面代码 (详见[此处](https://www.bilibili.com/video/BV1X14y1n7S4), 软件有修改);
- Bilibili 用户 [痕继痕迹](https://space.bilibili.com/39337803) 指导软件界面设计;
- GitHub 用户 [Zhu-Xinrong (Kendall)](https://github.com/Zhu-Xinrong) 指导软件图标设计.

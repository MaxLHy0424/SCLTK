<div align="center">

![title](./img/title.png)\
**Student Computer Lab Toolkit**\
**轻松破解机房控制, 基于 Modern C++**

</div>

> [!CAUTION]
> SCLTK 仅适用于中国大陆地区.\
> SCLTK's service is limited to the Chinese mainland.

> [!NOTE]
> 从 v6.1.0 起, 原 CRCSN (Computer Room Control Software Nemesis) 正式更名为 SCLTK (Student Computer Lab Toolkit).

# 📦 获取


| 分支            | 最新发行版                                                                  | 状态       | 生命周期                |
| --------------- | --------------------------------------------------------------------------- | ---------- | ----------------------- |
| SCLTK (CRCSN) 1 | [v1.0_Stable](https://github.com/MaxLHy0424/SCLTK/releases/tag/v1.0_Stable) | ❌ 停止维护 | 2023/12/17 - 2023/12/18 |
| SCLTK (CRCSN) 2 | [v2.5_Stable](https://github.com/MaxLHy0424/SCLTK/releases/tag/v2.5_Stable) | ❌ 停止维护 | 2024/01/01 - 2024/01/07 |
| SCLTK (CRCSN) 3 | [v3.0.1 \(30174\)](https://github.com/MaxLHy0424/SCLTK/releases/tag/30174)  | ❌ 停止维护 | 2024/02/09 - 2024/02/10 |
| SCLTK (CRCSN) 4 | [v4.10.7](https://github.com/MaxLHy0424/SCLTK/releases/tag/v4.10.7)         | ❌ 停止维护 | 2024/03/26 - 2024/06/19 |
| SCLTK (CRCSN) 5 | [v5.11.1](https://github.com/MaxLHy0424/SCLTK/releases/tag/v5.11.1)         | ❌ 停止维护 | 2024/08/25 - 2024/12/23 |
| SCLTK (CRCSN) 6 | [v6.3.3](https://github.com/MaxLHy0424/SCLTK/releases/tag/v6.3.3)           | ✅ 基线开发 | 2025/01/29 - present    |

# 📖 使用指南

> [!NOTE]
> 适用于当前分支的最新版本.

## 0 开始之前

**使用 SCLTK 时建议关闭防病毒软件. 若 SCLTK 被报为恶意软件, 请在防病毒软件中添加排除项.**

**请勿使用 SCLTK 扰乱课堂纪律, 造成的后果与开发者无关.**

## 1 启动

SCLTK 发行版文件名规则为 `SCLTK-<arch>-<runtime>.exe`, 其中 `<arch>` 一般为 `x86_64` 或 `i686`, `<runtime>` 一般为 `ucrt` 或 `msvcrt`.

`<arch>` 和 `<runtime>` 的具体信息如下:

- **`x86_64` & `ucrt` \(推荐\)**\
  开发工具链为 msys2 `mingw-w64-ucrt-x86_64-toolchain`, 64 位可执行文件, 运行时库为 Universal C Runtime \(UCRT\), 支持 Windows 10 以上的 Windows OS \(部分 Windows OS 在安装特定更新后可以运行\).
- **`i686` & `msvcrt`**\
  开发工具链为 msys2 `mingw-w64-i686-toolchain`, 32 位可执行文件, 运行时库为 Microsoft Visual C Runtime \(MSVCRT\), 支持大部分 Windows OS.

## 2 常规操作

- **退出 SCLTK**\
  `< 退出`
- **重新启动 SCLTK**\
  `< 重启`
- **查看 SCLTK 信息**\
  `> 关于`

## 3 配置 SCLTK

> [!NOTE]
> 配置文件 `config.ini` 中以 `#` 开头的行为注释.\
> 配置文件头部注释为 SCLTK 自动生成.

- **进入配置编辑页面**\
  `> 配置`

> [!NOTE]
> 配置解析规则如下:
> - 各个配置分类在配置文件中由不同标签区分, 标签的一般格式为 `[<标签名>]`, 标签名间与中括号见可以有任意数量的空格.
> - 如果匹配不到配置分类, 则当前读取到的标签到下一个标签的前一行都将被忽略.
> - 忽略每行末尾的空白字符, 包括空格, 换页符 (转义字符 0x0c `\f`), 横向制表符 (转义字符 0x09 `\t`), 纵向制表符 (转义字符 0x0b `\v`) 等.
> - 不会忽略每行的前导空白字符.

### 3.0 配置操作

- **暂存配置并返回**\
  `< 返回`
- **同步已保存的配置和暂存的配置**\
  `> 同步配置`
- **使用默认文本编辑器打开配置文件**\
  `> 打开配置文件`

### 3.1 选项

所有选项可在配置页面中修改. 具体描述见下文.

#### 3.1.1 选项分类: 破解与恢复

破解\/恢复控制软件相关选项.

- **劫持可执行文件**\
  破解时劫持控制软件的可执行文件, 恢复时撤销劫持.
- **设置服务启动类型**\
  破解时禁用控制软件的服务, 恢复时重新启用.
- **快速模式**\
  禁用详细信息输出, 启用多线程执行.

#### 3.1.2 选项分类: 窗口显示

SCLTK 窗口相关选项.

- **\* 置顶窗口**\
  每 100ms 强制显示窗口并设为置顶.
- **\* 极简标题栏**\
  禁用窗口上下文菜单, 隐藏所有窗口控件, 隐藏图标.
- **\* 半透明**\
  将窗口不透明度设为 90%.

#### 3.1.3 选项分类: 杂项

与上述分类无关的选项.

- **\*\* 禁用标 \* 选项热重载**\
  禁用上述标 \* 的选项的热重载, 可适当减少资源消耗. 本选项不可热重载.

### 3.2 自定义规则

执行自定义规则破解\/恢复时使用的规则.

配置文件中标签 `[ customized_rules ]` 到下一个标签的部分.

一项自定义规则遵循以下格式:

```
<flag>: <item>
```

`<flag>` 有以下选项 (区分大小写):
- `exec`: 表明该项自定义规则为以 `.exe` 为文件扩展名的可执行文件.
- `serv`: 表明该项自定义规则为某个 Windows 服务的服务名称 (**不是显示名称**).

`<item>` 的类型由 `<flag>` 决定.

如果自定义规则不符合格式 (如 `EXEC abc`), 则会被忽略.

> [!WARNING]
> SCLTK 不对自定义规则的正确性进行检测, 一些规则可能导致意想不到的错误. 在修改自定义规则时, 请仔细检查.

示例:
```ini
[ customized_rules ]
exec: abc_client_gui
exec: abc_client_server
exec: abc_protect_server
serv: abc_network
serv: abc_diag_track
```

## 4 工具箱

- **返回上一级页面**\
`< 返回`
- **启动命令提示符**\
`> 命令提示符`
- **尝试重新启用部分被禁用的操作系统组件**\
`> 恢复操作系统组件`

点击 `[ 快捷操作 ]` 下的项目, 二次确认后方可执行.

## 5 破解/恢复

- **破解控制**\
  `[ 破解 ]`
- **恢复控制**\
  `[ 恢复 ]`

每个控制软件有独立的破解/恢复选项, 可根据需求执行.

`[ 破解 ]`\/`[ 恢复 ]` 下的 `> 自定义` 将执行自定义规则, 配置参阅 [3.2 自定义规则](#32-自定义规则).

当启用特定选项 (详见 [3.1.1 选项分类: 破解与恢复](#311-选项分类-破解与恢复)) 时, 破解/恢复时将映像劫持可执行文件, 禁用相关服务.

# 📜 许可证

SCLTK 使用 [MIT License](./LICENSE), 详细内容请自行阅读.

# 🛠️ 二次开发

首先, 请确保您已经安装了 [msys2](https://www.msys2.org), 并在 [msys2](https://www.msys2.org) 的 `msys` 环境中安装了软件包 `make` 和 `git`.

然后, 使用 git 克隆本仓库到本地, 在仓库本地目录下运行 `.\make_env.ps1`, 这将会创建 `env` 文件, 文件内容如下:

```makefile
msys2_path = /path/to/msys2
pwsh_path  = /path/to/pwsh
```

其中 `/path/to/msys2` 为 msys2 的安装路径 \(以 `/` 作为路径分隔符, 若路径中存在空格, 需要先将路径用英文半角引号引起来 ,如 `"C:/dev tools/msys2"`, 下同\), `/path/to/pwsh` 为 PowerShell 的安装路径 \(可通过命令 `where.exe powershell.exe` 或 `where.exe pwsh.exe` 查询\).

接下来, 在仓库本地目录下打开终端, 执行:

```bash
make all
```

即开始安装工具链和依赖库, 并构建 SCLTK. 生成的 SCLTK 可执行文件位于 `build/debug` 和 `build/release` 中.

后续构建可使用:

```bash
make build
```

```bash
make debug
```

```bash
make release
```

> [!NOTE]
> 可在 `make build`, `make debug`, `make release` 后附加 `-j` 参数来加快构建速度.

更新工具链, 可使用:

```bash
make toolchain
```

# ❓ 常见问题

## 无法执行包含非 ASCII 字符的自定义规则

请尝试将配置文件 `config.ini` 使用 GBK 编码重新保存后重新启动 SCLTK.

> [!NOTE]
> 由于 Microsoft Windows OS 历史遗留问题, GBK 编码为语言设置为简体中文时的默认文本编码. 此方法仅能支持自定义规则中的大部分中文字符, 部分日文和韩文字符.

## SCLTK 窗口总是输出 "... 已被管理员禁用" 或 "找不到 ..."

可尝试使用 "恢复操作系统组件", 详见 [4 工具箱](#4-工具箱).

## 破解后一些软件运行时报错 "找不到文件"

在不影响软件正常运行的情况下, 可以给软件文件修改一个名称, 再试试. 或者打开注册表编辑器, 定位到 `HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options`, 找到和要运行的软件的文件名称相同的项, 删除即可.

## SCLTK 在某些版本的 Windows OS 上无法运行

SCLTK 不对 Windows Vista 及更早的版本保有任何技术支持. SCLTK 将会持续支持对当前受 Microsoft 支持的 Windows OS. 针对于 Windows 7\/8\/8.1, 我们将开始进行兼容性改进, 确保在 2027 年前保留对 Windows 7\/8\/8.1 的支持.

# ❤️ 鸣谢

- Bilibili 用户 [lateworker_晚工](https://space.bilibili.com/454920362) 提供早期版本界面底层代码 (详见[此处](https://www.bilibili.com/video/BV1X14y1n7S4), 有修改).
- Bilibili 用户 [痕继痕迹](https://space.bilibili.com/39337803) 指导早期版本界面设计.
- GitHub 用户 [Zhu-Xinrong \(Kendall\)](https://github.com/Zhu-Xinrong) 指导早期版本图标设计.
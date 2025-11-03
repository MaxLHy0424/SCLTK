<div align="center">

![title](./img/title.png)\
**Student Computer Lab Toolkit**\
**轻松破解机房控制，基于 Modern C++**

</div>

> [!CAUTION]
> SCLTK 仅适用于 Microsoft Windows OS。

# 📢 公告

**2025-09-07（UTC+08:00）** SCLTK v7.0.0 进入功能冻结阶段！

<details>

<summary>更多公告</summary>

**2025-02-18（UTC+08:00）** 从 v6.1.0 起，原 CRCSN（Computer Room Control Software Nemesis）正式更名为 SCLTK（Student Computer Lab Toolkit）。

</details>

# 📦 获取

| 分支     | 最新发行版                                                                  | 状态       | 生命周期（UTC+08:00）    |
| -------- | --------------------------------------------------------------------------- | ---------- | ------------------------ |
| SCLTK v1 | [v1.0_Stable](https://github.com/MaxLHy0424/SCLTK/releases/tag/v1.0_Stable) | ❌ 停止维护 | 2023-12-17/2023-12-18    |
| SCLTK v2 | [v2.5_Stable](https://github.com/MaxLHy0424/SCLTK/releases/tag/v2.5_Stable) | ❌ 停止维护 | 2024-01-01/2024-01-07    |
| SCLTK v3 | [v3.0.1](https://github.com/MaxLHy0424/SCLTK/releases/tag/30174)            | ❌ 停止维护 | 2024-02-09/2024-02-10    |
| SCLTK v4 | [v4.10.7](https://github.com/MaxLHy0424/SCLTK/releases/tag/v4.10.7)         | ❌ 停止维护 | 2024-03-26/2024-06-19    |
| SCLTK v5 | [v5.11.1](https://github.com/MaxLHy0424/SCLTK/releases/tag/v5.11.1)         | ❌ 停止维护 | 2024-08-25/2024-12-23    |
| SCLTK v6 | [v6.3.3](https://github.com/MaxLHy0424/SCLTK/releases/tag/v6.3.3)           | ⚠️ 即将弃用 | 2025-01-29/..            |
| SCLTK v7 | ⌛ 即将推出                                                                  | 🛠️ 正在开发 | 🕑 预计于 2025 年冬季发布 |

# 📖 使用指南

> [!NOTE]
> 适用于当前分支的最新版本。

## 0 开始之前

**使用 SCLTK 时建议关闭防病毒软件。若 SCLTK 被报为恶意软件，请在防病毒软件中添加排除项。**

**请勿使用 SCLTK 扰乱课堂纪律，造成的后果与开发者无关。**

**SCLTK 以 [MIT License](./LICENSE) 开源，详细内容请自行阅读。**

## 1 启动

SCLTK 发行版文件名规则为 `SCLTK-<arch>-<runtime>.exe`，其中 `<arch>` 一般为 `x86_64` 或 `i686`，`<runtime>` 一般为 `ucrt` 或 `msvcrt`。

`<arch>` 和 `<runtime>` 的具体信息如下：

- **`x86_64` & `ucrt`（推荐）**\
  开发工具链为 msys2 `mingw-w64-ucrt-x86_64-toolchain`，64 位可执行文件，运行时库为 Universal C Runtime（UCRT），支持 Windows 10 以上的 Windows OS（部分 Windows OS 在安装特定更新后可以运行）。
- **`i686` & `msvcrt`**\
  开发工具链为 msys2 `mingw-w64-i686-toolchain`，32 位可执行文件，运行时库为 Microsoft Visual C Runtime（MSVCRT），支持大部分 Windows OS。

## 2 常规操作

- **退出 SCLTK**\
  `< 退出`
- **重新启动 SCLTK**\
  `< 重启`
- **查看 SCLTK 信息**\
  `> 关于`

## 3 配置 SCLTK

> [!NOTE]
> 配置文件 `config.ini` 中以 `#` 开头的行是注释。\
> 配置文件头部注释为 SCLTK 自动生成。

- **进入配置编辑页面**\
  `> 配置`

> [!NOTE]
> 配置解析规则如下：
> - 配置以行为单位解析。
> - 各个配置分类在配置文件中由不同标签区分，标签的格式为 `[<标签名>]`，`<标签名>` 与中括号之间可以有若干空格。
> - 如果匹配不到配置分类，则当前读取到的标签到下一个标签的前一行都将被忽略。
> - 忽略每行前导和末尾的空白字符，包括空格，换页符（转义字符 0x0c `\f`），横向制表符（转义字符 0x09 `\t`），纵向制表符（转义字符 0x0b `\v`）等。
> - 如果当前行不是标签，则该行将由上一个标签处理。
> - 跳过注释。

### 3.0 配置操作

- **暂存配置并返回**\
  `< 返回`
- **查看上文所述的配置解析规则**\
  `> 查看解析规则`
- **同步已保存的配置和暂存的配置**\
  `> 同步配置`
- **使用默认文本编辑器打开配置文件**\
  `> 打开配置文件`

### 3.1 破解与恢复

破解/恢复控制软件相关选项。位于 `[ 选项 ]` 下。

- **劫持可执行文件**\
  破解时劫持控制软件的可执行文件，恢复时撤销劫持。
- **设置服务启动类型**\
  破解时禁用控制软件的服务，恢复时重新启用。

### 3.2 窗口显示

SCLTK 窗口相关选项。位于 `[ 选项 ]` 下。

- **置顶窗口（非实时）**\
  每 50ms 强制显示窗口并设为置顶。
- **极简标题栏（非实时）**\
  禁用窗口上下文菜单，隐藏所有窗口控件，隐藏图标。
- **半透明（非实时）**\
  将窗口不透明度设为 90%。

### 3.3 性能

影响 SCLTK 性能的选项. 位于 `[ 选项 ]` 下.

- **禁用非实时热重载（下次启动时生效）**\
  禁用标有 `（非实时）` 选项的热重载，可适当减少资源消耗。SCLTK 下次启动时生效。

### 3.2 自定义规则

执行自定义规则破解/恢复时使用的规则。

配置文件中标签 `[ custom_rules ]` 到下一个标签的部分。

一项自定义规则遵循以下格式：

```
<flag>: <item>
```

`<flag>` 有以下选项（区分大小写）：
- `exec`：表明该项自定义规则为以 `.exe` 为文件扩展名的可执行文件。
- `serv`：表明该项自定义规则为某个 Windows 服务的服务名称（**不是显示名称**）。

`<flag>` 后的冒号与 `<item>` 之间可以有若干个空白字符。

`<item>` 的类型由 `<flag>` 决定。

如果 `<item>` 为空，该项规则将会被忽略。

如果自定义规则不符合格式（如 `EXEC abc`），则会被忽略。

> [!NOTE]
> 可在 “配置” 页面下的 “自定义规则” 中，点击 `> 查看帮助信息` 阅读上述信息的简略版本。

> [!WARNING]
> SCLTK 不对自定义规则的正确性进行检测，一些规则可能导致意想不到的错误。在修改自定义规则时，请仔细检查。

示例:
```ini
[custom_rules]
exec: abc_frontend
exec: abc_backend
serv: abc_connect_service
serv: abc_proc_defender
```

## 4 工具箱

- **返回上一级页面**\
`< 返回`
- **在 SCLTK 窗口内直接启动命令提示符（可通过输入 `exit` 退出）**\
`> 启动命令提示符`
- **尝试重新启用部分被禁用的操作系统组件**\
`> 恢复操作系统组件`
- **终止 “学生机房管理助手” 生成的随机进程名的守护进程**\
`> 终止 "学生机房管理助手" 守护进程`

点击 `[ 快捷命令 ]` 下的项目，将调用命令提示符执行。

## 5 破解/恢复

- **破解控制**\
  `[ 破解 ]`
- **恢复控制**\
  `[ 恢复 ]`

每个控制软件有独立的破解/恢复选项，可根据需求执行。

当启用特定选项（详见 [3.1.1 破解与恢复](#31-破解与恢复)）时，破解/恢复时将映像劫持可执行文件，禁用相关服务。

`[ 破解 ]`/`[ 恢复 ]` 下的 `> 自定义` 将执行自定义规则，配置参阅 [3.2 自定义规则](#32-自定义规则)。

> [!NOTE]
> 这里针对 “学生机房管理助手” 做补充说明。
>
> 该软件在运行时会启动一个**进程名完全随机**的**守护进程**，当主进程被终止时，会尝试恢复主进程。如果恢复失败，则会 “蓝屏警告” 用户。由于性能问题，**守护进程并不在破解/恢复规则中**。所以，破解 “学生机房管理助手” 后，务必使用[工具箱](#4-工具箱)中的 `> 终止 "学生机房管理助手" 守护进程`。

# 🛠️ 二次开发

首先，请确保您已经安装了 [msys2](https://www.msys2.org)，并在 [msys2](https://www.msys2.org) 已安装软件包 `make` 和 `git`。

然后，使用 git 克隆本仓库到本地，在仓库本地目录下运行 `.\make_env.ps1`，这将会创建 `env.mk` 文件，文件内容如下：

```makefile
msys2_path = /path/to/msys2
pwsh_path  = /path/to/pwsh
```

其中 `/path/to/msys2` 为 msys2 的安装路径（以 `/` 作为路径分隔符，若路径中存在空格，需要先将路径用英文半角引号引起来，如 `"C:/dev tools/msys2"`，下同），`/path/to/pwsh` 为 PowerShell 的安装路径（可通过命令 `where.exe powershell.exe` 或 `where.exe pwsh.exe` 查询）。

接下来，在仓库本地目录下打开终端，执行：

```bash
make all
```

即开始安装工具链和依赖库，并构建 SCLTK。生成的 SCLTK 可执行文件位于 `build/debug` 和 `build/release` 中。

后续构建可使用：

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
> 可在 `make build`、`make debug`、`make release` 后附加 `-j` 参数来加快构建速度。

更新工具链，可使用：

```bash
make toolchain
```

打包 `build/release` 下的可执行文件和 `LICENSE`，可使用：

```bash
make pack
```

> [!NOTE]
> 如果想要构建后立马打包，可使用：
> ```bash
> make build -j && make pack
> ```

> [!WARNING]
> SCLTK 仓库下所有源代码文件均以 UTF-8 编码保存，如果使用其他文本编码保存源代码文件，可能导致非 ASCII 字符变成乱码。

# ❓ 常见问题

## 无法执行包含非 ASCII 字符的自定义规则

请尝试将配置文件 `config.ini` 使用 GBK 编码重新保存后重新启动 SCLTK。

> [!NOTE]
> 由于 Microsoft Windows OS 历史遗留问题，GBK 编码为语言设置为简体中文时的默认文本编码。此方法仅能支持自定义规则中的大部分中文字符，部分日文和韩文字符。

## SCLTK 窗口总是输出 “xxx 已被管理员禁用” 或 “找不到 xxx”

可尝试使用 “恢复操作系统组件”，详见 [4 工具箱](#4-工具箱)。

## 破解后一些软件运行时报错 “找不到文件”

在不影响软件正常运行的情况下，可以尝试修改可执行文件名称。另外，打开注册表编辑器，在 `HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options` 下找到和要运行的软件的文件名称相同的项，删除即可。

## SCLTK 在某些版本的 Windows OS 上无法运行

SCLTK 不对 Windows Vista 及更早的版本保有任何技术支持。SCLTK 将会持续支持对当前受 Microsoft 支持的 Windows OS。针对于 Windows 7/8/8.1，我们将开始进行兼容性改进，确保在 2027 年前保留对 Windows 7/8/8.1 的支持。

## SCLTK 使用内建规则破解不起作用

请尝试使用最新版本的 SCLTK 进行破解，如果仍不起作用，可能是因为内建规则已失效。此时，可使用自定义规则临时解决，如果条件允许，请且尽快提交 Issue。

# ❤️ 鸣谢

- [Zhu-Xinrong（Kendall）](https://github.com/Zhu-Xinrong) 指导早期版本图标设计。
- [fengliteam](https://github.com/fengliteam) 提供大量改进建议，开发姊妹项目 [SCA](https://github.com/fengliteam/SCA)。
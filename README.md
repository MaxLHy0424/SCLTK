# ✨ Intro

<div align="center">

**Student Computer Lab Toolkit**\
**轻松破解电子教室软件，基于 Modern C++**

**已加入 [“机房抗联”](https://clra.woen.pics)**

</div>

> [!IMPORTANT]
> SCLTK 仅适用于 Microsoft Windows。

# 📦 获取

可从下列页面中获取 SCLTK：

- [本仓库发行版页面](https://github.com/MaxLHy0424/SCLTK/releases)；
- [SCLTK 下载站（Vercel）](https://scltk.maxlhy0424.is-a.dev)；
- [SCLTK 下载站（Cloudflare Pages）](https://scltk.pages.dev)；
- [SCLTK 下载站（PinMe）](https://scltk.pinme.dev)。

> [!NOTE]
> “SCLTK 下载站” 完全开源。如需自行部署，请转到 [SCLTK-Website-Template](https://github.com/MaxLHy0424/SCLTK-Website-Template)。

# 📖 使用指南

## 0 开始之前

**使用 SCLTK 时建议关闭防病毒软件。若 SCLTK 被报为恶意软件，请在防病毒软件中添加排除项。**

**请勿使用 SCLTK 扰乱课堂纪律，造成的后果与开发者无关。**

## 1 启动

SCLTK-Mainline 的发行版文件名为 `SCLTK-Mainline.exe`，SCLTK-Legacy 的发行版文件名为 `SCLTK-Legacy.exe`。两者支持的 Windows 如下：

| 项目           | Windows 版本                                       | Windows 架构 |
| -------------- | -------------------------------------------------- | ------------ |
| SCLTK-Mainline | Windows 7/8/8.1（需安装 KB2999226），Windows 10/11 | 64 位        |
| SCLTK-Legacy   | Windows 7/8/8.1/10/11                              | 32/64 位     |

## 2 常规操作

- **退出 SCLTK**\
  `< 退出`
- **查看 SCLTK 信息**\
  `> 关于`

## 3 配置 SCLTK


- **进入配置编辑页面**\
  `> 配置`

> [!NOTE]
> 配置文件 `SCLTK-Mainline.conf` / `SCLTK-Legacy.conf` 中以 `#` 开头的行是注释。\
> 配置文件头部注释为 SCLTK 自动生成。
>
> 配置解析规则如下：
> - 跳过注释；
> - 配置以行为单位解析；
> - 各个配置分类在配置文件中由不同标签区分，标签的格式为 `[<标签名>]`，`<标签名>` 与中括号之间可以有若干空格；
> - 如果匹配不到配置分类，则当前读取到的标签到下一个标签的前一行都将被忽略；
> - 忽略每行前导和末尾的空白字符，包括空格，换页符（0x0c `\f`），横向制表符（0x09 `\t`），纵向制表符（0x0b `\v`）等；
> - 如果当前行不是标签，则该行将由上一个标签处理。

### 3.0 配置操作

- **暂存配置并返回**\
  `< 返回`
- **查看上文所述的配置解析规则**\
  `> 查看解析规则`
- **同步已保存的配置和暂存的配置**\
  `> 同步配置`
- **使用记事本打开配置文件**\
  `> 打开配置文件`

### 3.1 破解与恢复

破解/恢复电子教室软件相关选项。位于 `[ 选项 ]` 下。

- **启动时破解**\
  在 SCLTK 启动并完成必要的初始化工作后，根据配置，执行全部破解规则（包括内建规则和自定义规则）。
- **挂起进程**\
  破解时挂起电子教室软件的大部分进程。

### 3.2 窗口显示

SCLTK 窗口相关选项。位于 `[ 选项 ]` 下。

- **置顶窗口**\
  每 50ms 强制显示窗口并设为置顶。

### 3.3 自定义规则

执行自定义规则破解/恢复时使用的规则。

配置文件中标签 `[custom_rules]` 到下一个标签的部分。

一项自定义规则遵循以下格式：

```
<flag>{空格}:{空格}<item>
```

说明内容带有 `(RX)` 的 `<flag>`，使用正则表达式（调用 TRE，使用 POSIX ERE 语法，大小写敏感）。

合法的 `<flag>` 如下（大小写敏感）：
- `proc_name`：进程名称（RX）。
- `proc_path`：进程文件的路径（RX）。
- `proc_sign`：进程文件的数字签名签名者（RX）。
- `proc_vinfo`：进程文件的 “文件说明” “产品名称” “版权”（RX）。
- `serv_name`：服务名称。
- `crack_helper`：破解时执行的程序的命令行。
- `restore_helper`：恢复时执行的程序的命令行。

可在 “配置” 页面下的 “自定义规则” 中，点击 `> 查看帮助信息` 阅读上述信息的简略版本。

SCLTK 不对自定义规则的正确性进行检测，一些规则可能导致意想不到的错误。在修改自定义规则时，请仔细检查。

示例:

```ini
[custom_rules]
proc_name: ^abc_client[0-9]{10}\.exe$
proc_path: ^C:\\[a-z][0-9]{9}\\[a-z]{10}\.exe$
proc_sign: ^ABC eClass [a-z]{5}$
proc_vinfo: ^Copyright [0-9]{4} ABC eClass\.$
serv_name: abc_eclass
crack_helper: "abc toolkit.exe" crack
restore_helper: "abc toolkit.exe" restore
```

## 4 工具箱

- **返回上一级页面**\
`< 返回`

**快捷工具：**

- **在 SCLTK 窗口内启动命令提示符（可通过输入 `exit` 退出）**\
`> 启动命令提示符`
- **重启资源管理器**\
`> 重启资源管理器`
- **注销当前用户账户**\
`> 注销当前用户账户`
- **恢复被恶意篡改的部分操作系统设置**\
`> 恢复操作系统设置`
- **重置网络代理，重置防火墙规则，重置 Hosts，刷新 DNS，重启网络适配器**\
`> 修复网络访问`
- **删除 “学生机房管理助手” 的密码、配置、自启动项**\
`> 重置 学生机房管理助手 配置`
- **重置 Google Chrome、Microsoft Edge、Mozilla Firefox 的管理策略**\
`> 重置 Chrome & Edge & Firefox 管理策略`

## 5 破解/恢复

- **破解电子教室软件**\
  `[ 破解 (点击切换) ]`
- **恢复电子教室软件**\
  `[ 恢复 (点击切换) ]`

根据提示点击对应控件即可切换破解/恢复。

每个电子教室软件有独立的破解/恢复选项，可根据需求执行。可以通过点击 `> * 全部执行*` 一次性执行所有内建规则和自定义规则。

点击 `> * 自定义 *` 将执行自定义规则，配置自定义规则请参阅 [3.3 自定义规则](#33-自定义规则)。

# ❓ 常见问题

## 自定义规则乱码

请尝试将配置文件 `SCLTK-Mainline.conf` / `SCLTK-Legacy.conf` 使用 UTF-8 编码重新保存后重新启动 SCLTK。

## 有时 SCLTK 窗口内的项目无法点击

通常，这是由于电子教室软件置顶窗口和 SCLTK 发生冲突导致的。这时，可以尝试按下 Win + D 返回桌面，再次尝试点击。

## SCLTK 使用内建规则破解不起作用

请尝试使用最新版本的 SCLTK 进行破解，如果仍不起作用，可能是因为内建规则已失效。此时，可使用自定义规则临时解决。如果条件允许，请且尽快上报。

## SCLTK 无法终止某些进程

由于成本及安全原因，SCLTK 只使用了用户态 Windows API，对于受驱动程序保护的进程可能无法终止。可以通过 ARK（Anti-Rootkit）工具来终止这些进程。

# 🛠️ 二次开发

首先，请确保您已经安装了 [msys2](https://www.msys2.org)，并在 [msys2](https://www.msys2.org) 已安装软件包 `make` 和 `git`，[msys2](https://www.msys2.org) 的 `ucrt64`、`mingw32`、`msys` 环境应当添加到环境变量中。

然后，使用 git 克隆本仓库到本地。

本仓库支持构建 SCLTK-Mainline 与 SCLTK-Legacy。如需构建 SCLTK-Mainline，则下文中的 `<scltk-edition>` 为 `mainline`；如需构建 SCLTK-Legacy，则下文中的 `<scltk-edition>` 为 `legacy`

接下来，在仓库本地目录下打开终端（不是 [msys2](https://www.msys2.org) 的），执行：

```pwsh
.\make.ps1 -edition <scltk-edition> -target toolchain
```

脚本将会自动安装工具链和依赖库。

> [!NOTE]
> 即便您已经安装了 SCLTK 所需的工具链和依赖库，也务必使用上面的命令，因为 SCLTK 的开发总是使用最新的工具链和依赖库。

构建可使用：

```pwsh
.\make.ps1 -edition <scltk-edition> -target build
```

```pwsh
.\make.ps1 -edition <scltk-edition> -target debug
```

```pwsh
.\make.ps1 -edition <scltk-edition> -target release
```

更新工具链，可使用：

```pwsh
.\make.ps1 -edition <scltk-edition> -target toolchain
```

构建并打包 `build/release` 下的可执行文件和 `LICENSE.txt` 并签名，可使用：

```pwsh
.\make.ps1 -edition <scltk-edition> -target sign -gpg_key=<key-id>
```

其中，`<key-id>` 为签名所需的 GnuPG 密钥 ID。签名时所使用的 `gpg.exe` 为 `where.exe gpg.exe` 第一行的输出。

> [!NOTE]
> 发布时，推荐使用如下命令：
>
> ```pwsh
> .\make.ps1 -edition <scltk-edition> -target clean && .\make.ps1 -edition <scltk-edition> -target sign -gpg_key=<key-id>
> ```

> [!WARNING]
> 请勿绕过 `make.ps1` 执行构建。

> [!WARNING]
> 本仓库下所有源代码文件均以 UTF-8 编码保存，如果使用其他文本编码保存源代码文件，可能导致非 ASCII 字符变成乱码。

## ❤️ 致谢

- [lzh173](https://github.com/lzh173)；
- [CLRA-org](https://github.com/CLRA-org)。

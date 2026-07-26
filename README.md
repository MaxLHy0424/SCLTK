# ✨ Intro

<div align="center">

**Student Computer Lab Toolkit**\
**轻松破解电子教室软件，基于 Modern C++**

**已加入 [“机房抗联”](https://clra.woen.pics)**

</div>

> [!IMPORTANT]
> SCLTK & SCLTK-Legacy 仅适用于 Microsoft Windows。

# 📦 获取

**[SCLTK & SCLTK-Legacy 发行版页面](https://github.com/MaxLHy0424/SCLTK/releases)**

**官方下载站**：

- [SCLTK & SCLTK-Legacy 下载站（Vercel）](https://scltk.maxlhy0424.is-a.dev)
- [SCLTK & SCLTK-Legacy 下载站（Cloudflare Pages）](https://scltk.pages.dev)
- [SCLTK & SCLTK-Legacy 下载站（PinMe）](https://scltk.pinme.dev)

> [!NOTE]
> “SCLTK & SCLTK-Legacy 下载站” 完全开源。如需自行部署，请转到 [SCLTK-Website-Template](https://github.com/MaxLHy0424/SCLTK-Website-Template)。

| 主版本 | 最新正式发行版                                                              | 状态       | 生命周期              |
| ------ | --------------------------------------------------------------------------- | ---------- | --------------------- |
| v1     | [v1.0_Stable](https://github.com/MaxLHy0424/SCLTK/releases/tag/v1.0_Stable) | ❌ 停止维护 | 2023-12-17/2023-12-18 |
| v2     | [v2.5_Stable](https://github.com/MaxLHy0424/SCLTK/releases/tag/v2.5_Stable) | ❌ 停止维护 | 2024-01-01/2024-01-07 |
| v3     | [v3.0.1](https://github.com/MaxLHy0424/SCLTK/releases/tag/30174)            | ❌ 停止维护 | 2024-02-09/2024-02-10 |
| v4     | [v4.10.7](https://github.com/MaxLHy0424/SCLTK/releases/tag/v4.10.7)         | ❌ 停止维护 | 2024-03-26/2024-06-19 |
| v5     | [v5.11.1](https://github.com/MaxLHy0424/SCLTK/releases/tag/v5.11.1)         | ❌ 停止维护 | 2024-08-25/2024-12-23 |
| v6     | [v6.3.3](https://github.com/MaxLHy0424/SCLTK/releases/tag/v6.3.3)           | ❌ 停止维护 | 2025-01-29/2025-06-09 |
| v7     | [v7.1.5](https://github.com/MaxLHy0424/SCLTK/releases/tag/v7.1.5)           | ✅ 基线开发 | 2025-11-10/..         |

# 📖 使用指南

## 0 开始之前

**使用 SCLTK / SCLTK-Legacy 时建议关闭防病毒软件。若 SCLTK / SCLTK-Legacy 被报为恶意软件，请在防病毒软件中添加排除项。**

**请勿使用 SCLTK / SCLTK-Legacy 扰乱课堂纪律，造成的后果与开发者无关。**

**SCLTK & SCLTK-Legacy 以 [MIT License](./LICENSE.txt) 开源，详细内容请自行阅读。**

## 1 启动

SCLTK 的发行版文件名为 `SCLTK.exe`，SCLTK-Legacy 的发行版文件名为 `SCLTK-Legacy.exe`。两者支持的 Windows 如下：

| 项目         | Windows 版本                                       | Windows 架构 |
| ------------ | -------------------------------------------------- | ------------ |
| SCLTK        | Windows 7/8/8.1（需安装 KB2999226），Windows 10/11 | 64 位        |
| SCLTK-Legacy | Windows 7/8/8.1/10/11                              | 32/64 位     |

## 2 常规操作

- **退出 SCLTK / SCLTK-Legacy**\
  `< 退出`
- **查看 SCLTK / SCLTK-Legacy 信息**\
  `> 关于`

## 3 配置 SCLTK / SCLTK-Legacy


- **进入配置编辑页面**\
  `> 配置`

> [!NOTE]
> 配置文件 `SCLTK.conf` / `SCLTK-Legacy.conf` 中以 `#` 开头的行是注释。\
> 配置文件头部注释为 SCLTK / SCLTK-Legacy 自动生成。
>
> 配置解析规则如下：
> - 跳过注释；
> - 配置以行为单位解析；
> - 各个配置分类在配置文件中由不同标签区分，标签的格式为 `[<标签名>]`，`<标签名>` 与中括号之间可以有若干空格；
> - 如果匹配不到配置分类，则当前读取到的标签到下一个标签的前一行都将被忽略；
> - 忽略每行前导和末尾的空白字符，包括空格，换页符（转义字符 0x0c `\f`），横向制表符（转义字符 0x09 `\t`），纵向制表符（转义字符 0x0b `\v`）等；
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
  在 SCLTK / SCLTK-Legacy 启动并完成必要的初始化工作后，根据配置，执行全部破解规则（包括内建规则和自定义规则）。
- **挂起进程**\
  破解时挂起电子教室软件的大部分进程。

### 3.2 窗口显示

SCLTK / SCLTK-Legacy 窗口相关选项。位于 `[ 选项 ]` 下。

- **置顶窗口**\
  每 50ms 强制显示窗口并设为置顶。

### 3.3 自定义规则

执行自定义规则破解/恢复时使用的规则。

配置文件中标签 `[custom_rules]` 到下一个标签的部分。

一项自定义规则遵循以下格式：

```
<flag>:{可选的若干空格}<item>
```

说明内容带有 `(RX)` 的 `<flag>`，使用正则表达式（调用 `libtre`，使用 POSIX ERE 语法，大小写敏感）。

<details>
    <summary>POSIX ERE 语法说明</summary>

> **字面量与元字符**
>
> 正则表达式由普通字符（如字母、数字）和元字符组成。普通字符匹配其自身，元字符拥有特殊含义。`{ }`、`( )`、`|`、`+`、`?` 作为元字符使用不需要加 `\`。如果需要匹配它们的字面量，才需要用 `\` 转义（例如 `\+` 匹配加号本身）。核心元字符列表如下：
>
> | 元字符   | 名称         | 功能简述                               |
> | :------- | :----------- | :------------------------------------- |
> | `.`      | 点号         | 匹配任意单个字符（换行符通常除外）     |
> | `^`      | 脱字符       | 匹配行首（或字符串开头）               |
> | `$`      | 美元符       | 匹配行尾（或字符串结尾）               |
> | `[...]`  | 方括号       | 匹配方括号内定义的任一字符（字符类）   |
> | `[^...]` | 排除型方括号 | 匹配不在方括号内定义的任一字符         |
> | `*`      | 星号         | 匹配前面的元素 0 次或多次              |
> | `+`      | 加号         | 匹配前面的元素 1 次或多次（ERE 新增）  |
> | `?`      | 问号         | 匹配前面的元素 0 次或 1 次（ERE 新增） |
> | `{m,n}`  | 区间量词     | 匹配前面的元素 m 至 n 次（ERE 新增）   |
> | `\|`     | 竖线         | 逻辑或（交替/分支）（ERE 新增）        |
> | `(...)`  | 括号         | 分组和捕获（ERE 新增，括号无需转义）   |
> | `\`      | 反斜杠       | 转义后续字符，使其失去特殊含义         |
>
> **量词**
>
> 量词用于指定前一个元素（字符、字符类或分组）出现的次数。ERE 默认是贪婪匹配，即量词会尽可能多地匹配字符。
>
> | 语法    | 含义      | 示例（ERE）  | 匹配示例                             |
> | :------ | :-------- | :----------- | :----------------------------------- |
> | `*`     | 0次或多次 | `go*gle`     | "ggle"、"gogle"、"google"            |
> | `+`     | 1次或多次 | `go+gle`     | "gogle"、"google"（不匹配"ggle"）    |
> | `?`     | 0次或1次  | `colou?r`    | "color"、"colour"                    |
> | `{n}`   | 精确 n 次 | `[0-9]{4}`   | "2026"                               |
> | `{m,}`  | 至少 m 次 | `a{2,}`      | "aa"、"aaaaa"                        |
> | `{,n}`  | 至多 n 次 | `b{,3}`      | 空、"b"、"bb"、"bbb"（部分实现支持） |
> | `{m,n}` | m 到 n 次 | `[a-z]{2,5}` | "ab"、"hello"（长度2~5）             |
>
> **分组与捕获**
>
> 使用圆括号 `(...)` 可以将多个字符组合为一个原子单元，配合量词或交替使用。
>
> - 分组：将括号内的表达式视为一个整体。示例：`(abc){2}` 匹配 "abcabc"；`(ha)+` 匹配 "ha"、"haha"、"hahaha"。
> - 捕获：括号内的匹配结果会被引擎保存下来，供后续反向引用（如 `\1`、`\2`）使用。示例：`(abc)\1` 匹配 "abcabc"（但在部分 ERE 工具如 `grep -E` 中，反向引用支持不完整，需注意）。
>
> **交替**
>
> 竖线 `|` 表示逻辑“或”，用于匹配多个分支表达式中的任意一个。示例：`cat|dog` 匹配 "cat" 或 "dog"；结合分组：`gr(a|e)y` 匹配 "gray" 或 "grey"。注意：交替的优先级极低，通常配合括号使用。`ab|cd` 匹配 "ab" 或 "cd"，而非 "ab" + "c"/"d"。
>
> **锚点**
>
> 锚点不匹配具体字符，而是匹配文本中的位置。
>
> - `^` 匹配行首（或字符串开头），如 `^Hello` 匹配以 "Hello" 开头的行。
> - `$` 匹配行尾（或字符串结尾），如 `world$` 匹配以 "world" 结尾的行。
>
> **方括号表达式**
>
> 用于定义一组字符集合，匹配其中任意一个字符。
>
> 基本用法：
>
> - `[abc]`：匹配 'a'、'b' 或 'c'。
> - `[a-z]`：匹配任意小写字母（支持范围）。
> - `[0-9a-fA-F]`：匹配十六进制数字。
> - `[^0-9]`：匹配任意非数字字符（排除型）。
>
> POSIX 字符类（在方括号内，语法为 `[:class:]`，注意外层必须有 `[...]`）：
> - `[:alnum:]`：字母和数字（`[A-Za-z0-9]`）
> - `[:alpha:]`：字母
> - `[:digit:]`：数字
> - `[:lower:]`：小写字母
> - `[:upper:]`：大写字母
> - `[:space:]`：空白字符（空格、制表符、换行等）
> - `[:punct:]`：标点符号
> - `[:print:]`：可打印字符
> - `[:cntrl:]`：控制字符
>
> 示例：`[[:digit:]]{3}` 匹配三个数字。
>
> 高级用法（极少使用）：排序元素 `[.ch.]` 和等价类 `[=a=]`（匹配 `a`、`á`、`â` 等，依赖 Locale）。
>
> **运算符优先级（从高到低）**
> 1. 括号 `(...)` —— 最高
> 2. 量词 `*` `+` `?` `{m,n}` —— 紧贴其前面的元素
> 3. 连接/序列 —— 字符依次排列（如 `abc`）
> 4. 交替 `|` —— 最低
>
> 示例：`abc|de{2}` 被解析为 `(abc)|(de{2})`，即匹配 "abc" 或 "dee"。
>
> **应用示例**
>
> | 需求                           | ERE 表达式                                       | 匹配文本示例         |
> | :----------------------------- | :----------------------------------------------- | :------------------- |
> | 匹配身份证号码（18位，数字/X） | `[0-9]{17}[0-9Xx]`                               | "110101199003071234" |
> | 匹配 IPv4 地址（简化版）       | `([0-9]{1,3}\.){3}[0-9]{1,3}`                    | "192.168.1.1"        |
> | 匹配邮箱（简易）               | `[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}` | "test@example.com"   |
> | 匹配行首不以 # 开头的行        | `^[^#]`                                          | 有效配置行           |

</details>


合法的 `<flag>` 如下（大小写敏感）：
- `proc_name`：进程名称（RX）。
- `proc_path`：进程文件的路径（RX）。
- `proc_sign`：进程文件的数字签名签名者（RX）。
- `proc_vinfo`：进程文件的 “文件说明” “产品名称” “版权”（RX）。
- `serv_name`：服务名称。
- `crack_helper`：破解时执行的程序的命令行。
- `restore_helper`：恢复时执行的程序的命令行。

> [!NOTE]
> 可在 “配置” 页面下的 “自定义规则” 中，点击 `> 查看帮助信息` 阅读上述信息的简略版本。

> [!WARNING]
> SCLTK & SCLTK-Legacy 不对自定义规则的正确性进行检测，一些规则可能导致意想不到的错误。在修改自定义规则时，请仔细检查。

示例:
```ini
[custom_rules]
proc_name: ^abc_client[0-9]{10}\.exe$
proc_path: ^C:\\[a-z][0-9]{9}\\[a-z]{10}\.exe$
proc_sign: ^ABC eClass [a-z]{5}$
serv_name: abc_eclass
crack_helper: "abc toolkit.exe" crack
restore_helper: "abc toolkit.exe" restore
```

## 4 工具箱

- **返回上一级页面**\
`< 返回`

**快捷工具：**

- **在 SCLTK / SCLTK-Legacy 窗口内直接启动命令提示符（可通过输入 `exit` 退出）**\
`> 启动命令提示符`
- **一键重启资源管理器**\
`> 重启资源管理器`
- **直接注销当前用户账户**\
`> 注销当前用户账户`
- **恢复部分被恶意篡改的操作系统设置**\
`> 恢复操作系统设置`
- **重置防火墙规则，重置 Hosts，重置网络代理，刷新 DNS 缓存，重启网络适配器**\
`> 修复网络访问`
- **删除 “机房管理助手” 的密码、配置、自启动项**\
`> 重置 "机房管理助手" 配置`
- **重置 Google Chrome、Microsoft Edge、Mozilla Firefox 的管理策略**\
`> 重置 Chrome & Edge & Firefox 管理策略`

## 5 破解/恢复

- **破解电子教室软件**\
  `[ 破解 (点击切换) ]`
- **恢复电子教室软件**\
  `[ 恢复 (点击切换) ]`

根据提示点击对应控件即可切换破解/恢复。

每个电子教室软件有独立的破解/恢复选项，可根据需求执行。可以通过点击 `> 全部执行` 一次性执行所有内建规则和自定义规则。

点击 `> * 自定义 *` 将执行自定义规则，配置自定义规则请参阅 [3.3 自定义规则](#33-自定义规则)。

# ❓ 常见问题

## 自定义规则乱码

请尝试将配置文件 `SCLTK.conf` / `SCLTK-Legacy.conf` 使用 UTF-8 编码重新保存后重新启动 SCLTK。

## 有时 SCLTK / SCLTK-Legacy 窗口内的项目无法点击

通常，这是由于电子教室软件置顶窗口和 SCLTK / SCLTK-Legacy 发生冲突导致的。这时，可以尝试按下 Win + D 返回桌面，再次尝试点击。

## SCLTK / SCLTK-Legacy 使用内建规则破解不起作用

请尝试使用最新版本的 SCLTK / SCLTK-Legacy 进行破解，如果仍不起作用，可能是因为内建规则已失效。此时，可使用自定义规则临时解决。如果条件允许，请且尽快上报。

## SCLTK / SCLTK-Legacy 无法终止某些进程

由于成本（驱动程序的认证费用不菲）及安全原因，SCLTK & SCLTK-Legacy 只使用了用户态 Windows API，对于受驱动程序保护的进程可能无法终止。可以通过 ARK（Anti-Rootkit）工具来终止这些进程。推荐的 ARK 工具有 [OpenArk](http://openark.blackint3.com:88)、[KSwordARK](https://github.com/WangWei-CM/KSword)、[StarlightGUI](https://github.com/OpenStarlight/StarlightGUI)。

# 🛠️ 二次开发

首先，请确保您已经安装了 [msys2](https://www.msys2.org)，并在 [msys2](https://www.msys2.org) 已安装软件包 `make` 和 `git`，[msys2](https://www.msys2.org) 的 `ucrt64`、`mingw32`、`msys` 环境应当添加到环境变量中。

然后，使用 git 克隆本仓库到本地。

本仓库支持构建 SCLTK 与 SCLTK-Legacy。如需构建 SCLTK，则下文中的 `<scltk-edition>` 为 `mainline`；如需构建 SCLTK-Legacy，则下文中的 `<scltk-edition>` 为 `legacy`

接下来，在仓库本地目录下打开终端（不是 [msys2](https://www.msys2.org) 的），执行：

```pwsh
.\make.ps1 -edition <scltk-edition> -target toolchain
```

脚本将会自动安装工具链和依赖库。

> [!NOTE]
> 即便您已经安装了 SCLTK / SCLTK-Legacy 所需的工具链和依赖库，也务必使用上面的命令，因为 SCLTK & SCLTK-Legacy 的开发总是使用最新的工具链和依赖库。

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
.\make.ps1 -edition <scltk-edition> -target pack_and_sign -gpg_key=<key-id>
```

其中，`<key-id>` 为签名所需的 GnuPG 密钥 ID。签名时所使用的 `gpg.exe` 为 `where.exe gpg.exe` 第一行的输出。

> [!NOTE]
> 发布时，推荐使用如下命令：
> ```pwsh
> .\make.ps1 -edition <scltk-edition> -target clean && .\make.ps1 -edition <scltk-edition> -target pack_and_sign -gpg_key=<key-id>
> ```

> [!WARNING]
> 请勿绕过 `.\make.ps1` 执行构建。

> [!WARNING]
> 本仓库下所有源代码文件均以 UTF-8 编码保存，如果使用其他文本编码保存源代码文件，可能导致非 ASCII 字符变成乱码。

# ❤️ 鸣谢

- [fengliteam](https://github.com/fengliteam) 提供大量改进建议。
- [lzh173](https://github.com/lzh173) 提供 “机房管理助手” 逆向工程后的部分代码。

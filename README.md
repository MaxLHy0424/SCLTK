<div align="center">

![title](./img/title.png)\
**Student Computer Lab Toolkit**\
**轻松破解机房控制**\
©️ 2023 - present MaxLHy0424.

</div>

# 📦 获取

> [!NOTE]
> 从 v6.1.0 起, 原 CRCSN (Computer Room Control Software Nemesis) 正式改名为 SCLTK (Student Computer Lab Toolkit).

| 分支            | 最新发行版                                                                       | 状态       | 生命周期                |
| --------------- | -------------------------------------------------------------------------------- | ---------- | ----------------------- |
| SCLTK (CRCSN) 1 | [v1.0_Stable](https://github.com/MaxLHy0424/SCLTK/releases/tag/v1.0_Stable)      | ❌ 停止开发 | 2023/12/17 ~ 2023/12/18 |
| SCLTK (CRCSN) 2 | [v2.5_Stable](https://github.com/MaxLHy0424/SCLTK/releases/tag/v2.5_Stable)      | ❌ 停止开发 | 2024/01/01 ~ 2024/01/07 |
| SCLTK (CRCSN) 3 | [v3.0.1 \(Build 30174\)](https://github.com/MaxLHy0424/SCLTK/releases/tag/30174) | ❌ 停止开发 | 2024/02/09 ~ 2024/02/10 |
| SCLTK (CRCSN) 4 | [v4.10.7](https://github.com/MaxLHy0424/SCLTK/releases/tag/v4.10.7)              | ❌ 停止开发 | 2024/03/26 ~ 2024/06/19 |
| SCLTK (CRCSN) 5 | [v5.11.1](https://github.com/MaxLHy0424/SCLTK/releases/tag/v5.11.1)              | ❌ 停止开发 | 2024/08/25 ~ 2024/12/23 |
| SCLTK (CRCSN) 6 | [v6.2.0](https://github.com/MaxLHy0424/SCLTK/releases/tag/v6.2.0)                | ✔️ 主线开发 | 2025/01/29 ~            |

# 📖 使用指南

> [!NOTE]
> 适用于 v6.2.0.

## 0 开始之前

**使用 SCLTK 时建议关闭防病毒软件. 若 SCLTK 被报为恶意软件, 请在防病毒软件中添加排除项.**

**请勿使用 SCLTK 扰乱课堂纪律, 造成的后果与开发者无关.**

## 1 启动

SCLTK 发行版文件名规则为 `SCLTK-[ arch ]-[ runtime ].exe`, 其中 `[ arch ]` 一般为 `x86_64` 或 `i686`, `[ runtime ]` 一般为 `ucrt` 或 `msvcrt`.

`[ arch ]` 和 `[ runtime ]` 的具体信息如下:
- **`x86_64` & `ucrt` \(推荐\)**\
  开发工具链为 msys2 `mingw-w64-ucrt-x86_64-toolchain`, 64 位可执行文件, 运行时库为 *Universal C Runtime*, 支持 Windows 10 以上的 Windows OS \(部分 Windows OS 在安装特定更新后可以运行\).
- **`i686` & `msvcrt`**\
  开发工具链为 msys2 `mingw-w64-i686-toolchain`, 32 位可执行文件, 运行时库为 *Microsoft Visual C Runtime*, 支持大部分 Windows OS.

## 2 常规操作

- **退出 SCLTK**\
  `< 退出`
- **重新启动 SCLTK**\
  `< 重启`
  > 需要启用 "快速退出和重启" 选项 \(详见 [3.2.3 选项分类: 性能](#323-选项分类-性能)\).
- **查看 SCLTK 信息**\
  `> 关于`

## 3 配置 SCLTK

> [!NOTE]
> 配置文件 `config.ini` 中以 `#` 开头的行为注释.\
> 配置文件头部注释为 SCLTK 自动生成.

- **进入配置编辑页面**\
  `> 配置`

### 3.1 配置操作

- **暂存配置并返回**\
  `< 返回`
- **同步已保存的配置和暂存的配置**\
  `> 同步配置`
- **使用默认文本编辑器打开配置文件**\
  `> 打开配置文件`

### 3.2 常规选项

所有常规选项可在配置页面中修改.

具体描述见下文.

#### 3.2.1 选项分类: 破解/恢复

破解/恢复控制软件相关选项.

- **劫持可执行文件**\
  在破解\/恢复时对通过劫持可执行文件控制软件的可执行文件.
- **设置服务启动类型**\
  在破解\/恢复时启用\/禁用控制软件相关的服务.
- **\* 修复操作系统环境**\
  每隔 1s 修复部分被映像劫持的系统组件, 重新启用被禁用的部分系统组件.

#### 3.2.2 选项分类: 窗口显示

SCLTK 窗口相关选项.

- **\* 置顶窗口**\
  每 100ms 强制显示窗口并设为置顶.
- **\* 禁用关闭控件**\
  禁用窗口右上角和窗口上下文菜单中的关闭窗口的控件.
- **\* 半透明**\
  将窗口不透明度设为 90%.

#### 3.2.3 选项分类: 性能

与 SCLTK 性能相关的选项, 会一定程度上提升或降低性能.

- **\*\* 快速退出和重启**\
  通过提前分离线程来将加快退出速度, 同时添加 "重启" 控件. 本选项不可热重载. 注意: 在某些情况下, 这可能导致 SCLTK 退出或重启时时发生异常.
- **\*\* 禁用标 \* 选项热重载**\
  禁用上述标 \* 的选项的热重载. 启用后部分标 \* 选项仅会执行一次. 启用后可适当减少资源消耗. 本选项不可热重载.

### 3.3 自定义规则

在执行从自定义规则破解/恢复时使用自定义规则.

#### 3.3.1 可执行文件

配置文件中标签 `[ custom_rules_execs ]` 到下一个标签的部分, 每个项目保留文件扩展名.

示例:
```ini
[ custom_rules_execs ]
abc_client_gui.exe
abc_client_server.exe
abc_protect_server.com
```

#### 3.3.2 服务

配置文件中标签 `[ custom_rules_servs ]` 到下一个标签的部分.

每项规则为服务名称, 非显示名称.

示例:
```ini
[ custom_rules_servs ]
abc_network
abc_diag_track
```

## 4 工具箱

- **返回上一级页面**\
`< 返回`
- **启动命令提示符**\
`> 命令提示符`

点击 `[ 快捷操作 ]` 下的文本控件以执行相应操作.

## 5 破解/恢复

- **破解控制**\
  `[ 破解 ]`
- **恢复控制**\
  `[ 恢复 ] `

每个控制软件有独立的破解/恢复选项, 可根据需求执行.

`[ 破解 ]`/`[ 恢复 ]` 下的 `> 自定义` 将执行自定义规则, 配置参阅 [3.3 自定义规则](#33-自定义规则).

当启用特定选项 (详见 [3.2.1 选项分类: 破解/恢复](#321-选项分类-破解恢复)) 时, 破解/恢复时将映像劫持可执行文件, 禁用相关服务.

# 📜 许可证

SCLTK 使用 [MIT License](./LICENSE), 详细内容请自行阅读.

# 🛠️ 二次开发

首先, 请确保您已经安装了 [msys2](https://www.msys2.org), 并在 msys2 的 `msys` 环境中安装了软件包 `make` 和 `git`.

然后, 使用 git 克隆本仓库到本地, 在仓库本地目录下创建 `msys2` 文件, 内容如下:
```
msys2_path = /path/to/msys2
```

其中 `/path/to/msys2` 为 msys2 的安装路径 (以 `/` 作为路径分隔符, 如 `C:/msys2`).

接下来, 在仓库本地目录下打开终端, 执行:
```bash
make all
```

即开始安装工具链和依赖库, 并编译 SCLTK. 生成的 SCLTK 可执行文件位于 `bin/debug` 和 `bin/release` 中.

后续编译可使用:
```bash
make build
```
```bash
make debug
```
```bash
make release
```

# ❓ 常见问题

## 1 无法执行包含非 ASCII 字符的自定义规则.

请尝试将配置文件 `config.ini` 使用 GB18030 \/ GBK 编码重新保存后重新启动 SCLTK.

> [!NOTE]
> 由于 Microsoft Windows OS 历史遗留问题, GB18030 \/ GBK 编码为语言设置为简体中文时的默认文本编码. 此方法仅能支持自定义规则中的大部分中文字符, 部分韩文字符及日文字符.

## 2 SCLTK 窗口总是输出 "... 已被管理员禁用" 或 "找不到 ..."

可尝试启用 "修复操作系统环境", 详见 [3.2.1 选项分类: 破解/恢复](#321-选项分类-破解恢复).

## 3 破解后一些软件运行时报错 "找不到文件".

在不影响软件正常运行的情况下, 可以给软件文件修改一个名称, 再试试. 或者打开注册表编辑器, 定位到 `HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options`, 找到和要运行的软件的文件名称相同的项, 删除即可.

# ❤️ 鸣谢

- Bilibili 用户 [lateworker_晚工](https://space.bilibili.com/454920362) 提供界面底层代码 (详见[此处](https://www.bilibili.com/video/BV1X14y1n7S4), 有修改).
- Bilibili 用户 [痕继痕迹](https://space.bilibili.com/39337803) 指导界面设计.
- GitHub 用户 [Zhu-Xinrong \(Kendall\)](https://github.com/Zhu-Xinrong) 指导图标设计.

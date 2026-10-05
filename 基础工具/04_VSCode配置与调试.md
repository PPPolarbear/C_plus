# 04 · VS Code 配置与调试

> **适用**：日常用 VS Code，想学会调试配置和提效快捷键。
> **读 + 练约 1.5 小时。**

---

## ⚡ 现在就开始

```bash
cd /d D:\AI\C_plus_TEST
code .
```

`code .` 用 VS Code 打开当前目录。**记下这条命令，以后不用鼠标点。**

---

## 🎯 这一份解决什么问题

**你的 VS Code 现在多半只是个"带高亮的记事本"。**

配置好之后，它能：
- 一键编译 + 一键调试（不用切终端敲命令）
- 鼠标悬停看变量值、函数签名
- 直接跳转定义、查找引用
- 在编辑器里直接下断点

**每天能省 20–30 分钟。**

---

## 一、必装的扩展

按 `Ctrl+Shift+X` 打开扩展面板，装这三个：

| 扩展 | 作用 | 必需 |
|---|---|---|
| **C/C++**（Microsoft） | 语法高亮、智能提示、**调试** | 🔴 必须 |
| **CMake Tools**（Microsoft） | 在 VS Code 里配置/构建/调试 CMake 项目 | 🔴 必须 |
| **GitLens** | 显示每行是谁什么时候改的 | 🟡 推荐 |

**不要装**：
- 各种"XX 中文汉化主题"——菜单汉化了但报错还是英文，反而更难搜
- 多个 C++ 智能提示扩展同时装（会互相打架）

> **clangd 还是 C/C++？** `clangd` 提示更准但配置麻烦。**先用微软官方的 C/C++，够用。**

---

## 二、必学快捷键（15 个）

**别背，挑 5 个先用起来，一周后再加 5 个。**

### 编辑

| 快捷键 | 作用 |
|---|---|
| `Alt+↑` / `Alt+↓` | **整行上下移动**（最常用） |
| `Shift+Alt+↓` | **向下复制当前行** |
| `Ctrl+Shift+K` | 删除整行 |
| `Ctrl+/` | 注释/取消注释 |
| `Ctrl+D` | 选中下一个相同的词（多光标） |
| `Alt+点击` | 添加多光标 |
| `Ctrl+Alt+↓` | 向下加一个光标 |

### 导航

| 快捷键 | 作用 |
|---|---|
| `Ctrl+P` | **按文件名跳转**（最快） |
| `Ctrl+Shift+P` | **命令面板**（什么都能干） |
| `Ctrl+G` | 跳到指定行 |
| `F12` | 跳到定义 |
| `Shift+F12` | 查找所有引用 |
| `Alt+←` / `Alt+→` | 后退/前进（浏览历史） |
| `Ctrl+Shift+O` | 跳到文件内的符号（函数、类） |
| `` Ctrl+` `` | 打开/关闭终端 |

### 调试

| 快捷键 | 作用 |
|---|---|
| `F5` | **开始调试** |
| `F9` | **下/取消断点** |
| `F10` | 单步跳过（= gdb 的 `next`） |
| `F11` | 单步进入（= gdb 的 `step`） |
| `Shift+F5` | 停止调试 |
| `Shift+F11` | 跳出当前函数 |

> **`Ctrl+Shift+P` 是万能入口。** 忘了什么功能在哪，就敲这个然后输关键字。

---

## 三、`launch.json` —— 配置调试（**这份的重点**）

### 为什么需要它

`F5` 要能工作，VS Code 得知道：**运行哪个可执行文件？用哪个调试器？传什么参数？**

这些写在 `.vscode/launch.json` 里。

### 生成方式

1. 打开你的 `.cpp` 文件
2. 按 `F5`
3. VS Code 会问用什么调试器，选 **C++ (GDB/LLDB)**
4. 它会自动生成 `.vscode/launch.json`

### 一个够用的配置

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "调试当前 CMake 目标",
            "type": "cppdbg",
            "request": "launch",
            "program": "${command:cmake.launchTargetPath}",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "miDebuggerPath": "gdb",
            "setupCommands": [
                {
                    "description": "为 gdb 启用整齐打印",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ]
        }
    ]
}
```

**关键字段**：

| 字段 | 作用 |
|---|---|
| `program` | 要调试的可执行文件路径。用 `${command:cmake.launchTargetPath}` 会自动跟当前选中的 CMake 目标 |
| `args` | 命令行参数（`["./test.txt"]` 这样传） |
| `stopAtEntry` | `true` = 启动时停在 main 第一行 |
| `MIMode` | `gdb`（Linux/MINGW）或 `lldb`（Mac） |
| `miDebuggerPath` | gdb 在哪。PATH 里有就写 `gdb` 即可 |

### ⚠️ 调试必须用 Debug 编译

**这是新手最常踩的坑**：Release 模式下没有调试符号，断点会失效、变量显示 `<optimized out>`。

在 `CMakeLists.txt` 里确保：

```cmake
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
```

或者 CMake Tools 底部状态栏把它切到 `Debug`。

---

## 四、`.vscode/settings.json` —— 项目级设置

```json
{
    "files.associations": {
        "*.h": "cpp",
        "*.hpp": "cpp"
    },
    "editor.tabSize": 4,
    "editor.insertSpaces": true,
    "editor.rulers": [100],
    "files.trimTrailingWhitespace": true,
    "files.insertFinalNewline": true,
    "C_Cpp.default.cppStandard": "c++17",
    "C_Cpp.default.compilerPath": "g++",
    "cmake.configureOnOpen": false
}
```

| 设置 | 作用 |
|---|---|
| `files.associations` | 让 `.h` 文件按 C++ 高亮（不然默认可能是 C） |
| `editor.rulers` | 100 列处画一条竖线，提醒别写太长 |
| `trimTrailingWhitespace` | 保存时自动删掉行尾空格 |
| `insertFinalNewline` | 保存时自动在文件末尾加空行（Git 友好） |
| `cmake.configureOnOpen` | 打开项目时**不要**自动配置（大项目会很慢） |

> **改完记得把 `.vscode/` 加进 `.gitignore`** —— 这是个人偏好，不该提交。

**例外**：`launch.json` 里的调试配置如果是团队共享的，有些项目会提交。你自己练手就忽略掉。

---

## 五、CMake Tools 的用法

装好扩展后，**底部状态栏**会出现一排按钮：

```
[Build] [Debug] [main] [GCC 13.2.0] [Debug]
```

| 按钮 | 作用 |
|---|---|
| **Build** | 等于 `cmake --build build` |
| **Debug** | 编译 + 启动调试（等于 `F5`） |
| 目标选择 | 选当前编译哪个可执行文件 |
| 编译器 | 选 g++ / clang++ |
| **Debug/Release** | **切构建类型** |

**基本工作流**：

1. `Ctrl+Shift+P` → `CMake: Configure`（首次，等于 `cmake -B build`）
2. 底部选目标（比如 `app`）
3. `F5` 调试，或点 Build

---

## 六、在 VS Code 里直接用 gdb 的能力

调试启动后，左侧面板能看到：

| 面板 | 内容 |
|---|---|
| **VARIABLES** | 当前作用域所有变量（**鼠标悬停也能看**） |
| **WATCH** | 你自己盯着的表达式 |
| **CALL STACK** | 调用栈（等于 gdb 的 `bt`，点击可切换栈帧） |
| **BREAKPOINTS** | 所有断点（可设条件断点） |

**条件断点**：右键断点 → Edit Breakpoint → 输入条件，比如 `i == 500`。循环里只想停在第 500 次时特别有用。

**对照 Day 46–48 的 gdb 命令**：

| gdb 命令 | VS Code 等价 |
|---|---|
| `b file.cpp:42` | 点击行号左侧 |
| `r` | `F5` |
| `n` | `F10` |
| `s` | `F11` |
| `bt` | CALL STACK 面板 |
| `p x` | 悬停 / WATCH 面板 |
| `watch x` | WATCH 面板添加 |
| `c` | `F5`（继续） |

**建议**：**先用 gdb 命令行练熟（Day 46–48），再用 VS Code 的图形界面。** 面试只会问你 gdb，不会问你 VS Code 按钮在哪。

---

## 七、Windows 用户的进阶选项：WSL

你现在用的是 MINGW64。**以后实习和实验室大概率是 Linux 环境。**

VS Code 有 **WSL 扩展**，可以让你在 Windows 上写代码，实际在 Linux 子系统里编译运行：

```bash
# 先装 WSL（PowerShell 管理员）
wsl --install
```

然后 VS Code 装 **WSL** 扩展，左下角点 `><` 选 "Connect to WSL"。

**好处**：
- 真正的 Linux 环境（`perf`、`valgrind`、`apt` 都能用）
- 编译产物和部署环境一致
- 路径、权限、行尾符的坑全没了

**这一份不展开**，但建议你在开始 Day 16（阶段二）之前把 WSL 配好——**后面 `valgrind` 和 `perf` 在原生 Windows 上很难装。**

---

## ✅ 自测（不看小抄能做出来才算过）

- [ ] `code .` 能用命令行打开项目
- [ ] 装了 C/C++ 和 CMake Tools 扩展
- [ ] 会按文件名跳转（`Ctrl+P`）和跳定义（`F12`）
- [ ] `Alt+↑/↓` 移动整行、`Shift+Alt+↓` 复制整行
- [ ] **能在 VS Code 里下断点、`F5` 启动、`F10` 单步、看变量**
- [ ] 知道调试必须用 Debug 构建
- [ ] 会设条件断点
- [ ] 底部状态栏能切换 CMake 目标

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| `F5` 说找不到程序 | 先 `CMake: Configure` 并 Build 一次 |
| 断点变成空心圆（不生效） | 用了 Release 构建，切到 Debug |
| 变量显示 `<optimized out>` | 同上 |
| 头文件报红线但能编译 | `C_Cpp.default.includePath` 没配对，或用 CMake Tools 的 configure |
| 提示很慢 | 大项目正常。可以在设置里关掉 `C_Cpp.intelliSenseEngine` 的部分功能 |
| gdb 找不到 | `miDebuggerPath` 写成完整路径 |

---

## 🔜 下一份

`05_Vim快速编辑.md` —— 你提过想捡起来。

# Day 01 · 环境搭建与第一个多文件项目

**Block 1/19** · 阶段一（Day 1–15）· 主题：工具链与基础

---

## ⚡ 现在就开始

打开终端，敲这一行：

```bash
g++ --version && cmake --version && gdb --version && valgrind --version
```

**四个都有输出版本号？** 直接往下。
**缺哪个就先去装哪个**（Ubuntu：`sudo apt install build-essential cmake gdb valgrind`）。**没装齐不要往下看。**

---

## 🎯 今天唯一的目标

**能在纯命令行下，把一个多文件 C++ 项目编译成可执行文件。**

---

## 📖 读（约 30 分钟）

- learncpp「环境搭建」+「第一个程序」两节
- **只读这两节。** 别往下翻，别贪多。

---

## ✍️ 写（约 120 分钟）

建立这个结构（**不要用 IDE 一键生成，手动建**）：

```
cpp-camp/
├── CMakeLists.txt
├── include/
│   └── math_utils.h
└── src/
    ├── math_utils.cpp
    └── main.cpp
```

```cpp
// include/math_utils.h
#pragma once
int add(int a, int b);
int mul(int a, int b);
```

**手写 `CMakeLists.txt`**——一行行敲，不要复制：

```cmake
cmake_minimum_required(VERSION 3.16)
project(cpp_camp CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
add_executable(app src/main.cpp src/math_utils.cpp)
```

然后：

```bash
cmake -B build
cmake --build build
./build/app
```

---

## ✅ 验收（打勾才算过）

- [ ] 四个工具的 `--version` 都有输出
- [ ] `cmake -B build` 成功，生成了 `build/` 目录
- [ ] `cmake --build build` 成功，生成了可执行文件
- [ ] `./build/app` 打印出正确结果
- [ ] **全程没有点过 IDE 的一键编译按钮**

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| `cmake: command not found` | 装完后**重开终端**（PATH 没刷新） |
| `undefined reference to 'add(int, int)'` | `CMakeLists.txt` 里 `add_executable` 漏了 `math_utils.cpp` |
| `fatal error: math_utils.h: No such file` | 漏了 `include_directories(include)` |
| 改了代码但结果没变 | 记得重新 `cmake --build build` |

---

## 🔜 明天（Day 02）

理解编译→链接到底发生了什么，并加第二个模块。

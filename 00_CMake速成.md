# 00 · CMake 速成

> **Day 01 之前必读。** 1–2 小时，读完就够用 57 天。
> 训练营 Day 01 就让你手写 `CMakeLists.txt`，但你之前没接触过 CMake——这份补齐。
> **读完不用再找别的教程**，57 天需要的 CMake 知识全在这里。

---

## 一、CMake 解决什么问题

假设你有三个源文件：

```
main.cpp
math_utils.cpp
string_utils.cpp
```

最原始的编译方式：

```bash
g++ -std=c++17 -o app main.cpp math_utils.cpp string_utils.cpp
```

**四个麻烦**：

1. 文件一多，命令长到没法维护
2. 改一个文件要全部重编，慢
3. Windows / Linux / Mac 命令不一样
4. 链接第三方库（比如 pthread）时更复杂

`make` 能解决前两个，但**不跨平台**（Windows 上叫 nmake，Mac 上又是另一套）。

**CMake 四个都解决**：你写一份 `CMakeLists.txt`，它帮你生成对应平台的构建文件。

---

## 二、最核心的心智模型（**这一节最重要**）

> **`CMakeLists.txt` 不是构建脚本，它是"生成构建脚本的程序"。**

很多人学 CMake 卡住，就是把它当 Makefile 理解——**不对**。

正确理解是**两个阶段**：

```
        CMakeLists.txt
              │
              │  ① configure（配置）
              ▼
   Makefile / VS 工程 / ninja 文件      ← CMake 生成出来的
              │
              │  ② build（构建）
              ▼
          可执行文件
```

对应的两条命令：

```bash
cmake -B build          # ① 配置：读 CMakeLists.txt，在 build/ 里生成构建文件
cmake --build build     # ② 构建：调用上一步生成的东西，真正编译链接
```

**记住这个"两阶段"，CMake 90% 的困惑就没了。**

> 为什么写 `-B build`：`-B` 是 "build directory"，把生成物全塞进 `build/` 目录，**不污染源码目录**。这是必须养成的习惯（记得把 `build/` 写进 `.gitignore`）。

---

## 三、逐行拆解最小可用 CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.16)     # ① 声明最低 CMake 版本
project(cpp_camp CXX)                    # ② 项目名 + 语言
set(CMAKE_CXX_STANDARD 17)               # ③ 用 C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)      # ④ 编译器不支持就报错，别偷偷降级
add_executable(app src/main.cpp src/math_utils.cpp)   # ⑤ 要生成的可执行文件
```

| 行 | 干什么 | 不写会怎样 |
|---|---|---|
| ① | 声明最低版本 | 老版本 CMake 行为不一致 |
| ② | 项目名和语言 | 不写 `CXX` 可能不启用 C++ 编译器 |
| ③④ | 指定 C++ 标准 | 默认可能是 C++98，你的 `auto`/`nullptr` 全报错 |
| ⑤ | **核心**：源文件 → 可执行文件 | 没它什么都不生成 |

**关于 ② 里的 `CXX`**：C 语言写 `C`，C++ 写 `CXX`（因为 `+` 不能做标识符）。

**关于 ⑤**：`add_executable(目标名 源文件列表...)`
- 第一个参数 `app` 是**目标名**，也是生成的可执行文件名（Linux 下就是 `./build/app`）
- 后面的源文件**必须全部列出来**

---

## 四、现代 CMake：target 三件套

老教程里到处都是这种写法：

```cmake
include_directories(include)      # ❌ 老写法，全局污染
link_libraries(pthread)
```

**现在推荐"以 target 为中心"**：

```cmake
add_executable(app src/main.cpp)

target_include_directories(app PRIVATE include)      # ① 头文件在哪
target_link_libraries(app PRIVATE Threads::Threads)  # ② 链接什么库
target_compile_options(app PRIVATE -Wall -Wextra)    # ③ 编译选项
```

**为什么用 `target_*` 而不是全局命令**：

| | `include_directories` | `target_include_directories` |
|---|---|---|
| 作用范围 | **所有** target | **只有**指定的 target |
| 依赖传递 | 无法控制 | 可选 PRIVATE / PUBLIC / INTERFACE |

### PRIVATE / PUBLIC / INTERFACE

| 关键字 | 含义 | 什么时候用 |
|---|---|---|
| `PRIVATE` | 只有我自己用 | **90% 的情况，不确定就写这个** |
| `PUBLIC` | 我用，依赖我的人也要用 | 做库给别人用 |
| `INTERFACE` | 我不用，依赖我的人要用 | 纯头文件库 |

**现在只需要记住：不确定就写 `PRIVATE`。** Day 54 做工程化时会再碰到。

---

## 五、你 57 天里会用到的**全部** CMake 命令

就这些，**不用学别的**：

| 命令 | 作用 | 在哪天用 |
|---|---|---|
| `cmake_minimum_required` | 声明版本 | 每个文件开头 |
| `project` | 项目名 | 每个文件开头 |
| `set` | 设变量 | 设 C++ 标准 |
| `add_executable` | 生成可执行文件 | 天天用 |
| `add_library` | 生成库 | Day 54 |
| `target_include_directories` | 头文件路径 | Day 02 起 |
| `target_link_libraries` | 链接库 | Day 37、Day 54 |
| `target_compile_options` | 编译选项 | 可选 |
| `add_subdirectory` | 加子目录 | Day 54 |
| `enable_testing` / `add_test` | 注册测试 | Day 54 |
| `option` | 自定义开关 | Day 50（ASan） |
| `if` / `endif` | 条件判断 | Day 50 |

**CMake 有几百个命令，你只需要这 12 个。**

---

## 六、Debug / Release（**新手最容易忽略、后果最严重**）

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake -B build -DCMAKE_BUILD_TYPE=Release
```

| | Debug | Release |
|---|---|---|
| 优化级别 | `-O0` | `-O2` / `-O3` |
| 调试符号 | 有（能下断点、看变量） | 无 |
| 速度 | 慢 10–100 倍 | 快 |
| 什么时候用 | **调试、gdb、valgrind** | **benchmark、发布** |

**在 `CMakeLists.txt` 里设默认值**：

```cmake
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
```

⚠️ **Day 55 跑 benchmark 一定要 Release** —— 不然数字毫无意义。
⚠️ **Day 46–48 用 gdb 一定要 Debug** —— 不然变量全是 `<optimized out>`。

> **`-D` 是什么**：`-DXXX=YYY` 是给 CMake 传变量，相当于在 `CMakeLists.txt` 里写 `set(XXX YYY)`，但可以从命令行覆盖。

---

## 七、多目录项目

项目变大后，把文件分组：

```
mini-stl/
├── CMakeLists.txt          ← 顶层
├── include/
├── tests/
│   ├── CMakeLists.txt      ← 子目录
│   └── test_vector.cpp
└── bench/
    ├── CMakeLists.txt
    └── bench_vector.cpp
```

**顶层 `CMakeLists.txt`**：

```cmake
cmake_minimum_required(VERSION 3.16)
project(mini_stl CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 接口库：没有源文件，只是打包一组头文件路径
add_library(mini_stl INTERFACE)
target_include_directories(mini_stl INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

enable_testing()
add_subdirectory(tests)
add_subdirectory(bench)
```

**`tests/CMakeLists.txt`**：

```cmake
set(TEST_NAMES test_vector test_string test_diff)

foreach(name ${TEST_NAMES})
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE mini_stl)
    add_test(NAME ${name} COMMAND ${name})
endforeach()
```

**四个新概念**：

- **`add_library(名字 INTERFACE)`** —— 接口库。没有源文件，只是"打包一组头文件路径给别人用"。理解成「把 `include/` 目录公开出去」就够。
- **`foreach`** —— CMake 也有循环，避免重复写六遍。
- **`${CMAKE_CURRENT_SOURCE_DIR}`** —— 当前 `CMakeLists.txt` 所在目录的绝对路径。比写相对路径稳。
- **`${name}`** —— 取变量 `name` 的值。

---

## 八、ctest

```bash
cmake -B build
cmake --build build
cd build && ctest --output-on-failure
```

**`ctest` 会把所有 `add_test` 注册的测试跑一遍并汇总。**

| 参数 | 作用 |
|---|---|
| `--output-on-failure` | 只打印失败测试的输出（不然一片安静） |
| `-V` | 全部打印详细信息 |
| `-R 关键字` | 只跑名字匹配的测试 |

> Windows 上也可以 `ctest --test-dir build`，不用先 `cd`。

---

## 九、常见报错对照表（**收藏这一节**）

| 报错 | 原因 | 解法 |
|---|---|---|
| `cmake: command not found` | 没装，或 PATH 没刷新 | 装完**重开终端** |
| `undefined reference to 'add(int,int)'` | `add_executable` 里**漏了某个 .cpp** | 把文件加进去 |
| `fatal error: xxx.h: No such file` | 没设头文件路径 | 加 `target_include_directories` |
| `#include <xxx> not found` | 第三方库没链 | `target_link_libraries` |
| **改了 `CMakeLists.txt` 但没生效** | **没重新 configure** | 再跑一次 `cmake -B build` |
| **改了 .cpp 但结果没变** | 没重新 build | `cmake --build build` |
| `undefined reference to pthread_create` | 没链线程库 | `find_package(Threads REQUIRED)` + `target_link_libraries(app PRIVATE Threads::Threads)` |
| 一堆语法错误，像是 C++98 | 没设 C++ 标准 | 加 `set(CMAKE_CXX_STANDARD 17)` |
| **加了新 .cpp 但没被编译** | **CMake 不会自动发现源文件** | 必须手动加进 `add_executable` |
| `does not appear to contain CMakeLists.txt` | 当前目录没有 CMakeLists.txt | 检查路径 |

> **倒数第三条特别重要**：CMake **不会**自动扫描目录找源文件。加文件必须手动改 `CMakeLists.txt`。这是很多人的第一课，也是 Day 02 的验收点。

---

## 十、练习（**Day 01 之前做完**）

### 练习 1：从零建一个项目

```
cmake_practice/
├── CMakeLists.txt
├── include/hello.h
└── src/
    ├── hello.cpp
    └── main.cpp
```

- [ ] `cmake -B build` 成功
- [ ] `cmake --build build` 成功
- [ ] `./build/app` 打印正确内容
- [ ] C++17 生效（用 `auto` 和结构化绑定 `auto [a,b] = ...` 验证）
- [ ] 改一行代码，重新 build，发现**只有该文件被重编**（这就是增量构建）

### 练习 2：加一个文件（**体会"CMake 不自动发现"**）

新增 `src/greet.cpp` + `include/greet.h`，在 `main.cpp` 里调用 `greet()`。

- [ ] **先故意不加进 `CMakeLists.txt`**，观察报什么错
- [ ] 把错误信息抄下来
- [ ] 加进去，错误消失

### 练习 3：制造并修复错误

- [ ] 删掉 `target_include_directories`，看报错
- [ ] 删掉 `set(CMAKE_CXX_STANDARD 17)`，用 `auto` 看报错
- [ ] 用 `-DCMAKE_BUILD_TYPE=Debug` 重新配置，对比 `build/` 目录里的文件差异

### 练习 4：加一个子目录

把 `hello` 拆成一个库：

```cmake
add_library(hello STATIC src/hello.cpp)
target_include_directories(hello PUBLIC include)
```

然后 `main` 链接它：

```cmake
add_executable(app src/main.cpp)
target_link_libraries(app PRIVATE hello)
```

- [ ] 构建成功
- [ ] `build/` 里出现了库文件（`libhello.a`）

---

## 十一、暂时不用管的（Day 54 之后有兴趣再看）

- `find_package` 找第三方库的细节
- `install` / `export`
- `FetchContent`（自动下载依赖）
- CMake Presets
- 生成器表达式 `$<...>`
- `configure_file`

**57 天里用不到。看到别人写也别慌。**

---

## 十二、一张图记住

```
CMakeLists.txt  ──configure──▶  Makefile/ninja  ──build──▶  可执行文件
     │                              │                          │
  你写的                      CMake 生成的               你真正要的
     │                              │                          │
cmake -B build            cmake --build build            ./build/app
```

---

## 一句话

**CMake 的心智模型就一条：`CMakeLists.txt` 生成构建文件，构建文件生成可执行文件。**
**两条命令就够：`cmake -B build`，然后 `cmake --build build`。**

剩下的都是细节，遇到问题查第九节的表。

---

*读完这份再开始 Day 01。*

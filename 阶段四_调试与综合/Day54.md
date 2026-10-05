# Day 54 · CMake 工程化与项目结构

**Block 18/19** · 阶段四（Day 46–57）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && ls -R | head -40
```

---

## 🎯 今天唯一的目标

**把 53 天写散的文件整理成一个别人能 clone 下来就跑的仓库。**

`mini-stl` 从今天起是一个真正的项目，不是一堆练习。

---

## 📖 读（约 20 分钟）

- 搜「CMake 现代用法 target_link_libraries」
- 搜「GoogleTest CMake 集成」

---

## ✍️ 写（约 120 分钟）

### 目标结构

```
mini-stl/
├── CMakeLists.txt
├── README.md
├── .gitignore
├── LICENSE
├── include/
│   ├── myvector.h
│   ├── mystring.h
│   ├── myuniqueptr.h
│   ├── mysharedptr.h
│   ├── myoptional.h
│   └── scope_guard.h
├── tests/
│   ├── CMakeLists.txt
│   ├── test_vector.cpp
│   ├── test_string.cpp
│   ├── test_smartptr.cpp
│   ├── test_optional.cpp
│   ├── test_diff.cpp
│   └── test_exception.cpp
└── bench/
    ├── CMakeLists.txt
    └── bench_vector.cpp
```

### 顶层 CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.16)
project(mini_stl CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# ---- 库目标 ----
add_library(mini_stl INTERFACE)
target_include_directories(mini_stl INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# ---- ASan 开关 ----
option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
if(ENABLE_ASAN)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address)
endif()

# ---- 线程 ----
find_package(Threads REQUIRED)

# ---- 测试 ----
enable_testing()
add_subdirectory(tests)

# ---- 基准 ----
add_subdirectory(bench)
```

### tests/CMakeLists.txt

```cmake
set(TEST_NAMES
    test_vector
    test_string
    test_smartptr
    test_optional
    test_diff
    test_exception
)

foreach(name ${TEST_NAMES})
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE mini_stl Threads::Threads)
    add_test(NAME ${name} COMMAND ${name})
endforeach()
```

### 一条龙命令

```bash
cmake -B build
cmake --build build -j
cd build && ctest --output-on-failure
```

**`ctest` 会跑所有测试并汇总结果。**

### README.md 模板

```markdown
# mini-stl

从零实现的 C++ 标准库核心组件。

## 包含什么

| 组件 | 说明 |
|---|---|
| `MyVector<T>` | 动态数组，支持拷贝/移动语义、迭代器 |
| `MyString` | 字符串，支持运算符重载 |
| `MyUniquePtr<T>` | 独占所有权智能指针 |
| `MySharedPtr<T>` | 引用计数智能指针（原子计数） |
| `MyOptional<T>` | placement new 实现的可选值 |
| `ScopeGuard` | RAII 清理守卫 |

## 快速开始

    cmake -B build && cmake --build build -j
    cd build && ctest --output-on-failure

## 设计要点

- 全部使用 C++17，无第三方依赖
- 对拍测试：随机操作序列与 `std::` 标准库逐一比对
- 强异常安全保证（拷贝并交换 + 扩容回滚）
- valgrind / AddressSanitizer 零错误

## 已知限制

- 未实现 `weak_ptr`
- `MyVector` 未实现 `insert` / `erase`
- ...

## 许可

MIT
```

### .gitignore

```
build/
build-*/
*.o
*.log
vg.log
compile_commands.json
.cache/
```

---

## ✅ 验收（打勾才算过）

- [ ] 目录结构整理完成
- [ ] `cmake -B build && cmake --build build -j` 成功
- [ ] **`cd build && ctest` 跑通所有测试**
- [ ] README 写好（含项目说明、如何构建、设计要点、已知限制）
- [ ] `.gitignore` 写好
- [ ] `cmake -B build-asan -DENABLE_ASAN=ON` 也能构建

---

## ✅ Block 18 完成检查

- [ ] 异常安全的三个级别能讲清
- [ ] `MyVector` 有强异常安全保证
- [ ] 项目工程化（CMake + ctest + README）

**三条都打勾 → 进 Day 55。**

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| `ctest` 找不到测试 | 确认 `enable_testing()` 在顶层，`add_test` 在子目录 |
| INTERFACE 库链接报错 | INTERFACE 库只需要 `target_include_directories`，没有源文件 |
| ASan 和 valgrind 冲突 | 别同时用，分开两次构建 |

---

## 🔜 明天（Day 55）

benchmark——把你 57 天的成果变成简历上的数字。

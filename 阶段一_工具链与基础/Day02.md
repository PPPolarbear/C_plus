# Day 02 · 理解编译 → 链接

**Block 1/19** · 阶段一（Day 1–15）

---

## ⚡ 现在就开始

```bash
cd cpp-camp && ls -R
```

确认昨天的项目还在。**不在就先回去做 Day 01。**

---

## 🎯 今天唯一的目标

**亲手验证"编译"和"链接"是两个独立阶段。**

不是读懂，是**亲眼看到**它们各自会报什么错。

---

## 📖 读（约 30 分钟）

- learncpp 里讲"头文件 / 源文件 / 翻译单元"的部分
- 只读这一块

---

## ✍️ 写（约 120 分钟）

### 实验 1：制造一个编译错误

在 `math_utils.cpp` 里故意写：

```cpp
int add(int a, int b) {
    return a + b    // ← 故意漏分号
}
```

```bash
cmake --build build
```
看报错信息——**这是"编译错误"**。记下报错格式，改回来。

### 实验 2：制造一个链接错误

把 `math_utils.cpp` 里的 `mul` 实现**删掉**（保留头文件声明）。

```bash
cmake --build build
```
看报错——**这是"链接错误"**，形状和上面完全不同。记下来，改回来。

### 实验 3：加第三个模块

新增 `include/string_utils.h` + `src/string_utils.cpp`：

```cpp
// include/string_utils.h
#pragma once
#include <string>
std::string greet(const std::string& name);
int         str_length(const std::string& s);
```

在 `main.cpp` 里调用，并**在 CMakeLists.txt 里把它加进 `add_executable`**。

### 实验 4：验证 `#pragma once`

在 `main.cpp` 里 `#include "math_utils.h"` 写两次。能编译通过吗？
然后把 `#pragma once` 注释掉，再试一次。

---

## ✅ 验收（打勾才算过）

- [ ] 能**说出**编译错误和链接错误的区别
- [ ] 亲眼看到过这两种错误各自的报错长什么样
- [ ] 第三个模块加入后编译运行成功
- [ ] 能解释 `#pragma once` 挡住了什么
- [ ] 能说出 `.h` 为什么叫"声明"、`.cpp` 为什么叫"定义"

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| 加了新 .cpp 但报 undefined reference | `CMakeLists.txt` 的 `add_executable` 要手动加上它（**不会自动发现**） |
| 加了 `#pragma once` 还是重复定义 | 检查是不是把**函数定义**写进了 `.h` 文件 |

---

## 🔜 明天（Day 03）

完整流程跑通 + gdb 初体验，阶段一第一个小节点。

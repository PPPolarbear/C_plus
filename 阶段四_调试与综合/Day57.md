# Day 57 · ⭐ 里程碑 4：开源 + 简历条目

**Block 19/19** · 阶段四（Day 46–57）· **全程收尾**

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && cmake -B build && cmake --build build -j && cd build && ctest
```

---

## 🎯 今天唯一的目标

**把 57 天的成果变成别人能看到、能验证、能记住的东西。**

---

## 📖 读（约 20 分钟）

- **不读新章节。**逐项核对本项目 `README.md` 中的功能、构建命令、测试方法和已知限制；确保这些说明与最终代码一致。今天只做收尾。

---

## ✍️ 写（约 120 分钟）

### 任务 1：完善 README（**最重要**）

README 是别人对你项目的第一印象。按这个结构写：

```markdown
# mini-stl

从零实现的 C++ 标准库核心组件，无第三方依赖。

## 包含什么

| 组件 | 说明 | 关键技术点 |
|---|---|---|
| `MyVector<T>` | 动态数组 | 2 倍扩容、copy-and-swap、移动语义、随机访问迭代器 |
| `MyString` | 字符串 | RAII、运算符重载、友元 |
| `MyUniquePtr<T>` | 独占所有权智能指针 | 禁止拷贝、移动转移 |
| `MySharedPtr<T>` | 引用计数智能指针 | 原子计数、控制块 |
| `MyOptional<T>` | 可选值 | placement new、`alignas` |
| `ScopeGuard` | RAII 清理守卫 | 析构回调 |

## 快速开始

    cmake -B build -DCMAKE_BUILD_TYPE=Release
    cmake --build build -j
    cd build && ctest --output-on-failure

## 测试与验证

- **对拍测试**：`test_diff.cpp` 随机生成 10⁴ 次操作序列，与 `std::` 标准库逐一比对
- **异常安全**：`test_exception.cpp` 用会随机抛异常的 `Thrower` 验证强保证
- **内存检查**：valgrind / AddressSanitizer 全部零错误

## 性能数据

（把 Day 55 的表格贴进来）

## 设计取舍

- `reallocate` 采用 2 倍增长：均摊 O(1)，但最多浪费 50% 空间
- 拷贝赋值用 copy-and-swap：天然强异常安全，代价是多一次拷贝
- `MySharedPtr` 用堆上独立计数（而非控制块）：便于理解，但比标准库多一次分配

## 已知限制

- 未实现 `weak_ptr`（因此无法解决循环引用）
- `MyVector` 未实现 `insert` / `erase`
- `MyString` 未实现 `find` / `substr`
- 未做并发场景下的完整验证
```

### 任务 2：推到 GitHub

```bash
cd cpp-camp3
git init
git add .
git commit -m "feat: 从零实现 C++ 标准库核心组件

- MyVector: 2倍扩容、copy-and-swap、移动语义、迭代器
- MyString: RAII、运算符重载
- MyUniquePtr / MySharedPtr: 独占与引用计数（原子）
- MyOptional: placement new + alignas
- 对拍测试与 valgrind 验证通过"

# 在 GitHub 上建仓库后：
git remote add origin git@github.com:你的用户名/mini-stl.git
git branch -M main
git push -u origin main
```

**记得加 `LICENSE`（MIT 一行就行）。**

### 任务 3：写简历条目

把 57 天压缩成 4-5 行：

> **mini-STL：C++ 标准库核心组件复刻**（个人项目，2027.01–2027.03）
> `C++17` `CMake` `valgrind` `AddressSanitizer`
>
> - 从零实现 `MyVector` / `MyString` / `MyUniquePtr` / `MySharedPtr` / `MyOptional` 五个组件，覆盖 RAII、拷贝与移动语义、模板、placement new
> - 编写**对拍测试**：随机生成 10⁴ 次操作序列与 `std::` 标准库逐轮比对，通过率 100%；`valgrind` 与 ASan 零错误
> - `MyVector` 采用 2 倍扩容策略，基于 `emplace_back` 消除非平凡类型的拷贝开销（对比 `push_back`）
> - 通过 copy-and-swap 与扩容回滚实现**强异常安全保证**，用会随机抛异常的测试类验证
> - 源码：github.com/你的用户名/mini-stl

**⚠️ 数字必须是真的。** 面试官会追问每一个数据。

### 任务 4：全程复盘

填进 `进度追踪.md`：

| 问题 | 回答 |
|---|---|
| 总共写了多少行代码？ | （`find . -name "*.h" -o -name "*.cpp" \| xargs wc -l`） |
| 对拍抓出过几个 bug？ | |
| 最自豪的一个实现？ | |
| 最卡的地方？ | |
| 哪个知识点现在还没讲清？ | |

### 任务 5：面试预演（**最重要的收尾**）

**对着空气模拟一场 20 分钟的项目面试**，自问自答：

1. 介绍一下这个项目
2. `MyVector` 的扩容策略为什么是 2 倍？
3. 移动构造为什么必须 `noexcept`？
4. `shared_ptr` 是线程安全的吗？
5. 你的 `MyOptional` 为什么不能用 `T data_`？
6. copy-and-swap 为什么能提供强异常安全？
7. 跳过到函数调用时，栈上发生了什么？
8. 你这个项目和 `std::` 的差距在哪？为什么？

**第 8 题最难，也最能体现深度。**

---

## ✅ 验收（打勾才算过）

- [ ] README 完整（含性能数据、设计取舍、已知限制）
- [ ] 代码推到 GitHub，仓库能公开访问
- [ ] **clone 下来能一次构建成功**（找个同学试，或者换个目录自己试）
- [ ] 简历条目写好，**每个数字都经得起追问**
- [ ] 复盘表填完
- [ ] **八个面试问题能脱稿回答**

---

## 🎯 全程总验收

| 阶段 | 里程碑 | 打勾 |
|---|---|---|
| 一 | ScopeGuard，理解 RAII | ☐ |
| 二 | MyVector 对拍通过 | ☐ |
| 三 | MySharedPtr 手写完成 | ☐ |
| 四 | **开源项目 + 简历条目** | ☐ |

| 能力 | 验证方式 | 打勾 |
|---|---|---|
| RAII 与资源管理 | 能讲清为什么 C++ 不需要 GC | ☐ |
| 移动语义 | 能解释 `std::move` 到底做了什么 | ☐ |
| 智能指针 | 能回答 `shared_ptr` 的线程安全边界 | ☐ |
| 模板 | 能写类模板、可变参数模板 | ☐ |
| 调试 | 能用 gdb + valgrind 定位任意内存问题 | ☐ |
| 工程 | 能组织多模块 CMake 项目 | ☐ |

---

## 🎓 你现在的水平

对照 `04_C++要不要学_落实方案.md` 里的五级标准：

| 级别 | 目标 | 达成 |
|---|---|---|
| L1 会管资源（实习入场券） | 2027.6 | ✅ |
| L2 懂原理（大厂校招门槛） | 2028.8 | ✅ |
| L3 能诊断 | 2028.8 | ✅ |

**你提前一年半达到了原定 2028 年的目标。**

---

## 🔜 接下来

1. **别停**。`04_C++要不要学_落实方案.md` 的 §6 里有四条分支路线，现在可以开始选一条深入了
2. **LeetCode 继续每天 1–2 题**，用 C++ 写（复试机试 + 技术面试）
3. **408 继续**，把考研复习和技术地基合并成一件事

---

## 一句话

**57 天前你说"有基础但迷茫"。**
**现在你有一个能跑、能测、有数据、开源的项目，和一个能讲 20 分钟的故事。**

这不是终点，是你简历的第一段。

---

*训练营结束。新的开始。*

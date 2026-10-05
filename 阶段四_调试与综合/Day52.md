# Day 52 · 异常基础与测试类

**Block 18/19** · 阶段四（Day 46–57）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && touch tests/test_exception.cpp
```

---

## 🎯 今天唯一的目标

**造一个"会随机抛异常的 T"，用它来测试你的容器。**

明天的异常安全改造，全靠今天这个测试类。

---

## 📖 读（约 30 分钟）

- learncpp：异常章节
- 搜「C++ 异常安全 三个级别」

---

## ✍️ 写（约 120 分钟）

### 任务 1：异常安全的三个级别（先记住概念）

| 级别 | 保证 | 说明 |
|---|---|---|
| **基本保证** | 不泄漏、不破坏不变量 | 对象可用，但状态可能改变 |
| **强保证** | 要么成功，要么回滚到原状态 | 事务性 |
| **不抛保证** | 承诺不抛异常 | 只能用于 `noexcept` 函数 |

### 任务 2：写一个会抛异常的 T

```cpp
// tests/thrower.h
#pragma once
#include <stdexcept>
#include <iostream>

class Thrower {
public:
    static int throw_every;      // 每 N 次拷贝抛一次
    static int copy_count;

    int v = 0;

    explicit Thrower(int x = 0) : v(x) {}

    Thrower(const Thrower& o) : v(o.v) {
        ++copy_count;
        if (throw_every > 0 && copy_count % throw_every == 0) {
            throw std::runtime_error("Thrower: 拷贝失败");
        }
    }

    Thrower(Thrower&& o) noexcept : v(o.v) {}       // 移动不抛

    Thrower& operator=(const Thrower& o) {
        v = o.v;
        ++copy_count;
        if (throw_every > 0 && copy_count % throw_every == 0) {
            throw std::runtime_error("Thrower: 赋值失败");
        }
        return *this;
    }

    Thrower& operator=(Thrower&& o) noexcept { v = o.v; return *this; }
};

int Thrower::throw_every = 0;
int Thrower::copy_count  = 0;
```

### 任务 3：基础异常测试

```cpp
void test_basic_exception() {
    // 1. 基本捕获
    try {
        throw std::runtime_error("test");
    } catch (const std::exception& e) {
        std::cout << "捕获: " << e.what() << "\n";
    }

    // 2. 栈展开时会析构局部对象
    struct Guard {
        ~Guard() { std::cout << "Guard 析构（栈展开时）\n"; }
    };
    try {
        Guard g;
        throw std::runtime_error("boom");
    } catch (...) {
        std::cout << "捕获到\n";
    }

    // 3. 不要在析构函数里抛异常
    //    （会导致 std::terminate）
}
```

### 任务 4：观察你的 MyVector 在异常下的表现

```cpp
void test_vector_exception() {
    Thrower::throw_every = 3;
    Thrower::copy_count  = 0;

    MyVector<Thrower> v;
    v.reserve(2);            // 先预留，减少扩容干扰

    try {
        for (int i = 0; i < 20; ++i) {
            v.push_back(Thrower(i));
        }
    } catch (const std::exception& e) {
        std::cout << "push_back 抛异常: " << e.what() << "\n";
        std::cout << "此时 v.size() = " << v.size() << "\n";
        v.dump();            // ← 容器还是完好的吗？
    }

    // 关键：之后还能正常使用吗？
    v.push_back(Thrower(999));
    std::cout << "异常后仍可 push_back，size = " << v.size() << "\n";
}

// 跑完后检查：Thrower 的构造次数 == 析构次数（没有泄漏）
```

**今天的目标不是修好它，是观察它现在什么表现。**

---

## ✅ 验收（打勾才算过）

- [ ] `Thrower` 能在第 N 次拷贝时抛异常
- [ ] 基础异常测试三项都跑通
- [ ] 观察到 `MyVector` 在扩容抛异常时会怎样
- [ ] **valgrind 检查有没有泄漏**（可能会漏，这正是明天要修的）
- [ ] 能说出异常安全的**三个级别**分别是什么
- [ ] 能说出为什么**析构函数里不能抛异常**

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| 静态成员定义报错 | 在头文件里定义会被多文件重复定义，正式项目放 `.cpp` |
| 异常没被捕获导致崩溃 | 加 `catch (...)` 兜底 |
| 观察不出问题 | 把 `throw_every` 调到 1 或 2，让它抛得更频繁 |

---

## 🔜 明天（Day 53）

给 MyVector 加强异常安全保证。

# Day 38 · 循环引用实验

**Block 13/19** · 阶段三（Day 31–45）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && touch tests/test_cycle.cpp
```

---

## 🎯 今天唯一的目标

**亲手制造一个内存泄漏——不是崩溃，是"静悄悄的漏"。**

`shared_ptr` 有个致命弱点：**互相引用时永远不释放**。今天把它造出来，并理解 `weak_ptr` 为什么存在。

---

## 📖 读（约 30 分钟）

- LearnCpp 22.7 “Circular dependency issues with std::shared_ptr, and std::weak_ptr”
- 只读强引用形成环、对象无法释放的原因，以及 `std::weak_ptr` 如何打破环

---

## ✍️ 写（约 120 分钟）

### 任务 1：制造循环引用

```cpp
#include "mysharedptr.h"
#include <iostream>
#include <string>

struct Node {
    std::string name;
    MySharedPtr<Node> next;        // ← 强引用

    explicit Node(const std::string& n) : name(n) {
        std::cout << "构造 " << name << "\n";
    }
    ~Node() {
        std::cout << "析构 " << name << "\n";
    }
};

int main() {
    {
        MySharedPtr<Node> a(new Node("A"));
        MySharedPtr<Node> b(new Node("B"));

        a->next = b;      // A 引用 B
        b->next = a;      // B 引用 A  ← 循环！

        std::cout << "a.use_count = " << a.use_count() << "\n";   // 2
        std::cout << "b.use_count = " << b.use_count() << "\n";   // 2
    }   // ← 离开作用域

    std::cout << "离开作用域了\n";
    return 0;
}
```

**观察**：有没有打印"析构 A"和"析构 B"？

**答案是没有。** 因为：
- `a` 析构 → A 的计数 2→1（`b->next` 还持有）
- `b` 析构 → B 的计数 2→1（`a->next` 还持有）
- 两个计数都停在 1，**谁都不释放**

### 任务 2：用 valgrind 确认泄漏

```bash
valgrind --leak-check=full ./build/test_cycle
```

**应该看到 `definitely lost` 或者 `still reachable` 的块。** 抄下来。

### 任务 3：改成 weak_ptr（先只理解概念）

真实的解法是让**其中一个方向用弱引用**：

```cpp
// std::weak_ptr 不增加强引用计数
struct Node {
    std::string name;
    std::shared_ptr<Node> next;    // 强
    std::weak_ptr<Node>   prev;    // 弱 ← 不阻止释放
    // ...
};
```

**今天不实现 `MyWeakPtr`**（那是进阶内容）。但要能说清：

> `weak_ptr` 观察对象但**不拥有**它。它不增加强引用计数，所以不会阻止对象释放。使用时需要用 `lock()` 提升为 `shared_ptr`，并检查是否已失效。

### 任务 4：改写实验，验证理解

把 `next` 改成裸指针（模拟"弱引用"）：

```cpp
struct Node {
    std::string name;
    Node* next = nullptr;      // ← 不拥有
    // ...
};
```

**现在应该能正常析构了。** 但要自己管 `next` 的生命周期——这正是 `weak_ptr` 帮你做的事。

---

## ✅ 验收（打勾才算过）

- [ ] **亲眼看到"析构 A/B"没有打印**（对象泄漏了）
- [ ] valgrind 报出了泄漏
- [ ] 能画出循环引用的图并解释为什么计数停在 1
- [ ] 能说出 `weak_ptr` 解决什么问题
- [ ] 能说出 `weak_ptr` 不增加强引用计数
- [ ] 能说出 `weak_ptr` 使用时为什么要 `lock()` 并检查

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| 没看到泄漏 | 检查 `a->next = b` 和 `b->next = a` 是不是都写了 |
| valgrind 只报 `still reachable` | 也算泄漏（对象没析构），重点是**析构函数没被调用** |
| 想直接实现 weak_ptr | 今天不用，Day 44 之后有兴趣再展开 |

---

## 🔜 明天（Day 39）

多线程计数实验 + Block 13 验收。

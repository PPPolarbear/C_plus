# Day 13 · 析构函数 + Tracker 实验

**Block 5/19** · 阶段一（Day 1–15）

---

## ⚡ 现在就开始

```bash
cd cpp-camp2 && touch include/tracker.h src/tracker.cpp
```

---

## 🎯 今天唯一的目标

**亲眼看到对象在什么时候被构造、什么时候被析构。**

这是理解 RAII 的最后一块拼图——**析构函数的调用时机是确定的、可预测的**。

---

## 📖 读（约 40 分钟）

- learncpp：析构函数
- 搜「C++ 对象 生命周期 作用域」

---

## ✍️ 写（约 110 分钟）

### 任务 1：Tracker 类

```cpp
// include/tracker.h
#pragma once
#include <string>

class Tracker {
public:
    explicit Tracker(const std::string& name);
    ~Tracker();
    Tracker(const Tracker& other);              // 拷贝构造
    Tracker& operator=(const Tracker& other);   // 拷贝赋值
private:
    std::string name_;
};
```

每个函数被调用时打印一行，例如：
```
构造: A
构造: B
析构: B
析构: A
```

### 任务 2：六个实验（**今天的重点**）

在 `main` 里分别做，每次只看对应输出：

```cpp
// 实验 1：栈对象的析构顺序
void exp1() {
    Tracker a("A");
    Tracker b("B");
}   // 离开作用域，析构顺序？

// 实验 2：块作用域
void exp2() {
    Tracker a("A");
    { Tracker b("B"); }     // b 在这里就析构了
    Tracker c("C");
}

// 实验 3：提前 return
void exp3() {
    Tracker a("A");
    return;                 // a 会析构吗？
}

// 实验 4：抛异常
void exp4() {
    Tracker a("A");
    throw std::runtime_error("boom");    // a 会析构吗？
}

// 实验 5：堆对象
void exp5() {
    Tracker* p = new Tracker("heap");
    delete p;               // 不 delete 会怎样？
}

// 实验 6：拷贝
void exp6() {
    Tracker a("A");
    Tracker b = a;          // 会调用什么？
}
```

**每个实验都跑一遍，把输出抄下来。**

---

## ✅ 验收（打勾才算过）

- [ ] 实验 1：能说出析构顺序是**栈的反序**（后构造的先析构）
- [ ] 实验 2：`b` 在块结束时立即析构，不是函数结束时
- [ ] 实验 3：`return` 时 `a` **会**析构
- [ ] 实验 4：**抛异常时 `a` 也会析构** ← 这一条是 RAII 能工作的关键
- [ ] 实验 5：`new` 出来的对象**不会自动析构**，必须 `delete`
- [ ] 实验 6：能说出调用了构造函数还是拷贝构造
- [ ] 能用一句话说出：**"栈对象的析构是确定性的，堆对象不是"**

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| 实验 5 里没看到析构 | 正常，`new` 的对象要手动 `delete`。这正是问题所在 |
| 实验 4 里程序终止了 | 异常没被 catch 会终止程序，输出可能被冲掉。加个 `try-catch` |
| 输出顺序乱了 | `std::cout` 用 `"\n"` 而不是 `std::endl` 时可能缓冲，改成 `std::endl` |

---

## 🔜 明天（Day 14）

写 ScopeGuard——把今天的理解变成能用的工具。

# Day 32 · MyUniquePtr 骨架

**Block 11/19** · 阶段三（Day 31–45）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && touch include/myuniqueptr.h
```

---

## 🎯 今天唯一的目标

**写一个"独占所有权"的智能指针——核心是"禁止拷贝"。**

这是三种智能指针里最简单的，读懂它就理解了"所有权"这个概念。

---

## 📖 读（约 30 分钟）

- 搜「C++ unique_ptr 实现原理」
- 搜「C++ 所有权 ownership」

---

## ✍️ 写（约 120 分钟）

```cpp
// include/myuniqueptr.h
#pragma once
#include <utility>

template <typename T>
class MyUniquePtr {
public:
    // ---------- 构造与析构 ----------
    MyUniquePtr() noexcept : ptr_(nullptr) {}
    explicit MyUniquePtr(T* p) noexcept : ptr_(p) {}

    ~MyUniquePtr() { delete ptr_; }

    // ---------- 禁止拷贝（今天重点）----------
    MyUniquePtr(const MyUniquePtr&)            = delete;
    MyUniquePtr& operator=(const MyUniquePtr&) = delete;

    // ---------- 访问 ----------
    T& operator*()  const { return *ptr_; }
    T* operator->() const { return ptr_;  }
    T* get()        const noexcept { return ptr_; }

    explicit operator bool() const noexcept { return ptr_ != nullptr; }

private:
    T* ptr_;
};
```

### 三个关键设计

**① `explicit` 构造函数**

```cpp
MyUniquePtr<int> p = new int(5);     // ❌ 有 explicit 时编译失败
MyUniquePtr<int> p(new int(5));      // ✅
MyUniquePtr<int> p = MyUniquePtr<int>(new int(5));  // ✅
```
防止裸指针被**隐式**转成智能指针，避免"所有权悄悄转移"的意外。

**② `explicit operator bool`**

```cpp
MyUniquePtr<int> p(new int(5));
if (p) { ... }            // ✅
bool b = p;               // ❌ 有 explicit 时编译失败
int  x = p + 1;           // ❌ 防止意外参与算术运算
```

**③ 禁止拷贝 = 独占所有权**

```cpp
MyUniquePtr<int> a(new int(5));
// MyUniquePtr<int> b = a;    // ❌ 编译失败 —— 这就是"独占"
```

**先想清楚**：如果允许拷贝，`a` 和 `b` 都会在析构时 `delete` 同一个指针 → **double free**，就是 Day 17 那个病。

---

## ✅ 验收（打勾才算过）

- [ ] `MyUniquePtr<int> p(new int(42)); std::cout << *p;` 正常
- [ ] **尝试拷贝会编译失败**（这是正确行为，抄下报错信息）
- [ ] `if (p) {...}` 能用；`bool b = p;` 编译失败
- [ ] `MyUniquePtr<Foo> p(new Foo()); p->method();` 能用
- [ ] valgrind 零泄漏
- [ ] 能说出 `explicit operator bool` 挡掉了什么

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| `explicit operator bool` 语法报错 | 就是 `explicit operator bool() const noexcept`，没有返回类型 |
| 想拷贝但被禁止 | 这正是设计意图。要转移所有权就等明天写移动 |
| valgrind 报 leak | 检查析构里是不是 `delete ptr_`（不是 `delete[]`） |

---

## 🔜 明天（Day 33）

给 MyUniquePtr 加移动、release、reset。

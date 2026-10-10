# Day 35 · MySharedPtr 构造与析构

**Block 12/19** · 阶段三（Day 31–45）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && touch include/mysharedptr.h
```

---

## 🎯 今天唯一的目标

**实现最核心的三件事：构造、析构、release。**

析构函数是全部逻辑的集中地——**计数减到 0 才真正释放**。

---

## 📖 读（约 20 分钟）

- **不读新内容。**开始实现前回看 LearnCpp 22.6 “std::shared_ptr”中的构造、引用计数和析构行为
不读新内容，直接实现。

---

## ✍️ 写（约 120 分钟）

```cpp
// include/mysharedptr.h
#pragma once
#include <utility>

template <typename T>
class MySharedPtr {
public:
    // ---------- 构造 ----------
    MySharedPtr() noexcept
        : ptr_(nullptr), ref_count_(nullptr) {}

    explicit MySharedPtr(T* p)
        : ptr_(p), ref_count_(p ? new long(1) : nullptr) {}

    // ---------- 析构（今天重点）----------
    ~MySharedPtr() { release(); }

    // ---------- 访问 ----------
    T& operator*()  const { return *ptr_; }
    T* operator->() const { return ptr_;  }
    T* get()        const noexcept { return ptr_; }
    long use_count() const noexcept { return ref_count_ ? *ref_count_ : 0; }
    explicit operator bool() const noexcept { return ptr_ != nullptr; }

private:
    // 计数 -1，为 0 则真正释放
    void release() noexcept {
        if (ref_count_ == nullptr) return;

        if (--(*ref_count_) == 0) {
            delete ptr_;              // 释放对象
            delete ref_count_;        // 释放计数
        }
        ptr_ = nullptr;
        ref_count_ = nullptr;
    }

    T*    ptr_;
    long* ref_count_;
};
```

### 三个易错点

**① 空指针的特判**

```cpp
MySharedPtr<int> p;              // ptr_ = ref_count_ = nullptr
// release() 里必须先判断 ref_count_ 是否为空，否则解引用 nullptr
```

**② `--(*ref_count_)` 的括号**

```cpp
--*ref_count_        // 也能编译，但读起来容易误解
--(*ref_count_)      // 明确：先解引用，再自减
```

**③ 先减再判断**

```cpp
if (--(*ref_count_) == 0) { ... }   // ✅ 减完再比
if (*ref_count_-- == 0) { ... }     // ❌ 先比再减，逻辑反了
```

### 测试

```cpp
{
    MySharedPtr<int> a(new int(5));
    std::cout << a.use_count() << "\n";    // 1
}
// 离开作用域 → 计数 1→0 → 释放
// valgrind 应该干净
```

---

## ✅ 验收（打勾才算过）

- [ ] 创建后 `use_count() == 1`
- [ ] 离开作用域后 valgrind **零泄漏**（说明析构正确释放了）
- [ ] 默认构造的 `MySharedPtr` 析构不崩溃
- [ ] 能解释 `release()` 里为什么要先判断 `ref_count_ == nullptr`
- [ ] 能说出 `--(*ref_count_) == 0` 和 `(*ref_count_)-- == 0` 的区别

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| 默认构造析构崩溃 | `release()` 开头没判空 |
| valgrind 报泄漏 | 检查两个 `delete` 是否都执行了（对象 + 计数） |
| `use_count()` 返回垃圾 | `ref_count_` 为空时返回 0，别直接解引用 |

---

## 🔜 明天（Day 36）

拷贝/移动构造赋值——让计数真正被共享。

# Day 44 · MyOptional 实现

**Block 15/19** · 阶段三（Day 31–45）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && touch include/myoptional.h
```

---

## 🎯 今天唯一的目标

**实现一个 `MyOptional<T>`，支持 `T` 是不可默认构造的类型。**

这是今天最大的技术点：**不能用 `T data_;`**，因为那样会强制 `T` 必须可默认构造。

---

## 📖 读（约 20 分钟）

- LearnCpp 12.15 “std::optional”
- 对照 cppreference `std::optional` 页面中的 “Observers”与 “Monadic operations”之外的基础 “Modifiers”接口，聚焦 `has_value()`、`value()`、`reset()`、`emplace()`

---

## ✍️ 写（约 120 分钟）

```cpp
// include/myoptional.h
#pragma once
#include <new>
#include <stdexcept>
#include <utility>

template <typename T>
class MyOptional {
public:
    // ---------- 构造 ----------
    MyOptional() noexcept : has_value_(false) {}

    MyOptional(const T& value) : has_value_(true) {
        new (storage_) T(value);
    }

    MyOptional(T&& value) : has_value_(true) {
        new (storage_) T(std::move(value));
    }

    MyOptional(const MyOptional& other) : has_value_(other.has_value_) {
        if (has_value_) new (storage_) T(other.value());
    }

    MyOptional(MyOptional&& other) noexcept : has_value_(other.has_value_) {
        if (has_value_) new (storage_) T(std::move(other.value()));
    }

    // ---------- 析构 ----------
    ~MyOptional() { reset(); }

    // ---------- 赋值 ----------
    MyOptional& operator=(const MyOptional& other) {
        if (this != &other) {
            if (has_value_ && other.has_value_) {
                value() = other.value();        // 都有值：直接赋值
            } else if (has_value_) {
                reset();                        // 自己有、对方没有
            } else if (other.has_value_) {
                new (storage_) T(other.value()); // 自己有、对方有
                has_value_ = true;
            }
        }
        return *this;
    }

    // ---------- 状态 ----------
    bool has_value() const noexcept { return has_value_; }
    explicit operator bool() const noexcept { return has_value_; }

    // ---------- 访问 ----------
    T& value() {
        if (!has_value_) throw std::runtime_error("MyOptional: 无值");
        return *ptr();
    }
    const T& value() const {
        if (!has_value_) throw std::runtime_error("MyOptional: 无值");
        return *ptr();
    }

    T value_or(const T& default_val) const {
        return has_value_ ? *ptr() : default_val;
    }

    T& operator*()  { return *ptr(); }
    T* operator->() { return ptr();  }

    // ---------- 重置 ----------
    void reset() noexcept {
        if (has_value_) {
            ptr()->~T();          // 手动析构
            has_value_ = false;
        }
    }

private:
    T* ptr()             { return reinterpret_cast<T*>(storage_); }
    const T* ptr() const { return reinterpret_cast<const T*>(storage_); }

    alignas(T) unsigned char storage_[sizeof(T)];
    bool has_value_;
};
```

### 三个关键点

**① `alignas(T) unsigned char storage_[sizeof(T)]`**

不能用 `T data_;`，因为：
- 会强制要求 `T` 可默认构造
- 会**立刻**构造一个 `T`，哪怕"无值"状态下也需要

用原始缓冲区 + placement new，才能做到"有值时才构造"。

**② 析构必须手动**

因为 `storage_` 是 `unsigned char` 数组，编译器**不会**自动调用 `T` 的析构函数。必须 `reset()` 里手动调。

**③ 拷贝赋值要分三种情况**

| 自己 | 对方 | 处理 |
|---|---|---|
| 有值 | 有值 | 直接赋值（不重新构造） |
| 有值 | 无值 | 析构自己 |
| 无值 | 有值 | 原地构造 |

---

## ✅ 验收（打勾才算过）

- [ ] `MyOptional<int> o;` → `has_value() == false`
- [ ] `MyOptional<int> o(42);` → `*o == 42`
- [ ] **能存不能默认构造的类型**（写一个 `struct NoDefault { NoDefault(int); };` 测试）
- [ ] 无值时 `value()` 抛异常
- [ ] `value_or(99)` 在无值时返回 99
- [ ] valgrind 零泄漏（**特别注意 `reset()` 有没有真的析构**）
- [ ] 能解释为什么不能用 `T data_;`

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| `alignas(T) unsigned char` 报错 | 需要 `#include <new>` 和 C++17 |
| `ptr()->~T()` 不认 | 有些编译器要写 `ptr()->~T()` 或用 `std::destroy_at(ptr())` |
| 拷贝赋值漏了分支 | 对照上面那张三种情况的表 |

---

## 🔜 明天（Day 45）

MyOptional 测试 + 阶段三验收。**里程碑 3。**

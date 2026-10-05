# Day 33 · 移动、release、reset

**Block 11/19** · 阶段三（Day 31–45）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && cmake --build build
```

---

## 🎯 今天唯一的目标

**补全所有权转移的接口——`unique_ptr` 的核心能力全在今天。**

---

## 📖 读（约 30 分钟）

- 查 [cppreference 中文](https://zh.cppreference.com/) 的 `std::unique_ptr` 页面
- 对照接口列表

---

## ✍️ 写（约 120 分钟）

```cpp
    // ---------- 移动（转移所有权）----------
    MyUniquePtr(MyUniquePtr&& other) noexcept
        : ptr_(other.ptr_) {
        other.ptr_ = nullptr;             // ← 必须置空
    }

    MyUniquePtr& operator=(MyUniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr_;                  // 释放自己原有的
            ptr_ = other.ptr_;            // 接管
            other.ptr_ = nullptr;         // 置空对方
        }
        return *this;
    }

    // ---------- 所有权操作 ----------
    T* release() noexcept {               // 放弃所有权，返回裸指针
        T* p = ptr_;
        ptr_ = nullptr;
        return p;                         // ← 调用者负责 delete
    }

    void reset(T* p = nullptr) noexcept {  // 释放旧的，接管新的
        if (ptr_ != p) {
            delete ptr_;
            ptr_ = p;
        }
    }

    void swap(MyUniquePtr& other) noexcept {
        std::swap(ptr_, other.ptr_);
    }
```

### 关键对比

| 操作 | 语义 | 谁负责释放 |
|---|---|---|
| `release()` | **放弃**所有权 | 调用者 |
| `reset()` | **替换**所有权 | 智能指针 |
| 移动 | **转移**所有权 | 智能指针（新的那个） |

### 何时用 release vs reset

```cpp
// release：要把裸指针交给 C 风格 API
FILE* f = ...;
MyUniquePtr<Foo> p(new Foo());
some_c_api(p.release());      // API 接管了所有权

// reset：换一个对象管
p.reset(new Foo());           // 旧的被 delete，接管新的
```

### 测试

```cpp
MyUniquePtr<int> a(new int(1));
MyUniquePtr<int> b = std::move(a);      // 移动
assert(a.get() == nullptr);
assert(*b == 1);

int* raw = b.release();                 // 放弃
assert(b.get() == nullptr);
delete raw;                             // ← 现在是我负责

MyUniquePtr<int> c(new int(3));
c.reset(new int(4));                    // 旧的 3 被释放
assert(*c == 4);
```

---

## ✅ 验收（打勾才算过）

- [ ] 移动构造 / 移动赋值正确，源对象变 `nullptr`
- [ ] `release()` 后智能指针为空，返回的裸指针可用
- [ ] `reset()` 正确释放旧对象
- [ ] `reset(p)` 传**同一个指针**时不会误删（`if (ptr_ != p)` 的作用）
- [ ] 自我移动 `a = std::move(a)` 不崩溃
- [ ] valgrind 零错误
- [ ] 能说出 `release` 和 `reset` 的区别

---

## ✅ Block 11 完成检查

- [ ] 模板：函数模板、类模板、成员模板、特化
- [ ] `MyUniquePtr`：独占语义、禁止拷贝、移动转移
- [ ] 理解"所有权"这个概念

**三条都打勾 → 进 Day 34。**

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| 移动后 double free | `other.ptr_ = nullptr;` 漏了 |
| `reset` 自赋值崩溃 | 加 `if (ptr_ != p)` 判断 |
| `swap` 报错 | `#include <utility>` |

---

## 🔜 明天（Day 34）

进入 shared_ptr 的核心：引用计数。

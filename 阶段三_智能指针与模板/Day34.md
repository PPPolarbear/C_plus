# Day 34 · 引用计数与控制块设计

**Block 12/19** · 阶段三（Day 31–45）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && cmake --build build
```

---

## 🎯 今天唯一的目标

**想清楚"引用计数为什么要放在堆上"——这是 shared_ptr 的全部难点。**

今天不写完整实现，先把设计想明白。

---

## 📖 读（约 40 分钟）

- 搜「C++ shared_ptr 引用计数 实现 面试」
- 搜「C++ 控制块 control block」

---

## ✍️ 写（约 110 分钟）

### 任务 1：先写一个"错的"版本，看它怎么错

```cpp
template <typename T>
class BadSharedPtr {
public:
    explicit BadSharedPtr(T* p) : ptr_(p), count_(1) {}
    BadSharedPtr(const BadSharedPtr& o) : ptr_(o.ptr_), count_(o.count_ + 1) {}
    ~BadSharedPtr() {
        if (--count_ == 0) delete ptr_;
    }
private:
    T*   ptr_;
    long count_;       // ← ❌ 每个对象一份计数
};
```

**测试**：

```cpp
BadSharedPtr<int> a(new int(5));      // a.count_ = 1
BadSharedPtr<int> b = a;              // b.count_ = 2（但 a.count_ 还是 1！）
BadSharedPtr<int> c = a;              // c.count_ = 2
// 析构时：c.count_ 变 1，b.count_ 变 1，a.count_ 变 0 → delete
// 但 b 和 c 还活着！→ 悬垂指针
```

**把问题写下来**：计数是"每个对象各存一份"，根本没法共享。

### 任务 2：正确设计——把计数放到堆上

```cpp
template <typename T>
class MySharedPtr {
public:
    explicit MySharedPtr(T* p)
        : ptr_(p)
        , ref_count_(new long(1))        // ← 计数在堆上
    {}

private:
    T*    ptr_;
    long* ref_count_;                    // ← 指向堆上的计数
};
```

**现在所有副本都持有同一个 `ref_count_` 指针，改的就是同一个数。**

### 任务 3：画出你的设计

在纸上或注释里画出这个场景：

```
a ──→ [ ptr_ ──→ 堆上的 T 对象 ]
      [ ref_count_ ──┐
                     ├──→ 堆上的 long (值=3)
b ──→ [ ptr_ ──→ 同一个 T  ]
      [ ref_count_ ──┘
c ──→ [ ptr_ ──→ 同一个 T  ]
      [ ref_count_ ──┘
```

**画出来才算真懂。**

### 任务 4：进阶思考——控制块

真实的 `shared_ptr` 会用一个"控制块"结构同时装：强引用计数、弱引用计数、删除器、分配器。

```cpp
struct ControlBlock {
    long strong_count = 1;
    long weak_count   = 0;
    // 还有删除器、分配器...
};
```

**今天只需要理解这个概念，Day 44 之后有兴趣再展开。**

---

## ✅ 验收（打勾才算过）

- [ ] 亲手验证了"错的版本"确实出问题
- [ ] 能说出：**计数必须放在堆上，因为多个 shared_ptr 要共享同一个计数**
- [ ] 画出了 a/b/c 三个 shared_ptr 共享一个计数 + 一个对象的图
- [ ] 能说出 `long*` 而不是 `long` 的原因
- [ ] 知道"控制块"是什么（即使还没实现）

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| 画不出图 | 关键点：**ptr_ 和 ref_count_ 都是指针，副本之间共享它们指向的内容** |
| 不理解为什么错版本没崩 | `delete` 后访问是未定义行为，可能不崩但已经错了。用 valgrind 看 |

---

## 🔜 明天（Day 35）

实现 MySharedPtr 的构造与析构。

# Day 36 · 拷贝 / 移动构造赋值

**Block 12/19** · 阶段三（Day 31–45）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && cmake --build build
```

---

## 🎯 今天唯一的目标

**让引用计数真正工作起来——并且搞清赋值运算符里"先加后减"的顺序。**

---

## 📖 读（约 20 分钟）

- **不读新内容。**回看 LearnCpp 22.3 “Move constructors and move assignment”与22.6 “std::shared_ptr”，按“先增加新资源计数，再释放旧资源”的顺序检查赋值
不读新内容。

---

## ✍️ 写（约 120 分钟）

```cpp
    // ---------- 拷贝构造：计数 +1 ----------
    MySharedPtr(const MySharedPtr& other) noexcept
        : ptr_(other.ptr_), ref_count_(other.ref_count_) {
        if (ref_count_) ++(*ref_count_);
    }

    // ---------- 移动构造：直接偷，计数不变 ----------
    MySharedPtr(MySharedPtr&& other) noexcept
        : ptr_(other.ptr_), ref_count_(other.ref_count_) {
        other.ptr_ = nullptr;
        other.ref_count_ = nullptr;
    }

    // ---------- 拷贝赋值：先加新的，再减旧的 ----------
    MySharedPtr& operator=(const MySharedPtr& other) noexcept {
        if (this != &other) {
            // ① 先增加新引用
            if (other.ref_count_) ++(*other.ref_count_);

            // ② 再释放自己旧的
            release();

            // ③ 接管
            ptr_       = other.ptr_;
            ref_count_ = other.ref_count_;
        }
        return *this;
    }

    // ---------- 移动赋值 ----------
    MySharedPtr& operator=(MySharedPtr&& other) noexcept {
        if (this != &other) {
            release();
            ptr_       = other.ptr_;
            ref_count_ = other.ref_count_;
            other.ptr_ = nullptr;
            other.ref_count_ = nullptr;
        }
        return *this;
    }

    void reset() noexcept {
        release();
        ptr_ = nullptr;
        ref_count_ = nullptr;
    }
```

### ⭐ 为什么拷贝赋值必须"先加后减"

**自己先想 1 分钟。**

考虑这个场景：

```cpp
MySharedPtr<int> a(new int(1));
MySharedPtr<int> b = a;      // 计数 = 2

b = a;                        // 自我赋值？不是，但两边引用同一个对象
```

如果**先减后加**：

```
① release()  → 计数 2→1  （b 放弃，但 a 还持有）
   等等，如果计数从 2 减到 1，没到 0，没问题...
```

再看更危险的场景：

```cpp
MySharedPtr<int> a(new int(1));
MySharedPtr<int> b = a;

// 假设先减后加，且计数恰好为 1
MySharedPtr<int> c = a;    // 计数 2
c = a;                     // 先 release：2→1；再加：1→2。看起来没问题
```

**真正的危险场景**：如果 `this` 是**最后一个**持有者，而 `other` 引用的对象**恰好就是同一个**：

```cpp
MySharedPtr<int> a(new int(1));   // 计数 1
a = a;                             // 自我赋值：先 release → 计数变 0 → 对象被 delete！
                                   // 然后再 ++(*other.ref_count_) → 访问已释放内存 💥
```

**结论**：先加后减，保证在处理 `other` 时它引用的对象一定还活着。

（另外 `if (this != &other)` 也挡住了最直接的自我赋值，但**两者都要有**——因为存在 `a` 和 `b` 是不同对象但引用同一个底层资源的间接自我赋值。）

### 测试

```cpp
MySharedPtr<int> a(new int(5));
assert(a.use_count() == 1);

MySharedPtr<int> b = a;               // 拷贝
assert(a.use_count() == 2);
assert(b.use_count() == 2);

{
    MySharedPtr<int> c = a;           // 拷贝
    assert(a.use_count() == 3);
}                                      // c 析构
assert(a.use_count() == 2);

MySharedPtr<int> d = std::move(a);    // 移动
assert(a.use_count() == 0);            // a 空了
assert(d.use_count() == 2);            // 计数没变
```

---

## ✅ 验收（打勾才算过）

- [ ] 拷贝后计数正确增加
- [ ] 移走后源对象 `use_count() == 0` 且 `get() == nullptr`
- [ ] **计数归零时对象被释放**（valgrind 零泄漏）
- [ ] 拷贝赋值"先加后减"顺序正确
- [ ] 能解释为什么不能先减后加（**上面那个自我赋值场景**）
- [ ] valgrind 零错误

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| 计数越加越多 | 检查 `release()` 在赋值里有没有调用 |
| 自我赋值崩溃 | `if (this != &other)` 漏了 |
| 悬垂指针 | 先加后减的顺序颠倒了 |

---

## 🔜 明天（Day 37）

改成原子计数，开始碰线程安全。

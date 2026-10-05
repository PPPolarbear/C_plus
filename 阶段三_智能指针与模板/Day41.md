# Day 41 · 手写 Iterator 类

**Block 14/19** · 阶段三（Day 31–45）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && cmake --build build
```

---

## 🎯 今天唯一的目标

**手写一个迭代器类——这是面试的进阶考点。**

昨天用裸指针"作弊"了，今天写真的。理解 `iterator_traits` 那五个类型别名是干什么的。

---

## 📖 读（约 40 分钟）

- 搜「C++ 迭代器 iterator_traits 五个类型」
- 搜「C++ 手写迭代器 面试」
- 重点理解：`iterator_category` / `value_type` / `difference_type` / `pointer` / `reference`

---

## ✍️ 写（约 110 分钟）

```cpp
template <typename T>
class MyVector {
public:
    class Iterator {
    public:
        // ---------- 五个类型别名（traits）----------
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;

        // ---------- 构造 ----------
        explicit Iterator(T* p = nullptr) : p_(p) {}

        // ---------- 解引用 ----------
        reference operator*()  const { return *p_; }
        pointer   operator->() const { return p_;  }

        // ---------- 前置/后置 ++ -- ----------
        Iterator& operator++()    { ++p_; return *this; }
        Iterator  operator++(int) { Iterator tmp = *this; ++p_; return tmp; }
        Iterator& operator--()    { --p_; return *this; }
        Iterator  operator--(int) { Iterator tmp = *this; --p_; return tmp; }

        // ---------- 随机访问 ----------
        Iterator  operator+(difference_type n) const { return Iterator(p_ + n); }
        Iterator  operator-(difference_type n) const { return Iterator(p_ - n); }
        difference_type operator-(const Iterator& o) const { return p_ - o.p_; }
        Iterator& operator+=(difference_type n) { p_ += n; return *this; }
        Iterator& operator-=(difference_type n) { p_ -= n; return *this; }
        reference operator[](difference_type n) const { return p_[n]; }

        // ---------- 比较 ----------
        bool operator==(const Iterator& o) const { return p_ == o.p_; }
        bool operator!=(const Iterator& o) const { return p_ != o.p_; }
        bool operator< (const Iterator& o) const { return p_ <  o.p_; }
        bool operator> (const Iterator& o) const { return p_ >  o.p_; }
        bool operator<=(const Iterator& o) const { return p_ <= o.p_; }
        bool operator>=(const Iterator& o) const { return p_ >= o.p_; }

    private:
        T* p_;
    };

    // ---------- 改用自定义迭代器 ----------
    Iterator begin() noexcept { return Iterator(data_); }
    Iterator end()   noexcept { return Iterator(data_ + size_); }
    // ...
};
```

### 五个类型别名是干什么的

| 别名 | 作用 |
|---|---|
| `iterator_category` | 告诉标准库"我是什么级别的迭代器"（输入/前向/双向/**随机访问**） |
| `value_type` | 元素类型，`std::iterator_traits<It>::value_type` |
| `difference_type` | 两个迭代器相减的类型 |
| `pointer` | `operator->` 返回什么 |
| `reference` | `operator*` 返回什么 |

**为什么重要**：`std::sort` 等算法需要知道迭代器的能力——如果是随机访问，它就能用更快的算法（比如快排而不是归并）。

### 为什么 `std::sort` 要求随机访问迭代器

因为快排需要"随机跳到中间"（`it + n/2`）。链表的迭代器做不到这一点，所以 `std::list` 不能直接用 `std::sort`（它有成员函数 `sort`）。

---

## ✅ 验收（打勾才算过）

- [ ] 自定义 `Iterator` 能用于 range-for
- [ ] `it + n`、`it1 - it2`、`it[n]` 都能用
- [ ] 能说出五个类型别名各自的作用
- [ ] 能说出 `iterator_category` 有哪几档（输入/前向/双向/随机访问）
- [ ] 能解释为什么 `std::list` 不能直接用 `std::sort`

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| `iterator_category` 未定义 | `#include <iterator>` |
| `Iterator` 名字冲突 | 用 `class Iterator` 嵌套在 `MyVector` 里，外部写 `MyVector<T>::Iterator` |
| `operator+` 编译不过 | 返回新对象，不要返回引用 |

---

## 🔜 明天（Day 42）

用 `std::sort` 验证——容器实现正确的强证明。

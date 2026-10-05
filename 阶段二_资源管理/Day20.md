# Day 20 · push_back 与 reallocate

**Block 7/19** · 阶段二（Day 16–30）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && cmake --build build && ./build/app
```

---

## 🎯 今天唯一的目标

**让 `push_back` 自动扩容，并理解"为什么按倍数而不是按个加"。**

---

## 📖 读（约 30 分钟）

- learncpp：动态内存分配 + 类的相关章节
- 搜「C++ vector 扩容 2倍 均摊复杂度」

---

## ✍️ 写（约 120 分钟）

### 核心：reallocate

```cpp
template <typename T>
void MyVector<T>::reallocate(size_t new_cap) {
    T* new_data = new T[new_cap];              // 1. 分配新内存

    for (size_t i = 0; i < size_; ++i) {       // 2. 把旧数据搬过去
        new_data[i] = std::move(data_[i]);     //    （Day 26 再改成真正的移动）
    }

    delete[] data_;                            // 3. 释放旧内存
    data_     = new_data;
    capacity_ = new_cap;
}
```

### 改造 push_back

```cpp
template <typename T>
void MyVector<T>::push_back(const T& val) {
    if (size_ >= capacity_) {
        size_t new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;   // ← 2 倍增长
        reallocate(new_cap);
    }
    data_[size_++] = val;
}
```

### 关键实验：观察 capacity 增长

```cpp
MyVector<int> v;
for (int i = 0; i < 20; ++i) {
    std::cout << "push(" << i << ")  size=" << v.size()
              << "  cap=" << v.capacity() << "\n";
    v.push_back(i);
}
```

**把输出抄下来。** capacity 应该是 1, 2, 4, 8, 16, 32...

### 思考题（写进注释）

把 `capacity_ * 2` 改成 `capacity_ + 1`，重新跑：

- [ ] 加上计数器统计 `reallocate` 被调用了几次
- [ ] 两种策略各调用了多少次？（20 次 push：2 倍 → 约 5 次；+1 → 约 19 次）

**这就是均摊 O(1) 和 O(n²) 的区别。**

---

## ✅ 验收（打勾才算过）

- [ ] `push_back` 100 次不崩溃，`size()==100`、`capacity()==128`
- [ ] capacity 增长序列是 1, 2, 4, 8, 16...（抄下来了）
- [ ] 能说出 `+1` 和 `*2` 两种策略各调用了几次 `reallocate`
- [ ] 能解释：**为什么扩容不能直接 `realloc`**（因为 T 可能有非平凡的构造/析构）
- [ ] valgrind 零泄漏
- [ ] 能说出：为什么 2 倍扩容的 `push_back` 是**均摊 O(1)**

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| 扩容后数据丢失 | 检查搬迁循环的上界是 `size_` 不是 `capacity_` |
| valgrind 报越界 | `new T[new_cap]` 之后访问了 `>= new_cap` 的下标 |
| `std::move` 报错 | 加 `#include <utility>`。现在用不用它效果一样，Day 26 才见真章 |

---

## 🔜 明天（Day 21）

调试 + 系统性观察扩容策略，Block 7 收尾。

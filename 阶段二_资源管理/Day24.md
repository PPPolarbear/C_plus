# Day 24 · 补齐 API + 验收

**Block 8/19** · 阶段二（Day 16–30）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && cmake --build build && ./build/app
```

---

## 🎯 今天唯一的目标

**把 `MyVector` 的接口补齐到"像个真容器"，Block 8 收尾。**

---

## 📖 读（约 20 分钟）

- LearnCpp 16.2 “Introduction to std::vector and list constructors”、16.10 “std::vector resizing and capacity”
- 查 cppreference `std::vector` 页面中的 “Element access”、“Capacity”、“Modifiers”三节，对照补全 API

---

## ✍️ 写（约 120 分钟）

```cpp
// ---------- 元素访问 ----------
T&       at(size_t i);              // 越界抛 std::out_of_range
const T& at(size_t i) const;
T&       front();                   // == data_[0]
const T& front() const;
T&       back();                    // == data_[size_-1]
const T& back() const;
T*       data() noexcept       { return data_; }
const T* data() const noexcept { return data_; }

// ---------- 容量 ----------
void reserve(size_t new_cap);       // 只增不减
void shrink_to_fit();               // 容量缩到 size

// ---------- 修改 ----------
void resize(size_t n);              // 多的部分填 T{}
void resize(size_t n, const T& val);
void clear();                       // size_ = 0（不释放内存）

// ---------- 构造 ----------
MyVector(size_t n, const T& val);              // n 个 val
MyVector(std::initializer_list<T> init);       // {1, 2, 3}
```

### 关键实现点

**`at` 的越界检查**：
```cpp
T& at(size_t i) {
    if (i >= size_) throw std::out_of_range("MyVector::at");
    return data_[i];
}
```

**`resize` 的扩容逻辑**：
```cpp
void resize(size_t n, const T& val) {
    if (n > capacity_) reallocate(n);       // 不够才扩容
    for (size_t i = size_; i < n; ++i) data_[i] = val;   // 填新元素
    size_ = n;
}
```

**`initializer_list` 构造**：
```cpp
MyVector(std::initializer_list<T> init) : MyVector() {
    reserve(init.size());
    for (const auto& x : init) push_back(x);
}
```

### 测试清单

```cpp
MyVector<int> v{1, 2, 3, 4, 5};    // initializer_list
v.at(0);        // 1
v.at(99);       // 抛 out_of_range
v.front();      // 1
v.back();       // 5
v.resize(3);    // 1 2 3
v.resize(6, 9); // 1 2 3 9 9 9
v.clear();      // size 0, capacity 不变
```

---

## ✅ 验收（打勾才算过）

- [ ] `MyVector<int> v{1,2,3}` 能工作
- [ ] `at()` 越界真的抛 `std::out_of_range`（`try-catch` 验证）
- [ ] `resize` 缩小不改变 capacity，扩大时正确填充
- [ ] `clear()` 后 `size()==0` 但 `capacity()` 不变
- [ ] `reserve` 只增不减（传个更小的值试试）
- [ ] valgrind 零错误

---

## ✅ Block 8 完成检查

- [ ] 拷贝构造、拷贝赋值（copy-and-swap）、三法则
- [ ] 自我赋值安全
- [ ] 接口基本对齐 `std::vector`
- [ ] `MyVector<int>` 能当普通容器用了

**四条都打勾 → 进 Day 25。**

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| `initializer_list` 构造报错 | 加 `#include <initializer_list>` |
| 委托构造不认识 | `MyVector(std::initializer_list<T> init) : MyVector() {...}` 是 C++11 委托构造 |
| `shrink_to_fit` 实现麻烦 | 可以今天就 `reallocate(size_)`，或者标记 TODO 跳过 |

---

## 🔜 明天（Day 25）

进入移动语义——让拷贝次数大幅下降。

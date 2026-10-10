# Day 53 · MyVector 强异常安全

**Block 18/19** · 阶段四（Day 46–57）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && cmake --build build
```

---

## 🎯 今天唯一的目标

**让 `MyVector` 在扩容途中抛异常时，容器保持原样——一个元素都不丢。**

这就是"强异常安全保证"。

---

## 📖 读（约 30 分钟）

- LearnCpp 27.9 “Exception specifications and noexcept”、27.10 “std::move_if_noexcept”
- cppreference：`std::vector::push_back` 页面中的 “Exceptions”说明，对照强异常保证的适用条件
- 回看 Day 23 的 copy-and-swap 实现，沿着扩容中抛异常的路径检查原容器是否保持不变

---

## ✍️ 写（约 120 分钟）

### 问题分析：现在的 `reallocate` 有个洞

```cpp
template <typename T>
void MyVector<T>::reallocate(size_t new_cap) {
    T* new_data = new T[new_cap];
    for (size_t i = 0; i < size_; ++i) {
        new_data[i] = std::move(data_[i]);   // ← 如果这里抛异常？
    }
    delete[] data_;
    data_ = new_data;
    capacity_ = new_cap;
}
```

**问题**：如果搬到第 5 个元素时抛异常：
- `new_data` 泄漏了（没被 delete）
- `data_` 的前 5 个元素已经被移走（处于"有效但未指定"状态）
- `size_` 和 `capacity_` 还是旧值 → **容器数据损坏**

### 修复 1：用 RAII 保证 new_data 不泄漏

```cpp
template <typename T>
void MyVector<T>::reallocate(size_t new_cap) {
    T* new_data = new T[new_cap];

    size_t constructed = 0;
    try {
        for (; constructed < size_; ++constructed) {
            new_data[constructed] = std::move(data_[constructed]);
        }
    } catch (...) {
        // 回滚：析构已经搬过去的
        for (size_t i = 0; i < constructed; ++i) {
            new_data[i].~T();
        }
        ::operator delete[](new_data);      // 释放裸内存
        throw;                              // 重新抛出，data_ 完全没动
    }

    delete[] data_;
    data_     = new_data;
    capacity_ = new_cap;
}
```

**关键**：抛异常时 `data_`、`size_`、`capacity_` **一个都没改**，所以容器完好。

### 修复 2：`push_back` 的强保证

```cpp
template <typename T>
void MyVector<T>::push_back(const T& val) {
    if (size_ < capacity_) {
        data_[size_] = val;        // ← 这一句也可能抛
        ++size_;
    } else {
        reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
        data_[size_] = val;        // 扩容成功后再赋值
        ++size_;
    }
}
```

**如果 `data_[size_] = val` 抛异常**：`size_` 还没 ++，所以容器状态不变（只是那块内存里有半成品，但对外不可见）。**基本保证达成。**

### 修复 3：验证赋值运算符

Day 23 已经用 copy-and-swap 写过，天然有强保证：

```cpp
MyVector& operator=(const MyVector& other) {
    if (this != &other) {
        MyVector tmp(other);       // 抛异常 → *this 完好
        swap(tmp);                 // 不抛
    }
    return *this;
}
```

### 任务：写强异常安全测试

```cpp
void test_strong_guarantee() {
    Thrower::throw_every = 5;      // 第 5 次拷贝抛

    MyVector<Thrower> v;
    for (int i = 0; i < 3; ++i) v.push_back(Thrower(i));

    size_t old_size = v.size();
    size_t old_cap  = v.capacity();

    Thrower::copy_count = 0;

    try {
        for (int i = 0; i < 10; ++i) v.push_back(Thrower(100 + i));
    } catch (const std::exception& e) {
        std::cout << "抛异常: " << e.what() << "\n";
        std::cout << "size: " << old_size << " → " << v.size() << "\n";
        std::cout << "cap : " << old_cap  << " → " << v.capacity() << "\n";

        // 强保证：容量没变时，size 必须没变
        if (v.capacity() == old_cap) {
            assert(v.size() == old_size);
        }
    }

    // 容器仍可正常使用
    v.push_back(Thrower(999));
    std::cout << "异常后仍可用，size = " << v.size() << "\n";
}
```

---

## ✅ 验收（打勾才算过）

- [ ] `reallocate` 的 catch 分支正确回滚（析构已搬的 + 释放内存）
- [ ] 抛异常后**容器仍可正常使用**
- [ ] **valgrind 零泄漏**（这是关键——回滚必须干净）
- [ ] 容量未变时 `size` 保持不变（强保证验证）
- [ ] 能解释：为什么 `throw;` 之后 `data_` 是安全的
- [ ] 能说出 `::operator delete[]` 和 `delete[]` 的区别（**前者只释放内存不调析构**）

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| valgrind 报泄漏 | catch 里漏了释放 `new_data` |
| 崩溃在回滚时 | 回滚循环的上界是 `constructed`，不是 `size_` |
| `::operator delete[]` 报错 | `#include <new>`；也可先析构再 `operator delete[]` |
| 强保证测试过不了 | 检查 `size_` 是不是在可能抛异常的操作**之后**才 ++ |

---

## 🔜 明天（Day 54）

CMake 工程化——把散落的文件变成一个正经项目。

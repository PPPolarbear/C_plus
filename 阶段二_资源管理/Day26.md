# Day 26 · 移动赋值 + 移动版 push_back

**Block 9/19** · 阶段二（Day 16–30）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && cmake --build build && ./build/app
```

---

## 🎯 今天唯一的目标

**看到 `copy` 计数从 Day 21 记下的那个数字大幅下降。**

今天会有"原来移动语义真的有用"的实感。

---

## 📖 读（约 30 分钟）

- LearnCpp 22.3 “Move constructors and move assignment”、14.14 “Introduction to the copy constructor”、15.4 “Introduction to destructors”
- 用这三类特殊成员函数对照 Rule of Five：新增移动构造和移动赋值后，哪些操作需要自定义

---

## ✍️ 写（约 120 分钟）

### 任务 1：移动赋值

```cpp
template <typename T>
MyVector<T>& MyVector<T>::operator=(MyVector&& other) noexcept {
    if (this != &other) {
        delete[] data_;                 // 释放自己的资源

        data_     = other.data_;        // 偷过来
        size_     = other.size_;
        capacity_ = other.capacity_;

        other.data_     = nullptr;      // 置空对方
        other.size_     = 0;
        other.capacity_ = 0;
    }
    return *this;
}
```

### 任务 2：移动版 push_back

```cpp
void push_back(T&& val);
```

```cpp
template <typename T>
void MyVector<T>::push_back(T&& val) {
    if (size_ >= capacity_) {
        reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    data_[size_++] = std::move(val);    // ← 用移动赋值，不是拷贝
}
```

### 任务 3：reallocate 里也改成移动

```cpp
template <typename T>
void MyVector<T>::reallocate(size_t new_cap) {
    T* new_data = new T[new_cap];
    for (size_t i = 0; i < size_; ++i) {
        new_data[i] = std::move(data_[i]);    // ← Day 20 就写了，今天才真正生效
    }
    delete[] data_;
    data_     = new_data;
    capacity_ = new_cap;
}
```

### 任务 4：⭐ 见证时刻

重跑 Day 21 的 `Counted` 实验：

```cpp
MyVector<Counted> v;
Counted::copy = Counted::move = 0;
for (int i = 0; i < 10; ++i) v.push_back(Counted(i));
std::cout << "copy=" << Counted::copy << "  move=" << Counted::move << "\n";
```

**对比 Day 21 记下的数字。** 把两组数据都写进 `进度追踪.md`：

| | copy 次数 | move 次数 |
|---|---|---|
| Day 21（只有拷贝） | | — |
| Day 26（加了移动） | | |

### 任务 5：为什么必须 noexcept

把移动构造的 `noexcept` 去掉，重新跑上面的实验。**观察 copy 次数是不是又涨回去了？**

**原因**：`std::vector`（和你的 `reallocate`）在扩容时，**只有当移动构造是 `noexcept` 时才敢用它**——否则搬一半抛异常，原数据就毁了。

---

## ✅ 验收（打勾才算过）

- [ ] 移动赋值实现并测试（`a = std::move(b)` 后 `b` 为空）
- [ ] `push_back` 有移动版本
- [ ] **copy 次数相比 Day 21 明显下降**，数据记录在案
- [ ] 去掉 `noexcept` 后 copy 次数涨回去（**亲眼验证**）
- [ ] 能解释：为什么移动构造必须标 `noexcept`
- [ ] 能说出五法则的内容
- [ ] valgrind 零错误

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| 移动版 push_back 没被调用 | 传的是左值。用 `v.push_back(Counted(i))` 传临时对象，或者 `v.push_back(std::move(x))` |
| copy 次数没降 | 检查 `reallocate` 是不是用了 `std::move` |
| 去掉 noexcept 没变化 | 某些标准库实现较宽松，看 `copy` 计数有没有变即可 |

---

## 🔜 明天（Day 27）

emplace_back + 完整验证，Block 9 收尾。

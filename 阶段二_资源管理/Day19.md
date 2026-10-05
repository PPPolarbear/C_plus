# Day 19 · MyVector 骨架

**Block 7/19** · 阶段二（Day 16–30）· **分水岭**

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && touch include/myvector.h
```

**从今天起，你不再"学 C++"，而是"造东西"。**

---

## 🎯 今天唯一的目标

**让 `MyVector<int>` 能被创建、能存数据、能析构——不崩溃就行。**

今天**不写**扩容，不写拷贝，不写移动。只写骨架。

---

## 📖 读（约 40 分钟）

- learncpp：类 + 动态内存分配相关章节
- **只读够用的，别贪多**

---

## ✍️ 写（约 110 分钟）

```cpp
// include/myvector.h
#pragma once
#include <cstddef>

template <typename T>
class MyVector {
public:
    // ---------- 构造与析构（今天写）----------
    MyVector();                            // data_=nullptr, size_=cap_=0
    explicit MyVector(size_t n);           // n 个 T{}
    ~MyVector();                           // delete[] data_

    // ---------- 容量（今天写）----------
    size_t size() const     { return size_; }
    size_t capacity() const { return capacity_; }
    bool   empty() const    { return size_ == 0; }

    // ---------- 元素访问（今天写）----------
    T&       operator[](size_t i)       { return data_[i]; }
    const T& operator[](size_t i) const { return data_[i]; }

    // ---------- 修改（今天只写 push_back 的朴素版）----------
    void push_back(const T& val);          // 暂时：容量不够就 throw，不做扩容

    // ---------- 下面的以后写 ----------
    // 拷贝构造 / 拷贝赋值      → Day 22-23
    // 移动构造 / 移动赋值      → Day 25-26
    // at / front / back / data → Day 24
    // emplace_back             → Day 27
    // 迭代器                    → Day 40-42

private:
    void reallocate(size_t new_cap);       // Day 20 写

    T*     data_;
    size_t size_;
    size_t capacity_;
};
```

### 今天的 `push_back`（朴素版）

```cpp
template <typename T>
void MyVector<T>::push_back(const T& val) {
    if (size_ >= capacity_) {
        throw std::runtime_error("容量不足（明天实现扩容）");
    }
    data_[size_++] = val;
}
```

**先能跑，明天再让它变聪明。**

### 测试

```cpp
MyVector<int> v(10);          // capacity 10
v.push_back(1);
v.push_back(2);
std::cout << v.size() << "\n";        // 2
std::cout << v[0] << v[1] << "\n";    // 12
// v 离开作用域 → 析构 → delete[] data_
```

---

## ✅ 验收（打勾才算过）

- [ ] `MyVector<int> v(10)` 能创建，`size()==0`、`capacity()==10`
- [ ] `push_back` 后 `size()` 正确增加，`v[i]` 能取到值
- [ ] 离开作用域后 valgrind **零泄漏**（说明析构函数对了）
- [ ] `MyVector<int> v;` 默认构造后 `push_back` 会抛异常（**预期行为**）
- [ ] 能解释：为什么 `data_` 要初始化为 `nullptr`（**提示：`delete[] nullptr` 是安全的**）

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| 模板类报一堆链接错误 | 模板的实现要放在**头文件里**，不能放 `.cpp` |
| `T{}` 是什么 | C++11 的值初始化，`int{}` 是 0 |
| 不知道 `throw` 要 include 什么 | `#include <stdexcept>` |

---

## 🔜 明天（Day 20）

写 `reallocate`，让 `push_back` 能自动扩容。

# Day 22 · 拷贝构造函数

**Block 8/19** · 阶段二（Day 16–30）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && cmake --build build && ./build/app
```

---

## 🎯 今天唯一的目标

**正式解决 Day 17 那个 double free——写出正确的深拷贝。**

从今天起，`MyVector` 才是一个真正能用的容器。

---

## 📖 读（约 30 分钟）

- LearnCpp 14.14 “Introduction to the copy constructor”、15.4 “Introduction to destructors”、21.12 “Overloading the assignment operator”
- 对照这三种特殊成员函数，归纳 Rule of Three 的适用条件

---

## ✍️ 写（约 120 分钟）

### 任务 1：拷贝构造函数

```cpp
template <typename T>
MyVector<T>::MyVector(const MyVector& other)
    : data_(nullptr), size_(0), capacity_(0)      // 先把自己清零
{
    if (other.size_ > 0) {
        data_     = new T[other.size_];           // 分配和对方一样大的空间
        for (size_t i = 0; i < other.size_; ++i) {
            data_[i] = other.data_[i];            // 逐个拷贝元素
        }
        size_     = other.size_;
        capacity_ = other.size_;                  // 容量正好够用
    }
}
```

**注意**：这里 `capacity_ = other.size_` 而不是 `other.capacity_`——这是常见的实现选择（`std::vector` 也这么做）。

### 任务 2：验证深拷贝

```cpp
MyVector<int> a;
for (int i = 0; i < 5; ++i) a.push_back(i);

MyVector<int> b = a;      // 调用拷贝构造
b[0] = 999;               // 改 b

a.dump();                 // a[0] 应该还是 0 ← 关键验证
b.dump();                 // b[0] 是 999
```

**如果 `a[0]` 也变成 999，说明你写出了浅拷贝——回去检查。**

### 任务 3：三法则实验

把拷贝构造函数注释掉，重新编译运行 Day 17 那个崩溃场景。**崩溃回来了。**

这说明：

> **如果你需要自定义析构函数、拷贝构造、拷贝赋值中的任何一个，你通常三个都需要。**

写下你的理解。

---

## ✅ 验收（打勾才算过）

- [ ] `MyVector<int> b = a;` 后改 `b` **不影响** `a`
- [ ] valgrind 零泄漏、零错误
- [ ] 能说出三法则的内容
- [ ] 能解释：为什么 `MyVector` 需要自定义拷贝构造（**因为默认的是浅拷贝**）
- [ ] 能说出 `capacity_ = other.size_` 而不是 `other.capacity_` 的理由（**避免浪费**）

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| 拷贝后还是共享数据 | 检查是不是写了 `data_ = other.data_`（那是浅拷贝） |
| 空 vector 拷贝崩溃 | `other.size_ == 0` 时不该 `new T[0]`，加个判断 |
| 模板报错说找不到拷贝构造 | 拷贝构造不是模板函数，签名是 `MyVector(const MyVector&)` |

---

## 🔜 明天（Day 23）

拷贝赋值 + copy-and-swap + 自我赋值。

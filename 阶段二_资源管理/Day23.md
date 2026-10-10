# Day 23 · 拷贝赋值 + copy-and-swap

**Block 8/19** · 阶段二（Day 16–30）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && cmake --build build && ./build/app
```

---

## 🎯 今天唯一的目标

**写出一个"自我赋值安全"的拷贝赋值运算符。**

`v = v;` 这行看起来荒谬，但在真实代码里会以各种隐蔽方式发生（比如通过引用传递）。写不对就崩溃。

---

## 📖 读（约 40 分钟）

- LearnCpp 21.12 “Overloading the assignment operator”、14.14 “Introduction to the copy constructor”
- 结合本日代码检查自我赋值时的资源释放顺序；copy-and-swap 作为实现练习

---

## ✍️ 写（约 110 分钟）

### 任务 1：写一个"朴素版"（有 bug）

先自己写，然后**测试它的 bug**：

```cpp
template <typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector& other) {
    delete[] data_;                       // 先释放自己
    data_     = new T[other.size_];
    for (size_t i = 0; i < other.size_; ++i) data_[i] = other.data_[i];
    size_     = other.size_;
    capacity_ = other.size_;
    return *this;
}
```

**测试自我赋值**：

```cpp
MyVector<int> v;
v.push_back(1);
v = v;               // ← 会发生什么？
v.dump();
```

**观察崩溃。** 原因：`delete[] data_` 之后，`other` 就是 `this`，`other.data_` 已经是野指针了。

### 任务 2：加自我赋值检查

```cpp
if (this == &other) return *this;    // 自我赋值直接返回
```

能跑通了，但**还有别的问题**（异常安全差、代码重复）。

### 任务 3：copy-and-swap（**推荐写法**）

先加一个 `swap`：

```cpp
template <typename T>
void MyVector<T>::swap(MyVector& other) noexcept {
    std::swap(data_,     other.data_);
    std::swap(size_,     other.size_);
    std::swap(capacity_, other.capacity_);
}
```

然后拷贝赋值变成三行：

```cpp
template <typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector& other) {
    if (this != &other) {
        MyVector tmp(other);     // 1. 先拷贝一份（可能抛异常，但 *this 完好）
        swap(tmp);               // 2. 交换（不会抛异常）
    }                            // 3. tmp 析构，释放旧内存
    return *this;
}
```

### 任务 4：加"三件套"完整测试

```cpp
MyVector<int> a; a.push_back(1); a.push_back(2);
MyVector<int> b; b.push_back(9);

b = a;          // 拷贝赋值
b[0] = 100;
a.dump();       // 应该还是 1 2
b.dump();       // 100 2

b = b;          // 自我赋值
b.dump();       // 应该不变

// 链式赋值
MyVector<int> c, d;
c = d = a;
```

---

## ✅ 验收（打勾才算过）

- [ ] **先复现了朴素版的自我赋值崩溃**
- [ ] copy-and-swap 版本下 `v = v` 不崩溃、数据不变
- [ ] 链式赋值 `c = d = a` 正确工作
- [ ] 能说出 copy-and-swap 为什么更安全（**提示：先拷贝后交换，抛异常时原对象完好**）
- [ ] 能说出 `swap` 为什么可以标 `noexcept`（只交换指针和整数，不分配内存）
- [ ] valgrind 零错误

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| `swap` 里 `std::swap` 报错 | 加 `#include <utility>` |
| 自我赋值还是崩 | 检查 `if (this != &other)` 是不是写成了 `this != other` |
| 链式赋值不生效 | 检查 `return *this;`（不是 `return other;`） |

---

## 🔜 明天（Day 24）

补齐 API：at / front / back / data / resize / reserve / initializer_list。

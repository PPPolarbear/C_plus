# Day 25 · 右值引用与移动构造

**Block 9/19** · 阶段二（Day 16–30）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && cmake --build build && ./build/app
```

---

## 🎯 今天唯一的目标

**搞懂"右值引用"到底在解决什么问题——不是背 `&&` 语法。**

关键问题：**当一个临时对象马上要销毁时，为什么要浪费力气去拷贝它？**

---

## 📖 读（约 40 分钟）

- LearnCpp 12.2 “Value categories (lvalues and rvalues)”、16.5 “Returning std::vector, and an introduction to move semantics”、22.1 “Introduction to smart pointers and move semantics”、22.3 “Move constructors and move assignment”
- **这块难，允许读慢一点**

---

## ✍️ 写（约 110 分钟）

### 任务 1：认识左值和右值

```cpp
void lvalue_rvalue_demo() {
    int a = 1;              // a 是左值（有名字、可取地址）
    int b = a;              // 右值？
    
    int&& r = 42;           // 42 是右值，r 是右值引用
    // int&& r2 = a;        // ❌ 左值不能绑到右值引用
    
    std::string s = "hi";
    std::string&& r2 = std::string("tmp");   // ✅ 临时对象是右值
}
```

**判断标准（写进注释）**：
- 有名字、能取地址 → 左值
- 临时的、马上要销毁 → 右值

### 任务 2：给 MyVector 加移动构造

```cpp
template <typename T>
MyVector<T>::MyVector(MyVector&& other) noexcept
    : data_(other.data_)            // ← 偷指针，不拷贝！
    , size_(other.size_)
    , capacity_(other.capacity_)
{
    other.data_     = nullptr;      // ← 必须置空，否则 double free
    other.size_     = 0;
    other.capacity_ = 0;
}
```

**三行赋值 vs 循环拷贝——这就是全部区别。**

### 任务 3：验证移动确实发生了

```cpp
MyVector<int> make_vec() {
    MyVector<int> v;
    for (int i = 0; i < 5; ++i) v.push_back(i);
    return v;              // 会调用移动构造（或 RVO）
}

int main() {
    MyVector<int> a = make_vec();
    a.dump();

    MyVector<int> b = std::move(a);   // 显式移动
    b.dump();
    a.dump();              // a 应该是空的（有效但未指定）
}
```

### 任务 4：验证 std::move 不移动任何东西

```cpp
void what_is_move() {
    std::string s = "hello";
    std::cout << "移动前: " << s << "\n";
    std::string&& r = std::move(s);      // ← 什么都没发生！
    std::cout << "move 之后: " << s << "\n";   // 还是 "hello"
    std::string t = std::move(s);        // ← 这才是真的移动
    std::cout << "真正移动后: " << s << "\n";  // s 变空了
}
```

**结论写进注释**：`std::move` 只是个**类型转换**，把左值转成右值引用，**本身不做任何移动**。

---

## ✅ 验收（打勾才算过）

- [ ] 移动构造实现并测试通过
- [ ] `MyVector<int> b = std::move(a);` 后 `a` 是空的，`b` 有数据
- [ ] valgrind 零错误（**特别注意：置空那三行漏了就会 double free**）
- [ ] 能说出左值和右值的判断标准
- [ ] **能回答："std::move 移动了什么？"** → 答："什么都没移动，它只是个类型转换"
- [ ] 能说出移动后的对象处于什么状态 → **有效但未指定**

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| 移动后 double free | 检查有没有把 `other` 的三个成员置空 |
| `std::move` 未定义 | `#include <utility>` |
| 移动构造没被调用 | 可能被编译器优化成 RVO 了，试试 `std::move` 强制一下 |

---

## 🔜 明天（Day 26）

移动赋值 + 移动版 push_back，见证拷贝次数下降。

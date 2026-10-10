# Day 27 · emplace_back + 验证

**Block 9/19** · 阶段二（Day 16–30）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && cmake --build build && ./build/app
```

---

## 🎯 今天唯一的目标

**实现 `emplace_back`——原地构造，连移动都省掉。**

---

## 📖 读（约 30 分钟）

- cppreference：`std::vector::emplace_back` 页面中的 “Notes”与 “Example”
- cppreference：语言参考 “Parameter pack (since C++11)”条目；只看参数包展开与转发引用的基本形式

---

## ✍️ 写（约 120 分钟）

### 任务 1：可变参数模板热身

```cpp
template <typename... Args>
void print_all(Args&&... args) {
    ((std::cout << args << " "), ...);    // C++17 折叠表达式
    std::cout << "\n";
}

// print_all(1, 2.5, "hi", std::string("x"));
```

### 任务 2：emplace_back

```cpp
template <typename... Args>
T& emplace_back(Args&&... args) {
    if (size_ >= capacity_) {
        reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    new (data_ + size_) T(std::forward<Args>(args)...);   // ← placement new
    return data_[size_++];
}
```

**关键点**：
- `std::forward` 保持参数的值类别（左值保持左值，右值保持右值）
- `placement new` 在已分配的内存上直接构造对象

### 任务 3：对比实验

```cpp
struct Heavy {
    static int ctor, copy, move;
    std::string s;
    Heavy(const char* p) : s(p) { ++ctor; }
    Heavy(const Heavy& o) : s(o.s) { ++copy; }
    Heavy(Heavy&& o) noexcept : s(std::move(o.s)) { ++move; }
};

// 对照组 A：push_back 传临时对象
Heavy::ctor = Heavy::copy = Heavy::move = 0;
MyVector<Heavy> v1;
v1.push_back(Heavy("hello"));

// 对照组 B：emplace_back 直接构造
Heavy::ctor = Heavy::copy = Heavy::move = 0;
MyVector<Heavy> v2;
v2.emplace_back("hello");
```

**把两组的 `ctor / copy / move` 三个数字都记下来。**

预期：`emplace_back` 的 `copy` 和 `move` 都是 0——**对象直接在容器内存里构造出来了**。

### 任务 4：`reserve` 后再测

```cpp
MyVector<Heavy> v3;
v3.reserve(10);              // ← 提前分配，避免扩容干扰
Heavy::ctor = Heavy::copy = Heavy::move = 0;
v3.emplace_back("world");
```

**数字应该是最干净的：ctor=1, copy=0, move=0。**

---

## ✅ 验收（打勾才算过）

- [ ] `emplace_back("hello")` 能构造出 `Heavy` 对象
- [ ] `emplace_back` 的 copy/move 次数**低于或等于** `push_back`
- [ ] `reserve` 后 `emplace_back` 实现 **ctor=1, copy=0, move=0**
- [ ] 能解释 `std::forward` 和 `std::move` 的区别（**forward 保持原值类别，move 一律转右值**）
- [ ] 能说出 `placement new` 是什么
- [ ] valgrind 零错误

---

## ✅ Block 9 完成检查

- [ ] 移动构造、移动赋值、`noexcept` 的作用
- [ ] `std::move` 的本质是类型转换
- [ ] `emplace_back` + placement new + 完美转发
- [ ] 有实测数据支撑（copy 次数对比）

**四条都打勾 → 进 Day 28。**

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| `placement new` 报错 | `#include <new>` |
| 折叠表达式不认识 | 需要 C++17，检查 `CMAKE_CXX_STANDARD 17` |
| emplace 后 valgrind 报错 | 检查 `data_ + size_` 的偏移是否正确（不是 `data_ + capacity_`） |

---

## 🔜 明天（Day 28）

开始写 MyString——用同一套内存管理思想做一遍字符串。

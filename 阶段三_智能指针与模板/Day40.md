# Day 40 · 裸指针迭代器 + range-for

**Block 14/19** · 阶段三（Day 31–45）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && cmake --build build
```

---

## 🎯 今天唯一的目标

**让你的 `MyVector` 支持 `for (auto x : v)`。**

今天的版本用裸指针就够了——**指针本身就是最完美的随机访问迭代器**。

---

## 📖 读（约 30 分钟）

- LearnCpp 16.8 “Range-based for loops (for-each)”、18.2 “Introduction to iterators”
- 对照范围 for 的遍历语法与 `begin()` / `end()` 迭代器接口

---

## ✍️ 写（约 120 分钟）

### 任务 1：给 MyVector 加迭代器接口

```cpp
    // ---------- 迭代器（裸指针版）----------
    T*       begin()  noexcept { return data_; }
    T*       end()    noexcept { return data_ + size_; }
    const T* begin()  const noexcept { return data_; }
    const T* end()    const noexcept { return data_ + size_; }
    const T* cbegin() const noexcept { return data_; }
    const T* cend()   const noexcept { return data_ + size_; }

    // ---------- 反向迭代器 ----------
    std::reverse_iterator<T*>       rbegin()  noexcept { return std::reverse_iterator<T*>(end()); }
    std::reverse_iterator<T*>       rend()    noexcept { return std::reverse_iterator<T*>(begin()); }
```

需要 `#include <iterator>`。

### 任务 2：为什么 range-for 能工作

**range-for 的本质**（写进注释）：

```cpp
for (auto x : v) { ... }
// 编译器展开成：
{
    auto __begin = v.begin();
    auto __end   = v.end();
    for (; __begin != __end; ++__begin) {
        auto x = *__begin;
        ...
    }
}
```

**所以只要 `begin()` / `end()` / `!=` / `++` / `*` 都在，range-for 就能用。** 指针全都满足。

### 任务 3：测试

```cpp
MyVector<int> v{1, 2, 3, 4, 5};

// range-for
for (auto x : v) std::cout << x << " ";
std::cout << "\n";

// 修改元素
for (auto& x : v) x *= 2;
v.dump();     // 2 4 6 8 10

// const 对象
const MyVector<int>& cv = v;
for (auto x : cv) std::cout << x << " ";
```

### 任务 4：手动迭代

```cpp
for (auto it = v.begin(); it != v.end(); ++it) {
    std::cout << *it << " ";
}

// 反向
for (auto it = v.rbegin(); it != v.rend(); ++it) {
    std::cout << *it << " ";
}
```

---

## ✅ 验收（打勾才算过）

- [ ] `for (auto x : v)` 能遍历
- [ ] `for (auto& x : v) x *= 2;` 能修改元素
- [ ] const 对象也能遍历（走 `const` 版本的重载）
- [ ] 能**默写出 range-for 的展开形式**
- [ ] 能说出迭代器需要哪五个操作（`begin`/`end`/`!=`/`++`/`*`）
- [ ] 反向迭代器能用

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| `reverse_iterator` 报错 | `#include <iterator>` |
| const 对象遍历报错 | 加 const 版本的 `begin()/end()` |
| range-for 改不了元素 | 要用 `auto&` 而不是 `auto` |

---

## 🔜 明天（Day 41）

手写一个真正的 Iterator 类。

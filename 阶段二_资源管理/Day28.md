# Day 28 · MyString 骨架

**Block 10/19** · 阶段二（Day 16–30）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && touch include/mystring.h
```

---

## 🎯 今天唯一的目标

**用同一套内存管理思想做一遍字符串——检验你是真会了，还是只会照抄 `MyVector`。**

关键区别：字符串要处理 **`'\0'` 结尾**。

---

## 📖 读（约 20 分钟）

- cppreference：`std::strlen`、`std::strcpy`、`std::memcpy` 三个函数条目，只核对参数、返回值和缓冲区要求
- LearnCpp 19.1 “Dynamic memory allocation with new and delete”，回看本日字符串缓冲区的分配与释放

---

## ✍️ 写（约 120 分钟）

```cpp
// include/mystring.h
#pragma once
#include <cstddef>
#include <ostream>

class MyString {
public:
    // ---------- 构造与析构 ----------
    MyString();                                  // 空串（size_=0，但 data_ 不是 nullptr）
    explicit MyString(const char* s);            // 从 C 字符串
    MyString(const MyString& other);             // 拷贝
    MyString(MyString&& other) noexcept;         // 移动
    ~MyString();

    // ---------- 容量与访问 ----------
    size_t      size() const;
    size_t      capacity() const;
    bool        empty() const;
    const char* c_str() const;                   // 返回 data_，保证 '\0' 结尾

    char&       operator[](size_t i);
    const char& operator[](size_t i) const;

    // ---------- 修改 ----------
    void push_back(char c);
    void append(const char* s);
    void clear();
    void reserve(size_t n);

private:
    void reallocate(size_t new_cap);

    char*  data_;        // 保证以 '\0' 结尾
    size_t size_;        // 不含 '\0'
    size_t capacity_;    // 实际分配的大小
};
```

### 关键设计点

**约定**：`size_` 是**可见字符数**（不含 `'\0'`），但 `data_` 总是 `'\0'` 结尾。

```cpp
MyString::MyString() : data_(new char[1]{'\0'}), size_(0), capacity_(1) {}
```

**为什么 `data_` 不能是 `nullptr`**：因为 `c_str()` 要返回一个合法的 C 字符串，`nullptr` 会让 `strlen` 崩溃。

### 实现 `append`

```cpp
void MyString::append(const char* s) {
    size_t n = std::strlen(s);
    if (size_ + n + 1 > capacity_) {
        reallocate(capacity_ == 0 ? 1 : capacity_ * 2 + n);   // 至少够放下
    }
    std::memcpy(data_ + size_, s, n);
    size_ += n;
    data_[size_] = '\0';        // ← 别忘了结尾
}
```

---

## ✅ 验收（打勾才算过）

- [ ] `MyString s("hello");` 后 `size()==5`、`c_str()` 返回 `"hello"`
- [ ] `MyString s;` 的 `c_str()` 返回空字符串（不是崩溃）
- [ ] `push_back('a')` 和 `append("bc")` 工作正常
- [ ] `strlen(s.c_str()) == s.size()` **恒成立**
- [ ] valgrind 零泄漏
- [ ] 能解释：为什么 `data_` 不能初始化为 `nullptr`

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| `strlen` 报错 | `#include <cstring>` |
| 忘了结尾符导致乱码 | 每次修改后都补 `data_[size_] = '\0'` |
| 扩容后字符串截断 | `reallocate` 里搬迁的是 `size_ + 1` 个字节（**含结尾符**） |

---

## 🔜 明天（Day 29）

运算符重载：`+`、`==`、`<<`。

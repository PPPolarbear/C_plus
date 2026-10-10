# Day 29 · 运算符重载与友元

**Block 10/19** · 阶段二（Day 16–30）

---

## ⚡ 现在就开始

```bash
cd cpp-camp3 && cmake --build build && ./build/app
```

---

## 🎯 今天唯一的目标

**让 `MyString` 用起来像 `std::string`。**

重点是理解：**为什么有些运算符必须写成友元函数。**

---

## 📖 读（约 40 分钟）

- LearnCpp 21.1 “Introduction to operator overloading”、21.3 “Overloading operators using normal functions”、15.8 “Friend non-member functions”
- 对照 `a + b` 与 `a += b`，区分普通非成员运算符函数和友元函数的适用场景

---

## ✍️ 写（约 110 分钟）

```cpp
// ---------- 成员运算符 ----------
MyString& operator+=(const MyString& rhs);       // 追加
MyString& operator+=(const char* rhs);

bool operator==(const MyString& rhs) const;
bool operator!=(const MyString& rhs) const;
bool operator<(const MyString& rhs) const;       // 字典序

// ---------- 友元运算符（重点）----------
friend MyString operator+(const MyString& a, const MyString& b);
friend std::ostream& operator<<(std::ostream& os, const MyString& s);
friend std::istream& operator>>(std::istream& is, MyString& s);
```

### 为什么 `operator+` 要写成友元？

**自己先想 30 秒再往下看。**

因为**两边对称**。如果写成成员函数：

```cpp
MyString s = "hello" + a;    // ❌ 编译错误！
// 编译器会去找 operator+("hello" 的类型的, MyString)
```

而友元函数两边都是参数，支持隐式转换：

```cpp
MyString s = "hello" + a;    // ✅ 左边 const char* 隐式转成 MyString
```

### 为什么 `operator<<` 必须是友元？

因为**左操作数是 `std::ostream`，不是 `MyString`**——成员函数的第一个参数永远是 `*this`。

### 完整实现

```cpp
MyString operator+(const MyString& a, const MyString& b) {
    MyString result(a);       // 拷贝一份
    result += b;              // 追加
    return result;            // 返回值优化 / 移动
}

std::ostream& operator<<(std::ostream& os, const MyString& s) {
    os << s.c_str();
    return os;                // ← 必须返回 os，才能链式
}
```

---

## ✅ 验收（打勾才算过）

- [ ] `a + b`、`a += b`、`a == b`、`a < b` 都正确
- [ ] **`"hello" + a` 能编译通过**（验证友元的必要性）
- [ ] `std::cout << a << b;` 链式输出正常
- [ ] 能解释：为什么 `operator+` 写成成员函数会导致 `"hello" + a` 编译失败
- [ ] 能解释：为什么 `operator<<` 不可能是成员函数
- [ ] valgrind 零错误

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| 友元函数在类外找不到 | 在类内 `friend` 声明，类外定义时**不加 `MyString::` 前缀** |
| `operator==` 里访问私有成员报错 | 成员函数可以直接访问；友元函数也可以；非友元不行 |
| 链式输出只有第一个 | `operator<<` 忘了 `return os;` |

---

## 🔜 明天（Day 30）

**对拍测试——你第一个"客观判分器"。里程碑 2。**

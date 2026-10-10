# Day 08 · 引用与 const 正确性

**Block 3/19** · 阶段一（Day 1–15）

---

## ⚡ 现在就开始

```bash
cd cpp-camp2 && cmake --build build && ./build/app
```

---

## 🎯 今天唯一的目标

**彻底分清 `const int*` / `int* const` / `const int* const`。**

这三个写法是面试第一道筛子，也是后面写 `MyVector` 时必须用对的东西。

---

## 📖 读（约 40 分钟）

- LearnCpp 12.3 “Lvalue references”、12.4 “Lvalue references to const”、12.9 “Pointers and const”、12.14 “Type deduction with pointers, references, and const”
- 对照 12.9 中指向常量的指针与常量指针，写出 `const int*` / `int* const` / `const int* const` 三种声明

---

## ✍️ 写（约 110 分钟）

### 任务 1：引用基础

```cpp
void ref_basics();                    // int& r = x; 改 r 就是改 x
int& get_element(int* arr, int i);    // 返回引用
void modify_via_ref(int& x);
void ref_cannot_rebind();             // 引用一旦绑定不能改绑（用注释写明）
```

### 任务 2：const 三种写法（**今天的重点**）

```cpp
void const_three_forms() {
    int a = 1, b = 2;

    const int* p1 = &a;   // 指向常量的指针：*p1 不能改，p1 可以改
    // *p1 = 10;          // ❌ 编译错误
    p1 = &b;              // ✅ ok

    int* const p2 = &a;   // 常量指针：*p2 可以改，p2 不能改
    *p2 = 10;             // ✅ ok
    // p2 = &b;           // ❌ 编译错误

    const int* const p3 = &a;  // 都不能改
    // *p3 = 10;  p3 = &b;      // ❌ 都编译错误
}
```

**把注释掉的每一行都真跑一遍，看编译器的报错。** 报错信息要抄下来。

### 任务 3：const 成员函数

```cpp
class Demo {
public:
    int  value() const { return v_; }    // const 成员函数
    void set(int x)    { v_ = x; }
private:
    int v_ = 0;
};
// 实验：const Demo d; d.value() 能调用，d.set(1) 编译失败
```

### 任务 4：两版 swap

```cpp
void swap_ptr(int* a, int* b);
void swap_ref(int& a, int& b);
```

---

## ✅ 验收（打勾才算过）

- [ ] **能默写出 `const int* p` / `int* const p` / `const int* const p` 三者的区别**
- [ ] 亲手触发过这三种写法的编译错误，并抄下了报错
- [ ] 能解释：为什么函数参数优先用 `const T&` 而不是 `T`
- [ ] 能说出引用和指针的三个区别（**不能为空、必须初始化、不能重新绑定**）
- [ ] const 成员函数里改成员的编译错误见过一次

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| 记不住哪个 const 管哪个 | **从右往左读**：`int* const p` → p 是 const，指向 int |
| const 对象调用不了成员函数 | 那个成员函数没标 `const` |

---

## 🔜 明天（Day 09）

综合 + 面试题自测，Block 3 收尾。

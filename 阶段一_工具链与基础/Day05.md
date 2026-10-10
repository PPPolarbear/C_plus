# Day 05 · 函数传值 / 传引用 + static

**Block 2/19** · 阶段一（Day 1–15）

---

## ⚡ 现在就开始

```bash
cd cpp-camp2 && cmake --build build && ./build/app
```

---

## 🎯 今天唯一的目标

**亲眼看到"传值不改变原变量、传引用改变"——不是背下来，是看到。**

这是理解 C++ 的第一道分水岭。

---

## 📖 读（约 30 分钟）

- LearnCpp 2.4 “Introduction to function parameters and arguments”、11.5 “Default arguments”
- LearnCpp 12.3 “Lvalue references”、12.4 “Lvalue references to const”、12.5 “Pass by lvalue reference”、12.6 “Pass by const lvalue reference”
- LearnCpp 7.11 “Static local variables”
- 重点看：**值传递 vs 引用传递的区别**

---

## ✍️ 写（约 120 分钟）

```cpp
// ---- 核心对比实验（今天的重点）----
void swap_wrong(int a, int b);        // 传值：调用后原变量【不变】
void swap_right(int& a, int& b);      // 传引用：调用后原变量【交换】

// ---- 参数传递的三种方式 ----
void take_by_value(int x);            // 拷贝一份
void take_by_ref(int& x);             // 引用，可改
void take_by_const_ref(const int& x); // 引用，不可改

// ---- 默认参数 ----
int  power(int base, int exp = 2);    // power(3) == 9

// ---- 函数重载 ----
void print(int x);
void print(double x);
void print(const std::string& s);

// ---- static 局部变量 ----
int  counter_static();                // 每次调用累加：1, 2, 3...
int  counter_normal();                // 每次调用都是 1
```

**在 `main` 里对每个都做对比实验并打印。**

---

## ✅ 验收（打勾才算过）

- [ ] `swap_wrong` 调用后原变量**没变**，`swap_right` 调用后**变了**
- [ ] 能解释 `swap_wrong` 为什么失效（**提示：形参是实参的副本**）
- [ ] `counter_static()` 连续调用返回 1、2、3；`counter_normal()` 永远返回 1
- [ ] 能说出 `static` 局部变量的**生命周期**和**作用域**（不是一回事）
- [ ] `print(1)` / `print(1.5)` / `print("hi")` 各自调用到正确的重载

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| `print("hi")` 报错歧义 | `"hi"` 是 `const char*`，需要 `print(const std::string&)` 能隐式转换或加一个 `const char*` 重载 |
| 忘了引用怎么写 | `int& x` —— `&` 在类型后面是引用，在变量前是取地址 |

---

## 🔜 明天（Day 06）

基础语法综合验收，Block 2 收尾。

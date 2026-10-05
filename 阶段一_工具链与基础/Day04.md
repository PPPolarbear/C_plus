# Day 04 · 类型、转换与控制流

**Block 2/19** · 阶段一（Day 1–15）

---

## ⚡ 现在就开始

```bash
cd cpp-camp2 && code src/main.cpp
```

在 `main.cpp` 里写今天的函数，写完就编译运行。

---

## 🎯 今天唯一的目标

**把"以为自己会、其实有洞"的基础语法扫一遍，把洞找出来。**

今天不追进度，追的是**诚实**——遇到"咦这个我不知道"就停下来。

---

## 📖 读（约 30 分钟）

- learncpp：基本数据类型、类型转换、控制流 三块
- **略读**。看到"我早就会了"的跳过；看到陌生的停下来

---

## ✍️ 写（约 120 分钟）

按签名实现，写进 `src/main.cpp`：

```cpp
// ---- 类型与转换 ----
int    to_int(double d);          // 截断而非四舍五入
double to_double(int i);
bool   is_even(int n);
char   to_upper(char c);

// 观察实验
void   int_overflow_demo();       // int 加到溢出会怎样？用 INT_MAX 试
void   float_precision_demo();    // 0.1 + 0.2 == 0.3 吗？打印出来看

// ---- 控制流 ----
int    sum_1_to_n(int n);         // 循环
int    fib_iter(int n);           // 迭代版斐波那契
int    fib_rec(int n);            // 递归版
void   print_multiplication_table(int n);
int    count_digits(int n);       // 用 while
```

**每个函数在 `main` 里调用一次并打印结果。**

---

## ✅ 验收（打勾才算过）

- [ ] `int_overflow_demo()` 打印出**负数**（亲眼看到溢出）
- [ ] `float_precision_demo()` 打印出 `0.1 + 0.2 != 0.3`
- [ ] 能解释：`int` 加到溢出后为什么会变成负数
- [ ] 能解释：`fib_rec(40)` 为什么慢（**不要求优化，只要求说出原因**）
- [ ] 今天至少发现了一个"原来我不知道"的点，写进 `进度追踪.md` 的卡壳列

---

## 🤔 卡住了怎么办

| 症状 | 解法 |
|---|---|
| 不知道 `INT_MAX` 在哪 | `#include <climits>` |
| 浮点比较总是失败 | 浮点数不能用 `==` 直接比，用 `std::abs(a-b) < 1e-9` |

---

## 🔜 明天（Day 05）

函数传值 vs 传引用——这是理解 C++ 的第一个关键点。
